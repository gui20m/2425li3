#include "Interactive/info.h"
#include "Interactive/interactive.h"
#include "Main/interpreter.h"
#include "Enteties/spotilixo.h"
#include "Main/parser.h"
#include "Main/utils.h"
#include "Main/queries.h"
#include "Enteties/artists.h"
#include "Enteties/users.h"
#include "Enteties/musics.h"
#include "Enteties/history.h"
#include "Main/output.h"
#include "Main/queries.h"
#include <string.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_info(SETTINGS setts, char* datasets){
    FILE *users_file, *artists_file, *musics_file, *users_error_file, *musics_error_file, *artists_error_file, *albums_file, *albums_error_file, *history_file, *history_error_file;

    // Construir caminho do arquivo de usuários sem modificar `datasets` diretamente
    char user_path[128];
    snprintf(user_path, sizeof(user_path), "%s/users.csv", datasets);

    char artists_path[128];
    snprintf(artists_path, sizeof(artists_path), "%s/artists.csv", datasets);

    char musics_path[128];
    snprintf(musics_path, sizeof(musics_path), "%s/musics.csv", datasets);

    char albums_path[128];
    snprintf(albums_path, sizeof(albums_path), "%s/albums.csv", datasets);

    char history_path[128];
    snprintf(history_path, sizeof(history_path), "%s/history.csv", datasets);

    // Abrir o arquivo de usuários
    users_file = fopen(user_path, "r");
    if (!users_file) {
        fprintf(stderr, "Error: Unable to open %s\n", user_path);
        return 0;
    }


    artists_file = fopen(artists_path, "r");
    if (!users_file) {
        fprintf(stderr, "Error: Unable to open %s\n", artists_path);
        return 0;
    }

    albums_file = fopen(albums_path, "r");
    if (!albums_file) {
        fprintf(stderr, "Error: Unable to open %s\n", albums_path);
        return 0;
    }

    musics_file = fopen(musics_path, "r");
    if (!musics_file) {
        fprintf(stderr, "Error: Unable to open %s\n", musics_path);
        return 0;
    }

    history_file = fopen(history_path, "r");
    if (!history_file) {
        fprintf(stderr, "Error: Unable to open %s\n", history_path);
        return 0;
    }

    users_error_file = fopen("resultados/users_errors.csv", "w");
    if (!users_error_file) {
        fprintf(stderr, "Error: Unable to open users_errors.csv\n");
        fclose(users_file);
        fclose(musics_file);
        return 0;
    }

    artists_error_file = fopen("resultados/artists_errors.csv", "w");
    if (!artists_error_file) {
        fprintf(stderr, "Error: Unable to open artists_errors.csv\n");
        fclose(artists_file);
        return 0;
    }

    albums_error_file = fopen("resultados/albums_errors.csv", "w");
    if (!albums_error_file) {
        fprintf(stderr, "Error: Unable to open albums_errors.csv\n");
        fclose(albums_error_file);
        return 0;
    }

    musics_error_file = fopen("resultados/musics_errors.csv", "w");
    if (!musics_error_file) {
        fprintf(stderr, "Error: Unable to open musics_errors.csv\n");
        fclose(musics_file);
        return 0;
    }

    history_error_file = fopen("resultados/history_errors.csv", "w");
    if (!musics_error_file) {
        fprintf(stderr, "Error: Unable to open history_errors.csv\n");
        fclose(history_file);
        return 0;
    }

    SPOTILIXO data = create_spotilixo();
    parseF(artists_file, 7, build_artist, data, artists_error_file);
    parseF(albums_file, 5, build_album, data, albums_error_file);
    parseF(musics_file, 8, build_music, data, musics_error_file);
    parseF(users_file, 8, build_user, data, users_error_file);

    STATISTICS estatisticas = get_spotilixo_estatisticas(data);

    parseF(history_file, 6, build_history, data, history_error_file);

    fclose(users_file);
    fclose(history_file);
    fclose(artists_file);
    fclose(musics_file);
    fclose(albums_file);
    fclose(users_error_file);
    fclose(artists_error_file);
    fclose(musics_error_file);
    fclose(history_error_file);
    fclose(albums_error_file);


    ARTISTS_T artistas = get_spotilixo_artistas(data);
    MUSICS_T musicas = get_spotilixo_musicas(data);
    TOPARTISTS topartistas = get_top_artists(estatisticas);
    VIEWS views = get_statistics_views(estatisticas);

    update_duration_for_each_artist(musicas, artistas); // carrega os tempos todos de cada artista
    GList *top_artists = top_artists_by_duration(artistas); // ordena toda a informacao mal o programa executa (assim so tem de fazer 1x)
    set_top_artists(topartistas, top_artists);

    process_artists_recipe(views, artistas, musicas);

    LEADERBOARD leaderboard = get_statistics_leaderboard(estatisticas);
    process_top10_weekly(leaderboard);

    set_manager(setts, data);

    return 1;
}

int process_inputs(SETTINGS setts, char* inputsPath){
    FILE *input_file = fopen(inputsPath, "r");
    if (!input_file) {
        return 0;
    }

    return 1;
}

int process_data(SETTINGS setts){

    char* inputs = get_inputsPath(setts);
    SPOTILIXO data = get_manager(setts);

    FILE *input_file = fopen(inputs, "r");

    FILE* output_file;
    char *line = NULL;
    size_t lsize = 0;
    int cmd_n = 1;
    void* result;

    while (getline(&line, &lsize, input_file) != -1) {
        
        if (strlen(line) > 0) {
            int query_id = line[0] - '0';

            line[strlen(line) - 1] = '\0';

            result = parserQuery(data,line);

            output_file = create_output_file(cmd_n);

            if (result != NULL) {
                output_query(output_file, result, query_id);
                free(result);
            }
            fclose(output_file);
            cmd_n++;
        }
    }

    free(line);
    fclose(input_file);
    free(inputs);

    return 1;
}