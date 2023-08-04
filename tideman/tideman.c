#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9
#define MAX_pairs MAX *((MAX - 1) / 2)
// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool is_cycle(int winner, int loser);
void print_winner(void);
void selection_sort(pair unsorted_pairs[MAX_pairs]);
int find_next_largest(pair unsorted_pairs[MAX_pairs], int start);
void swap(pair *posA, pair *posB);

/// @brief
/// @param argc
/// @param argv
/// @return
int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];
        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO

    if (name != NULL)
    {
        for (int i = 0; i < candidate_count; i++)
        {
            if (strcmp(name, candidates[i]) == 0)
            {
                ranks[rank] = i;
                return true;
            }
            else if (strcmp(name, candidates[i]) != 0 && i == MAX - 1)
            {
                return false;
            }
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i >= j)
            {
                continue;
            }
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    // int pair_counter = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] != 0 && preferences[i][j] > preferences[j][i])
            {
                pair newPair;
                newPair = (pair){
                    .winner = i, .loser = j};
                pairs[pair_count] = newPair;
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    selection_sort(pairs);
    return;
}

void selection_sort(pair unsorted_pairs[MAX_pairs])
{
    for (int i = 0; i < MAX_pairs; i++)
    {
        int largest_pair = find_next_largest(unsorted_pairs, i);
        swap(&pairs[largest_pair], &pairs[i]);
    }
}

int find_next_largest(pair unsorted_pairs[MAX_pairs], int start)
{
    pair largest_pair = pairs[start];
    int largest_pair_position = start;
    for (int i = start + 1; i < MAX_pairs; i++)
    {
        if (preferences[pairs[i].winner][pairs[i].loser] >= preferences[largest_pair.winner][largest_pair.loser])
        {
            largest_pair = pairs[i];
            largest_pair_position = i;
        }
    }
    return largest_pair_position;
}

void swap(pair *posA, pair *posB)
{
    pair temp = *posA;
    *posA = *posB;
    *posB = temp;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < MAX_pairs; i++)
    {
        if (preferences[pairs[i].winner][pairs[i].loser])
        {
            if (is_cycle(pairs[i].winner, pairs[i].loser))
            {
                // locked[pairs[i].loser][pairs[i].winner] = false;
                continue;
            }
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}
bool is_cycle(int winner, int loser)
{
    if (winner == loser)
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            if(is_cycle(winner, i))
            {
                return true;
            }
        }
    }
    return false;
}
// Print the winner of the election
void print_winner(void)
{
    // TODO
    int max_value = preferences[pairs[0].winner][pairs[0].loser];
    int winners[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        winners[i] = 0;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i != j)
            {
                if (locked[i][j])
                {
                    winners[j] += 1;
                }
            }
        }
    }
    // if candidate(s) i was never locked, then that candidate is the winner
    for (int i = 0; i < candidate_count; i++)
    {
        if (winners[i] == 0)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}