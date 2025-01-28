#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdio.h>
#include <glib.h>
#include "Main/utils.h"
#include "topartists.h"
#include "individualalbums.h"
#include "agedata.h"
#include "views.h"
#include "leaderboard.h"
#include "annualsummary.h"
#include "algorithm.h"
 
/// @brief Armazena várias informações e métricas relacionadas a dados estatísticos
typedef struct statistics* STATISTICS;

/// @brief Cria e inicializa uma nova instância da estrutura statistics
/// @return Ponteiro para uma estrutura statistics recém-criada, retorna NULL em caso de falha na alocação de memória
STATISTICS create_statistics(void);

/// @brief Obtém o campo agedata da estrutura statistics
/// @param table Ponteiro para a estrutura statistics
/// @return Ponteiro para o campo agedata
AGEDATA get_statistics_agedata(STATISTICS table);

/// @brief Obtém o campo top_artists da estrutura statistics
/// @param table Ponteiro para a estrutura statistics
/// @return Ponteiro para o campo top_artists
void* get_top_artists(STATISTICS table);

/// @brief Obtém o campo views da estrutura statistics
/// @param table Ponteiro para a estrutura statistics
/// @return Ponteiro para o campo views
void* get_statistics_views(STATISTICS table);

/// @brief Obtém o campo ind_album da estrutura statistics
/// @param table Ponteiro para a estrutura statistics
/// @return Ponteiro para o campo ind_album
INDIVIDUAL_ALBUM get_statistics_ind_album(STATISTICS table);

/// @brief Obtém o campo leaderboard da estrutura statistics
/// @param table Ponteiro para a estrutura statistics
/// @return Ponteiro para o campo leaderbord
LEADERBOARD get_statistics_leaderboard(STATISTICS table);

/// @brief Obtém o campo annualsummary da estrutura statistics
/// @param table Ponteiro para a estrutura statistics
/// @return Ponteiro para o campo annualsummary
SUMMARY get_statistics_annualsummary(STATISTICS table);

/// @brief Obtém o campo algorithm da estrutura statistics
/// @param table Ponteiro para a estrutura statistics
/// @return Ponteiro para o campo algorithm
ALGORITHM get_statistics_algorithm(STATISTICS table);

/// @brief Liberta a memória ocupada pela estrutura statistics
/// @param table Ponteiro para a estrutura statistics a ser libertada
void free_statistics(STATISTICS table);

#endif