#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <stdio.h>
#include <glib.h>

/// @brief Representa o quadro de líderes com duas hash tables: leaderboard e top10
typedef struct leaderboard* LEADERBOARD;

/// @brief Representa um artista no "Top 10" com o nome do artista e a duração acumulada de reprodução em segundos.
typedef struct top10* TOP10;

/// @brief Cria uma nova instância da estrutura LEADERBOARD
/// @return Ponteiro para a nova instância de LEADERBOARD, ou NULL em caso de erro
LEADERBOARD create_leaderboard(void);

/// @brief Liberta a memória alocada para os dados de uma semana
/// @param data Ponteiro para os dados semanais a serem libertados
void free_weekdata(gpointer data);

/// @brief Liberta a memória alocada para a tabela LEADERBOARD
/// @param table Ponteiro para a tabela a ser libertada
void free_leaderboard(LEADERBOARD table);

/// @brief Atualiza a duração semanal associada a artistas em uma tabela LEADERBOARD
/// @param table Estrutura LEADERBOARD onde os dados serão atualizados
/// @param artists_table Tabela de artistas
/// @param artists_id Identificadores dos artistas
/// @param timestamp Data e hora no formato "AAAA/MM/DD HH:MM:SS"
/// @param duration Duração acumulada de reprodução em segundos do artista
void weekly_duration_update(LEADERBOARD table, void* artists_table, char* artists_id, char* timestamp, int duration);

/// @brief Processa o top 10 semanal para cada ano e semana em uma tabela LEADERBOARD
/// @param data Estrutura LEADERBOARD contendo os dados
void process_top10_weekly(LEADERBOARD data);

/// @brief Obtém o artista mais tocado em todos os tops 10
/// @param data Estrutura LEADERBOARD contendo os dados
/// @return Array de strings contendo o nome do artista e a sua contagem de aparições
char** get_most_played_artist(LEADERBOARD data);

/// @brief Obtém o artista mais tocado em todos os tops 10 entre duas datas
/// @param data Estrutura LEADERBOARD contendo os dados
/// @param year1 Ano inicial
/// @param mes1 Mês inicial
/// @param dia1 Dia inicial
/// @param year2 Ano final
/// @param mes2 Mês final
/// @param dia2 Dia final
/// @return Array de strings contendo o nome do artista e a sua contagem de aparições
char** get_most_played_artist_between_dates(LEADERBOARD data, int year1, int mes1, int dia1, int year2, int mes2, int dia2);

#endif