/***************************************************************************//**
  @file         main.c
  @author       Harshit Joshi & Eklavya Chopra & Gaurav
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
#include <time.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <dirent.h>
#define ash_RL_BUFSIZE 1024
#define ash_TOK_BUFSIZE 64
#define ash_TOK_DELIM " \t\r\n\a"

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
// int ash_mkdir(char **args);
/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "list ls"
  "copy cp"
};

int (*builtin_func[]) (char **) = {
  &ash_cd,
  &ash_help,
  &ash_exit
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




 // int ash_mkdir(char **args)
 // {

 //            if(args[1]==NULL){
 //              fprintf(stderr, "ash: expected argument to \"mkdir\"\n");

 //            }else{

 //                    if(mkdir(args[1],0777)==-1 ){
 //                          perror("+--- Error in mkdir ");
 //                    }
 //              }

 //    return 1;
 // }

/* list cwd contents*/
void ash_ls()
{
    int i=0;
    struct dirent **listr;
    int listn = scandir(".", &listr, 0, alphasort);
    if (listn >= 0)
    {
        printf("+--- Total objects in this directory\n");
        printf("%d",listn-2),"\n";
        for(i = 0; i < listn; i++ )
        {
            if(strcmp(listr[i]->d_name,".")==0 || strcmp(listr[i]->d_name,"..")==0)
            {
                continue;
            }
            else {
              printf(listr[i]->d_name,"    \t");
              printf("\t");
          }
            if(i%8==0) printf("\n");
        }
        printf("\n");
    }
    else
    {
        perror ("+--- Error in ls ");
    }

}

/**
   @brief Builtin command: copy a file
   @param args List of args.  args[0] is "cp".  args[1] is the file to be copied and args [2] is the coopied name of file.
   @return Always returns 1, to continue executing.
 */
void ash_copy(char* file1, char* file2)
{
    FILE *f1,*f2;
    struct stat t1,t2;
    f1 = fopen(file1,"r");
    if(f1 == NULL)
    {
        perror("+--- Error in cp file1 ");
        return;
    }
    f2 = fopen(file2,"r");// if file exists
    if(f2)
    {
        // file2 exists
        // file1 must be more recently updated
        stat(file1, &t1);
        stat(file2, &t2);
        if(difftime(t1.st_mtime,t2.st_mtime) < 0)
        {
            printf("+--- Error in cp : %s is more recently updated than %s\n",file2,file1);
            fclose(f1);
            fclose(f2);
            return;
        }
    }
    f2 = fopen(file2,"ab+"); // create the file if it doesn't exist
    fclose(f2);

    f2 = fopen(file2,"w+");
    if(f2 == NULL)
    {
        perror("Error in cp file2 ");
        fclose(f1);
        return;
    }
    //if(access(file2,W_OK)!=0 || access(file1,R_OK)!=0 || access(file2,F_OK)!=0)
    if(open(file2,O_WRONLY)<0 || open(file1,O_RDONLY)<0)
    {
        perror("Error in cp access ");
        return;
    }
    char cp;
    while((cp=getc(f1))!=EOF)
    {
        putc(cp,f2);
    }

    fclose(f1);
    fclose(f2);
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
  if(strcmp(args[0],"cp")==0)
    {
        char* file1 = args[1];
        char* file2 = args[2];
      if(strlen(file1) > 0 && strlen(file2) > 0)
        {
            ash_copy(file1,file2);
        }
        else
        {
            printf("+--- Error in cp : insufficient parameters\n");
        }
      }
    else if(strcmp(args[0],"ls")==0)
        {
            char* optional = args[1];
            ash_ls();
        }
  else{
  for (i = 0; i < ash_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
 }
  return ash_launch(args);
}

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
      printf("\033[1;32m");
      printf("%s", cwd);
      printf("\033[1;31m");
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