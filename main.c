#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>


#define HISTORY_MAX 100


typedef struct aliases
{
        char *alias_name;
        char *real_name;
} alias;

typedef struct bin
{
        char *name;
        int (*func)();
} builtin;

char* get_alias(alias *aliases, char *command);
int get_builtin(builtin *builtins, char *command);
int exit_function();
int env_function();
void add_to_history(char *command);
int print_history();
char *find_path(char **environ, char *command);

char *history[HISTORY_MAX];
int history_count = 0;
extern char **environ;

int main(int ac __attribute__((unused)), char **av __attribute__((unused)))
{
        alias my_aliases[] = {
                {"ls", "ls --color=auto"},
                {"ll", "ls -alF"},
                {"la", "ls -A"},
                {NULL, NULL}
        };

        builtin builtins[] = {
                {"exit", exit_function},
                {"env", env_function},
                {"history", print_history},
                {NULL, NULL}
        };

        int int_mode = 1;
        char *buffer = NULL;
        char *token = NULL;
        size_t counter = 0;
        char **toks = NULL;
        int built_in_ret;
        /* char *path = NULL; */
        /* char **dirs = NULL; */
        /* char *path_token = NULL; */
        /* int x = 0; */
        /* char *cwd = NULL; */
        /* struct stat sb; */


        while (int_mode)
        {
                int_mode = isatty(STDIN_FILENO);
                if (int_mode == 1)
                {
                        write(STDOUT_FILENO, "cisfun: ~$ ", 13);
                }

                getline(&buffer, &counter, stdin);

                add_to_history(buffer);

                token = strtok(buffer, " \n");
                toks = malloc(sizeof(char*) * 1);
                counter = 0;
		while (token != NULL)
                {
                        toks[counter] = get_alias(my_aliases, token);
                        token = strtok(NULL, " \n");
                        counter++;
                        toks = realloc(toks, sizeof(char*) * (counter + 1));
                }
                toks[counter] = NULL;

                if (strcmp(toks[0], "exit") == 0)
                {
                        exit(EXIT_SUCCESS);
                }
                built_in_ret = get_builtin(builtins, toks[0]);
                if (built_in_ret == -2)
                {
                        exit(EXIT_SUCCESS);
                }


                /* path = find_path(environ, toks[0]);

                path_token = strtok(path, ":");
                while (path_token != NULL)
                {
                        dirs[x] = strdup(path_token);
                        path_token = strtok(NULL, ":");
                        x++;
                }
                dirs[x] = NULL;

                cwd = getcwd(NULL, 0);

                x = 0;
                while (dirs[x] != NULL)
                {
                        chdir(dirs[x]);
                        if (stat(toks[0], &sb) == 0)
                        {
                                toks[0] = strconcat(dirs[x], toks[0]);
                                break;
                        }
                        x++;
                }
                chdir(cwd); */


                /* else
                {
                        execute(toks);
                } */
        }
	free(buffer);
        buffer = NULL;
        return (0);
}
char* get_alias(alias *aliases, char *command)
{
        int i = 0;

        while (aliases[i].alias_name != NULL)
        {
                if (strcmp(command, aliases[i].alias_name) == 0)
                {
                        return (aliases[i].real_name);
                }
                i++;
        }
        return (command);
}

int get_builtin(builtin *builtins, char *command)
{
    int i = 0, built_in_ret = -2;

    while (builtins[i].name != NULL)
    {
        if (strcmp(command, builtins[i].name) == 0)
        {
            built_in_ret = builtins[i].func();
            break;
        }
        i++;
    }
    return (built_in_ret);
}

int exit_function()
{
    exit(EXIT_SUCCESS);
}

int env_function()
{
    int i = 0;

    while (environ[i] != NULL)
    {
        printf("%s\n", environ[i]);
        i++;
    }
    return (0);
}
void add_to_history(char *command)
{
        /* add the command to the history array */
        int i;
        history[history_count] = strdup(command);
        history_count++;

        /* if the history array is full, remove the oldest entry */
        if (history_count > HISTORY_MAX)
        {
                free(history[0]);
                for (i = 1; i < history_count; i++)
                {
                        history[i - 1] = history[i];
                }
                history_count--;
        }
}

int print_history()
{
        int i;

        for (i = 0; i < history_count; i++)
        {
                printf("%d: %s\n", i+1, history[i]);
        }
        return (0);
}

char *find_path(char **environ, char *command  __attribute__((unused)))
{
        int x = 0;

        while (environ[x] != NULL)
        {
                if (strcmp(environ[x], "PATH") == 0)
                        return(environ[x]);
                x++;
        }
        return (NULL);
}









