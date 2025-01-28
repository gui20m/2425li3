#ifndef COMPARE_FILES_H
#define COMPARE_FILES_H

#include "test.h"
#include <glib.h>

/// @brief Compara dois ficheiros
/// @param expectedOutput Argumento que guarda o caminho da diretoria dos resultados esperados
/// @param n NÚmero de querys existentes
/// @param lista Lista ligada que guarda as querys por ordem de input
void compare_files(char* expectedOutput, int n, GList* lista);

#endif