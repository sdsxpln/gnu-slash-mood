/*
  GNU/Mood: A mood indicator for GNU Social
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

#include "gnu-slash-mood.h"

/* default trigger words */

char * trigger_word_en[] = {
    "gnusocial", "mastodon", "stallman", "postactiv", "signal", "linux", "libreboot", "#nsfw",
    NULL
};

char ** trigger_lang = trigger_word_en;

/**
 * @brief Reads triggers from a custom file /var/lib/gnu-slash-mood/triggers.txt
 * @param str The text to be analysed
 * @return Zero or greater trigger word detections, -1 if the file doesn't exist
 */
int read_trigger_words(char * str)
{
    FILE * fp;
    char linestr[256], custom_trigger_word[256], word[256];
    int i, j, ctr=0, trigger=0;

    fp = fopen("/var/lib/gnu-slash-mood/triggers.txt", "r");
    if (!fp) return -1;

    while (!feof(fp)) {
        if (fgets(linestr , 254 , fp) != NULL ) {
            if (strlen(linestr) == 0) continue;

            /* extract the trigger word */
            j = 0;
            for (i = 0; i < strlen(linestr); i++)
                if ((linestr[i] != 10) && (linestr[i] != 13) &&
                    (linestr[i] != ' ') && (linestr[i] != ','))
                    custom_trigger_word[j++] = tolower(linestr[i]);

            /* string terminator */
            custom_trigger_word[j] = 0;

            /* is the trigger word within the text? */
            ctr = 0;
            for (i = 0; i < strlen(str); i++) {
                if ((terminating_char(str[i]) == 1) || (i == strlen(str)-1)) {
                    if (i == strlen(str)-1)
                        word[ctr++] = tolower(str[i]);
                    if (ctr > 0) {
                        /* string terminator */
                        word[ctr] = 0;
                        if (strlen(word) > 1) {
                            if (strlen(word) == strlen(custom_trigger_word)) {
                                if (strcmp(word, custom_trigger_word) == 0) {
                                    trigger++;
                                    break;
                                }
                            }
                        }
                    }
                    ctr = 0;
                }
                else {
                    if (ctr < 254)
                        word[ctr++] = tolower(str[i]);
                }
            }

        }
    }

    fclose(fp);
    return trigger;
}

/**
 * @brief Returns the number of trigger words found
 * @param str The text to be analysed
 * @return Number of matching trigger words
 */
int get_triggers(char * str)
{
    /* if custom trigger words exist then read them */
    int retval = read_trigger_words(str);
    if (retval != -1) return retval;

    return match_words(str, trigger_lang, NULL);
}
