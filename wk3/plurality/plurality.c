#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }
    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // if there is the given name in the candidates list
        if (strcmp(candidates[i].name, name) == 0)
        {
            // increase their number of votes
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // array index of the winner
    int highest_votes_index = 0;
    // first pass, finding the first "winner"
    for (int j = 0; j < candidate_count; j++)
    {
        if (candidates[j + 1].votes > candidates[j].votes)
        {
            highest_votes_index = j + 1;
        }
    }
    // prints the first winner
    printf("%s\n", candidates[highest_votes_index].name);
    // checks to see if anyone has the same votes as the winner, and prints the other tied winners
    for (int k = 0; k < candidate_count; k++)
    {
        if ((candidates[k].votes == candidates[highest_votes_index].votes) && (k != highest_votes_index))
        {
            printf("%s\n", candidates[k].name);
        }
    }
    return;
}

