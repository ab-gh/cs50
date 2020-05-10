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
    // For all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // If the name is found
        if (strcmp(candidates[i].name, name) == 0)
        {   
            // Record the index
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
        // not sure if this is needed but hey it works
        candidates[c].votes = 0;
    }

    // Count votes
    // for each vote
    for (int d = 0; d < voter_count; d++)
    {
        // for each preference
        for (int e = 0; e < candidate_count; e++)
        {
            // If the candidate in this preference slot isnt eliminated
            if (!candidates[preferences[d][e]].eliminated)
            {
                // They get a vote
                candidates[preferences[d][e]].votes++;
                // Stop because weve counted the highest position preference which isnt eliminated
                break;
            }
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
    // Forces float conversion
    const int QUOTA = ceil(voter_count / 2.0);
    // For candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // If they are not eliminated
        if (!candidates[i].eliminated)
        {
            // If they have over the quota
            if (candidates[i].votes > QUOTA)
            {
                // They win
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
    // Guess that the first candidate has the minimum
    int min = candidates[0].votes;
    for (int i = 0; i < candidate_count; i++)
    {
        // If the candidate isnt eliminated
        if (!candidates[i].eliminated)
        {
            // If the next canidate has less than the current
            if (candidates[i].votes < min)
            {
                // They are the new minimum
                min = candidates[i].votes;
            }

        }
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // For all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // If not eliminated
        if (!candidates[i].eliminated)
        {
            // If they dont have the minimum vote
            if (candidates[i].votes != min)
            {
                // No tie
                return false;
            }
        }

    }
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
            candidates[c].eliminated = true;
        }

    }
    return;
}
