#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>

void encString (const char * const str, const int lenStr, 
        const char * const key, const int lenKey, 
        char * const buf, const size_t sizeBuf);

void decString (const char * const str, const int lenStr, 
        const char * const key, const int lenKey);


int main() {

    char * key = "ICE";
    char * str = "Burning 'em, if you ain't quick and nimble\n"
        "I go crazy when I hear a cymbal";
    char buffer [2048];

    encString(str, strlen(str), key, strlen(key), buffer, sizeof(buffer));
    decString(buffer, strlen(buffer), key, strlen(key));

    return 0;
}

void encString (const char * const str, const int lenStr, 
        const char * const key, const int lenKey, 
        char * const buf, const size_t sizeBuf) {

    int idx, idxKey, idxBuf;
    char xor;

    for (idx = idxKey = idxBuf = 0 ; idx < lenStr ; idx++, idxKey++, idxBuf+=2) {
        xor = *(str + idx) ^ *(key + (idxKey % lenKey));
        printf("%02x", xor);
        if (idxBuf < sizeBuf) {
            sprintf(buf + idxBuf, "%02x", xor);
        }
    }

    *(buf+idxBuf) = '\0';
    printf("\n");
}

void decString (const char * const str, const int lenStr, 
        const char * const key, const int lenKey) {

    int idx, idxKey;
    
    for (idx = idxKey =  0 ; idx < lenStr ; idx+=2, idxKey++) {
        char buf[5] = {'0', 'x', str[idx], str[idx+1], '\0'};
        printf("%c", (char)strtol(buf, NULL, 0) ^ *(key + (idxKey % lenKey)));
    }

    printf("\n");
}
