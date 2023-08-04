# TODO
# Print a pyramid depending on user input
def print_pyramid():
    try:
        size = int(input("Height: "))
    except ValueError:
        print_pyramid()
    if size <= 0 or size > 8:
        print_pyramid()
    else:
        for i in range(1, size + 1):
            space = " " * (size - i)
            sym = "#" * i
            gap = " " * 2
            print(f"{space}{sym}{gap}{sym}")


if __name__ == '__main__':
    print_pyramid()