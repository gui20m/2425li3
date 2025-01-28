#ifndef TOPARTISTS_H
#define TOPARTISTS_H

#include "Enteties/artists_t.h"
#include <glib.h>

/// @brief Representa uma lista de artistas considerados "top"
typedef struct topartists* TOPARTISTS;

/// @brief Cria e inicializa uma nova instância da estrutura topartists
/// @return Um ponteiro para uma estrutura topartists alocada e retorna NULL em caso de falha na alocação de memória
TOPARTISTS create_top_artists(void);

/// @brief Obtém uma cópia da lista de artistas
/// @param data Ponteiro para a estrutura contendo a lista de artistas
/// @return Uma cópia da lista de artistas
GList* get_top_artists_list(TOPARTISTS data);

/// @brief Define a lista de artistas na estrutura
/// @param data Ponteiro para a estrutura que terá sua lista de artistas atualizada
/// @param list Nova lista de artistas a ser atribuída
void set_top_artists(TOPARTISTS data, GList* list);

/// @brief Liberta a memória ocupada pela estrutura topartists
/// @param data Ponteiro para a estrutura topartists a ser libertada
void free_top_artists(TOPARTISTS data);

#endif