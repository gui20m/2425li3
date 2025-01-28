#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <glib.h>
#include "spotilixo.h"
#include "Main/dataval.h"

/// @brief Struct que representa um histórico
typedef struct history* HISTORY;

/// @brief Obtém o ID da música associado a um histórico
/// @param history Ponteiro para a estrutura do histórico
/// @return Uma cópia do ID da música associado ao histórico
char* get_history_music_id(HISTORY history);

/// @brief Liberta os recursos alocados para um histórico
/// @param history Ponteiro para a estrutura do histórico a ser libertada
void free_history(HISTORY history);

/// @brief Constrói um histórico com base nos campos fornecidos e processa as informações associadas
/// @param history_fields Array de strings representando os campos do histórico
/// @param data Ponteiro para os dados globais necessários para o processamento
/// @return 1 se o histórico foi construído e processado com sucesso, 0 caso contrário
int build_history(char **history_fields, void *data);

#endif