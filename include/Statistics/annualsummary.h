#ifndef ANNUALSUMMARY_H
#define ANNUALSUMMARY_H

#include <stdio.h>
#include <glib.h>

extern int interactive_mode;

/// @brief Struct que contém o resumo anual e informações de artistas favoritos
typedef struct summary* SUMMARY;

/// @brief Struct que contém as estatísticas de um artista
typedef struct artiststats* ARTISTSTATS;

/// @brief Struct que contém as estatísticas de um usuário
typedef struct userstats* USERSTATS;

/// @brief Obtém o tempo total de audição de uma estrutura USERSTATS
/// @param data Ponteiro para a estrutura USERSTATS
/// @return Tempo de audição total em segundos
int get_userstats_listeningtime(USERSTATS data);

/// @brief Obtém o número total de músicas reproduzidas de uma estrutura USERSTATS
/// @param data Ponteiro para a estrutura USERSTATS
/// @return Número total de músicas reproduzidas
int get_userstats_num_songs(USERSTATS data);

/// @brief Obtém o identificador do artista de uma estrutura ARTISTSTATS
/// @param data Ponteiro para a estrutura ARTISTSTATS
/// @return String contendo o identificador do artista 
char* get_artiststats_id(ARTISTSTATS data);

/// @brief Obtém o tempo total de audição associado a um artista
/// @param data Ponteiro para a estrutura ARTISTSTATS
/// @return Tempo total de audição em segundos
int get_artiststats_listeningtime(ARTISTSTATS data);

/// @brief Obtém o número total de músicas associadas a um artista
/// @param data Ponteiro para a estrutura ARTISTSTATS
/// @return Número total de músicas associadas
int get_artiststats_numsongs(ARTISTSTATS data);

/// @brief Cria uma nova instância de SUMMARY
/// @return Um ponteiro para a estrutura SUMMARY criada
SUMMARY create_annualsummary(void);

/// @brief Liberta a memória alocada para a estrutura de resumo anual
/// @param data Ponteiro para a estrutura `SUMMARY` que contém os dados do resumo anual
void free_annualsummary(SUMMARY data);

/// @brief Obtém o tempo total de audição de um user em um ano específico
/// @param summary Ponteiro para a estrutura SUMMARY
/// @param user_id Identificador do user
/// @param ano Ano específico
/// @return Tempo total de audição em segundos ou -1 em caso de erro
int get_annualsummary_listeningtime(SUMMARY summary, char* user_id, char* ano);

/// @brief Obtém o número total de reproduções de um user em um ano específico
/// @param summary Ponteiro para a estrutura SUMMARY
/// @param user_id Identificador do user
/// @param ano Ano específico
/// @return Número total de reproduções ou -1 em caso de erro
int get_annualsummary_numplays(SUMMARY summary, char* user_id, char* ano);

/// @brief Obtém o artista mais reproduzido de um user em um ano específico
/// @param summary Ponteiro para a estrutura SUMMARY
/// @param user_id Identificador do usuário
/// @param ano Ano específico
/// @return String contendo o artista mais reproduzido ou NULL em caso de erro
char* get_annualsummary_mostplayed_artist(SUMMARY summary, char* user_id, char* ano);

/// @brief Obtém o género mais reproduzido por um user em um ano específico
/// @param summary Ponteiro para a estrutura SUMMARY
/// @param user_id Identificador do user
/// @param ano Ano específico
/// @return String contendo o género mais reproduzido ou NULL em caso de erro
char* get_annualsummary_mostplayed_genre(SUMMARY summary, char* user_id, char* ano);

/// @brief Obtém o álbum mais reproduzido por um user em um ano específico
/// @param summary Ponteiro para a estrutura SUMMARY
/// @param user_id Identificador do usuário
/// @param ano Ano específico
/// @return String contendo o álbum mais reproduzido ou NULL em caso de erro
char* get_annualsummary_mostplayed_album(SUMMARY summary, char* user_id, char* ano);

/// @brief Obtém o dia mais reproduzido por um user em um ano específico
/// @param summary Ponteiro para a estrutura SUMMARY
/// @param user_id Identificador do user
/// @param ano Ano específico
/// @return String contendo a data mais reproduzida (no formato "YYYY-MM-DD") ou NULL em caso de erro
char* get_annualsummary_mostplayed_day(SUMMARY summary, char* user_id, char* ano);

/// @brief Obtém a hora favorita de reprodução de um user em um ano específico
/// @param summary Ponteiro para a estrutura SUMMARY contendo os dados do resumo anual
/// @param user_id Identificador único do user
/// @param ano Ano específico para análise, representado como string 
/// @return String contendo a hora favorita (formato "HH") ou NULL em caso de erro ou se os dados não existirem
char* get_annualsummary_favouritehour(SUMMARY summary, char* user_id, char* ano);

/// @brief Obtém a lista dos artistas favoritos de um user em um determinado ano
/// @param data Ponteiro para a estrutura `SUMMARY` contendo os dados do resumo anual
/// @param user_id String representando o identificador do user
/// @param ano String representando o ano de interesse (formato "AAAA")
/// @param n Número máximo de artistas a serem retornados na lista
/// @return Uma lista (`GList*`) contendo estruturas `ARTISTSTATS` dos artistas favoritos
GList* get_annualsummary_favouriteartists(SUMMARY data, char* user_id, char* ano, int n);

/// @brief Processa os dados de audição de um user, atualizando seu resumo anual
/// @param data Estrutura de resumo anual (`SUMMARY`) contendo os dados de todos os users
/// @param user_id Identificador único do user
/// @param music_id Identificador único da música ouvida
/// @param timestamp Data e hora do evento em formato string ("AAAA/MM/DD HH:MM:SS")
/// @param duration Duração da reprodução em segundos
/// @param musics Ponteiro genérico para a estrutura que armazena informações das músicas
/// @param artistas Ponteiro genérico para a estrutura que armazena informações dos artistas
void process_annualsummary(SUMMARY data, char* user_id, char* music_id, char* timestamp, int duration, void* musics, void* artists);

/// @brief Liberta a memória associada a uma lista de estatísticas de artistas
/// @param artiststats_list Lista encadeada (`GList*`) contendo ponteiros para estruturas do tipo `artiststats`
void free_artiststats_list(GList* artiststats_list);

/// @brief Garante a inicialização de estatísticas anuais para um user
/// @param data Estrutura de resumo anual (`SUMMARY`) contendo os dados de todos os users
/// @param user_id Identificador único do user
/// @param ano Ano específico como string 
void pre_process_users(SUMMARY data, char* user_id, char* ano);

#endif