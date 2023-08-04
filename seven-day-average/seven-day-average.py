import csv
import requests


def main():
    # Read NYTimes Covid Database
    download = requests.get(
        "https://raw.githubusercontent.com/nytimes/covid-19-data/master/us-states.csv"
    )
    decoded_content = download.content.decode("utf-8")
    file = decoded_content.splitlines()
    reader = csv.DictReader(file)

    # Construct 14 day lists of new cases for each states
    (new_cases, last_id) = calculate(reader)

    # Create a list to store selected states
    states = []
    print("Choose one or more states to view average COVID cases.")
    print("Press enter when done.\n")

    while True:
        state = input("State: ")
        if state in new_cases:
            states.append(state)
        if len(state) == 0:
            break

    print(f"\nSeven-Day Averages")

    # Print out 7-day averages for this week vs last week
    comparative_averages(new_cases, states, last_id)


# TODO: Create a dictionary to store 14 most recent days of new cases by state
def calculate(reader):
    id_row = -1
    buffer_size = 14
    n_c = {}
    curr_id_case = {}
    curr_id_case_e = {}
    for row in reader:
        id_row += 1
        if id_row == 0:
            continue
        else:
            curr_case = int(row["cases"])
            state = row["state"]
            if state in n_c:
                if curr_id_case[state] <= buffer_size - 1 and len(n_c[state]) < buffer_size:
                    prev_case = n_c[state][curr_id_case[state]]
                    n_c[state].append(curr_case - prev_case)
                    curr_id_case[state] += 1
                    curr_id_case_e[state] += 1
                    continue
                elif len(n_c[state]) >= buffer_size:
                    if curr_id_case[state] == buffer_size - 1:
                        curr_id_case[state] = 0
                        curr_id_case_e[state] += 1
                    else:
                        curr_id_case[state] += 1
                        curr_id_case_e[state] += 1
                    prev_case = n_c[state][curr_id_case[state] - 1]
                    n_c[state][curr_id_case[state]] = curr_case - prev_case
            else:
                n_c[state] = [int(row["cases"])]
                curr_id_case[state] = 0
                curr_id_case_e[state] = 0
    return (n_c, curr_id_case_e)


# TODO: Calculate and print out seven day average for given state
def comparative_averages(new_cases, states, last_id):
    for state in states:
        loc = new_cases[state]
        old_avg = 0
        seven_avg = 0
        for i in range (7):
            start_old = last_id[state] % 14 + 1
            idx = (start_old) + i
            if idx > len(loc) - 1:
                idx = idx - len(loc)
            old_avg += loc[idx]
        for i in [0, -1, -2, -3, -4, -5, -6]:
            start_new = last_id[state] % 14
            idx = (start_new) + i
            seven_avg += loc[idx]

        old_avg = int(old_avg / 7)
        seven_avg = int(seven_avg / 7)

        try:
            percent = int(((old_avg - seven_avg) / old_avg) * 100)
        except ZeroDivisionError:
            percent = 100 * seven_avg
        direction = "an increase" if (percent < 0) else "a decrease"
        print(
            f"{state} had a 7-day average of {seven_avg} and {direction} of {abs(percent)}%.")


main()
