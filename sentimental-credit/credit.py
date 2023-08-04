from math import floor
import re


def get_size(userInput):
    if userInput > 0:
        string_rep = str(userInput)
        return len(string_rep)
    return 0

# Check that the card is infact a valid card


def checksum(usr_input, inputSize):
    # x2Sum will be used to aggregate the digits multiplied by 2
    x2Sum = 0
    secondToLastIndex = inputSize - 2
    for i in range(secondToLastIndex, -1, -2):
        # if the value of index i is >= 10 split it into 2 and then add otherwise add it as is to x2Sum
        if usr_input[i] * 2 < 10:
            x2Sum += usr_input[i] * 2
        else:
            rem = int(floor(usr_input[i] * 2) % 10)
            quo = int(floor(usr_input[i] * 2) / 10)
            x2Sum += rem + quo
    sum = 0
    for i in range(inputSize - 1, -1, -2):
        sum += usr_input[i]
    total = x2Sum + sum
    # if total is a multiple of 10
    if int(floor(total % 10 == 0)):
        return True
    return False

# Check if the card is an American Exchange card


def IsAE(usr_input,  inputSize):
    if (inputSize != 15):
        return False
    # Yes at this point usr_input is valid so you can check at aindex 0 and 1
    if ((usr_input[0] == 3 and usr_input[1] == 4) or (usr_input[0] == 3 and usr_input[1] == 7)):
        return True
    return False

# / Check if the card is a VisaCard


def IsVC(usr_input,  inputSize):
    if (inputSize != 16 and inputSize != 13):
        return False
    if (usr_input[0] == 4):
        return True
    return False


# Check if the card is a MasterCard
def IsMC(usr_input,  inputSize):
    if (inputSize != 16):
        return False
    if (
        (usr_input[0] == 5 and usr_input[1] == 1) or
        (usr_input[0] == 5 and usr_input[1] == 2) or
        (usr_input[0] == 5 and usr_input[1] == 3) or
        (usr_input[0] == 5 and usr_input[1] == 4) or
            (usr_input[0] == 5 and usr_input[1] == 5)):
        return True
    return False


def main():
    raw_input = input("Input your card number: ")
    valid = re.fullmatch("^[0-9]*", raw_input)
    if (not valid):
        print("INVALID\n")
        return 0
    card_num = int(valid.string)
    count = get_size(card_num)
    if (count < 13 or count > 16 or count == 14):
        print("INVALID\n")
        return 0
    split_input = [0] * count
    # get a power of 10 which we will use to split the usr_input into integer singletons.
    # i.e a number with a size of 13 needs 10^12 to get the 1st number.
    count_2 = pow(10, count - 1)

    # tracks the index to store the parsed number in the split_input array.
    index = 0

    # now put these singeltons in an array
    while (count_2 >= 1):
        temp = card_num / count_2
        rem = card_num % count_2
        split_input[index] = int(floor(temp))
        card_num = rem
        index += 1

        # reduce count_2 by a factor of /10
        count_2 = int(floor(count_2 / 10))

    # if the card is at least a valid card then procede to checking if it is one of them after which you print out the corresponding name.
    if (checksum(split_input, count)):
        if (IsAE(split_input, count)):
            print("AMEX\n")
        elif (IsMC(split_input, count)):
            print("MASTERCARD\n")
        elif (IsVC(split_input, count)):
            print("VISA\n")
        else:
            print("INVALID\n")
    else:
        print("INVALID\n")

    return 0


if __name__ == '__main__':
    main()
