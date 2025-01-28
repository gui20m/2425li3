#include "Statistics/topartists.h"

struct topartists {
    GList* top_artists;
};

TOPARTISTS create_top_artists(void){
    TOPARTISTS new = malloc(sizeof(struct topartists));
    if (!new) {
        fprintf(stderr, "Erro ao alocar memória para topartists.\n");
        return NULL;
    }
    new->top_artists = NULL;
    return new;
}

GList* get_top_artists_list(TOPARTISTS data) {
    GList* copy = NULL;
    for (GList* node = data->top_artists; node != NULL; node = node->next) {
        copy = g_list_append(copy, node->data);
    }

    return copy;
}

void set_top_artists(TOPARTISTS data, GList* list){
    data->top_artists = list;
}

void free_top_artists(TOPARTISTS data) {
    g_list_free(data->top_artists);
    free(data);
}
