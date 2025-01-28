#include "Enteties/spotilixo.h"
#include "Enteties/users.h"
#include "Enteties/artists.h"
#include "Enteties/musics.h"
#include <stdio.h>

struct spotilixo{
    ARTISTS_T artistas;
    MUSICS_T musicas;
    USERS_T users;
    ALBUMS_T albums;
    HISTORY_T history;
    STATISTICS estatisticas;
};

SPOTILIXO create_spotilixo(void){
    SPOTILIXO new = malloc(sizeof(struct spotilixo));
    if (!new) {
        fprintf(stderr, "Falha ao alocar memória para Spotilixo.\n");
        return NULL;
    }

    new->users = create_users_t();
    new->artistas = create_artists_t();
    new->musicas = create_musics_t();
    new->albums = create_albums_t();
    new->history = create_history_t();
    new->estatisticas = create_statistics();

    return new;
}

void* get_spotilixo_artistas(SPOTILIXO data){
    return data->artistas;
}

void* get_spotilixo_musicas(SPOTILIXO data){
    return data->musicas;
}

void* get_spotilixo_users(SPOTILIXO data){
    return data->users;
}

void* get_spotilixo_albums(SPOTILIXO data){
    return data->albums;
}

void* get_spotilixo_estatisticas(SPOTILIXO data){
    return data->estatisticas;
}

void* get_spotilixo_history(SPOTILIXO data){
    return data->history;
}

void free_spotilixo(SPOTILIXO data){
    free_artists_table(data->artistas);
    free_musics_table(data->musicas);
    free_users_table(data->users);
    free_albums_table(data->albums);
    free_history_table(data->history);
    free_statistics(data->estatisticas);
    free(data);
}