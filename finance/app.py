import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
import numbers
from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    stock_details_1 = []
    stock_details_2 = {}
    """Show portfolio of stocks"""
    user_id = session["user_id"]
    grand_total = 0
    rows = db.execute(
        "SELECT stocks.name as stock_name, shares.count as share_count, users.cash as user_balance, users.username as user_name FROM shares JOIN users on users.id = shares.user_id AND shares.user_id = ? JOIN stocks on stocks.id = shares.stock_id", user_id)
    if len(rows) > 0:
        for row in rows:
            stock_name = row["stock_name"]
            share_count = row["share_count"]
            stock_api_data = lookup(stock_name)
            current_value = stock_api_data["price"]
            holding = current_value * row["share_count"]
            grand_total += holding
            stock_details_1.append({"stock_name": stock_name, "share_count": share_count,
                                   "current_value": float(current_value), "holding": float(holding)})

    user_rows = db.execute("SELECT * FROM users WHERE id = ?", user_id)
    user_name = user_rows[0]["username"]
    balance = user_rows[0]["cash"]
    grand_total += balance
    stock_details_2["user_name"] = user_name
    stock_details_2["balance"] = float(balance)
    stock_details_2["grand_total"] = float(grand_total)
    print(stock_details_2)
    return render_template("index.html", stock_details_1=stock_details_1, stock_details_2=stock_details_2)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        symbol = request.form.get("symbol")
        try:
            '''
            Get the share the user want to buy for the form, if the number of shares
            the user wants to buy is a negative integer, or if the share count is not an integer
            tell the user the input is a bad input
            '''
            try:
                shares = int(request.form.get("shares"))
            except:
                return apology("Bad Input")
            if shares < 0:
                return apology("Bad Input")
        except TypeError:
            return apology("Bad Input")
        if symbol:
            '''
            Check if the symbol exists in the api db, if it does,
            check if we have that symbol in our stocks db, if we don't
            add it to the db either ways, get the id of the stock.
            '''
            quote_dict = lookup(symbol)
            if not quote_dict:
                return apology("Invalid company")
            db_stock_rows = db.execute(
                "SELECT * FROM stocks WHERE name = ?", symbol)
            if not db_stock_rows:
                '''If the stock is a new stock, insert it into the stocks table'''
                stock_id = db.execute(
                    "INSERT INTO stocks (name) VALUES(?)", symbol)
            else:
                stock_id = db_stock_rows[0]["id"]
            """
            Check if the user has enough money to purchase the quantity of stock they
            want to. If they do, then substract the stock price * stock count from their cash.
            """
            user_id = session["user_id"]
            user_cash_row = db.execute(
                "SELECT cash FROM users WHERE id = ?", user_id)
            user_cash = float(user_cash_row[0]["cash"])
            stock_price = float(quote_dict["price"])
            if (user_cash - (stock_price * shares) < 0):
                return apology("Sorry, you do not have enough cash to complete this purchase")
            else:
                user_cash -= stock_price * shares
                user_cash_rows = db.execute(
                    "UPDATE users SET cash = ? WHERE id = ?", user_cash, user_id)
                """Now update the shares db"""
                if not user_cash_rows:
                    return apology("Sorry cannot complete transaction")
                share_id_rows = db.execute(
                    "SELECT shares.id, shares.count FROM shares JOIN users ON users.id = shares.user_id AND shares.user_id = ? JOIN stocks ON stocks.id = shares.stock_id AND shares.stock_id = ?", user_id, stock_id)
                if (len(share_id_rows) == 0):
                    share_id_row = [db.execute(
                        "INSERT INTO shares (user_id, stock_id, count) VALUES(?,?,?)", user_id, stock_id, shares)]
                    transaction_rows = db.execute("INSERT INTO transactions(user_id, stock_id, share_id, transaction_type, count, transaction_value) VALUES(?,?,?,?,?,?)", user_id, stock_id, share_id_row,
                                                  "buy", shares, stock_price * shares)
                else:
                    print(share_id_rows)
                    share_id_row = share_id_rows[0]["id"]
                    print(share_id_row)
                    shares_count_rows = db.execute(
                        "UPDATE shares SET count = ?  WHERE id = ?", int(share_id_rows[0]["count"]) + shares, share_id_row)
                    if shares_count_rows:
                        transaction_rows = db.execute("INSERT INTO transactions(user_id, stock_id, share_id, transaction_type, count, transaction_value) VALUES(?,?,?,?,?,?)", user_id, stock_id, share_id_row,
                                                      "buy", shares, stock_price * shares)
                    if not transaction_rows:
                        return apology("Server error", 500)

                return redirect(url_for("index"))

    return apology("Server Error", 500)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    transaction_rows = db.execute(
        "SELECT t.transaction_type, stocks.name, t.count, t.transaction_value, t.transaction_time FROM transactions as t JOIN stocks on stocks.id = t.stock_id and t.user_id =?  ORDER BY t.transaction_time DESC", user_id)
    transactions= []
    for x in transaction_rows:
        x["transaction_value"] = float(x["transaction_value"])
        transactions.append(x)

    if not transaction_rows:
        return apology("You haven't made any transactions... yet!")
    transaction_rows = transactions
    return render_template("history.html", transaction_rows=transaction_rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        if symbol:
            quote_dict = lookup(symbol)
            if not quote_dict:
                return apology("Invalid company")
            return render_template("quoted.html", quote_dict=quote_dict)
    return apology("Bad request")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == 'GET':
        return render_template("register.html")
    else:
        try:
            username = request.form.get("username")
            password = request.form.get("password")
            confirmation = request.form.get("confirmation")

            if not username:
                return apology("Name is required")
            if not password or not confirmation:
                return apology("password and password confirmation are required")
            if password != confirmation:
                return apology("password and password confirmation don't match")
            user = db.execute(
                "SELECT * FROM users WHERE username = ?", username)
            if len(user) == 0:
                id = db.execute("INSERT INTO users (username, hash) VALUES(?,?)",
                                username, generate_password_hash(password))
                if id:
                    return redirect(url_for("login"))
                else:
                    return apology("Unable to create new accounts. Please try again at a later time")

            else:
                return apology("That username is taken")

        except:
            return apology("Bad Request")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    if request.method == "GET":
        # get all available stocks
        user_stocks = db.execute(
            "SELECT stocks.name as stock_name FROM shares JOIN users on users.id = shares.user_id AND shares.user_id = ? JOIN stocks on stocks.id = shares.stock_id", user_id)
        if not user_stocks:
            return apology("Sorry, you need to purchase stocks to unlock this feature")

        return render_template("sell.html", user_stocks=user_stocks)
    else:
        symbol = request.form.get("symbol")

        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("Bad Input")

        '''
        Get the share the user want to sell from the select in the form, if the number of shares
        the user wants to sell is a negative integer or greater that what they hold, or if the share count is not an integer
        tell the user the input is a bad input or if the stock is not a valid stock.
        '''
        if shares < 0:
            return apology("Shares must be a positive number and at most equal to the amount of the specific companie's stock you own")
        user_id = session["user_id"]
        user_stocks_dict = {row["stock_name"]: row["share_count"] for row in
                            db.execute("SELECT stocks.name as stock_name, shares.count as share_count FROM shares JOIN users on users.id = shares.user_id AND shares.user_id = ? JOIN stocks on stocks.id = shares.stock_id", user_id)}
        if not user_stocks_dict or symbol not in user_stocks_dict.keys():
            return apology(f"Sorry, you don't have {symbol} in your portfolio")
        if user_stocks_dict[symbol] < shares:
            return apology(f"Sorry, you can't buy {shares} amount of {symbol}")

        current_amount = lookup(symbol)["price"]
        sale_value = current_amount * shares
        """Insert into transactions table, update users table, update shares table

        1: update users table, setting their balance to current_balance + sale_value
        3: Update shares table, setting count of the specific share to current_value - shares
        2: Insert into transactions table user_id, stock_id, share_id
        """
        # update users table
        user_cash = float(db.execute(
            "SELECT cash as balance FROM users WHERE id = ? LIMIT 1", user_id)[0]["balance"])
        update_users_table_count = db.execute(
            "UPDATE users SET cash  = ? WHERE id = ?", sale_value + user_cash, user_id)
        if not update_users_table_count:
            return apology("Server error: Unable to sell at this time", 500)
        # update shares table
        shares_stocks_id_rows = db.execute(
            "SELECT shares.id as share_id, stocks.id as stock_id FROM shares JOIN users on users.id = shares.user_id AND users.id = ? JOIN stocks on stocks.id = shares.stock_id  AND stocks.name = ?", user_id, symbol)
        if (not shares_stocks_id_rows):
            return apology("Server error", 500)
        share_id = shares_stocks_id_rows[0]["share_id"]
        stock_id = shares_stocks_id_rows[0]["stock_id"]
        update_shares_table_count = db.execute(
            "UPDATE shares SET count  = ? WHERE id = ?", user_stocks_dict[symbol] - shares, share_id)
        if not update_shares_table_count:
            return apology("Server error: Unable to sell at this time", 500)

        # update transaction table
        transaction_id = db.execute("INSERT INTO transactions(user_id, stock_id, share_id, transaction_type, count, transaction_value) VALUES(?,?,?,?,?,?)",
                                    user_id, stock_id, share_id, "sell", shares, sale_value)

        if (transaction_id):
            return redirect("/")

    return apology("Server Error", 500)


@app.route("/successful_registration", methods=["GET"])
def successful_registration():
    return render_template("successful_registration.html")


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "GET":
        return render_template("change_password.html")
    else:
        user_id = session["user_id"]
        old_password = request.form.get("old_password")
        new_password = request.form.get("new_password")
        if new_password:
            user = db.execute("SELECT * FROM users WHERE id = ?", user_id)
            if not check_password_hash(user[0]["hash"], old_password):
                return apology("Incorrect old password")
            else:
                new_password = generate_password_hash(new_password)
                alter_count = db.execute(
                    "UPDATE users SET hash = ?", new_password)
                if (alter_count):
                    return redirect("/")
                else:
                    return apology("Sorry could not change your password")
        else:
            return apology("You must enter a new password")
