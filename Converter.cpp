#include <string.h>
#include <cstdlib>
#include <stdlib.h>
bool TryConvertToInteger(const char* inputString, int* convertedValue) {
    char* endptr;
    *convertedValue = strtol(inputString, &endptr, 10);
    if (endptr == inputString) {
        return false;
    }
    return true;
}

bool TryConvertToDouble(const char* inputString, double* convertedValue) {
    char* endpointer;
    *convertedValue = strtod(inputString, &endptr);
    if (endpointer == inputString) {
        return 0;
    }
    return 1;
}
