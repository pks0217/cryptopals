#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

int base64encode (const void * data_buf, size_t dataLength, char * result, size_t resultSize) {
   const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   const uint8_t *data = (const uint8_t *)data_buf;
   size_t resultIndex = 0;
   size_t x;
   uint32_t n = 0;
   int padCount = dataLength % 3;
   uint8_t n0, n1, n2, n3;

   for (x = 0; x < dataLength; x += 3) 
   {
      n = ((uint32_t)data[x]) << 16;
      
      if ((x+1) < dataLength)
         n += ((uint32_t)data[x+1]) << 8;
      
      if ((x+2) < dataLength)
         n += data[x+2];

      n0 = (uint8_t)(n >> 18) & 63;
      n1 = (uint8_t)(n >> 12) & 63;
      n2 = (uint8_t)(n >> 6) & 63;
      n3 = (uint8_t)n & 63;
            
      if (resultIndex >= resultSize)
          return __LINE__;
      
      result[resultIndex++] = base64chars[n0];
      
      if (resultIndex >= resultSize)
          return __LINE__;
      
      result[resultIndex++] = base64chars[n1];

      if ((x+1) < dataLength)
      {
         if (resultIndex >= resultSize) 
            return __LINE__;
         
         result[resultIndex++] = base64chars[n2];
      }

      if ((x+2) < dataLength)
      {
         if (resultIndex >= resultSize)
             return __LINE__;
         
         result[resultIndex++] = base64chars[n3];
      }
   }  

   if (padCount > 0) 
   { 
      for ( ; padCount < 3 ; padCount++) 
      { 
         if (resultIndex >= resultSize)
             return __LINE__;
         
         result[resultIndex++] = '=';
      } 
   }

   if (resultIndex >= resultSize)
       return __LINE__;
   
   result[resultIndex] = 0;

   return 0;
}

int main() {

    char * str = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    char * value;
    char * result;
    
    int lenStr = strlen(str);
    int lenValue = lenStr / 2;
    int lenResult = lenValue / 3 * 4 + (lenValue % 3) + 1;
    
    int idx, errLine;

    if (NULL == (value = (char*)malloc(lenValue))) {
        perror("malloc (value)");
        exit(1);
    }
    
    if (NULL == (result = (char*)malloc(lenResult))) {
        free(value);
        perror("malloc (result)");
        exit(1);
    }

    for (idx = 0 ; idx < strlen(str) ; idx+=2) {
        char buf[5] = {'0', 'x', str[idx], str[idx+1], '\0'};
        *(value + (idx/2)) = strtol(buf, NULL, 0);
    }

    if (0 == (errLine = base64encode(value, lenValue, result, lenResult))) {
        printf("%s\n", result);
    } else {
        printf("Error!! Line (%d)\n", errLine);
    }

    free(value);
    free(result);

    return 0;
}
