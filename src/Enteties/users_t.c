#include "Enteties/users_t.h"

#include <stdio.h>

struct users_table {
    GHashTable* users;
};

USERS_T create_users_t(void){
    USERS_T new = malloc(sizeof(struct users_table));
    if (!new){
        fprintf(stderr, "Falha ao alocar memória para users table.\n");
        return NULL;
    }

    new->users = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)free_user);

    return new;
}

void insert_user(void* user, USERS_T table, char* key){
    char* key_copy = strdup(key);
    g_hash_table_insert(table->users, key_copy, user);
}

void* search_user(USERS_T table, char* key){
    return g_hash_table_lookup(table->users, key);
}

void free_users_table(USERS_T table){
    g_hash_table_destroy(table->users);
    free(table);
}

int users_t_size(USERS_T data){
    return (int)g_hash_table_size(data->users);
}
