#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

int main() {

    char * str[2] = {"1c0111001f010100061a024b53535009181c", "686974207468652062756c6c277320657965"};
    int lenStr = strlen(str[0]);
    int idx;

    for (idx = 0 ; idx < lenStr ; idx+=2) {
        char buf[2][5] = {
            {'0', 'x', str[0][idx], str[0][idx+1], '\0'},
            {'0', 'x', str[1][idx], str[1][idx+1], '\0'}
        };

        printf("%x", (int)(strtol(buf[0], NULL, 0) ^ strtol(buf[1], NULL, 0)));
    }

    printf("\n");

    return 0;
}
