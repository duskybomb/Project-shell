/***************************************************************************//**

  @file         main.c

  @author       Harshit Joshi & Eklavya Chopra

  @date         Friday,  9 November 2018

  @brief        ASH (Ares SHell)

*******************************************************************************/

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>

const char rocket[] =
"           _\n\
          /^\\\n\
          |-|\n\
          | |\n\
          | |\n\
          |A|\n\
          |S|\n\
          |H|\n\
         /| |\\\n\
        / | | \\\n\
       |  | |  |\n\
        `-\"\"\"-`\n\
";





/*
  Function Declarations for builtin shell commands:
 */
int ash_cd(char **args);
int ash_help(char **args);
int ash_exit(char **args);
int ash_mkdir(char **args);
/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "mkdir"
};

int (*builtin_func[]) (char **) = {
  &ash_cd,
  &ash_help,
  &ash_exit,
  &ash_mkdir
};

int ash_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
int ash_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "ash: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("ash");
    }
  }
  return 1;
}

/**
   @brief Builtin command: print help.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */
int ash_help(char **args)
{
  int i;
  printf("%s",rocket );
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < ash_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined
   @return Always returns 0, to terminate execution.
 */
int ash_exit(char **args)
{
  return 0;
}



/**
   @brief Builtin command: make directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */




 int ash_mkdir(char **args)
 {

            if(args[1]==NULL){
              fprintf(stderr, "ash: expected argument to \"mkdir\"\n");

            }else{

                    if(mkdir(args[1],0777)==-1 ){
                          perror("+--- Error in mkdir ");
                    }
              }

    return 1;
 }




 /**
   @brief Launch a program and wait for it to terminate.
   @param args Null terminated list of arguments (including program).
   @return Always returns 1, to continue execution.
  */



int ash_launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("ash");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("ash");
  } else {
    // Parent process
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int ash_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < ash_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return ash_launch(args);
}

#define ash_RL_BUFSIZE 1024
/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *ash_read_line(void)
{
  int bufsize = ash_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "ash: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    if (c == EOF) {
      exit(EXIT_SUCCESS);
    } else if (c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += ash_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "ash: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

#define ash_TOK_BUFSIZE 64
#define ash_TOK_DELIM " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **ash_split_line(char *line)
{
  int bufsize = ash_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "ash: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, ash_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += ash_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        free(tokens_backup);
        fprintf(stderr, "ash: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, ash_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
void ash_loop(void)
{
  char *line;
  char **args;
  int status;

  char cwd[PATH_MAX];
  do {
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("%c[1m",27);
      printf("\033[1;33m");
      printf("%s", cwd);
      printf("\033[1;36m");
      printf(">> ");
      printf("\033[0m");
      printf("%c[0m",27);
      line = ash_read_line();
      args = ash_split_line(line);
      status = ash_execute(args);

      free(line);
      free(args);
    } else {
      perror("cannot get working dir");
      exit(EXIT_FAILURE);
    }
  } while (status);
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  // Load config files, if any.
  system("clear");

  char ch, filename[25] = ".ashrc";
  FILE *fp;

  fp = fopen(filename, "r");

  if (fp == NULL) {
    printf("Cannot find .ashrc :: using default settings\n");
  }
  // Run command loop.
  ash_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}
