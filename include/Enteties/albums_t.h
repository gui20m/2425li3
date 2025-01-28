#ifndef ALBUMS_T_H
#define ALBUMS_T_H

#include <stdio.h>
#include <glib.h>
#include "albums.h"

/// @brief Struct que armazena uma hash table de álbuns
typedef struct albumstable* ALBUMS_T;

/// @brief Cria e inicializa uma nova tabela de álbuns
/// @return Ponteiro para a nova tabela de álbuns ou NULL em caso de falha
ALBUMS_T create_albums_t(void);

/// @brief Insere um álbum na tabela
/// @param album Ponteiro para o álbum a ser inserido
/// @param table Ponteiro para a tabela de álbuns
/// @param key Chave associada ao álbum
void insert_album(void* album, ALBUMS_T table, char* key);

/// @brief Procura um álbum na tabela
/// @param albums Ponteiro para a tabela de álbuns
/// @param key Chave do álbum a ser procurado
/// @return Ponteiro para o álbum encontrado ou NULL se não encontrado
void* search_album(ALBUMS_T albums, char* key);

/// @brief Libera a memória alocada para a tabela de álbuns
/// @param albums Ponteiro para a tabela de álbuns
void free_albums_table(ALBUMS_T albums);

#endif