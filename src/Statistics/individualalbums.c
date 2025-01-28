#include "Statistics/individualalbums.h"
#include "Main/utils.h"

struct individualalbum {
    GHashTable* albumMapping;
};

INDIVIDUAL_ALBUM create_individualalbum(void){
    INDIVIDUAL_ALBUM new = malloc(sizeof(struct individualalbum));
    if (!new){
        fprintf(stderr, "Falha ao alocar memória para artists table.\n");
        return NULL;
    }

    new->albumMapping = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);

    return new;
}

int search_numAlbum(INDIVIDUAL_ALBUM table, char* key){
    return GPOINTER_TO_INT(g_hash_table_lookup(table->albumMapping, key));
}

void free_individualalbum(INDIVIDUAL_ALBUM table) {
    g_hash_table_destroy(table->albumMapping);
    free(table);
}

void update_ind_album(INDIVIDUAL_ALBUM table, char* key) {
    int valor = search_numAlbum(table, key);
    if (!valor) g_hash_table_insert(table->albumMapping, strdup(key), GINT_TO_POINTER(1));
    g_hash_table_replace(table->albumMapping, strdup(key), GINT_TO_POINTER(valor+1));
}
