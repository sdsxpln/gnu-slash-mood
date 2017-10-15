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

/**
 * @brief Detects whether the given character is a terminator
 * @param c The character
 * @return Zero if this is not a terminator, 1 otherwise.
 */
int terminating_char(char c)
{
    if ((c == ' ') || (c == ',') ||
        (c == '.') || (c == ';'))
        return 1;
    return 0;
}

/**
 * @brief Matches the given string against an array of words
 * @param str The text to be matched with
 * @param words_array Array containing words to be detected within the text
 * @param word_values Values associated with each matched word
 * @return Number of matched words
 */
int match_words(char * str, char * words_array[], int * word_values)
{
    int i, j, ctr=0, matches=0;
    char word[256];

    for (i = 0; i < strlen(str); i++) {
        if ((terminating_char(str[i]) == 1) || (i == strlen(str)-1)) {

            /* final word */
            if ((i == strlen(str)-1) && (terminating_char(str[i]) != 1))
                word[ctr++] = tolower(str[i]);

            if (ctr > 0) {
                word[ctr] = 0;
                if (strlen(word) > 1) {
                    j = 0;
                    while (words_array[j] != NULL) {
                        if (strlen(word) == strlen(words_array[j])) {
                            if (strcmp(word, words_array[j]) == 0) {
                                if (word_values != NULL)
                                    matches += word_values[j];
                                else
                                    matches++;
                                break;
                            }
                        }
                        j++;
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

    return matches;
}
