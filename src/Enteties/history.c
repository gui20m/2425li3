#include "Enteties/history.h"
#include "Main/dataval.h"
#include "Enteties/history_t.h"
#include "Statistics/annualsummary.h"

struct history{
    char *music_id;
};

static HISTORY create_history(void) {

    HISTORY history = malloc(sizeof(struct history));

    history->music_id=NULL;

    return history;
}

static void set_history_music_id(HISTORY history, char* key){
    history->music_id = strdup(key);
}

char* get_history_music_id(HISTORY history){
    return strdup(history->music_id);
}

void free_history(HISTORY history) {
    free(history->music_id);

    free(history);
}

static int verify_history(char** fields, void* data){
    if (!(fields[0]) || !(fields[1]) || !(fields[2]) ||
        !(fields[3]) || !(fields[4]) || !(fields[5])) return 0;

    USERS_T users = get_spotilixo_users(data);

    if(!validate_user(fields[1], users)) return 0;

    MUSICS_T musics = get_spotilixo_musicas(data);

    if(!validate_music(fields[2], musics)) return 0;

    if(!validate_plataform(fields[5])) return 0;

    return 1;
}

int build_history(char **history_fields, void *data){

    HISTORY_T table = get_spotilixo_history(data);
    MUSICS_T musics = get_spotilixo_musicas(data);
    ARTISTS_T artistas = get_spotilixo_artistas(data);

    for (int i = 0; i < 6; i++) {
        remove_char(history_fields[i]);
    }

    if (!verify_history(history_fields, data)) return 0;

    //"id";"user_id";"music_id";"timestamp";"duration";"platform"
    HISTORY history = create_history();

    set_history_music_id(history, history_fields[2]);

    insert_history(history, table, history_fields[0]);

    views_counting(history_fields[2], data);

    MUSIC music = search_music(musics, history_fields[2]);
    char* artists_id = get_music_artist(music);

    STATISTICS estatisticas = get_spotilixo_estatisticas(data);
    LEADERBOARD leaderboard = get_statistics_leaderboard(estatisticas);
    SUMMARY annualsummary = get_statistics_annualsummary(estatisticas);

    int duration = calculate_duration(history_fields[4]);

    weekly_duration_update(leaderboard, artistas, artists_id, history_fields[3], duration);

    ALGORITHM algorithm = get_statistics_algorithm(estatisticas);

    char* genre = get_music_genre(music);
    insert_matriz_user(algorithm, history_fields[1], genre);
    free(genre);

    process_annualsummary(annualsummary, history_fields[1], history_fields[2], history_fields[3], duration, musics, artistas);

    free((char*)artists_id);

    return 1;
}
