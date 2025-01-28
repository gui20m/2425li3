#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Enteties/spotilixo.h"

/// @brief Função principal que abre e processa os arquivos de dados, carrega informações em uma estrutura de dados SPOTILIXO, e executa consultas baseadas em um arquivo de entrada
/// @param datasets Caminho para o diretório contendo os arquivos de dados 
/// @param inputs Caminho para o arquivo de entrada contendo as consultas
void interpreter(char* datasets, char* inputs);

#endif