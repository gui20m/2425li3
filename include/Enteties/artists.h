#ifndef ARTISTS_H
#define ARTISTS_H

#include "spotilixo.h"
#include "musics.h"
#include <glib.h>

/// @brief Estrutura que é utilizada para armazenar informações sobre um artista
typedef struct artist *ARTIST; 

/// @brief Atualiza a duração total do artista, somando value ao valor atual
/// @param artist Ponteiro para a estrutura artist
/// @param value Valor a ser adicionado ao campo totalDuration
void update_artist_totalDuration(ARTIST artist, int value);

/// @brief Retorna uma cópia da string que representa o nome do artista
/// @param artist Ponteiro para a estrutura artist
/// @return Cópia da string name
char* get_artist_name(ARTIST artist);

/// @brief Retorna uma cópia da string que representa o país do artista.
/// @param artist Ponteiro para a estrutura artist
/// @return Cópia da string country
char* get_artist_country(ARTIST artist);

/// @brief Retorna a duração total de músicas do artista
/// @param artist Ponteiro para a estrutura artist
/// @return Valor de totalDuration
int get_artist_totalDuration(ARTIST artist);

/// @brief Retorna uma cópia da string que representa o tipo do artista
/// @param artist Ponteiro para a estrutura artist
/// @return Cópia da string type
char* get_artist_type(ARTIST artist);

/// @brief Obtém a receita por stream associada ao artista
/// @param artist Ponteiro para o ARTIST
/// @return String duplicada contendo a receita por stream
char* get_artist_rps(ARTIST artist);

/// @brief Obtém o identificador constituinte do artista
/// @param artist Ponteiro para o ARTIST
/// @return String duplicada contendo o identificador
char* get_artist_id_const(ARTIST artist);

/// @brief Libera a memória alocada para a estrutura artist
/// @param artist Ponteiro para a estrutura artist
void free_artist(ARTIST artist);

/// @brief Cria e insere um artista na tabela hash de data se os campos forem válidos
/// @param artist_fields Array de strings com informações do artista
/// @param data Ponteiro que contém a tabela hash de artistas
/// @return 1 se o artista for criado e inserido com sucesso; 0 caso contrário
int build_artist(char **artist_fields, void *data);

#endif