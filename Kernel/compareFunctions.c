#include <stdint.h>
#include <compareFunctions.h>

int stringCompare(char *string, char *otherString) {
     int i = 0;
     while(string[i] != 0 && otherString[i] != 0) {
          if(string[i] - otherString[i] != 0) {
               return string[i] - otherString[i];
          }
          i++;
     }
     if(string[i] == 0 && otherString[i] == 0) {
          return EQUALS;
     }
     else {
          return string[i] - otherString[i];
     }
}

int uintCompare(uint64_t number, uint64_t otherNumber) {
     return number - otherNumber;
}