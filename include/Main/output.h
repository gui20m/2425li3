#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>

/// @brief Generalização de função
typedef void (*output_query_func)(FILE*, void*);

/// @brief A função output_query é responsável por chamar uma das funções de saída específicas (output_query1, output_query2 ou output_query3) com base no valor de query_id
/// @param output_file Ponteiro para um arquivo onde a saída será escrita
/// @param output Ponteiro genérico para os dados que serão escritos no arquivo
/// @param query_id Identificador que seleciona qual função de saída deve ser chamada (1, 2, ou 3)
void output_query(FILE* output_file, void* output, int query_id);

/// @brief A função output_query1 escreve o conteúdo apontado por output no arquivo file com uma nova linha (\n) no final
/// @param file Ponteiro para um arquivo onde a saída será escrita
/// @param output Ponteiro genérico que será convertido para char* e escrito no arquivo
void output_query1(FILE* file, void* output);

/// @brief A função output_query2 escreve o conteúdo apontado por output no arquivo file sem adicionar uma nova linha ao final
/// @param file Ponteiro para um arquivo onde a saída será escrita
/// @param output Ponteiro genérico que será convertido para char* e escrito no arquivo
void output_query2(FILE* file, void* output);

/// @brief A função output_query3 escreve o conteúdo apontado por output no arquivo file sem adicionar uma nova linha ao final
/// @param file Ponteiro para um arquivo onde a saída será escrita
/// @param output Ponteiro genérico que será convertido para char* e escrito no arquivo
void output_query3(FILE* file, void* output);

/// @brief A função output_query4 escreve o conteúdo apontado por output no arquivo file sem adicionar uma nova linha ao final
/// @param file Ponteiro para um arquivo onde a saída será escrita
/// @param output Ponteiro genérico que será convertido para char* e escrito no arquivo
void output_query4(FILE* file, void* output);

/// @brief A função output_query5 escreve o conteúdo apontado por output no arquivo file sem adicionar uma nova linha ao final
/// @param file Ponteiro para um arquivo onde a saída será escrita
/// @param output Ponteiro genérico que será convertido para char* e escrito no arquivo
void output_query5(FILE* file, void* output);

/// @brief A função output_query6 escreve o conteúdo apontado por output no arquivo file sem adicionar uma nova linha ao final
/// @param file Ponteiro para um arquivo onde a saída será escrita
/// @param output Ponteiro genérico que será convertido para char* e escrito no arquivo
void output_query6(FILE* file, void* output);


#endif