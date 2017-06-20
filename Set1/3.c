#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>

int main() {

    char * str = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    int lenStr = strlen(str);

    char * value;
    int lenValue = lenStr / 2;

    int idx, x, idxMaxScore = -1;

    int score;
    int maxScore = 0;

    if(NULL == (value = (char*) malloc(lenValue))) {
        perror("malloc (value)");
        exit(1);
    }

    for (idx = 0 ; idx < lenStr ; idx+=2) {
        char buf[5] = {'0', 'x', str[idx], str[idx+1], '\0'};
        *(value + idx / 2) = (char)strtol(buf, NULL, 0);
    }

    for (idx = 0x00 ; idx <= 0xFF ; idx++) {
        score = 0;
        for (x = 0 ; x < lenValue ; x++) {
            char c = *(value + x) ^ idx;
            if(isalpha(c) || isspace(c)) {
                score += 1;
            }
        }

        if (score > maxScore) {
            maxScore = score;
            idxMaxScore = idx;
        }
    }

    if (idxMaxScore > 0) {
        for (x = 0 ; x < lenValue ; x++) {
            printf("%c", *(value + x) ^ idxMaxScore);
        }
        printf("\n");
    }

    free(value);

    return 0;
}
