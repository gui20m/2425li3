#include "Enteties/albums_t.h"

struct albumstable{
    GHashTable* albumstab;
};

ALBUMS_T create_albums_t(void){
    ALBUMS_T new = malloc(sizeof(struct albumstable));
    if (!new){
        fprintf(stderr, "Falha ao alocar memória para musics table.\n");
        return NULL;
    }

    new->albumstab = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)free_album);

    return new;
}


void insert_album(void* album, ALBUMS_T table, char* key){
    char* key_copy = strdup(key);
    g_hash_table_insert(table->albumstab, key_copy, album);
}

void* search_album(ALBUMS_T albums, char* key){
    return g_hash_table_lookup(albums->albumstab, key);
}

void free_albums_table(ALBUMS_T albums){
    g_hash_table_destroy(albums->albumstab);
    free(albums);
}
