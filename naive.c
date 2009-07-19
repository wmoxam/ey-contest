#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sha1.h"

int WORDCHARS_LENGTH = 62;
char WORDCHARS[] = {"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
char TEST_PHRASE[] = {"Cloud Ruby DHH one eight six active record controller data rspec mongrel\0"};
char PHRASE[] = {"I am not a big believer in fortune telling\0"};

int count = 0;
int phrase_msg[5];

int best_distance = 1000;

SHA1Context sha;

unsigned hamdist(unsigned x, unsigned y)
{
    unsigned dist = 0, val = x ^ y;

    // Count the number of set bits (Knuth's algorithm)
    while(val)
    {
        ++dist; 
        val &= val - 1;
    }

    return dist;
}

void calculate(char *phrase)
{
    SHA1Reset(&sha);
    SHA1Input(&sha, (const unsigned char *) phrase, strlen(phrase));

    if (!SHA1Result(&sha))
    {
        fprintf(stderr, "ERROR-- could not compute message digest\n");
        return;
    }
    else
    {
        int distance = 0;
        for(int i = 0; i < 5 ; i++)
        {
            distance += hamdist(phrase_msg[i], sha.Message_Digest[i]);
        }

        if(distance < best_distance) {
            printf("%d - %s\n", distance, phrase);
            best_distance = distance;
        }
    }
}

void calculate_with_letters(char *phrase, int letter_length)
{
    calculate(phrase);

    count ++;
    if(count > 100000) {
        printf("Done\n");
        exit(0);
    }

    int plength = strlen(phrase);
    int blength;

    if(letter_length == 0) {
        blength = plength + 3;
    } else {
        blength = plength + 2;
    }

    char buffer[blength];
    if(letter_length == 0) {
        sprintf(buffer, "%s ", phrase);
    } else {
        sprintf(buffer, "%s", phrase);
    }

    buffer[blength - 1] = '\0';

    for(int i = 0; i < WORDCHARS_LENGTH; i++) {
        buffer[blength - 2] = WORDCHARS[i];

        calculate(buffer);
        if(letter_length < 4) {
            calculate_with_letters(buffer, letter_length + 1);
        }
    }
}

int main(int argc, char **argv)
{
    SHA1Reset(&sha);
    SHA1Input(&sha, (const unsigned char *) PHRASE, strlen(PHRASE));

    if (!SHA1Result(&sha))
    {
        fprintf(stderr, "ERROR-- could not compute message digest\n");
    }
    else
    {
        for(int i = 0; i < 5 ; i++)
        {
            phrase_msg[i] = sha.Message_Digest[i];
        }
    }

    calculate_with_letters(TEST_PHRASE, 0);
}

