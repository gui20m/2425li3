#ifndef TEST_H
#define TEST_H

/// @brief Função principal que abre e processa os arquivos de dados, carrega informações em uma estrutura de dados SPOTILIXO, e executa consultas baseadas em um arquivo de entrada
/// @param datasets Caminho para o diretório contendo os arquivos de dados 
/// @param inputs Caminho para o arquivo de entrada contendo as consultas
/// @param expectedOutput Argumento que guarda o caminho da diretoria dos resultados esperados
void test(char* datasets, char* inputs, char* expectOutput);

#endif