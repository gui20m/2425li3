#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <glib.h>
#include <stdio.h>

/// @brief Struct que representa dados relacionados a users e géneros musicais
typedef struct algorithm* ALGORITHM;

/// @brief Cria uma nova estrutura ALGORITHM
/// @return Ponteiro para a nova estrutura `ALGORITHM`
ALGORITHM create_algorithm(void);

/// @brief Liberta a memória alocada para a estrutura ALGORITHM.
/// @param data Ponteiro para a estrutura `ALGORITHM` a ser libertada
void free_algorithm(ALGORITHM data);

/// @brief Insere ou atualiza a matriz com dados de um user e género
/// @param data Ponteiro para a estrutura `ALGORITHM`
/// @param user String representando o ID do user
/// @param genre String representando o género a ser atualizado
void insert_matriz_user(ALGORITHM data, char* user, char* genre);

/// @brief Ajusta o tamanho da matriz de dados em uma estrutura ALGORITHM, expandindo ou reduzindo o número de users
/// @param data Ponteiro para a estrutura ALGORITHM a ser ajustada
/// @param num_total Novo número total de users
void expand_matriz(ALGORITHM data, int num_total);

/// @brief Processa a recomendação de users com base na matriz de géneros
/// @param data Ponteiro para a estrutura `ALGORITHM`
/// @param user_id String representando o ID do user para o qual serão feitas recomendações
/// @param num_recomendados Número máximo de users recomendados a serem retornados
/// @return Array de strings contendo os IDs dos users recomendados
char** process_recommended_users(ALGORITHM data, char* user_id, int num_recomendados);

#endif