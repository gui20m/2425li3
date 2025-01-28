#ifndef INDIVIDUALALBUMS_H
#define INDIVIDUALALBUMS_H

#include <stdio.h>
#include <glib.h>

/// @brief Struct que representa o mapeamento de álbuns individuais
typedef struct individualalbum* INDIVIDUAL_ALBUM;

/// @brief Cria uma nova instância da estrutura INDIVIDUAL_ALBUM
/// @return Ponteiro para a nova instância de INDIVIDUAL_ALBUM, ou NULL em caso de erro
INDIVIDUAL_ALBUM create_individualalbum(void);

/// @brief Procura o número de álbuns associados a uma chave específica na tabela
/// @param table Estrutura INDIVIDUAL_ALBUM onde a pesquisa será feita
/// @param key Chave associada ao álbum
/// @return Número de álbuns associados à chave, ou 0 se a chave não for encontrada
int search_numAlbum(INDIVIDUAL_ALBUM table, char* key);

/// @brief Liberta a memória alocada para uma estrutura INDIVIDUAL_ALBUM
/// @param table Ponteiro para a tabela a ser libertada
void free_individualalbum(INDIVIDUAL_ALBUM table);

/// @brief Atualiza a tabela de álbuns individuais com uma chave específica
/// @param table Estrutura INDIVIDUAL_ALBUM onde os dados serão atualizados
/// @param key Chave que identifica o álbum
/// @details Se a chave não existir, será inserida com valor inicial 1, caso contrário, incrementa o valor associado à chave
void update_ind_album(INDIVIDUAL_ALBUM table, char* key);

#endif