#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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


bool TryConvertToInteger(const char* inputString, int* convertedValue) {
    char* endptr;
    *convertedValue = strtol(inputString, &endptr, 10);
    if (endptr == inputString) {
        return false;
    }
    return true;
}

int TryConvertToDouble(const char* inputString, double* convertedValue) {
    char* endpointer;
    *convertedValue = strtod(inputString, &endptr);
    if (endpointer == inputString) {
        return 0;
    }
    return 1;
}

void CovertTradeRecordFromCSVToXML(FILE* stream) {
    char line[1024];
    TradeRecord record[1024];
    int lineCount = 0;
    int objectCount = 0;

    while (fgets(line, sizeof(line), stream)) {
        char* fields[3];
        int fieldCount = 0;
        char* token = strtok(line, ",");
        while (token != NULL) {
            fields[fieldCount++] = token;
            token = strtok(NULL, ",");
        }

        if (fieldCount != 3) {
            fprintf(stderr, "WARN: Line %d malformed. Only %d field(s) found.\n", lineCount + 1, fieldCount);
            continue;
        }

        if (strlen(fields[0]) != 6) {
            fprintf(stderr, "WARN: Trade currencies on line %d malformed: '%s'\n", lineCount + 1, fields[0]);
            continue;
        }

        int tradeAmount;
        if (!GetFromString(fields[1], &tradeAmount)) {
            fprintf(stderr, "WARN: Trade amount on line %d not a valid integer: '%s'\n", lineCount + 1, fields[1]);
        }

        double tradePrice;
        if (!ConvertToDouble(fields[2], &tradePrice)) {
            fprintf(stderr, "WARN: Trade price on line %d not a valid decimal: '%s'\n", lineCount + 1, fields[2]);
        }

        strncpy(objects[objectCount].SourceCurrency, fields[0], 3);
        strncpy(objects[objectCount].DestinationCurrency, fields[0] + 3, 3);
        objects[objectCount].Lots = tradeAmount / LotSize;
        objects[objectCount].Price = tradePrice;
        objectCount++;
        lineCount++;
    }

    FILE* outFile = fopen("output.xml", "w");
    fprintf(outFile, "<TradeRecords>\n");
    for (int index = 0; index < objectCount; index++) {
        fprintf(outFile, "\t<TradeRecord>\n");
        fprintf(outFile, "\t\t<SourceCurrency>%s</SourceCurrency>\n", objects[index].SourceCurrency);
        fprintf(outFile, "\t\t<DestinationCurrency>%s</DestinationCurrency>\n", objects[index].DestinationCurrency);
        fprintf(outFile, "\t\t<Lots>%d</Lots>\n", objects[index].Lots);
        fprintf(outFile, "\t\t<Price>%f</Price>\n", objects[index].Price);
        fprintf(outFile, "\t</TradeRecord>\n");
    }
    fprintf(outFile, "</TradeRecords>");
    fclose(outFile);
    printf("INFO: %d trades processed\n", objectCount);
}

