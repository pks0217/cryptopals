#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>

char maxValue[31];
int maxScore = 0;

void findString (const char * const, const int);

int main() {

    char str[256];
    int lenStr;

    FILE * fp = fopen("4.txt", "r");

    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    while(NULL != fgets(str, sizeof(str), fp)) {
        str[strlen(str)-1] = '\0';
        lenStr = strlen(str);
        findString(str, lenStr);
    }

    printf("%s\n", maxValue);

    if (0 != fclose(fp)) {
        perror("fclose");
    }

    return 0;
}

void findString (const char * const str, const int lenStr) {

    char value[lenStr / 2];
    int lenValue = lenStr / 2;

    int idx, x;

    int  score;

    for (idx = 0 ; idx < lenStr ; idx+=2) {
        char buf[5] = {'0', 'x', str[idx], str[idx+1], '\0'};
        value[idx / 2] = (char)strtol(buf, NULL, 0);
    }

    for (idx = 0x00 ; idx <= 0xFF ; idx++) {
        score = 0;
        for (x = 0 ; x < lenValue ; x++) {
            char c = value[x] ^ idx;
            if(isalpha(c) || isspace(c)) {
                score += 1;
            }
        }

        if (score > maxScore) {
            maxScore = score;
            memset(maxValue, '\0', sizeof(maxValue));
            for (x = 0 ; x < lenValue ; x++) {
                maxValue[x] = value[x] ^ idx;
            }
        }
    }
}
