#ifndef DATAVAL_H
#define DATAVAL_H

#include "Enteties/musics_t.h"
#include "Enteties/artists_t.h"
#include <glib.h>

/// @brief A função validate_date é responsável por validar se uma data fornecida em formato de string está em um formato correto e se é uma data válida em relação à data atual
/// @param date_string Uma string contendo uma data no formato YYYY/MM/DD
/// @return A função retorna 1 se a data for válida e 0 se a data for inválida
int validate_date(char* date_string);

/// @brief A função validate_email verifica se um endereço de e-mail é válido
/// @param email Ponteiro para uma string que contém o endereço de e-mail a ser validado
/// @return 1 se o e-mail for considerado válido ou 0 se inválido
int validate_email(char* email);

/// @brief A função validate_subscription_type verifica se uma string representa um tipo de assinatura válido ("normal" ou "premium")
/// @param str Ponteiro para uma string que contém o tipo de assinatura a ser validado
/// @return 1 se str for "normal" ou "premium" 0 para qualquer outra string
int validate_subscription_type(char* str);

/// @brief A função validate_array verifica se uma string representa um array, ou seja, se a string começa com [ e termina com ], e se possui pelo menos um caractere entre eles
/// @param str Ponteiro para uma string que contém o texto a ser verificado
/// @return 1 se str tiver no formato de um array e pelo menos um caractere entre [ e ] 0 caso contrário
int validate_array(char* str);
/// @brief A função validate_id_constituent verifica a validade de um identificador com base no tipo de constituinte ("group" ou "individual")
/// @param str Ponteiro para uma string que representa o identificador do constituinte
/// @param type Ponteiro para uma string que especifica o tipo de constituinte
/// @return 1 se o identificador for válido, ou seja, se type é "group" ou se type é "individual" e str é um array vazio representado por "[]" 0 caso contrário
int validate_id_constituent(char* str, char* type);

/// @brief A função validate_music_artist verifica se todos os artistas listados em uma string existem em uma tabela hash de artistas
/// @param str Ponteiro para uma string que contém os identificadores de artistas
/// @param artists_table Ponteiro para uma tabela hash que contém os dados dos artistas
/// @return 1 se todos os artistas listados na string str existem na tabela artists_table 0 se pelo menos um dos artistas listados na string str não for encontrado na tabela
int validate_artists_id(char* str, void* artists_table);

/// @brief Valida se um ID de álbum existe na tabela de álbuns
/// @param str String representando o ID do álbum
/// @param albums_table Tabela de álbuns
/// @return 1 se o álbum for válido, 0 caso contrário
int validate_album_id(char* str, void* albums_table);

/// @brief A função validate_duration verifica se uma string representa uma duração válida
/// @param str Ponteiro para uma string que representa o tempo
/// @return 1 se time_string é uma duração válida no formato HH:MM:SS 0 se não está no formato correto ou contém valores fora do intervalo esperado
int validate_duration(char* str);

/// @brief A função validate_liked_songs verifica se todas as músicas listadas em uma string estão presentes em uma tabela hash de músicas
/// @param str Ponteiro para uma string contendo identificadores de músicas
/// @param musics_table Ponteiro para uma tabela hash que contém as músicas
/// @return 1 se todas as músicas listadas em str estão presentes em musics_table 0 se pelo menos uma das músicas listadas em str não foi encontrada
int validate_liked_songs(char* str, MUSICS_T musics_table);

/// @brief Valida se um user existe na tabela de users
/// @param str String representando o ID do user
/// @param users Tabela de users
/// @return 1 se o user for válido, 0 caso contrário
int validate_user(char* str, void* users);

/// @brief Valida se uma música existe na tabela de músicas
/// @param str String representando o ID da música
/// @param musics Tabela de músicas
/// @return 1 se a música for válida, 0 caso contrário
int validate_music(char* str, void* musics);

/// @brief Valida se a plataforma informada é válida
/// @param str String representando a plataforma 
/// @return 1 se for válida, 0 caso contrário
int validate_plataform(char* str);;

#endif