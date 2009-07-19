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

unsigned hamdist()
{
    unsigned distance = 0;
    unsigned dist;
    unsigned val;
    for(int i = 0; i < 5 ; i++)
    {
        dist = 0;
        val = phrase_msg[i] ^ sha.Message_Digest[i];

        // Count the number of set bits (Knuth's algorithm)
        while(val)
        {
            ++dist;
            val &= val - 1;
        }

        distance += dist;
    }
    return distance;
}

void calculate(char *phrase)
{
    count ++;
    if(count > 1000000) {
        printf("Done\n");
        exit(0);
    }

    SHA1Reset(&sha);
    SHA1Input(&sha, (const unsigned char *) phrase, strlen(phrase));

    if (!SHA1Result(&sha))
    {
        fprintf(stderr, "ERROR-- could not compute message digest\n");
        return;
    }
    else
    {
        int distance = hamdist();

        if(distance < best_distance) {
            printf("%d - %s\n", distance, phrase);
            best_distance = distance;
        }
    }
}

void calculate_with_letters(char *phrase, int letter_length)
{
    calculate(phrase);

//printf("%s\n", phrase);


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
        if(letter_length < 5) {
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

