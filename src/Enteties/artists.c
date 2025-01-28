#include "Enteties/artists.h"
#include "Main/dataval.h"
#include "Enteties/spotilixo.h"
#include "Enteties/artists_t.h"
#include "Enteties/musics.h"
#include <stdio.h>
#include <string.h>

struct artist{
    char *name;
    char *country;
    int totalDuration;
    char *type;
    char *recipePerStream;
    char *id_const;
};

static ARTIST create_artist(void) {

    ARTIST artist = malloc(sizeof(struct artist));

    artist->name=NULL;
    artist->country=NULL;
    artist->totalDuration=0;
    artist->type=NULL;
    artist->recipePerStream=NULL;
    artist->id_const=NULL;
    return artist;
}

static void set_artist_name(ARTIST artist, char *name){
    artist->name = strdup(name);
}

static void set_artist_country(ARTIST artist, char *country){
    artist->country = strdup(country);
}

void update_artist_totalDuration(ARTIST artist, int value){
    artist->totalDuration += value;
}

static void set_artist_type(ARTIST artist, char *type){
    artist->type = strdup(type);
}

static void set_artist_rps(ARTIST artist, char *rps){
    artist->recipePerStream = strdup(rps);
}

void set_artist_id_const(ARTIST artist, char *id_const){
    artist->id_const = strdup(id_const);
}

char* get_artist_name(ARTIST artist){
    return strdup(artist->name);
}

char* get_artist_country(ARTIST artist){
    return strdup(artist->country);
}

int get_artist_totalDuration(ARTIST artist){
    return artist->totalDuration;
}

char* get_artist_type(ARTIST artist){
    return strdup(artist->type);
}

char* get_artist_rps(ARTIST artist){
    return strdup(artist->recipePerStream);
}

char* get_artist_id_const(ARTIST artist){
    return strdup(artist->id_const);
}

void free_artist(ARTIST artist) {
    free(artist->name);
    free(artist->country);
    free(artist->type);
    free(artist->recipePerStream);
    free(artist->id_const);

    free(artist);
}

static int verify_artist(char** fields){
    if (!(fields[0]) || !(fields[1]) || !(fields[2]) ||
        !(fields[3]) || !(fields[4]) || !(fields[5]) ||
        !(fields[6])) return 0;

    if(!validate_id_constituent(fields[4], fields[6])) return 0;

    return 1;
}

int build_artist(char **artist_fields, void *data){
    void* artistas = get_spotilixo_artistas(data);

    for (int i = 0; i < 7; i++) {
        if (i == 2) continue;
        remove_char(artist_fields[i]);
    }

    if (!verify_artist(artist_fields)) return 0;

    //"id";"name";"description";"recipe_per_stream";"id_constituent";"country";"type"
    ARTIST artist = create_artist();

    set_artist_name(artist, artist_fields[1]);
    set_artist_country(artist,artist_fields[5]);
    set_artist_type(artist,artist_fields[6]);
    set_artist_rps(artist, artist_fields[3]);
    set_artist_id_const(artist, artist_fields[4]);
    
    insert_artista(artist, artistas, artist_fields[0]);

    return 1;
}
