#include "Enteties/musics_t.h"
#include "Enteties/musics.h"
#include <ctype.h>

#include <stdio.h>

struct musics_table{
    GHashTable* musics;
};

MUSICS_T create_musics_t(void){
    MUSICS_T new = malloc(sizeof(struct musics_table));
    if (!new){
        fprintf(stderr, "Falha ao alocar memória para musics table.\n");
        return NULL;
    }

    new->musics = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)free_music);

    return new;
}

void insert_musica(void* musica, MUSICS_T table, char* key){
    char* key_copy = strdup(key);
    g_hash_table_insert(table->musics, key_copy, musica);
}

void* search_music(MUSICS_T musicas, char* key){
    return g_hash_table_lookup(musicas->musics, key);
}

void free_musics_table(MUSICS_T musicas){
    g_hash_table_destroy(musicas->musics);
    free(musicas);
}

static void process_update_duration(char *input, int duration, void* artists_table) {
    int len = strlen(input);
    if (len < 2 || input[0] != '[' || input[len - 1] != ']') {
        return;
    }

    const char* start = input + 1;
    const char* end = input + len - 2;  

    const char* token_start = start;
    for (const char* p = start; p <= end; p++) {
        if (*p == ',' || p == end) {
            int token_length = p - token_start;
            if (p == end && *p != ',') {
                token_length++;
            }

            char token[token_length + 1];
            strncpy(token, token_start, token_length);
            token[token_length] = '\0';

            char* token_ptr = token;
            while (*token_ptr && isspace((unsigned char)*token_ptr)) {
                token_ptr++;
            }
            memmove(token, token_ptr, strlen(token_ptr) + 1);
            int end_idx = strlen(token) - 1;
            while (end_idx >= 0 && isspace((unsigned char)token[end_idx])) {
                token[end_idx] = '\0';
                end_idx--;
            }

            ARTIST artist = search_artist(artists_table, token);
            if (artist != NULL) {
                update_artist_totalDuration(artist, duration);
            }

            token_start = p + 1;
        }
    }

    return;
}

static void update_duration_callback(gpointer key, gpointer value, gpointer user_data) {
        ARTISTS_T artists_table = (ARTISTS_T)user_data;
        MUSIC music = (MUSIC)value;

        int duration = get_music_duration(music);

        char* composers = get_music_artist(music);

        process_update_duration(composers, duration, artists_table);

        free(composers);
}

void update_duration_for_each_artist(MUSICS_T tabela, void* artistas) {
    ARTISTS_T artists_table = artistas;
    GHashTable* musicas = tabela->musics;

    g_hash_table_foreach(musicas, update_duration_callback, artists_table);
}