import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    dic_months = {1: 31, 2: 29, 3: 31, 4: 30, 5: 31,
                  6: 30, 7: 31, 8: 31, 9: 30, 10: 31, 11: 30, 12: 31}
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        try:
            name = request.form.get("name")
            month = int(request.form.get("month"))
            day = int(request.form.get("day"))
        except:
            return redirect("/error.html")

        if (not dic_months[month]
            or not dic_months[month]
            or day > dic_months[month]
            or day > 31 or day < 1
            or not name):
            return redirect("/error.html")
        else:
            db.execute("INSERT INTO birthdays (name, month, day) VALUES(?,?,?)", name, month, day)
            return redirect("/")
    else:

        # TODO: Display the entries in the database on index.html
        birthdays = db.execute("SELECT name, day || '/' || month as b_d FROM birthdays")
        return render_template("index.html", birthdays=birthdays)


@app.route("/error.html", methods=["GET"])
def error():
    return render_template("error.html")