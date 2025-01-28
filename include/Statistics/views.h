#ifndef VIEWS_H
#define VIEWS_H

#include <stdio.h>
#include "Enteties/spotilixo.h"
#include <glib.h>
#include "statistics.h"

/// @brief Armazena informações sobre reproduções e receitas de músicas e artistas
typedef struct views* VIEWS;

/// @brief Cria uma nova estrutura de dados VIEWS 
/// @return Um ponteiro para a nova estrutura alocada
VIEWS create_views(void);

/// @brief Procura a receita acumulada de um artista solo
/// @param table Estrutura de dados que contem as hash tables
/// @param key Nome do artista a ser procurado
/// @return Ponteiro para a receita acumulada ou NULL se o artista não estiver presente
float* search_soloArtistRecipe(VIEWS table, char* key);

/// @brief Procura a receita acumulada de uma banda
/// @param table Estrutura de dados que contem as hash tables
/// @param key Nome da banda a ser procurada
/// @return Ponteiro para a receita acumulada ou NULL se a banda não estiver presente
float* search_bandArtistRecipe(VIEWS table, char* key);

/// @brief Procura a receita de uma música colaborativa
/// @param table Estrutura de dados que contem as hash tables
/// @param key ID da música colaborativa
/// @return Ponteiro para a receita acumulada ou NULL se a música não estiver presente
float* search_collabSongRecipe(VIEWS table, char* key);

/// @brief Liberta a memória ocupada pela estrutura VIEWS
/// @param table Estrutura a ser libertada
void free_views(VIEWS table);

/// @brief Atualiza o número de reproduções de uma música
/// @param music_id ID da música
/// @param data Dados que contêm as tabelas de estatísticas
void views_counting(char* music_id, void* data);

/// @brief Processa todas as músicas e atualiza as receitas acumuladas de artistas
/// @param table Estrutura de dados que contem as hash tables
/// @param artistas Estrutura que contém os dados dos artistas
/// @param musicas Estrutura com informações das músicas
void process_artists_recipe(VIEWS table, void* artistas, void* musicas);

#endif