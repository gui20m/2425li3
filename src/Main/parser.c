#include "Main/parser.h"
#include "Main/utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void* parserQuery(SPOTILIXO dataset, char* inputLine) {
    int index = 0;
    char** parameters = malloc(sizeof(char*) * MAX_ARGS);
    if (!parameters) return NULL;

    char* lineCopy = strdup(inputLine);
    if (!lineCopy) {
        free(parameters);
        return NULL;
    }

    char* current = lineCopy;
    while (index < MAX_ARGS) {
        while (*current == ' ') current++;

        if (*current == '"') {
            current++;
            char* closingQuote = strchr(current, '"');
            if (!closingQuote) break;

            parameters[index] = strndup(current, closingQuote - current);
            index++;
            current = closingQuote + 1;
        } else {
            size_t wordLength = strcspn(current, " ");
            parameters[index] = strndup(current, wordLength);
            index++;
            current += wordLength;
        }

        while (*current == ' ') current++;
        if (*current == '\0') break;
    }

    free(lineCopy);

    int queryType = (parameters[0][1] != '0') ? parameters[0][0] - '0' : 1;

    char* delimiter = ";";
    if (strcmp(&parameters[0][1], "S") == 0) delimiter = "=";

    static queries_func queryHandlers[] = {queryOne, queryTwo, queryThree, queryFour, queryFive, querySix};

    void* output = queryHandlers[queryType - 1](dataset, parameters + 1, index-1, delimiter);

    for (int paramIndex = 0; paramIndex < index; paramIndex++) {
        free(parameters[paramIndex]);
    }
    free(parameters);

    return output;
}

void parseF (FILE* inputFile, int maxFields, void_function callback, void* context, FILE *logFile){
    int status = 0;
    char* currentLine = NULL;
    size_t bufferSize = 0;

    if (inputFile == NULL) {
        fprintf(stderr, "Error opening file\n");
        return;
    }

    if (getline(&currentLine, &bufferSize, inputFile) != -1) {
        fprintf(logFile, "%s", currentLine);
    } else {
        fprintf(stderr, "Error reading first line from file\n");
        free(currentLine);
        return;
    }

    while(getline(&currentLine, &bufferSize, inputFile) != -1){
        currentLine[strlen(currentLine)-1] = '\0';
        char* duplicateLine = strdup(currentLine);

        char **parsedFields = parseL(currentLine, maxFields);
        status = callback(parsedFields, context);

        if (status == 0) {
            fprintf(logFile, "%s\n", duplicateLine);
        }
        free(duplicateLine);
        free(parsedFields);
    }
    free(currentLine);
}

char** parseL(char* inputLine, int maxFields) {

    char** extractedFields = malloc(sizeof(char*) * maxFields);
    char* delimiterPosition = strstr(inputLine, ";");
    char* singleField = NULL;

    int fieldIndex;
    for (fieldIndex = 0; delimiterPosition != NULL; (delimiterPosition = strstr(inputLine, ";")), fieldIndex++) {
        singleField = delimiterPosition != inputLine ? inputLine : NULL;

        extractedFields[fieldIndex] = singleField;
        inputLine = delimiterPosition + 1;
        *delimiterPosition = '\0';
    }

    singleField = inputLine;
    extractedFields[fieldIndex] = singleField;

    return extractedFields;
}
