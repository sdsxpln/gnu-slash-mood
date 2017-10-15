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

#ifndef GNU_SLASH_MOOD_H
#define GNU_SLASH_MOOD_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bbb.h"
#include "gnusocial.h"

enum {
    GENDER_UNKNOWN,
    GENDER_FEMININE,
    GENDER_MASCULINE
};

void set_language(char * lang);
int terminating_char(char c);
int match_words(char * str, char * words_array[], int * word_values);
int get_valence(char * str);
int get_egocentrism(char * str);
int get_gender(char * str);
int get_triggers(char * str);
void sentiment_to_rgb(char * str, int * r, int * g, int * b, int * frequency);
int get_gender_from_name(char * ch);

#endif
