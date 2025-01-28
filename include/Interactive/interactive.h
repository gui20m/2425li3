#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include <stdio.h>
#include "Enteties/spotilixo.h"

/// @brief Representa a configuração do programa
typedef struct settings* SETTINGS;

/// @brief Cria uma nova estrutura de configurações
/// @return Um ponteiro para a nova estrutura SETTINGS
SETTINGS create_settings(void);

/// @brief Define o caminho do dataset nas configurações
/// @param setts Estrutura SETTINGS
/// @param path Caminho do dataset
void set_datasetPath(SETTINGS setts, char* path);

/// @brief Define o caminho dos arquivos de entrada nas configurações
/// @param setts Estrutura SETTINGS
/// @param path Caminho dos arquivos de entrada
void set_inputsPath(SETTINGS setts, char* path);

/// @brief Define o caminho de saída na estrutura de configurações
/// @param setts Um ponteiro para uma estrutura de configurações (`SETTINGS`) onde o caminho será definido
/// @param path Uma string representando o novo caminho de saída a ser armazenado
void set_outputPath(SETTINGS setts, char* path);

/// @brief Define os argumentos na estrutura de configurações
/// @param setts Um ponteiro para a estrutura de configurações (`SETTINGS`) onde os argumentos serão definidos
/// @param args Um array de strings contendo os argumentos a serem armazenados
/// @param n O número de argumentos fornecidos
void set_argSETTINGS(SETTINGS setts, char** args, int n);

/// @brief Define o gerenciador de dados nas configurações
/// @param setts Estrutura SETTINGS
/// @param data Gerenciador de dados do tipo SPOTILIXO
void set_manager(SETTINGS setts, SPOTILIXO data);

void set_nArtists(SETTINGS setts, char* n);

/// @brief Obtém o caminho do dataset das configurações.
/// @param setts Estrutura SETTINGS
/// @return Uma cópia do caminho do dataset.
char* get_datasetPath(SETTINGS setts);

/// @brief Obtém o caminho dos arquivos de entrada das configurações
/// @param setts Estrutura SETTINGS
/// @return Uma cópia do caminho dos arquivos de entrada
char* get_inputsPath(SETTINGS setts);

/// @brief Obtém o caminho de saída armazenado na estrutura `SETTINGS`
/// @param setts Ponteiro para a estrutura `SETTINGS` contendo o caminho de saída
/// @return Uma cópia do caminho de saída como uma string alocada dinamicamente. Retorna `NULL` se `outputPath` for `NULL` ou se a alocação falhar
char* get_outputPath(SETTINGS setts);

/// @brief Define o número de artistas em uma estrutura de configurações
/// @param setts Estrutura de configurações onde o número de artistas será definido
/// @param n Cadeia de caracteres representando o número de artistas a ser armazenado
void set_nArtists(SETTINGS setts, char* n);

/// @brief Obtém o número de artistas armazenado na estrutura de configurações
/// @param setts Estrutura de configurações da qual o número de artistas será extraído
/// @return Uma nova string duplicada contendo o número de artistas
char* get_nArtists(SETTINGS setts);

/// @brief Obtém o array de argumentos armazenados na estrutura `SETTINGS`
/// @param setts Ponteiro para a estrutura `SETTINGS`
/// @return Um ponteiro para o array de argumentos (`args`). Retorna `NULL` se `setts` ou `setts->args` for `NULL`
char** get_args(SETTINGS setts);

/// @brief Obtém o número de argumentos armazenados na estrutura `SETTINGS`
/// @param setts Ponteiro para a estrutura `SETTINGS`
/// @return O número de argumentos (`argsN`). Retorna `-1` se `setts` for `NULL`
int get_argsN(SETTINGS setts);

/// @brief Obtém o gerenciador de dados das configurações
/// @param setts Estrutura SETTINGS
/// @return Ponteiro para o gerenciador de dados SPOTILIXO
SPOTILIXO get_manager(SETTINGS setts);

/// @brief LiberTa a memória da estrutura SETTINGS
/// @param setts Ponteiro para a estrutura SETTINGS a ser liberTada
void free_settings(SETTINGS setts);

/// @brief Função principal do modo interativo
void interactive(void);

#endif