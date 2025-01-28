#include "Statistics/algorithm.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Statistics/recomendador.h"

struct algorithm {
    int **matriz;
    char **generos;
    char **users;
    int num_users;
    int num_generos;
};

ALGORITHM create_algorithm(void) {
    ALGORITHM new = malloc(sizeof(struct algorithm));
    new->matriz = NULL;
    new->generos = NULL;
    new->users = NULL;
    new->num_users = 0;
    new->num_generos = 0;

    return new;
}

void free_algorithm(ALGORITHM data) {
    for (int i = 0; i < data->num_users; i++) {
        free(data->matriz[i]);
    }
    for (int i = 0; i < data->num_generos; i++) {
        free(data->generos[i]);
    }
    for (int i = 0; i < data->num_users; i++) {
        free(data->users[i]);
    }
    free(data->matriz);
    free(data->generos);
    free(data->users);
    free(data);
}

static int find_genre_index(char **array, int size, char *value) {
    for (int i = 0; i < size; i++) {
        if (strcmp(array[i], value) == 0) {
            return i;
        }
    }
    return -1;
}

static void expand_generos(ALGORITHM data, char *genre) {
    data->generos = realloc(data->generos, (data->num_generos + 1) * sizeof(char *));
    data->generos[data->num_generos] = strdup(genre);
    data->num_generos++;

    for (int i = 0; i < data->num_users; i++) {
        data->matriz[i] = realloc(data->matriz[i], data->num_generos * sizeof(int));
        for (int j = 0; j < data->num_generos; j++) {
            if (j >= data->num_generos - 1) {
                data->matriz[i][j] = 0;
            }
        }
    }
}

static char* generate_user_id(int user_index) {
    char *user_id = malloc(13 * sizeof(char));
    snprintf(user_id, 13, "U%07d", user_index);
    return user_id;
}

static void expand_users(ALGORITHM data, int user_id, char *user) {
    if (user_id >= data->num_users) {
        data->users = realloc(data->users, (user_id + 1) * sizeof(char *));
        data->matriz = realloc(data->matriz, (user_id + 1) * sizeof(int *));
        for (int i = data->num_users; i <= user_id; i++) {
            data->users[i] = generate_user_id(i+1);
            data->matriz[i] = malloc(data->num_generos * sizeof(int));
            for (int j = 0; j < data->num_generos; j++) {
                data->matriz[i][j] = 0;
            }
        }
        data->num_users = user_id + 1;
    }

    if (data->users[user_id] == NULL) {
        data->users[user_id] = strdup(user);
    }
}

void insert_matriz_user(ALGORITHM data, char *user, char *genre) {
    int user_id = atoi(user + 1) - 1;

    int genre_index = find_genre_index(data->generos, data->num_generos, genre);
    if (genre_index == -1) {
        expand_generos(data, genre);
        genre_index = find_genre_index(data->generos, data->num_generos, genre);
    }

    expand_users(data, user_id, user);

    data->matriz[user_id][genre_index]++;
}

void expand_matriz(ALGORITHM data, int num_total) {
    if (num_total > data->num_users) {
        data->users = realloc(data->users, num_total * sizeof(char *));
        data->matriz = realloc(data->matriz, num_total * sizeof(int *));
        
        for (int i = data->num_users; i < num_total; i++) {
            data->users[i] = generate_user_id(i+1);
            data->matriz[i] = malloc(data->num_generos * sizeof(int));
            for (int j = 0; j < data->num_generos; j++) {
                data->matriz[i][j] = 0;
            }
        }
        data->num_users = num_total;
    }
    else if (num_total < data->num_users) {
        for (int i = num_total; i < data->num_users; i++) {
            free(data->users[i]);
            free(data->matriz[i]);
        }
        data->users = realloc(data->users, num_total * sizeof(char *));
        data->matriz = realloc(data->matriz, num_total * sizeof(int *));
        data->num_users = num_total;
    }
}

char** process_recommended_users(ALGORITHM data, char* user_id, int num_recomendados){
    return recomendaUtilizadores( user_id,
                                data->matriz, 
                                data->users,
                                data->generos,
                                data->num_users,
                                data->num_generos,
                                num_recomendados);
}
