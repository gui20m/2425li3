#ifndef HOME_H
#define HOME_H

#include <stdio.h>
#include "Interactive/interactive.h"

/// @brief Desenha um padrão de fundo repetitivo na tela usando o terminal
void draw_background();

/// @brief Função principal da interface de menu inicial
/// @param setts Estrutura SETTINGS contendo as configurações e caminhos
void home(SETTINGS setts);

#endif