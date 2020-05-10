#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>



int main(void)
{

    int candidate_count = 10;
    int candidates[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    
    for (int k = 0; k < candidate_count; k++)
    {
        printf("%ith preferecnce: %i\n", k, candidates[k]);
    }
    
    bool redistributing = false;
    // for each preference
    for (int i = 0; i < candidate_count; i++)
    {
        // if eleminated candidate
        if (candidates[i] == 0)
        {
            redistributing = true;
        }
        if (redistributing)
        {
            if (i < candidate_count)
            {
                candidates[i] = candidates[i + 1];
            }
            else
            {
                candidates[i] = -1;
            }
        }
        
        
        
    }
    candidate_count--;
    printf("\n\n");

    for (int k = 0; k < candidate_count; k++)
    {
        printf("%ith preferecnce: %i\n", k, candidates[k]);
    }
}
