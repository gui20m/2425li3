#ifndef MUSICS_H
#define MUSICS_H

#include "spotilixo.h"
#include "artists_t.h"
#include "albums_t.h"
#include <glib.h>

/// @brief Estrutura que representa uma música com informações sobre o artista, duração e gênero
typedef struct music *MUSIC;

/// @brief Obtém o valor do campo artist
/// @param music Ponteiro para a estrutura MUSIC
/// @return Uma nova string contendo o nome do artista
char* get_music_artist(MUSIC music);

/// @brief Obtém o valor do campo duration
/// @param music Ponteiro para a estrutura MUSIC
/// @return O valor de duration da música
int get_music_duration(MUSIC music);

/// @brief Obtém o valor do campo genre
/// @param music Ponteiro para a estrutura MUSIC
/// @return Uma nova string contendo o gênero da música
char* get_music_genre(MUSIC music);

/// @brief Obtém o valor do campo album
/// @param music Ponteiro para a estrutura MUSIC
/// @return Uma nova string contendo o albúm da música
char* get_music_album(MUSIC music);

/// @brief Libera a memória alocada para a estrutura music
/// @param music Ponteiro para a estrutura MUSIC
void free_music(MUSIC music);

/// @brief Constrói uma estrutura music e a insere na tabela hash se os dados fornecidos forem válidos
/// @param user_fields Array de strings contendo os campos da música
/// @param data Ponteiro genérico, que deve incluir a tabela hash de músicas e de artistas
/// @return 1 se a música foi construída e inserida com sucesso 0 caso contrário
int build_music(char **user_fields, void *data);

#endif