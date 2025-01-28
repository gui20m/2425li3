#include "Enteties/albums.h"
#include "Main/dataval.h"
#include <string.h>
#include <ctype.h>

struct album{
    char *artists_id;
};

static ALBUM create_album(void) {

    ALBUM album = malloc(sizeof(struct album));

    album->artists_id=NULL;

    return album;
}

static void set_album_artists_id(ALBUM album, char *name){
    album->artists_id = strdup(name);
}

char* get_album_artists(ALBUM album){
    return strdup(album->artists_id);
}

void free_album(ALBUM album) {
    free(album->artists_id);

    free(album);
}

static int verify_album(char** fields){
    if (!(fields[0]) || !(fields[1]) || !(fields[2]) ||
        !(fields[3]) || !(fields[4])) return 0;

    if(!validate_array(fields[2])) return 0;

    return 1;
}

static void process_album_artists(char *input, void* ind_albums, void* artistas) {
    int len = strlen(input);
    if (len < 2 || input[0] != '[' || input[len - 1] != ']') {
        return;
    }

    const char* start = input + 1;
    const char* end = input + len - 2;  

    const char* token_start = start;
    for (const char* p = start; p <= end; p++) {
        if (*p == ',' || p == end) {
            int token_length = p - token_start;
            if (p == end && *p != ',') {
                token_length++;
            }

            char token[token_length + 1];
            strncpy(token, token_start, token_length);
            token[token_length] = '\0';

            char* token_ptr = token;
            while (*token_ptr && isspace((unsigned char)*token_ptr)) {
                token_ptr++;
            }
            memmove(token, token_ptr, strlen(token_ptr) + 1);
            int end_idx = strlen(token) - 1;
            while (end_idx >= 0 && isspace((unsigned char)token[end_idx])) {
                token[end_idx] = '\0';
                end_idx--;
            }

            ARTIST artista = search_artist(artistas, token);
            if (!artista) return;

            char *type = get_artist_type(artista);
            update_ind_album(ind_albums, token);
            free((char*)type);

            token_start = p + 1;
        }
    }

    return;
}

int build_album(char **album_fields, void *data){
    void* albums = get_spotilixo_albums(data);
    ARTISTS_T artistas = get_spotilixo_artistas(data);
    STATISTICS estatisticas = get_spotilixo_estatisticas(data);
    INDIVIDUAL_ALBUM ind_albums = get_statistics_ind_album(estatisticas);

    for (int i = 0; i < 5; i++) {
        if (i == 1 || i == 3 || i == 4) continue;
        remove_char(album_fields[i]);
    }

    if (!verify_album(album_fields)) return 0;

    //"id";"title";"artist_id";"year";"producers"
    ALBUM album = create_album();

    set_album_artists_id(album, album_fields[2]);

    insert_album(album, albums, album_fields[0]);

    process_album_artists(album_fields[2], ind_albums, artistas);

    return 1;
}