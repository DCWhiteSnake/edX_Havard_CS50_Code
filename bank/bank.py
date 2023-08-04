from cs50 import get_string

greeting = get_string("Greeting: ").lower().strip()
if "hello" in greeting[0:5]:
    loss = 0
elif greeting[0] == 'h':
    loss = 20
else:
    loss = 100
print(f"${loss}")