#include "builtin.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
// #include <zconf.h>

/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */


const char rocket[] =
"======================+\n\
           _          ||\n\
          /^\\         ||\n\
          |-|         ||  ASH (Ares SHell) - Unix Shell made in C\n\
          | |         ||\n\
          | |         ||  Last update: 12 November 2018\n\
          |A|         ||  Contributors:\n\
          |S|         ||    duskybomb (Harshit Joshi)\n\
          |H|         ||    Eklavya Chopra\n\
         /| |\\        ||    Gaurav\n\
        / | | \\       ||    Dhairya Kathapalia\n\
       |  | |  |      ||    Hardik Kapoor\n\
        `-\"\"\"-`       ||\n\
======================+\n\
";

int ash_cd(char **args);
int ash_help(char **args);
int ash_exit(char **args);
//int ash_mkdir(char **args);

char *builtin_str[] = {
        "cd",
        "help",
        "exit",
//        "mkdir"
};

int (*builtin_func[]) (char **) = {
        &ash_cd,
        &ash_help,
        &ash_exit,
//        &ash_mkdir
};

int ash_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int ash_cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "ash: expected argument to \"cd\"\n");
    } else {
      if(strcmp(args[1],"--help")==0){
          printf("cd: cd [-L|[-P [-e]] [-@]] [dir]\n\
                  Change the shell working directory.\n\
                  \n\
                  Change the current directory to DIR.  The default DIR is the value of the\n\
                  HOME shell variable.\n\
                  \n\
                  The variable CDPATH defines the search path for the directory containing\n\
                  DIR.  Alternative directory names in CDPATH are separated by a colon (:).\n\
                  A null directory name is the same as the current directory.  If DIR begins\n\
                  with a slash (/), then CDPATH is not used.\n\
                  \n\
                  If the directory is not found, and the shell option `cdable_vars' is set,\n\
                  the word is assumed to be  a variable name.  If that variable has a value,\n\
                  its value is used for DIR.\n\
                  \n\
                  Options:\n\
                    -L  force symbolic links to be followed: resolve symbolic\n\
                      links in DIR after processing instances of `..'\n\
                    -P  use the physical directory structure without following\n\
                      symbolic links: resolve symbolic links in DIR before\n\
                      processing instances of `..'\n\
                    -e  if the -P option is supplied, and the current working\n\
                      directory cannot be determined successfully, exit with\n\
                      a non-zero status\n\
                    -@  on systems that support it, present a file with extended\n\
                      attributes as a directory containing the file attributes\n\
                 \n\
                  The default is to follow symbolic links, as if `-L' were specified.\n\
                  `..' is processed by removing the immediately previous pathname component\n\
                  back to a slash or the beginning of DIR.\n\
                  \n\
                  Exit Status:\n\
                  Returns 0 if the directory is changed, and if $PWD is set successfully when\n\
                  -P is used; non-zero otherwise.\n");
        }
        else if (chdir(args[1]) != 0) {
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


//
//
//int ash_mkdir(char **args)
//{
//
//    if(args[1]==NULL){
//        fprintf(stderr, "ash: expected argument to \"mkdir\"\n");
//
//    }else{
//
//        if(mkdir(args[1],0777)==-1 ){
//            perror("+--- Error in mkdir ");
//        }
//    }
//
//    return 1;
//}