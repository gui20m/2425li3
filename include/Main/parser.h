#ifndef PARSER_H
#define PARSER_H

#define MAX_ARGS 10

#include "Enteties/spotilixo.h"
#include "Main/output.h"
#include "Main/queries.h"
#include <stdio.h>
#include <glib.h>

/// @brief Generalização de função
typedef void* (*queries_func)(SPOTILIXO data, char** query_args, int N, char* separador);

/// @brief Generalização de função
typedef int (*void_function)(char**, void*);

/// @brief Interpreta e executa uma consulta baseada na linha line e nos dados data
/// @param data Estrutura SPOTILIXO contendo os dados principais
/// @param line  String contendo a linha de entrada da consulta
/// @return Ponteiro para o resultado da consulta
void* parserQuery(SPOTILIXO data, char* line);

/// @brief Lê e processa cada linha de um arquivo CSV, chamando uma função de validação para cada linha e escrevendo as linhas inválidas em um arquivo de erro
/// @param f Ponteiro para o arquivo CSV a ser lido
/// @param max_fields Número máximo de campos esperados em cada linha do arquivo
/// @param func Função de validação a ser aplicada em cada linha
/// @param data Dados auxiliares que podem ser usados pela função de validação func
/// @param error_file Ponteiro para o arquivo onde as linhas inválidas serão registradas
void parseF (FILE* f, int max_fields, void_function func, void* data, FILE *error_file);

/// @brief Recebe uma linha CSV e divide seus valores, separados por ;, em um array de strings
/// @param line String contendo a linha CSV a ser processada
/// @param max_fields Número máximo de campos esperados na linha
/// @return Retorna um array de strings, onde cada índice representa um campo da linha
char** parseL(char* line, int max_fields);

#endif
