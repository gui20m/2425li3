#ifndef HISTORY_T_H
#define HISTORY_T_H

#include <stdio.h>
#include <glib.h>
#include "history.h"
#include "spotilixo.h"

/// @brief Struct que representa uma tabela de histórico
typedef struct historytable* HISTORY_T;

/// @brief Cria uma nova tabela de histórico
/// @return Um ponteiro para a tabela de histórico criada ou NULL em caso de falha
HISTORY_T create_history_t(void);

/// @brief Insere um elemento de histórico na tabela
/// @param history Ponteiro para os dados do histórico a serem inseridos
/// @param table Ponteiro para a tabela de histórico
/// @param key Chave associada aos dados a serem inseridos
void insert_history(void* history, HISTORY_T table, char* key);

/// @brief Liberta os recursos alocados para a tabela de histórico
/// @param table Ponteiro para a tabela de histórico a ser libertada
void free_history_table(HISTORY_T table);

#endif