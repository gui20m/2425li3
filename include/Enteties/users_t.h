#ifndef USERS_T_H
#define USERS_T_H

#include "users.h"
#include <glib.h>

/// @brief Armazena informações de users em uma hash table
typedef struct users_table* USERS_T;

/// @brief Cria uma nova tabela de users
/// @return Um ponteiro para a estrutura alocada. Retorna NULL em caso de erro.
USERS_T create_users_t(void);

/// @brief Insere um novo user na tabela
/// @param user Ponteiro para os dados do user a serem inseridos
/// @param table Ponteiro para a tabela de users
/// @param key Chave associada ao user
void insert_user(void* user, USERS_T table, char* key);

/// @brief Procura um usuário na tabela pelo identificador
/// @param table Ponteiro para a tabela de users
/// @param key Chave do user a ser procurado
/// @return Ponteiro para os dados do user associado à chave, ou NULL se a chave não estiver na tabela
void* search_user(USERS_T table, char* key);

/// @brief Obtém o número de users armazenados na estrutura USERS_T
/// @param data Ponteiro para a estrutura de users
/// @return Número de users presentes na tabela
int users_t_size(USERS_T data);

/// @brief Liberta a memória ocupada pela tabela de users
/// @param table Ponteiro para a tabela de users a ser liberada
void free_users_table(USERS_T table);

#endif