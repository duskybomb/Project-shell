#include <stdlib.h>
#include <stdio.h>
#include <libconfig.h>
#include <string.h>

#include "dotfileconfig.h"

char prompt[15], color_location[15], color_prompt[15];

void prompt_ash() {
    printf("%s", prompt);
}

// char *color_location_ash() {
//     return &(color_location);
// }
// char *color_prompt_ash() {
//     return &(color_prompt);
// }

int read_config() {

    config_t cfg;
    config_setting_t *setting;
    const char *str;
    
    config_init(&cfg);

    /* Read the file. If there is an error, report it and exit. */
    if(! config_read_file(&cfg, ".ashrc")) {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return(EXIT_FAILURE);
    }

    if(config_lookup_string(&cfg, "prompt", &str)){
    	strcpy(prompt, str);
    }
    else
        fprintf(stderr, "No 'prompt' setting in configuration file.\n");

    // if(config_lookup_string(&cfg, "color-location", &str)) {
    //     strcpy(color_location, str);
    //     //printf("%s", str);
    // }
    // else
    //     fprintf(stderr, "No 'color-location' setting in configuration file.\n");

    // if(config_lookup_string(&cfg, "color-prompt", &str))
    //     strcpy(color_prompt, str);
    // else
    //     fprintf(stderr, "No 'color-prompt' setting in configuration file.\n");


    config_destroy(&cfg);
    return(EXIT_SUCCESS);
}