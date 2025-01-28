#ifndef MUSICS_T_H
#define MUSICS_T_H

#include "Main/utils.h"
#include <glib.h>

/// @brief Struct que representa a tabela de músicas
typedef struct musics_table* MUSICS_T;

/// @brief Cria e inicializa uma nova tabela de músicas  
/// @return Um ponteiro para a nova tabela de músicas (MUSICS_T)
MUSICS_T create_musics_t(void);

/// @brief Insere uma música na tabela
/// @param musica Ponteiro para a música a ser inserida
/// @param table Tabela de músicas onde a música será armazenada
/// @param key Chave associada à música
void insert_musica(void* musica, MUSICS_T table, char* key);

/// @brief Procura uma música na tabela usando sua chave
/// @param musicas Tabela de músicas
/// @param key Chave da música a ser procurada
/// @return Ponteiro para a música correspondente, ou NULL se não encontrada
void* search_music(MUSICS_T musicas, char* key);

/// @brief Liberta a memória associada à tabela de músicas
/// @param musicas Tabela de músicas a ser libertada
void free_musics_table(MUSICS_T musicas);

/// @brief Itera sobre todas as músicas na tabela e atualiza a duração acumulada para os artistas
/// @param tabela Tabela de músicas
/// @param artistas Tabela de artistas
void update_duration_for_each_artist(MUSICS_T tabela, void* artistas);

#endif