menu = {
    "Baja Taco": 4.00,
    "Burrito": 7.50,
    "Bowl": 8.50,
    "Nachos": 11.00,
    "Quesadilla": 8.50,
    "Super Burrito": 8.50,
    "Super Quesadilla": 9.50,
    "Taco": 3.00,
    "Tortilla Salad": 8.00
}
total = 0.00
while True:
    try:
        order = input("Item: ")
        if (order.title() not in menu and order):
            continue
        elif (order.title() in menu):
            total += menu[order.title()]
        else:
            break
    except EOFError:
        break
    except KeyboardInterrupt:
        break
print("${0:.2f}".format(total))
