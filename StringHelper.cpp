#include <string.h>
#include <cstdlib>
#include <stdlib.h>

char** SplitString(const char* inputString, char delimiter) {
    int delimiterCount = 0;
    const char* ptr = inputString;
    while (*ptr != '\0') {
        if (*ptr++ == delimiter) {
            delimiterCount++;
        }
    }

    char** tokens = (char**)malloc(sizeof(char*) * (delimiterCount + 2));
    int row = 0;
    ptr = inputString;
    char* token = (char*)malloc(strlen(inputString) + 1);
    int column = 0;
    while (*ptr != '\0') {
        if (*ptr == delimiter) {
            token[column] = '\0';
            tokens[row++] = strdup(token);
            column = 0;
        } else {
            token[column++] = *ptr;
        }
        ptr++;
    }
    token[column] = '\0';
    tokens[row++] = strdup(token);
    tokens[row] = NULL;
    free(token);
    return tokens;
}

int GetFromString(const char* inputString, int* value) {
    char* endptr;
    *value = strtol(inputString, &endptr, 10);
    if (endptr == inputString) {
        return 0;
    }
    return 1;
}
