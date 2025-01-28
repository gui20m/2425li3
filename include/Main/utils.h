#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#define SYSTEM_DATE "2024/09/09"

/// @brief A função remove_char remove todas as aspas duplas e simples da string
/// @param str Ponteiro para uma string
void remove_char(char *str);

/// @brief Esta função calcula a idade de um usuário
/// @param currentDate Ponteiro para uma string que reprensenta a data atual
/// @param birthDate Ponteiro para uma string que reprensenta a data de nascimento do usuário
/// @return Retorna a idade calculada do usuário em anos
int calculate_age(char* currentDate, char* birthDate);

/// @brief A função create_output_file cria um ficheiro para a query
/// @param n O número do command
/// @return Retorna o ponteiro para o ficheiro criado
FILE* create_output_file(int n);

/// @brief A função int_to_string converte um interger numa string
/// @param number O integer a ser convertido
/// @return Retorna a String convertida
char* int_to_string(int number);

/// @brief Esta função calcula a duração total de uma música em segundos
/// @param duration Ponteiro para uma string que representa a duração da música no formato HH:MM:SS
/// @return Retorna a duração total da música em segundos
int calculate_duration(char *duration);

/// @brief A função split_array separa uma string em sub-strings representadas no formato de arrays
/// @param array Ponteiro para a string de entrada
/// @return Retorna sub-strings representadas no formato de arrays
char** split_array(const char* array);

/// @brief A função free_tokens libera a memória alocada para um array de strings
/// @param tokens Ponteiro para um array de strings
void free_tokens(char** tokens);

/// @brief A função convert_seconds_to_time converte um valor em segundos numa string no formato HH:MM:SS
/// @param seconds Tempo total em segundos a ser convertido
/// @return Um ponteiro para uma string que representa o tempo no formato HH:MM:SS
char* convert_seconds_to_time(int seconds);

/// @brief A função verifica se um caractere é um digito
/// @param c Caractere a ser verificado
/// @return A função retorna um bool
int isDigit(char c);

/// @brief A função verifica se um caractere é uma letra
/// @param c Caractere a ser verificado
/// @return A função retorna um bool
int isLetter(char c);

/// @brief Conta o número de elementos em um array representado como string
/// @param str String representando um array no formato [item1, item2, ...]
/// @return Número de elementos no array
int count_array_elements(char *str);

/// @brief Verifica se uma string contém apenas dígitos numéricos
/// @param str Ponteiro para a string a ser verificada
/// @return Retorna 1 se a string contiver apenas dígitos, caso contrário, retorna 0
int is_number(char *str);

#endif