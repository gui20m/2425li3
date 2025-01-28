#include "Main/utils.h"
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void remove_char(char *str) {
    int i = 0, j = 0;
    int in_array = 0, in_quotes = 0;

    while (str[i]) {
        char current_char_1 = str[i];
        char current_char_2 = str[i + 1];
        
        if (current_char_1 == '[') {
            in_array = 1;
        } else if (current_char_1 == ']') {
            in_array = 0;
        } else if (current_char_1 == '"') {
            in_quotes = !in_quotes;
        }

        if (in_array && current_char_1 == '\'' && in_quotes) {
            i++;
            continue;
        }

        if (current_char_1 != '"') {
            str[j++] = current_char_1;
        }

        if (current_char_2 != '\0') {
            if (current_char_2 == '[') {
                in_array = 1;
            } else if (current_char_2 == ']') {
                in_array = 0;
            } else if (current_char_2 == '"') {
                in_quotes = !in_quotes;
            }

            if (in_array && current_char_2 == '\'' && in_quotes) {
                i += 2;
                continue;
            }
            if (current_char_2 != '"') {
                str[j++] = current_char_2;
            }
            i += 2;
        } else {
            i++;
        }
    }

    str[j] = '\0';
}

int calculate_age(char* currentDate, char* birthDate){
    int currentYear, currentMonth, currentDay;
    int birthYear, birthMonth, birthDay;

    sscanf(currentDate, "%d/%d/%d", &currentYear, &currentMonth, &currentDay);
    sscanf(birthDate, "%d/%d/%d", &birthYear, &birthMonth, &birthDay);

    int age = currentYear - birthYear;

    // Making sure if the user's birthday has already occured this year
    if (birthMonth > currentMonth || (birthMonth == currentMonth && birthDay > currentDay)) {
        age--;
    }

    return age;
}

FILE* create_output_file(int n){

    char* path = malloc(sizeof(char*)*512);
    sprintf(path, "resultados/command%d_output.txt", n);
    FILE* f = fopen(path, "w");
    free(path);

    return f;
}

char* int_to_string(int number){
    char* result = malloc(20 * sizeof(char));
    snprintf(result, 20, "%d", number);
    return result;
}

int calculate_duration(char *duration){
    int hours, minutes, seconds;
    sscanf(duration, "%d:%d:%d", &hours, &minutes, &seconds);
    return hours * 3600 + minutes * 60 + seconds;
}

char** split_array(const char* array) {
    int len = strlen(array);
    if (len < 2 || array[0] != '[' || array[len - 1] != ']') {
        fprintf(stderr, "Formato inválido de array: %s\n", array);
        return NULL;
    }

    char* array_copy = strndup(array + 1, len - 2);
    if (array_copy == NULL) {
        perror("Falha ao alocar memória para array");
        return NULL;
    }

    int count = 1;
    for (int i = 0; array_copy[i]; i++) {
        if (array_copy[i] == ',') count++;
    }

    char** tokens = malloc((count + 1) * sizeof(char*));
    if (tokens == NULL) {
        perror("Falha ao alocar memória para tokens");
        free(array_copy);
        return NULL;
    }

    int index = 0;
    char* token = strtok(array_copy, ",");
    while (token != NULL) {
        while (*token == ' ') token++;
        tokens[index] = strdup(token);
        if (tokens[index] == NULL) {
            perror("Falha ao duplicar token");
            for (int j = 0; j < index; j++) {
                free(tokens[j]);
            }
            free(tokens);
            free(array_copy);
            return NULL;
        }
        index++;
        token = strtok(NULL, ",");
    }

    tokens[index] = NULL;

    free(array_copy);

    return tokens;
}

void free_tokens(char** tokens) {
    if (tokens == NULL) return;
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

char* convert_seconds_to_time(int seconds) {
    int hours, minutes;

    hours = seconds / 3600;
    seconds %= 3600;
    minutes = seconds / 60;
    seconds %= 60;

    char time_str[16];
    snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d", hours, minutes, seconds);

    // Retorna uma cópia dinâmica da string formatada
    return strdup(time_str);
}

int isDigit(char c){
    return(c >= '0' && c <= '9');
}

int isLetter(char c){
    return((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int count_array_elements(char *str) {
    char** subarray = split_array(str); 
    int count = 0;

    while(subarray[count] != NULL) {
        count++;
    }

    free_tokens(subarray);

    return count;
}

int is_number(char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}
