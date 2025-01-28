#ifndef QUERIES_H
#define QUERIES_H

#include "Enteties/spotilixo.h"

/// @brief A função queryOne tem como objetivo procurar informações de um usuário específico ou de um artista
/// @param data Ponteiro para a estrutura principal Spotilixo
/// @param args Array de strings com os argumentos necessários. O primeiro argumento é o userid que identifica o usuário/artista a ser consultado
/// @param N Número de argumentos fornecidos
/// @param separador String utilizada para separar os campos no resultado
/// @return A função retorna um ponteiro para uma string que contém as informações do usuário ou do artista
void* queryOne(SPOTILIXO data, char** args, int N, char* separador);

/// @brief A função queryTwo tem como objetivo procurar os top artistas consoante o tempo de discografia de certo país
/// @param data Ponteiro para a estrutura principal Spotilixo
/// @param args Array de strings com os argumentos necessários. O primeiro argumento é o número de artistas e em segundo o país, se existir
/// @param N Número de argumentos fornecidos
/// @param separador String utilizada para separar os campos no resultado
/// @return A função retorna um ponteiro para uma string que contém as informações dos artistas
void* queryTwo(SPOTILIXO data, char** args, int N, char* separador);

/// @brief A função queryTwoNoCountry tem como objetivo procurar os top artistas consoante o tempo de discografia
/// @param data Ponteiro para a estrutura principal Spotilixo
/// @param count Número de artistas
/// @param separador String utilizada para separar os campos no resultado
/// @return A função retorna um ponteiro para uma string que contém as informações dos artistas
void* queryTwoNoCountry(SPOTILIXO data, int count, char* separador);

/// @brief A função queryThree lista os gêneros musicais mais populares entre os usuários de uma faixa etária
/// @param data Ponteiro para a estrutura principal Spotilixo
/// @param args Um array de strings contendo os argumentos. O primeiro argumento representa a idade mínima e o segundo a idade máxima
/// @param N Número de elementos em args
/// @param separador String utilizada para separar os campos no resultado
/// @return A função retorna uma string formatada contendo os gêneros musicais e o número de likes associados
void* queryThree(SPOTILIXO data, char** args, int N, char* separador);

/// @brief A função queryFour tem como objetivo procurar o artista que estve mais vezes no top 10
/// @param data Ponteiro para a estrutura principal Spotilixo
/// @param args Argumentos da consulta. Pode incluir datas no formato "AAAA/MM/DD"
/// @param N Número de argumentos fornecidos
/// @param separador String utilizada para separar os campos no resultado
/// @return String formatada que contém as informações do artista que estve mais vezes no top 10
void* queryFour(SPOTILIXO data, char** args, int N, char* separador);

/// @brief A função queryFive tem como objetivo recomendar utilizadores com gostos parecidos
/// @param data Ponteiro para a estrutura principal Spotilixo
/// @param args  Id do utilizador para o qual deve ser gerada a lista de utilizadores semelhantes e o número de utilizadores a retornar
/// @param N Número de argumentos fornecidos
/// @param separador String utilizada para separar os campos no resultado
/// @return String formatada contendo a lista de utilizadores semelhantes
void* queryFive(SPOTILIXO data, char** args, int N, char* separador);

/// @brief A função querySix tem como objetivo retornar um resumo anual para um usuário específico
/// @param data Ponteiro para a estrutura principal Spotilixo
/// @param args  Argumentos da consulta, args[0] contém o ID do usuário e args[1] o ano
/// @param N Número de argumentos fornecidos
/// @param separador String utilizada para separar os campos no resultado
/// @return String formatada contendo o resumo anual do usuário
void* querySix(SPOTILIXO data, char** args, int N, char* separador);

#endif