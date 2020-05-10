#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        printf("No winner this round\n");
        // Eliminate last-place candidates
        int min = find_min();
        printf("Min: %i\n", min);
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            printf("tied\n");
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }
        printf("No tie\n");

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // Reset
    for (int c = 0; c < candidate_count; c++)
    {
        candidates[c].votes = 0;
    }
    // Count votes
    for (int d = 0; d < voter_count; d++)
    {
        if (!candidates[preferences[d][0]].eliminated)
        {
            candidates[preferences[d][0]].votes++;
        }
    }
    for (int x = 0; x < candidate_count; x++)
    {
        printf("Candidate %s: %i\n", candidates[x].name, candidates[x].votes);
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{

    const int QUOTA = ceil(voter_count / 2);
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            if (candidates[i].votes > QUOTA)
            {
                printf("%s\n", candidates[i].name);
                return true;
            }
        }

    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min = candidates[0].votes;
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            if (candidates[i].votes < min)
            {
                min = candidates[i].votes;
            }

        }
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // Count up the remaining candidates
    int candidates_left = 0;
    for (int c = 0; c< candidate_count; c++)
    {
        if (!candidates[c].eliminated)
        {
            candidates_left++;
        }
    }
    for (int i = 0; i < candidate_count - 1; i++)
    {
        if (!(candidates[i].votes == candidates[i + 1].votes))
        {
            return false;
        }
    }
    printf("Tied\n");
    return true;
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    // for each candidate
    for (int c = 0; c < candidate_count; c++)
    {
        // if candidate has minimum number of votes, we need to eliminate them
        if (candidates[c].votes == min)
        {
            printf("eliminating candidate %s\n", candidates[c].name);
            bool shifting = false;
            // For each vote(r)
            for (int v = 0; v < voter_count; v++)
            {
                // For each preference
                for (int p = 0; p < candidate_count; p++)
                {
                    // If preference is the eliminated candidate we will start eliminating
                    if (preferences[v][p] == c)
                    {
                        shifting = true;
                    }
                    // if we are eliminating
                    if (shifting)
                    {
                        if (p < candidate_count)
                        {
                            preferences[v][p] = preferences[v][p + 1];
                        }
                    }
                }
            }
            candidates[c].eliminated = true;
        }

    }
    return;
}
