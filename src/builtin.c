//
// Created by duskybomb on 11/11/18.
//

#include "builtin.h"
#include <stdio.h>
#include <zconf.h>

/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */


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
