#include "Enteties/history_t.h"

struct historytable{
    GHashTable* history;
};

HISTORY_T create_history_t(void){
    HISTORY_T new = malloc(sizeof(struct historytable));
    if (!new){
        fprintf(stderr, "Falha ao alocar memória para musics table.\n");
        return NULL;
    }

    new->history = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)free_history);

    return new;
}

void insert_history(void* history, HISTORY_T table, char* key){
    char* key_copy = strdup(key);
    g_hash_table_insert(table->history, key_copy, history);
}

void free_history_table(HISTORY_T table){
    g_hash_table_destroy(table->history);
    free(table);
}