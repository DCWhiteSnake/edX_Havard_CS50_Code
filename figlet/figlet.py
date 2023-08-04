import pyfiglet
from sys import argv, exit
from cs50 import get_string
from random import randrange

def main():
    length = len(argv)
    fonts = pyfiglet.FigletFont.getFonts()
    if length not in [1,3]:
        print("Invalid usage")
        exit(1)
    else:
        if length == 3:
            if argv[1] not in ["-f", "--font"] or argv[2] not in fonts:
                print("Invalid usage")
                exit(2)
            else:
                f = argv[2].lower()
        else:
            f_i = randrange(len(fonts))
            f = fonts[f_i]
    text = get_string("Input: ")
    pf = pyfiglet.Figlet(font=f)
    print(pf.renderText(text))


main()

