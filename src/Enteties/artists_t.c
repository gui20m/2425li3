#include "Enteties/artists_t.h"
#include <stdio.h>
#include "Main/utils.h"
#include <glib.h>

struct artists_table {
    GHashTable* artists;
};

ARTISTS_T create_artists_t(void){
    ARTISTS_T new = malloc(sizeof(struct artists_table));
    if (!new){
        fprintf(stderr, "Falha ao alocar memória para artists table.\n");
        return NULL;
    }

    new->artists = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)free_artist);

    return new;
}

void insert_artista(void* artista, ARTISTS_T table, char* key){
    char* key_copy = strdup(key);
    g_hash_table_insert(table->artists, key_copy, artista);
}

void* search_artist(ARTISTS_T artistas, char* key){
    return g_hash_table_lookup(artistas->artists, key);
}

void free_artists_table(ARTISTS_T artistas){
    g_hash_table_destroy(artistas->artists);
    free(artistas);
}

GList* find_artists_by_country(ARTISTS_T artist_table, char *country_filter) {
    if (!country_filter) return NULL;
    
    GList *artist_in_country = NULL;
    GHashTable* tabela = artist_table->artists;

    char* country = country_filter;
    
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, tabela);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        ARTIST artist = (ARTIST)value;
        char* ar_country = get_artist_country(artist);
        if (strcmp(ar_country,country)==0) {
            artist_in_country = g_list_prepend(artist_in_country, artist);
        }
        free((char*)ar_country);
    }

    return g_list_sort(artist_in_country,compare_artists_by_time_desc);
}

int compare_artists_by_time_desc(gconstpointer a, gconstpointer b) {
    const ARTIST artist_a = (ARTIST)a;
    const ARTIST artist_b = (ARTIST)b;

    return get_artist_totalDuration(artist_b) - get_artist_totalDuration(artist_a);
}

GList* top_artists_by_duration(ARTISTS_T tabela) {
    GList *artist_list = g_hash_table_get_values(tabela->artists);

    return g_list_sort(artist_list, compare_artists_by_time_desc);
}