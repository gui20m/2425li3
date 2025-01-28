#include "Enteties/musics.h"
#include "Main/dataval.h"
#include "Main/utils.h"
#include <glib.h>

struct music {
    char* album_id;
    char *artist;
    int duration;
    char *genre;
};

static MUSIC create_music(void) {
    MUSIC music = malloc(sizeof(struct music));

    music->album_id = NULL;
    music->artist = NULL;
    music->duration = 0;
    music->genre = NULL;
    return music;
}

static void set_music_album_id(MUSIC music, char *album_id){
    music->album_id = strdup(album_id);
}

static void set_music_artist(MUSIC music, char *artist){
    music->artist = strdup(artist);
}

static void set_music_duration(MUSIC music, int value){
    music->duration = value;
}

static void set_music_genre(MUSIC music, char *genre){
    music->genre = strdup(genre);
}

char* get_music_artist(MUSIC music){
    return strdup(music->artist);
}

int get_music_duration(MUSIC music){
    return music->duration;
}

char* get_music_genre(MUSIC music){
    return strdup(music->genre);
}

char* get_music_album(MUSIC music){
    return strdup(music->album_id);
}

void free_music(MUSIC music) {
    free(music->album_id);
    free(music->artist);
    free(music->genre);

    free(music);
}

static int verify_music(char** fields, void* data){

    ARTISTS_T artists_table = get_spotilixo_artistas(data);
    ALBUMS_T albums_table = get_spotilixo_albums(data);

    if (!(fields[0]) || !(fields[1]) || !(fields[2]) ||
        !(fields[3]) || !(fields[4]) || !(fields[5]) ||
        !(fields[6]) || !(fields[7])) return 0;

    if (!validate_array(fields[2])) return 0;

    if(!validate_artists_id(fields[2], artists_table)) return 0;

    if (!validate_album_id(fields[3], albums_table)) return 0;

    if(!validate_duration(fields[4])) return 0;

    return 1;
}

int build_music(char **user_fields, void *data){

    MUSICS_T musicas = get_spotilixo_musicas(data);

    for (int i = 0; i < 8; i++) {
        if (i == 1 || i == 6 || i == 7) continue;
        remove_char(user_fields[i]);
    }

    if (!verify_music(user_fields, data)) return 0;

    //"id";"title";"artist_id";"album_id";"duration";"genre";"year";"lyrics"
    MUSIC music = create_music();
    int duration = calculate_duration(user_fields[4]);

    set_music_album_id(music, user_fields[3]);
    set_music_artist(music, user_fields[2]);
    set_music_duration(music, duration);
    set_music_genre(music,user_fields[5]);

    insert_musica(music, musicas, user_fields[0]);

    return 1;
}