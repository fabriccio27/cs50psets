#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j //arrow goes from i to j? true. No arrow? false
bool locked[MAX][MAX];

// Each pair has a winner, loser. Using winner index and loser index
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2]; //la cantidad de combinaciones de a dos, descontando para tener en cuenta a-b b-a

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool check_for_loop(int endofpair, int startofpair);

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
    for (int i = 0; i < candidate_count; i++) //arma arreglo de candidates
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
    for (int i = 0; i < voter_count; i++) //para cada votante
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];//defino el tamaño del ranking en base a la cantidad de candidatos

        // Query for each rank
        for (int j = 0; j < candidate_count; j++) //para cada fila del ranking, pregunto a quien quiere poner
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks); //el votante i tiene en j a indice de candidato

        printf("\n");
    }

    add_pairs();
    sort_pairs(); //determines which victories were larger. The larger one is locked in lock_pairs
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    //I have as much ranks[] as voters
    //ranks[] is an array of all of that particular voter ranks
    for (int i = 0; i < candidate_count; i++)
    {
        if(strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    //load each ranks (no. of ranks is equal to number of voters) in preferences
    // preferences is different from runoff.
    // preferences has no. of candidates rows and no. of candidates columns (in runoff it had no. of voters rows)

    for (int i = 0; i < candidate_count-1; i++)
    {
        for(int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }

    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i+1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
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
    pair temp; //temp pair to store while doing bubble sort
    int delta_l;
    int delta_r;
    for (int i = pair_count - 1; i > 0; i--)
    {
        delta_r = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
        delta_l = preferences[pairs[i-1].winner][pairs[i-1].loser] - preferences[pairs[i-1].loser][pairs[i-1].winner];

        if(delta_r > delta_l)
        {
            temp = pairs[i - 1];
            pairs[i - 1] = pairs[i];
            pairs[i] = temp;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    //locked[i][j] means i is locked in over j //arrow goes from i to j? true
    // must incorporate a way to check if adding a certain pair will cause a cycle. A dedicated function might be useful
    for (int i = 0; i < pair_count; i++)
    {
        if(!check_for_loop(pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }

    }
    return;
}
//checkear si del perdedor puedo volver al ganador, lo que sería un ciclo
bool check_for_loop(int endofpair, int startofpair)
{
    //base case
    if (endofpair == startofpair)
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        //si existe el par donde el perdedor del par analizado es ganador
        if (locked[endofpair][i])
        {
            //return true;
            //no lo descarto inmediatamente
            if(check_for_loop(i, startofpair))
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
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < pair_count; j++)
        {
            if (locked[j][i] == false)
            {
                printf("%s\n", candidates[i]);
            }
        }
    }

}

