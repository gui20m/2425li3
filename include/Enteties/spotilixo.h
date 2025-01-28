#ifndef SPOTILIXO_H
#define SPOTILIXO_H

#include "users.h"
#include "artists_t.h"
#include "musics_t.h"
#include "users_t.h"
#include "Statistics/statistics.h"
#include "albums_t.h"
#include "history_t.h"
#include <glib.h>

/// @brief Estrutura que é utilizada para armazenar toda a informação do programa
typedef struct spotilixo *SPOTILIXO;

/// @brief A função create_spotilixo é responsável por criar e inicializar uma nova instância da estrutura SPOTILIXO
/// @return A função retorna um ponteiro para a nova instância alocada e inicializada da estrutura SPOTILIXO
SPOTILIXO create_spotilixo(void);

/// @brief Obtém a tabela de artistas da estrutura spotilixo
/// @param data Ponteiro para a estrutura spotilixo
/// @return Ponteiro para a tabela de artistas (artistas)
void* get_spotilixo_artistas(SPOTILIXO data);

/// @brief Obtém a tabela de músicas da estrutura spotilixo
/// @param data Ponteiro para a estrutura spotilixo
/// @return Ponteiro para a tabela de músicas (musicas)
void* get_spotilixo_musicas(SPOTILIXO data);

/// @brief Obtém a tabela de users da estrutura spotilixo
/// @param data Ponteiro para a estrutura spotilixo
/// @return Ponteiro para a tabela de users (users)
void* get_spotilixo_users(SPOTILIXO data);

/// @brief Obtém a tabela de albuns da estrutura spotilixo
/// @param data Ponteiro para a estrutura spotilixo
/// @return Ponteiro para a tabela de albuns (albuns)
void* get_spotilixo_albums(SPOTILIXO data);

/// @brief Obtém a tabela de estatisticas da estrutura spotilixo
/// @param data Ponteiro para a estrutura spotilixo
/// @return Ponteiro para os dados estatísticos (estatisticas)
void* get_spotilixo_estatisticas(SPOTILIXO data);

/// @brief Obtém o histórico da estrutura spotilixo
/// @param data Ponteiro para a estrutura spotilixo
/// @return Ponteiro para o histórico (history)
void* get_spotilixo_history(SPOTILIXO data);

/// @brief A função free_spotilixo é responsável por liberar a memória associada a uma instância da estrutura SPOTILIXO
/// @param data Um ponteiro para a instância da estrutura SPOTILIXO que deve ser liberada
void free_spotilixo(SPOTILIXO data);

#endif