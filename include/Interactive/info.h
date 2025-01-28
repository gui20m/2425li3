#ifndef INFO_H
#define INFO_H

#include "Interactive/interactive.h"

/// @brief Processa informações do dataset e realiza configurações iniciais
/// @param setts Estrutura SETTINGS contendo as configurações
/// @param datasets Caminho para o diretório contendo os datasets
/// @param inputs Caminho para o arquivo de entradas
/// @return Retorna 1 em caso de sucesso e 0 em caso de erro
int process_info(SETTINGS setts, char* datasets);

/// @brief Processa um arquivo de entrada para carregar configurações ou dados
/// @param setts Um ponteiro para uma estrutura de configurações (`SETTINGS`) que pode ser usada para armazenar os dados processados
/// @param inputsPath Uma string representando o caminho para o arquivo de entrada
/// @return `0` se o arquivo não puder ser aberto ou`1` se o arquivo foi aberto com sucesso 
int process_inputs(SETTINGS setts, char* inputsPath);

/// @brief Processa os dados das consultas e gera os arquivos de saída correspondentes
/// @param setts Estrutura SETTINGS contendo as configurações
/// @return Retorna 1 em caso de sucesso e 0 em caso de erro
int process_data(SETTINGS setts);

#endif