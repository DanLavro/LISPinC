/*
 * repl.c
 *
 * This file implements a Read-Eval-Print Loop (REPL) for a simple Lisp interpreter.
 * It supports basic line editing and command history on Unix-like systems through
 * the use of the editline library. On other systems, it falls back to a simpler
 * input mechanism. This REPL reads input from the user, evaluates it, and prints
 * the result back to the user, looping until an EOF signal is received.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__unix__) || defined(__APPLE__)
#include <editline/readline.h>
#define USE_EDITLINE
#endif

char *read_input(char *prompt)
{
#ifdef USE_EDITLINE
    char *input = readline(prompt);
    if (input)
    {
        add_history(input);
    }
    return input;
#else
    printf("%s", prompt);
    size_t bufsize = 1024;
    char *input = malloc(bufsize);
    if (!input)
    {
        fprintf(stderr, "Allocation error\n");
        return NULL;
    }
    getline(&input, &bufsize, stdin);
    return input;
#endif
}

char *eval_input(char *input)
{
    return input;
}

char *rep(char *input)
{
    char *eval = eval_input(input);
#ifndef USE_EDITLINE
    free(input);
#endif
    return eval;
}

int main()
{
    puts("Welcome to the Lisp REPL.");
    puts("Enter your command, or press Ctrl-D (Unix) / Ctrl-Z (Windows) to exit.");
    while (1)
    {
        char *input = read_input("user> ");
        if (!input)
        {
            fprintf(stderr, "Failed to read input. Exiting.\n");
            break;
        }
        char *result = rep(input);
        printf("%s\n", result);
#ifdef USE_EDITLINE
        free(input);
#endif
    }
    return 0;
}
