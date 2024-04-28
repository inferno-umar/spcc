#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_ARGS 20

bool contains(char *str, char *substr)
{
    return strstr(str, substr) != NULL;
}

int count(char **argv) {
    int i = 0;
    while (argv[i++]);
    return i - 1;
}

int index_of(char *word, char **args)
{
    int i = 0;
    while (strcmp(args[i++], word) != 0);
    return i - 1;
}

typedef struct mnt {
    char *name;
    int no_of_params;
    int index;
} mnt_t;

void insert_mnt(mnt_t *mnt, char *macro_name, size_t no_of_params, int index) {
    mnt->name = (char *) malloc(strlen(macro_name) * sizeof(char));
    strcpy(mnt->name, macro_name);
    mnt->no_of_params = no_of_params;
    mnt->index = index;
}

typedef struct mdt {
    int index;
    char *definition;
} mdt_t;

void insert_mdt(mdt_t *mdt, int index, char *definition) {
    mdt->index = index;
    mdt->definition = (char *)malloc(strlen(definition) * sizeof(char));
    strcpy(mdt->definition, definition);
    free(definition);
}

typedef struct ala {
  int positional[MAX_ARGS];
  char *actual[MAX_ARGS];
} ala_t;

void insert_ala(ala_t *ala, char **argv) {
    for (int i = 0; i < count(argv); i++) {
        ala->positional[i] = i + 1;
        ala->actual[i] = (char *)malloc(strlen(argv[i]) * sizeof(char *));
        strcpy(ala->actual[i], argv[i]);
    }
}

char *get_macro_name(char *line) { 
    return strtok(line, " "); 
}

char **gets_args(char *macro_name) {
    macro_name = strtok(NULL, ", ");
    int i = 0;
    char **args = (char **)malloc(sizeof(char *) * MAX_ARGS);
    while (macro_name != NULL) {
        args[i++] = macro_name;
        macro_name = strtok(NULL, ", ");
    }
    if (i > 0) {
        args[i - 1][strlen(args[i - 1]) - 1] = '\0';
    }
    return args;
}

bool is_arg(char *word, char **argv) {
    int i = 0;
    while (argv[i] != NULL) {
        if (strcmp(argv[i++], word) == 0) {
            return true;
        }
    }
    return false;
}

char *definition(char **argv, FILE *fp) {
    char *line;
    size_t len = 0;

    char *def = (char *)malloc(sizeof(char *));

    while (getline(&line, &len, fp) != -1) {

        char *word = strtok(line, " ");
        while (word != NULL) {
            if (word[strlen(word) - 1] == '\n') {
                word[strlen(word) - 1] = '\0';
            }
            bool comma = false;
            if (word[strlen(word) - 1] == ',') {
                comma = true;
                word[strlen(word) - 1] = '\0';
            }
            if (is_arg(word, argv)) {
                char num[count(argv)];
                sprintf(num, "#%d", index_of(word, argv) + 1);
                def = (char *) realloc(def, 1 + (strlen(def) + strlen(num) + 1) * sizeof(char *));
                strcat(def, num);
            } else {
                def = (char *) realloc(def, 1 + (strlen(def) + strlen(word)) * sizeof(char *));
                strcat(def, word);
            }
            if (comma == true) {
                strcat(def, ", ");
            } else
            strcat(def, " ");
            word = strtok(NULL, " ");
        }
        strcat(def, "\n");
        if (strcmp(line, "MEND") == 0 || strcmp(line, "MEND\n") == 0) {
            break;
        }
    }
    def[strlen(def) - 2] = '\0';
    return def;
}

bool is_macro_invocation(mnt_t *mnt, int len, char *line) {
    for (int i = 0; i < len; i++) {
        if (contains(line, mnt[i].name)) {
            return true;
        }
    }
    return false;
}

char *get_def(mnt_t *mnt, mdt_t *mdt, char *macro_name) {
    int i = 0;
    while (strcmp(macro_name, mnt[i++].name) != 0);
    return mdt[i - 1].definition;
}

void display_mnt_mdt(mnt_t *mnt, mdt_t *mdt, int no_of_macros)
{
    printf("=========================| MNT |=========================\n");
    printf("---------------------------------------------------------\n");
    printf("Name\tParam\tIndex\n");
    printf("---------------------------------------------------------\n");
    for (int j = 0; j < no_of_macros; j++) {
        printf("%s\t%d\t%d\n", mnt[j].name, mnt[j].no_of_params, mnt[j].index);
        printf("---------------------------------------------------------\n");
    }

    printf("\n\n");
    printf("=========================| MDT |=========================\n");
    printf("---------------------------------------------------------\n");
    printf("Definition\t\tIndex\n");
    printf("---------------------------------------------------------\n");
    for (int j = 0; j < no_of_macros; j++) {
        printf("%s\t\t\t%d\n", mdt[j].definition, mdt[j].index);
        printf("---------------------------------------------------------\n");
    }
    printf("\n\n");
}
