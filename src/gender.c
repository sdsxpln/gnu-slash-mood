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

char * gender_word_en[] = {
    "she", "her", "woman", "female", "feminine",
    "fem", "mother", "sister", "daughter", "girlfriend",
    "wife", "aunt", "mrs", "miss", "girl",
    "girls", "ms", "women", "mothers", "sisters",
    "women's", "womens", "mother's", "sister's", "lady",
    "ladies", "lady's",
    "he", "his", "him", "man", "male",
    "masculine", "butch", "macho", "father", "brother",
    "husband", "uncle", "son", "mr", "boy",
    "boys", "men", "husbands", "fathers", "brothers",
    "mens", "men's", "husband's", "father's", "brother's",
    "sir",
    NULL
};

static int gender_value[] = {
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1,
    -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1,
    -1
};

char ** gender_lang = gender_word_en;

/**
 * @brief Returns a value indicating the masculine/feminine content of the given text
 * @param str The text to be analysed
 * @return Value indicating masuline/feminine, with feminine in the positive direction
 */
int get_gender(char * str)
{
    int i, j, ctr=0, gender=0, name_gender;
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
                    while (gender_lang[j] != NULL) {
                        if (strlen(word) == strlen(gender_lang[j])) {
                            if (strcmp(word, gender_lang[j]) == 0) {
                                gender += gender_value[j];
                                break;
                            }
                            else {
                                if (strlen(word) > 2) {
                                    name_gender = get_gender_from_name(word);
                                    if (name_gender == GENDER_FEMININE) gender+=2;
                                    if (name_gender == GENDER_MASCULINE) gender-=2;
                                }
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

    return gender;
}
