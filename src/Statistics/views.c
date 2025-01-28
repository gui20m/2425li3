#include "Statistics/views.h"
#include <string.h>
#include <ctype.h>
#include <math.h>

struct views {
    GHashTable* countplays;
    GHashTable* soloartist; // artista -> receita por stream acumulada (+receipt per stream)
    GHashTable* bandartist; // artistas -> receita por stream acumulada (+(rps/artistas)
    GHashTable* collabsong;
};

VIEWS create_views(void){
    VIEWS new = malloc(sizeof(struct views));
    if (!new){
        fprintf(stderr, "Falha ao alocar memória para artists table.\n");
        return NULL;
    }

    new->countplays = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
    new->soloartist = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
    new->bandartist = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
    new->collabsong = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);

    return new;
}

static int* search_countPlays(VIEWS table, char* key){
    return g_hash_table_lookup(table->countplays, key);
}

float* search_soloArtistRecipe(VIEWS table, char* key){
    return g_hash_table_lookup(table->soloartist, key);
}

float* search_bandArtistRecipe(VIEWS table, char* key){
    return g_hash_table_lookup(table->bandartist, key);
}

float* search_collabSongRecipe(VIEWS table, char* key){
    return g_hash_table_lookup(table->collabsong, key);
}

void free_views(VIEWS table) {
    g_hash_table_destroy(table->countplays);
    g_hash_table_destroy(table->soloartist);
    g_hash_table_destroy(table->bandartist);
    g_hash_table_destroy(table->collabsong);
    free(table);
}

void views_counting(char* music_id, void* data){
    STATISTICS estatisticas = get_spotilixo_estatisticas(data);
    VIEWS views = get_statistics_views(estatisticas);

    int* likes = search_countPlays(views, music_id);
    if (!likes) {
        int* likes_ptr = g_new(int, 1);
        *likes_ptr = 1;
        g_hash_table_insert(views->countplays, strdup(music_id), likes_ptr);
    } else {
        (*likes)++;
    }
    
}

static void process_song_artists_recipe(int* num_views, char* input, void* artistas, VIEWS views) {
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
            while (*token_ptr && isspace((unsigned char)*token_ptr)) token_ptr++;
            memmove(token, token_ptr, strlen(token_ptr) + 1);

            int end_idx = strlen(token) - 1;
            while (end_idx >= 0 && isspace((unsigned char)token[end_idx])) {
                token[end_idx--] = '\0';
            }

            ARTIST artist = search_artist(artistas, token);
            if (!artist) {
                token_start = p + 1;
                continue;
            }

            char* rps = get_artist_rps(artist);
            char* type = get_artist_type(artist);

            if (!rps || !type) {
                free(rps);
                free(type);
                token_start = p + 1;
                continue;
            }

            float adjusted_rps = atof(rps);
            if (num_views) {
                adjusted_rps *= *num_views;
            }
            
            if (strcasecmp(type, "individual") == 0 || strcasecmp(type, "group") == 0) {
                float* valor = g_hash_table_lookup(views->soloartist, token);

                if (valor) {
                    adjusted_rps += *valor;
                }

                float* new_rps_ptr = malloc(sizeof(float));
                if (new_rps_ptr) {
                    *new_rps_ptr = adjusted_rps;
                    g_hash_table_insert(views->soloartist, strdup(token), new_rps_ptr);
                } else {
                    fprintf(stderr, "Erro ao alocar memória para novo valor de RPS.\n");
                }
            }

            if (strcasecmp(type, "group") == 0) {
                char* id_const = get_artist_id_const(artist);
                if (id_const) {
                    char** split_id_const = split_array(id_const);
                    if (split_id_const) {
                        int num_id_const = count_array_elements(id_const);

                        for (int z = 0; split_id_const[z] != NULL; z++) {
                            float* bandR_ptr = search_bandArtistRecipe(views, split_id_const[z]);
                            float bandR = (bandR_ptr != NULL) ? *bandR_ptr : 0;

                            float final_rps = atof(rps)*(*num_views) / num_id_const + bandR;

                            float* final_rps_ptr = malloc(sizeof(float));
                            if (final_rps_ptr) {
                                *final_rps_ptr = final_rps;
                                g_hash_table_insert(views->bandartist, strdup(split_id_const[z]), final_rps_ptr);
                            } else {
                                fprintf(stderr, "Erro ao alocar memória para valor final de RPS para bandartist.\n");
                            }
                        }
                        free_tokens(split_id_const);
                    }

                    free(id_const);
                }
            }

            free(rps);
            free(type);

            token_start = p + 1;
        }
    }
}

void process_artists_recipe(VIEWS table, void* artistas, void* musicas){
    GHashTable* plays = table->countplays;

    GList* keys = g_hash_table_get_keys(plays);

    for (GList* l = keys; l != NULL; l = l->next) {
        char* song_id = l->data;
        int* num_views = g_hash_table_lookup(plays, song_id);
        MUSIC music = search_music(musicas, song_id);
        if (!music) return;
        char* artists = get_music_artist(music);
        if (num_views){
            process_song_artists_recipe(num_views, artists, artistas, table);
        }
        free((char*)artists);
    }

    g_list_free(keys);
}
