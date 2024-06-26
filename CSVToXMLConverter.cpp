#include "CSVToXMLConverter.h"

void CovertTradeRecordFromCSVToXML(FILE* stream) {
    char line[1024];
    TradeRecord record[1024];
    int lineCount = 0;
    int objectCount = 0;
    int LotSize = 3;

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
        if (!TryConvertToDouble(fields[2], &tradePrice)) {
            fprintf(stderr, "WARN: Trade price on line %d not a valid decimal: '%s'\n", lineCount + 1, fields[2]);
        }

        strncpy(record[objectCount].SourceCurrency, fields[0], 3);
        strncpy(record[objectCount].DestinationCurrency, fields[0] + 3, 3);
        record[objectCount].Lots = tradeAmount / LotSize;
        record[objectCount].Price = tradePrice;
        objectCount++;
        lineCount++;
    }

    FILE* outFile = fopen("output.xml", "w");
    fprintf(outFile, "<TradeRecords>\n");
    for (int index = 0; index < objectCount; index++) {
        fprintf(outFile, "\t<TradeRecord>\n");
        fprintf(outFile, "\t\t<SourceCurrency>%s</SourceCurrency>\n", record[index].SourceCurrency);
        fprintf(outFile, "\t\t<DestinationCurrency>%s</DestinationCurrency>\n", record[index].DestinationCurrency);
        fprintf(outFile, "\t\t<Lots>%d</Lots>\n", record[index].Lots);
        fprintf(outFile, "\t\t<Price>%f</Price>\n", record[index].Price);
        fprintf(outFile, "\t</TradeRecord>\n");
    }
    fprintf(outFile, "</TradeRecords>");
    fclose(outFile);
    printf("INFO: %d trades processed\n", objectCount);
}

