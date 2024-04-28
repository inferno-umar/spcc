/*
Implementation of Two-Pass Macro Processor

Experiment: 3

@Learner: TE-CO
Name: Siddiqui Umar Farooque
Roll No: 21co57
Batch: 3
Academic Year: 2024
Sem - 6
*/

// Program:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define MAX_MACROS 5

int no_of_macros;
mnt_t mnt[MAX_MACROS];
mdt_t mdt[MAX_MACROS];
ala_t ala;

void pass_one(FILE *file)
{
  char *line = NULL;
  size_t len = 0;

  int i = 0;

  while (getline(&line, &len, file) != -1)
  {
    if (contains(line, "MACRO"))
    {
      if (getline(&line, &len, file) == -1)
      {
        exit(1);
      }

      char *macro_name = get_macro_name(line);

      char **argv = gets_args(macro_name);

      insert_mnt((mnt + i), macro_name, count(argv), i);

      insert_mdt((mdt + i), i, definition(argv, file));

      i++;
    }
    no_of_macros = i;
  }
}

void pass_two(FILE *input)
{
  FILE *output = fopen("output.asm", "w");
  char *line = NULL;
  size_t len;

  bool is_macro_definition = false;
  while (getline(&line, &len, input) != -1)
  {
    if (contains(line, "MACRO"))
    {
      fprintf(output, "%s", line);
      is_macro_definition = true;
    }
    else if (is_macro_definition || !is_macro_invocation(mnt, no_of_macros, line))
    {
      fprintf(output, "%s", line);
      is_macro_definition = false;
    }
    else
    {
      char *macro_name = get_macro_name(line);
      char *definiton = get_def(mnt, mdt, macro_name);
      char **args = gets_args(macro_name);

      insert_ala(&ala, args);

      definiton[strlen(definiton) - 4] = '\0';
      char *word = strtok(definiton, " ");

      while (word != NULL)
      {
        if (word[0] == '#')
        {
          word++;
          char *arg = ala.actual[atoi(word) - 1];
          fprintf(output, "%s", arg);
          if (word[strlen(word) - 1] == ',')
          {
            fprintf(output, "%s", ", ");
          }
        }
        else
        {
          fprintf(output, "%s ", word);
        }
        word = strtok(NULL, " ");
      }
      fprintf(output, "%s", "\n");
    }
  }

  fclose(output);
}

int main(void)
{
  FILE *input = fopen("input.asm", "r");
  if (input == NULL)
  {
    printf("Target file: `input.asm` not found in the current directory\n");
    return 1;
  }

  printf("Pass one in progress.\n");
  pass_one(input);
  printf("Pass one completed.\n");
  fclose(input);

  display_mnt_mdt(mnt, mdt, no_of_macros);

  input = fopen("input.asm", "r");

  printf("Pass two in progress\n");
  pass_two(input);
  printf("Pass two completed.\n");

  fclose(input);

  return 0;
}

/*
 Output:
aiktc@aiktc-OptiPlex-3046:~/Desktop/21co54/Macro$ gcc macro.c
aiktc@aiktc-OptiPlex-3046:~/Desktop/21co54/Macro$ ./a.out
Pass one in progress.
Pass one completed.
=========================| MNT |=========================
---------------------------------------------------------
Name    Param   Index
---------------------------------------------------------
ADD1    2       0
---------------------------------------------------------


=========================| MDT |=========================
---------------------------------------------------------
Definition              Index
---------------------------------------------------------
 OAD arg1
ADD #2
 TORE arg1
 END

 DD1 N1, N2

 ACRO
 UL1 &arg3, &arg4
 OV A, 00
 OV C, &arg4
 epeat: ADD &arg3
 NZ Repeat
 END

MUL1 N3, N4             0
---------------------------------------------------------


Pass two in progress
Pass two completed.
*/