#ifndef ALBUMS_H
#define ALBUMS_H

#include <stdio.h>
#include <glib.h>
#include "spotilixo.h"

/// @brief Struct que representa um álbum.
typedef struct album* ALBUM;

/// @brief Obtém o(s) identificador(es) do(s) artista(s) associado(s) ao álbum
/// @param album Ponteiro para o álbum
/// @return Cópia do(s) identificador(es) do(s) artista(s)
char* get_album_artists(ALBUM album);

/// @brief Liberta a memória alocada para a estrutura do álbum
/// @param album Ponteiro para o álbum
void free_album(ALBUM album);

/// @brief Constrói e insere um álbum na estrutura de dados correspondente
/// @param album_fields Array de strings representando os campos do álbum
/// @param data Ponteiro para a estrutura de dados principal
/// @return 1 se o álbum foi criado e inserido com sucesso, 0 caso contrário
int build_album(char **album_fields, void *data);

#endif