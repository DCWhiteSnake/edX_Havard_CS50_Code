import csv
from sys import argv
from contextlib import ExitStack


def main():

    # TODO: Check for command-line usage
    if (len(argv) != 3):
        print("Incorrect number of arguments\nUsage: ./dna database.csv sequence.txt")
    # TODO: Read database file into a variable
    with open(argv[1], "r") as r_db:
        reader = csv.DictReader(r_db)
        fields = reader.fieldnames[1:]
        db = convert_to_dict(reader)
    # TODO: Read DNA sequence file into a variable
    with open(argv[2], "r") as r_sq:
        sq = ""
        for x in r_sq:
            sq += x

    # TODO: Find longest match of each STR in DNA sequence
    l_str = find_longest_match(sq, fields)
    # Find the matching profile
    match = find_matching_profile(l_str, db, fields)
    print(match)

# l_str = a dictionary of the length of the longest match for each str in the sequence


def find_longest_match(sq, fields):
    """ Find the longest matches for each entry in fields and store it as a kvp of STR -> longest seq count"""
    l_str = {}
    for str in fields:
        l_str[str] = longest_match(sq, str)
    return l_str

# l_vals = the lengths of the longest seq for each str


def find_matching_profile(l_str, db, fields):
    # TODO: Check database for matching profiles
    match = "No match"
    for individual in db:
        db_vals = [int(x) for x in db[individual]]
        l_vals = [0] * len(fields)
        i = 0
        for key_2 in l_str:
            l_vals[i] = l_str[key_2]
            i = i + 1
        if db_vals == l_vals:
            match = individual
    return match


def convert_to_dict(reader):
    db = {}
    neuc_len = len(reader.fieldnames) - 1
    for row in reader:
        if row["name"] not in db:
            db[row["name"]] = [0] * neuc_len
            for i in range(0, neuc_len):
                if i > neuc_len - 1:
                    break
                db[row["name"]][i] = row[reader.fieldnames[i + 1]]
    return db


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
