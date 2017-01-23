/*
    Mood indicator tests
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

#include <stdio.h>
#include <bbb.h>
#include <gnusocial.h>
#include <assert.h>
#include "../src/gnu-slash-mood.h"

void test_egocentrism()
{
    char * text_me = "I did something with my computer";
    char * text_they = "They did something with their computers";

    set_language("en");
    assert(get_egocentrism(text_me) > 0);
    assert(get_egocentrism(text_they) < 0);
}

void test_valence()
{
    char * text_positive = "I love the smell of roses";
    char * text_negative = "I fucking hate Nazis";

    set_language("en");
    assert(get_valence(text_positive) > 0);
    assert(get_valence(text_negative) < 0);
}

void test_gender()
{
    char * text_feminine = "She wrote a famous book";
    char * text_masculine = "His father fought the fascists";

    set_language("en");
    assert(get_gender(text_feminine) > 0);
    assert(get_gender(text_masculine) < 0);
}

void test_gender_name()
{
    char * text_feminine = "Anne read a book";
    char * text_masculine = "Bob made the tea";

    set_language("en");
    assert(get_gender(text_feminine) > 0);
    assert(get_gender(text_masculine) < 0);
}

void test_rgb()
{
    int r, g, b, frequency;
    char * text = "The subjective sensory stimuli as a source of dreams have the obvious advantage that, unlike objective stimuli, they are independent of external accidents. They are, so to speak, at the disposal of the interpretation whenever they are required. But they are inferior to the objective sensory stimuli by the fact that their claim to the role of dream-inciters - which observation and experiment have established in the case of objective stimuli - can in their case be verified with difficulty or not at all. The main proof of the dream-inciting power of subjective sensory stimuli is afforded by the so-called hypnogogic hallucinations, which have been described by Johann Muller as 'phantastic visual manifestations.' They are those very vivid and changeable pictures which with many people occur constantly during the period of falling asleep, and which may linger for a while even after the eyes have been opened.";

    sentiment_to_rgb(text, &r, &g, &b, &frequency);

    assert(r >= 0);
    assert(g >= 0);
    assert(b >= 0);

    assert(r <= 1000);
    assert(g <= 1000);
    assert(b <= 1000);

    assert(r+g+b > 0);
}

int main()
{
    test_egocentrism();
    test_valence();
    test_gender();
    test_gender_name();
    test_rgb();
    printf("All tests passed\n");

    return 0;
}
