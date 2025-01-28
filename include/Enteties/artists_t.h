#ifndef ARTISTS_T_H
#define ARTISTS_T_H

#include "artists.h"
#include <glib.h>

/// @brief Struct para armazenar artistas
typedef struct artists_table* ARTISTS_T;

/// @brief Cria uma nova tabela de artistas
/// @return Um ponteiro para a tabela de artistas criada ou NULL em caso de falha
ARTISTS_T create_artists_t(void);

/// @brief Insere um artista na tabela hash
/// @param artist Artista
/// @param table Estrutura que guarda os artistas
/// @param artistid Identificador do artista.
void insert_artista(void* artista, ARTISTS_T table, char* key);

/// @brief Procura um artista na tabela de artistas
/// @param artistas Ponteiro para a tabela de artistas
/// @param key Chave associada ao artista
/// @return Um ponteiro para o objeto ARTIST encontrado ou NULL se não existir
void* search_artist(ARTISTS_T artistas, char* key);

/// @brief Liberta a memória ocupada pela tabela de artistas
/// @param artistas Ponteiro para a tabela de artistas a ser libertada
void free_artists_table(ARTISTS_T artists);

/// @brief Encontra artistas de um determinado país na tabela
/// @param artist_table Ponteiro para a tabela de artistas
/// @param country_filter String representando o país a ser filtrado
/// @return Uma lista de artistas pertencentes ao país especificado, ordenados por duração total (decrescente)
GList* find_artists_by_country(ARTISTS_T artist_table, char *country_filter);

/// @brief Retorna uma lista de artistas, ordenada por totalDuration em ordem decrescente
/// @param artists_table Tabela hash contendo os artistas
/// @return Lista de artistas ordenada por totalDuration
GList* top_artists_by_duration(ARTISTS_T tabela);

/// @brief Compara dois artistas com base na duração total de suas músicas (ordem decrescente)
/// @param a Ponteiro para o primeiro artista
/// @param b Ponteiro para o segundo artista
/// @return Um valor negativo, zero ou positivo dependendo da ordem
int compare_artists_by_time_desc(gconstpointer a, gconstpointer b);

#endif