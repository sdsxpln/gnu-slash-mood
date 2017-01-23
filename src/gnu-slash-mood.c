/*
    Mood indicator
    Copyright (C) 2017 Bob Mottram
    bob@freedombone.net

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <time.h>
#include "gnu-slash-mood.h"

/* The maximum number of characters obtained from the timeline */
#define MAX_TEXT_LENGTH    (1024*4)

/* Check the timeline every few minutes */
#define CHECK_TIMELINE_SEC (60 * 5)

/* whether to show a test pattern */
unsigned char use_test_pattern = 0;

/* whether to enable debugging output */
unsigned char debug = 0;

/* login details */
char gnusocial_username[512];
char gnusocial_password[512];
char gnusocial_domain_name[512];

/* time to turn off and on again */
unsigned char hour_off = 0;
unsigned char hour_on = 7;

struct gss_account acc;

/**
 * @brief Show help
 */
void show_help()
{
    printf("\nGNU/Mood: The mood of the community\n\n");
    printf("Pinout: Red P8/13, Green P9/14, Blue P8/19\n\n");
    printf(" -h --help              Show help\n");
    printf(" -u --username [name]   Username\n");
    printf(" -d --domain [name]     Site domain name\n");
    printf(" -p --password [text]   Password\n\n");
    printf(" -t --test              Test mode\n");
    printf(" -c --config [filename] Read login details from file\n");
    printf("    --off [hour]        Hour when to turn off\n");
    printf("    --on [hour]         Hour when to turn on\n");
    printf("    --debug             Enable debugging output\n\n");
}

/**
 * @brief Returns 1 if the mood ball should be active
 */
int is_active()
{
    int state = 1;
    time_t t = time(NULL);
    struct tm *tmp = gmtime(&t);

    if (hour_off > hour_on) {
        if ((int)tmp->tm_hour > (int)hour_off)
            state = 0;
        if ((int)tmp->tm_hour < (int)hour_on)
            state = 0;
    }
    else {
        if (((int)tmp->tm_hour > (int)hour_off) &&
            ((int)tmp->tm_hour < (int)hour_on))
            state = 0;
    }

    return state;
}

/**
 * @brief Reads an individual line from the config file
 * @param linestr The line of text
 * @param Returned parameter value
 * @return Zero on success
 */
int read_login_param(char * linestr, char * paramstr)
{
    int i, j;

    for (i = 0; i < strlen(linestr); i++) {
        if ((linestr[i] == '=') || (linestr[i] == ':')) break;
    }

    if (i == strlen(linestr))
        return 1;

    /* skip any spaces or tabs */
    i++;
    while (((linestr[i] == ' ') || (linestr[i] == '\t')) &&
           (i < strlen(linestr)-1))
        i++;

    j = 0;
    while (i < strlen(linestr)) {
        if ((linestr[i] != 10) && (linestr[i] != 13))
            paramstr[j++] = linestr[i];
        i++;
    }
    paramstr[j] = 0;

    return 0;
}

int read_login_credentials(char * config_filename)
{
    FILE * fp;
    char linestr[512], paramstr[512];

    /* clear login credentials */
    gnusocial_username[0] = 0;
    gnusocial_password[0] = 0;
    gnusocial_domain_name[0] = 0;

    fp = fopen(config_filename, "r");
    if (!fp) return 1;

    while (!feof(fp)) {
        if (fgets(linestr , 511 , fp) != NULL ) {
            if (strlen(linestr) == 0) continue;
            if (linestr[0] == '#') continue;

            linestr[0] = tolower(linestr[0]);
            if ((strstr(linestr,"username =") != NULL) ||
                (strstr(linestr,"username :") != NULL) ||
                (strstr(linestr,"username:") != NULL) ||
                (strstr(linestr,"username=") != NULL)) {
                if (read_login_param(linestr, paramstr) != 0)
                    return 2;
                sprintf(gnusocial_username, "%s", paramstr);
            }

            if ((strstr(linestr,"password =") != NULL) ||
                (strstr(linestr,"password :") != NULL) ||
                (strstr(linestr,"password:") != NULL) ||
                (strstr(linestr,"password=") != NULL)) {
                if (read_login_param(linestr, paramstr) != 0)
                    return 3;
                sprintf(gnusocial_password, "%s", paramstr);
            }

            if ((strstr(linestr,"domain =") != NULL) ||
                (strstr(linestr,"domain :") != NULL) ||
                (strstr(linestr,"domain:") != NULL) ||
                (strstr(linestr,"domain=") != NULL)) {
                if (read_login_param(linestr, paramstr) != 0)
                    return 4;
                sprintf(gnusocial_domain_name, "%s", paramstr);
            }

            if ((strstr(linestr,"off =") != NULL) ||
                (strstr(linestr,"off :") != NULL) ||
                (strstr(linestr,"off:") != NULL) ||
                (strstr(linestr,"off=") != NULL)) {
                if (read_login_param(linestr, paramstr) != 0)
                    return 5;
                hour_off = (unsigned char)atoi(paramstr);
            }

            if ((strstr(linestr,"on =") != NULL) ||
                (strstr(linestr,"on :") != NULL) ||
                (strstr(linestr,"on:") != NULL) ||
                (strstr(linestr,"on=") != NULL)) {
                if (read_login_param(linestr, paramstr) != 0)
                    return 6;
                hour_on = (unsigned char)atoi(paramstr);
            }
        }
    }

    fclose(fp);

    if ((strlen(gnusocial_username) == 0) ||
        (strlen(gnusocial_password) == 0) ||
        (strlen(gnusocial_domain_name) == 0))
        return 7;

    return 0;
}

int set_leds(int r, int g, int b)
{
    if (pwm_output(P8, 13, r) != 0) return 1;
    if (pwm_output(P9, 14, g) != 0) return 2;
    if (pwm_output(P8, 19, b) != 0) return 3;
    return 0;
}

int get_timeline_text(char * text)
{
    struct status * posts;
    const int no_of_posts = 5;
    int i, j, ctr=0;

    if (verify_account(acc) == -1) return 1;
    posts = read_timeline(acc, HOME_TIMELINE, no_of_posts);
    for (i = 0; i < no_of_posts; i++) {
        for (j = 0; j < strlen(posts[i].text); j++) {
            if (ctr >= MAX_TEXT_LENGTH-1) {
                text[ctr]=0;
                return 0;
            }
            text[ctr++] = posts[i].text[j];
        }
        text[ctr++] = '.';
    }
    text[ctr]=0;
    return 0;
}

int mood_loop()
{
    int curr_r=0, curr_g=0, curr_b=0;
    int target_r, target_g, target_b, frequency = 0;
    int ctr_msec=0, phase = 0, intensity = 100, state = 0;
    char text[MAX_TEXT_LENGTH];

    if (get_timeline_text(text) != 0) {
        printf("Failed to grab text from the timeline\n");
        return 1;
    }

    printf("GNU Social login success\n");

    if (use_test_pattern == 0) {
        sentiment_to_rgb(text, &curr_r, &curr_g, &curr_b, &frequency);
        target_r = curr_r;
        target_g = curr_g;
        target_b = curr_b;
    }
    else
        printf("Running test pattern\n");

    if (set_leds(curr_r, curr_g, curr_b) != 0) {
        printf("Unable to set LED pwm values\n");
        return 2;
    }

    while(1) {
        if (is_active() == 0) {
            /* fade out */
            target_r = 0;
            target_g = 0;
            target_b = 0;
        }

        /* shift the colour towards the target */
        if (target_r > curr_r) curr_r++;
        if (target_g > curr_g) curr_g++;
        if (target_b > curr_b) curr_b++;

        if (target_r < curr_r) curr_r--;
        if (target_g < curr_g) curr_g--;
        if (target_b < curr_b) curr_b--;

        if (use_test_pattern != 0) {
            if (curr_r < 1000)
                curr_r += 100;
            else
                if (curr_b < 1000)
                    curr_b += 100;
                else
                    if (curr_g < 1000)
                        curr_g += 100;
                    else {
                        curr_r = 0;
                        curr_b = 0;
                        curr_g = 0;
                    }
        }

        if (debug != 0)
            printf("Active: %d  Colour: %d %d %d  Intensity: %d  Frequency: %d\n",
                   is_active(), curr_r, curr_g, curr_b, intensity, frequency);

        if (set_leds(curr_r*intensity/100,
                     curr_g*intensity/100,
                     curr_b*intensity/100) != 0) {
            printf("Unable to set LED pwm values\n");
        }

        if (ctr_msec >= CHECK_TIMELINE_SEC*1000) {
            ctr_msec = 0;

            if (is_active() == 1) {
                /* update the timeline */
                if (get_timeline_text(text) != 0) {
                    printf("Failed to grab text from the timeline\n");
                }
                else {
                    /* calculate the new target colour */
                    sentiment_to_rgb(text, &target_r, &target_g, &target_b, &frequency);
                    if (debug != 0)
                        printf(">>>> Updated from timeline <<<<\n");
                }
            }
        }

        /* wait */
        usleep(1000*100);
        fflush(stdout);

        ctr_msec += 100;
        if (frequency != 0) {
            if (state == 0) {
                intensity -= 70/(frequency/2);
                if (phase > frequency/2) {
                    state = 1;
                }
            }
            else {
                intensity += 70/(frequency/2);
                if (phase > frequency) {
                    state = 0;
                    phase = 0;
                    intensity = 100;
                }
            }
            if (intensity > 100) intensity = 100;
            if (intensity < 30) intensity = 30;
            phase++;
        }
        else {
            intensity = 100;
            phase = 0;
            state = 0;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int i;
    char username[1024], domain_name[1024], password[1024];

    /* clear login values */
    username[0] = 0;
    domain_name[0] = 0;
    password[0] = 0;

    /* parse the arguments */
    for (i = 1; i < argc; i++) {
        if (i < argc-1) {
            if ((strcmp(argv[i], "-u") == 0) ||
                (strcmp(argv[i], "--user") == 0) ||
                (strcmp(argv[i], "--username") == 0)) {
                sprintf(username, "%s", argv[i+1]);
                i++;
                continue;
            }

            if ((strcmp(argv[i], "-d") == 0) ||
                (strcmp(argv[i], "--domain") == 0) ||
                (strcmp(argv[i], "--domainname") == 0)) {
                sprintf(domain_name, "%s", argv[i+1]);
                i++;
                continue;
            }

            if ((strcmp(argv[i], "-p") == 0) ||
                (strcmp(argv[i], "--password") == 0) ||
                (strcmp(argv[i], "--passphrase") == 0)) {
                sprintf(password, "%s", argv[i+1]);
                i++;
                continue;
            }

            if ((strcmp(argv[i], "-c") == 0) ||
                (strcmp(argv[i], "--config") == 0)) {
                if (read_login_credentials(argv[i+1]) == 0) {
                    sprintf(username, "%s", gnusocial_username);
                    sprintf(password, "%s", gnusocial_password);
                    sprintf(domain_name, "%s", gnusocial_domain_name);
                }
                else {
                    printf("Invalid configuration file\n");
                    return 4;
                }
                i++;
                continue;
            }

            if (strcmp(argv[i], "--off") == 0) {
                hour_off = (unsigned char)atoi(argv[i+1]);
                i++;
                continue;
            }

            if (strcmp(argv[i], "--on") == 0) {
                hour_on = (unsigned char)atoi(argv[i+1]);
                i++;
                continue;
            }
        }

        if ((strcmp(argv[i], "-h") == 0) ||
            (strcmp(argv[i], "--help") == 0)) {
            show_help();
            return 0;
        }

        if ((strcmp(argv[i], "-t") == 0) ||
            (strcmp(argv[i], "--test") == 0))
            use_test_pattern = 1;

        if (strcmp(argv[i], "--debug") == 0)
            debug = 1;
    }

    if (argc <= 1) {
        show_help();
        return 0;
    }

    if (strlen(username) == 0) {
        printf("Please specify a GNU Social username\n");
        return 1;
    }
    if (strlen(domain_name) == 0) {
        printf("Please specify a GNU Social domain name\n");
        return 2;
    }
    if (strlen(password) == 0) {
        printf("Please specify a GNU Social password\n");
        return 3;
    }

    init_account(&acc, "https", (char*)username, (char*)domain_name, (char*)password);
    mood_loop();

    return 0;
}
