def main():
    sentence = input("Text: ")
    index = calculate_coleman_liau_index(sentence)
    if (index < 1):
        print("Before Grade 1\n")
    elif (index >= 1 and index <= 16):
        print("Grade %i\n", index)
    else:
        print("Grade 16+\n")


# counts the letters in text


def count_letters(text):
    letter_count = 0
    sentence_length = len(text)

    if (sentence_length > 0):
        for i in range(0, sentence_length):
            if text[i].isalpha():
                letter_count = letter_count + 1
    return letter_count


# counts the number of words in text
def count_words(text):
    word_count = 0
    sentence_length = len(text)

    if (sentence_length > 0):
        for i in range(0, sentence_length):
            if (i == 0):
                word_count = word_count + 1
            if (text[i] == " "):
                word_count = word_count + 1
    return word_count

# counts the amount of sentences in the input aka text.


def count_sentences(text):

    sentence_count = 0
    sentence_length = len(text)

    if (sentence_length > 0):
        for i in range(0, sentence_length):
            # If we encounter a ".', "?" or an "!" then
            # that indicates a sentence ending
            if (text[i] == "." or text[i] == "!" or text[i] == "?"):
                sentence_count = sentence_count + 1
    return sentence_count

# @brief  Calculates the Coleman-Liau index.
# @param sentence The sentence to calculate the Coleman-Liau index for.
# @return the Coleman-Liau index


def calculate_coleman_liau_index(sentence):
    # get the required variables to calculate the index
    letter_count = count_letters(sentence)
    word_count = count_words(sentence)
    sentence_count = count_sentences(sentence)

    # calculate the letter count  and sentence count per 100 words
    l = (letter_count / word_count) * 100
    s = (sentence_count / word_count) * 100

    # finally calculate the Coleman-Liau index
    index = 0.0588 * l - 0.296 * s - 15.8

    # return the integer representation of the index using the round method
    return round(index)


main()
