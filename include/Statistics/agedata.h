#ifndef AGEDATA_H
#define AGEDATA_H

#include <glib.h>
#include <stdio.h>

/// @brief Struct para armazenar tabelas de dados de idades e géneros.
typedef struct agedata* AGEDATA;

/// @brief Representa um género e a quantidade de likes associados
typedef struct genreLikes* GenreLikes;

/// @brief Cria uma nova estrutura GenreLikes
/// @param genre O nome do género
/// @param likes O número inicial de likes
/// @return Um ponteiro para a nova estrutura GenreLikes ou NULL em caso de erro
char* get_genreLikes_genre(GenreLikes l);

/// @brief Obtém o nome do género de uma estrutura GenreLikes
/// @param gl Ponteiro para a estrutura GenreLikes
/// @return O nome do género
int get_genreLikes_likes(GenreLikes gl);

/// @brief Cria uma nova estrutura agedata
/// @return Um ponteiro para a nova estrutura agedata ou NULL em caso de erro
AGEDATA create_agedata(void);

/// @brief Insere uma tabela de géneros na tabela de dados de idades
/// @param genretable Tabela hash de géneros e likes
/// @param table Tabela de dados de idades
/// @param key Chave representando a idade
void insert_ageMapGenre(GHashTable* genretable, AGEDATA table, char* key);

/// @brief Procura uma tabela de géneros na tabela de dados de idades
/// @param table Tabela de dados de idades
/// @param key Chave representando a idade
/// @return Ponteiro para a tabela de gêneros ou NULL se não encontrada
GHashTable* search_ageMapGenre(AGEDATA table, char * key);

/// @brief Liberta a memória de uma estrutura agedata
/// @param table Ponteiro para a estrutura agedata a ser libertada
void free_agedata(AGEDATA table);

/// @brief Liberta a memória de uma tabela de géneros.
/// @param data Ponteiro para a tabela de géneros a ser libertada.
void free_genretable(gpointer data);

/// @brief Liberta a memória de uma estrutura GenreLikes
/// @param data Ponteiro para a estrutura GenreLikes a ser libertada
void free_genreLikes(gpointer data);

/// @brief Soma os likes de géneros para uma faixa etária especificada
/// @param data Estrutura agedata contendo os dados
/// @param min_age Idade mínima da faixa
/// @param max_age Idade máxima da faixa
/// @return Uma lista ordenada de géneros e seus likes
void* sum_genre_likes(AGEDATA data, int min_age, int max_age);

#endif