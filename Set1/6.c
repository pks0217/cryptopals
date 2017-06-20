#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

int base64value(const char c);

int base64decode (const void * const data_buf, const size_t dataLength, 
        char * const result, const size_t resultSize, int * resultLen);

int calcDist(const char * const str1, const char * const str2, const int size);

int findKeySize(const char * const value, const int length);

void findKey(const char * const value, const int length, 
        const int keysize, char * const keyValue);

void decrypt(const char * const value, const int length, 
        const char * const key, const int keysize);


int main() {

    char line[64];
    char buf[4000];

    char val[4000];
    int lenVal;

    char key[1024];
    int keysize;

    int errLine;

    FILE * fp = fopen("6.txt", "r");

    if (NULL == fp) {
        perror("fopen");
        exit(1);
    }

    memset(buf, '\0', sizeof(buf));
    while (NULL != fgets(line, sizeof(line), fp)) {
        if(line[strlen(line)-1] == '\n')
            line[strlen(line)-1] = '\0';
        strcat(buf, line);
    }

    if (0 != fclose(fp)) {
        perror("fclose");
        exit(1);
    }


    if (0 != (errLine = base64decode(buf, strlen(buf),
                    val, sizeof(val), &lenVal))) {

        printf("Error !! Line (%d)\n", errLine);
        exit(1);
    }

    keysize = findKeySize(val, lenVal);

    printf("[KEYSIZE] %d\n", keysize);

    findKey(val, lenVal, keysize, key);

    printf("[KEY] %s\n\n", key);

    decrypt(val, lenVal, key, keysize);

    return 0;
}

int base64value(const char c) {

    if ('A' <= c && c <= 'Z')
        return c - 'A';
    if ('a' <= c && c <= 'z')
        return c - 'a' + 26;
    if ('0' <= c && c <= '9')
        return c - '0' + 52;

    if (c == '+') return 62;
    if (c == '/') return 63;
    if (c == '=') return 0;

    printf("Error !! Line (%d) : %c \n", __LINE__, c);

    return 0;
}

int base64decode (const void * const data_buf, const size_t dataLength, 
        char * const result, const size_t resultSize, int * const resultLen) {

    char c[4];
    int idxBuf, idxResult, idx;
    const char * const data = (const char* const)data_buf;

    for (idxBuf = idxResult = 0 ; idxBuf < dataLength ; idxBuf += 4) {
        for (idx = 0 ; idx < 4 && (idxBuf + idx) < dataLength ; idx++) {
            c[idx] = base64value(data[idxBuf + idx]);
        }

        if(idxResult > resultSize) return __LINE__;

        result[idxResult++] = (((c[0] & 0x3F) << 2) & 0xFC) 
            + (((c[1] & 0x3F) >> 4) & 0x03);

        if(idxResult > resultSize) return __LINE__;

        result[idxResult++] = (((c[1] & 0x3F) << 4) & 0xF0)
            + (((c[2] & 0x3F) >> 2) & 0x0F);

        if(idxResult > resultSize) return __LINE__;

        result[idxResult++] = (((c[2] & 0x3F) << 6) & 0xC0)
            + (c[3] & 0x3F);
    }

    result[idxResult] = '\0';
    while(result[idxResult] == '\0') idxResult--;

    *resultLen = idxResult;

    return 0;
}

int calcDist(const char * const str1, const char * const str2, const int size) {

    int idxStr, idxBit;
    int dist = 0;
    char c;

    for (idxStr=0 ; idxStr < size ; idxStr++) {
        c = *(str1 + idxStr) ^ *(str2 + idxStr);
        for (idxBit = 0 ; idxBit < 8 ; idxBit++) {
            if(((c >> idxBit) & 0x01) > 0)
                dist++;
        }
    }

    return dist;
}

int findKeySize(const char * const value, const int length) {

    int idxValue;
    int dist, minDist = INT_MAX;
    int keySize, minKeySize;

    for (keySize = 2 ; keySize <= 40 ; keySize++) {
        dist = 0;
        for (idxValue = 0 ; idxValue < (length - keySize) ; idxValue += keySize) {
            dist += calcDist((value + idxValue), (value + idxValue + keySize), keySize);
        }

        if(minDist > dist) {
            minDist = dist;
            minKeySize = keySize;
        }
    }

    //printf("keysize : %d , distance : %d\n", minKeySize, minDist);

    return minKeySize;
}

void findKey(const char * const value, const int length, 
        const int keysize, char * const keyValue) {

    char c;
    int key;
    int idxKey, block, cntAlpha, maxCntAlpha;


    for (idxKey = 0 ; idxKey < keysize ; idxKey++) {
        maxCntAlpha = 0;
        for (key = 0x00 ; key < 0xFF ; key++) {
            cntAlpha = 0;
            for ( block = 0 ; block < (length - keysize) ; block+=keysize ) {
                c = value[block + idxKey] ^ key;
                if (isalpha(c) || isspace(c))
                    cntAlpha++;
            }

            if (cntAlpha > maxCntAlpha) {
                maxCntAlpha = cntAlpha;
                keyValue[idxKey] = key;
            }
        }
    }

    keyValue[idxKey] = '\0';

    return;
}

void decrypt(const char * const value, const int length, 
        const char * const key, const int keysize) {

    int idx, idxKey;

    for (idx = idxKey = 0 ; idx < length ; idx++, idxKey++) {
        printf("%c", value[idx] ^ key[idxKey % keysize]);
    }

    return;
}
