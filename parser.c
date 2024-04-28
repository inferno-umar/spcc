/*
Implementation of Parser

Experiment: 5

@Learner: TE-CO
Name: Siddiqui Umar Farooque
Roll No: 21co57
Batch: 3
Academic Year: 2024
Sem - 6
*/

// Program:

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void parser(char *S)
{
    int n = strlen(S);
    int start = 0, end = 0;
    int i, j;

    for (i = 0; i < n; i++)
    {
        if (S[i] == '>')
        {
            start = i + 1;
            break;
        }
    }

    while (S[start] == ' ')
    {
        start++;
    }

    for (i = start; i < n; i++)
    {
        if (S[i] == '<')
        {
            end = i - 1;
            break;
        }
    }

    for (j = start; j <= end; j++)
    {
        printf("%c", S[j]);
    }

    printf("\n");
}

int main()
{
    char input1[] = "<h1>This is a statement</h1>";
    char input2[] = "<h1>         This is a statement with some spaces</h1>";
    char input3[] = "<p> This is a statement with some @ #$ ., / special characters</p>         ";

    printf("Parsed Statements:\n");

    parser(input1);
    parser(input2);
    parser(input3);

    return 0;
}

/*
Output:
Parsed Statements:
This is a statement
This is a statement with some spaces
This is a statement with some @ #$ ., / special characters
*/