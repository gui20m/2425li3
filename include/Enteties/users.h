#ifndef USERS_H
#define USERS_H

#include "musics_t.h"
#include "users_t.h"
#include "Statistics/statistics.h"
#include <glib.h>

/// @brief Estrutura que armazena informações sobre um usuário
typedef struct user *USER;

/// @brief Retorna uma cópia da string que representa o email do user
/// @param user Ponteiro para a estrutura user
/// @return Cópia da string email
char* get_user_email(USER user);

/// @brief Retorna uma cópia da string que representa o primeiro nome do user
/// @param user Ponteiro para a estrutura user
/// @return Cópia da string first_name
char* get_user_first_name(USER user);

/// @brief Retorna uma cópia da string que representa o último nome do user
/// @param user Ponteiro para a estrutura user
/// @return Cópia da string last_name
char* get_user_last_name(USER user);

/// @brief Retorna uma cópia da string que representa o país do user
/// @param user Ponteiro para a estrutura user
/// @return Cópia da string contry
char* get_user_country(USER user);

/// @brief Retorna uma cópia do inteiro que representa a idade do user
/// @param user Ponteiro para a estrutura user
/// @return Valor do inteiro age
int get_user_age(USER user);

/// @brief Retorna uma cópia da string que representa o tipo de subscrição do user
/// @param user Ponteiro para a estrutura user
/// @return Cópia da string subscription_type
char* get_user_subscription_type(USER user);

/// @brief Retorna uma cópia da string que representa o id das liked songs do user
/// @param user Ponteiro para a estrutura user
/// @return Cópia da string liked_songs_id
char* get_user_liked_songs_id(USER user);

/// @brief Libera toda a memória alocada para os campos de uma estrutura user e para a própria estrutura
/// @param user Ponteiro para a estrutura user 
void free_user(USER user);

/// @brief Constrói um novo USER, validando e inserindo o usuário em uma tabela hash
/// @param user_fields Array de strings com os campos de um usuário
/// @param data Estrutura contendo as tabelas users_table e musics_table
/// @return 1 se o usuário foi construído e inserido com sucesso, 0 caso contrário
int build_user(char **user_fields, void *data);

#endif