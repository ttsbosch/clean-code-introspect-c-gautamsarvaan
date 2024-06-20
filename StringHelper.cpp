#include <string.h>
char** SplitString(const char* inputString, char delimiter) {
    int delimiterCount = 0;
    const char* ptr = inputString;
    while (*ptr != '\0') {
        if (*ptr++ == delimiterCount) {
            delimiterCount++;
        }
    }

    char** tokens = (char**)malloc(sizeof(char*) * (count + 2));
    int row = 0;
    ptr = str;
    char* token = (char*)malloc(strlen(str) + 1);
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

int GetFromString(const char* str, int* value) {
    char* endptr;
    *value = strtol(str, &endptr, 10);
    if (endptr == str) {
        return 0;
    }
    return 1;
}
