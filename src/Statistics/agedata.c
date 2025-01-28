#include "Statistics/agedata.h"

struct agedata {
    GHashTable* agedata;
};

struct genreLikes {
    char* genre;
    int likes;
};

GenreLikes create_genreLikes(const char* genre, int likes) {
    GenreLikes new_genre = malloc(sizeof(struct genreLikes));
    if (!new_genre) {
        fprintf(stderr, "Erro ao alocar memória para GenreLikes.\n");
        return NULL;
    }
    new_genre->genre = strdup(genre);
    new_genre->likes = likes;
    return new_genre;
}

char* get_genreLikes_genre(GenreLikes gl) {
    return gl->genre;
}

int get_genreLikes_likes(GenreLikes gl){
    return gl->likes;
}

AGEDATA create_agedata(void){
    AGEDATA new = malloc(sizeof(struct agedata));
    if (!new){
        fprintf(stderr, "Falha ao alocar memória para artists table.\n");
        return NULL;
    }

    new->agedata = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)free_genretable);

    return new;
}

void insert_ageMapGenre(GHashTable* genretable, AGEDATA table, char* key){
    g_hash_table_insert(table->agedata, key, genretable);
}

GHashTable* search_ageMapGenre(AGEDATA table, char* key){
    return g_hash_table_lookup(table->agedata, key);
}

gpointer genre_lookup(AGEDATA data, char* key){
    return g_hash_table_lookup(data->agedata, key);
}

void free_agedata(AGEDATA table) {
    g_hash_table_destroy(table->agedata);
    free(table);
}

void free_genretable(gpointer data) {
    GHashTable* agedata = (GHashTable*) data;
    g_hash_table_destroy(agedata);
}

void free_genreLikes(gpointer data) {
    GenreLikes gl = (GenreLikes)data;
    free(gl->genre);
    free(gl);
}

void increment_genreLikes_likes(GenreLikes gl, int likes) {
    if (gl) {
        gl->likes += likes;
    }
}

GenreLikes find_or_add_genre(GList** genre_list, const char* genre, int likes) {
    for (GList* l = *genre_list; l != NULL; l = l->next) {
        GenreLikes gl = (GenreLikes)l->data;
        if (strcmp(get_genreLikes_genre(gl), genre) == 0) {
            increment_genreLikes_likes(gl, likes);
            return gl;
        }
    }
    GenreLikes new_genre = create_genreLikes(genre, likes);
    *genre_list = g_list_append(*genre_list, new_genre);

    return new_genre;
}

void accumulate_genre_likes(gpointer key, gpointer value, gpointer user_data) {
    GList** genre_list = (GList**)user_data;
    const char* genre = (const char*)key;
    int likes = *((int*)value);
    find_or_add_genre(genre_list, genre, likes);
}

int compare_genre_likes(gconstpointer a, gconstpointer b) {
    GenreLikes genre_a = (GenreLikes) a;
    GenreLikes genre_b = (GenreLikes) b;

    if (genre_a->likes != genre_b->likes) {
        return genre_b->likes - genre_a->likes;
    }

    return strcmp(genre_a->genre, genre_b->genre);
}

void* sum_genre_likes(AGEDATA data, int min_age, int max_age) {
    GList* genre_list = NULL;

    for (int age = min_age; age <= max_age; age++) {
        char age_key[12];
        snprintf(age_key, sizeof(age_key), "%d", age);

        GHashTable* genre_likes = g_hash_table_lookup(data->agedata, age_key);

        if (genre_likes) {
            GList* genre_keys = g_hash_table_get_keys(genre_likes);

            for (GList* genre_node = genre_keys; genre_node != NULL; genre_node = genre_node->next) {
                const char* genre = (const char*)genre_node->data;
                int* likes = (int*)g_hash_table_lookup(genre_likes, genre);

                if (likes) {
                    find_or_add_genre(&genre_list, genre, *likes);
                }
            }

            g_list_free(genre_keys);
        }
    }

    return g_list_sort(genre_list, compare_genre_likes);
}
