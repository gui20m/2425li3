#include "Main/queries.h"
#include "Enteties/users.h"
#include <stdio.h>
#include "Enteties/spotilixo.h"
#include "Enteties/artists.h"
#include "Enteties/users.h"
#include "Main/utils.h"
#include "Enteties/users.h"
#include <ctype.h>

void* queryOne(SPOTILIXO data, char** args, int N, char* separador) {
    char *id = args[0];
    USERS_T users_table = get_spotilixo_users(data);
    USER user = search_user(users_table, id);

    if (user){
        const char* email = get_user_email(user);
        const char* first_name = get_user_first_name(user);
        const char* last_name = get_user_last_name(user);
        int age = get_user_age(user);
        const char* country = get_user_country(user);

        int size = strlen(email) + strlen(first_name) + strlen(last_name) + snprintf(NULL, 0, "%d", age) + strlen(country) + 5 + 1;
        char* result = malloc(size);

        snprintf(result, size, "%s%s%s%s%s%s%d%s%s", email, separador, first_name, separador, last_name, separador, age, separador, country);

        free((char*)email);
        free((char*)first_name);
        free((char*)last_name);
        free((char*)country);

        return result;
    }

    ARTISTS_T artists_table = get_spotilixo_artistas(data);

    ARTIST artist = search_artist(artists_table, id);

    if (artist) {

        STATISTICS estatisticas = get_spotilixo_estatisticas(data);
        INDIVIDUAL_ALBUM albums = get_statistics_ind_album(estatisticas);
        VIEWS views = get_statistics_views(estatisticas);

        const char* name = get_artist_name(artist);
        const char* type = get_artist_type(artist);
        const char* country = get_artist_country(artist);
        int num_album = search_numAlbum(albums, id);
        float final = 0;

        float* soloR = search_soloArtistRecipe(views, id);
        float* bandR = search_bandArtistRecipe(views, id);
        float* collabR = search_collabSongRecipe(views, id);

        if (soloR) final += *soloR;
        if (bandR) final += *bandR;
        if (collabR) final += *collabR;

        int size = strlen(name) + strlen(type) + strlen(country) + 
                snprintf(NULL, 0, "%d", num_album) + 
                snprintf(NULL, 0, "%f", final) +
                3 * strlen(separador) + 1;

        char* result = malloc(size);
        if (!result) {
            fprintf(stderr, "Erro ao alocar memória\n");
            exit(EXIT_FAILURE);
        }

        final += 0.00005;

        snprintf(result, size, "%s%s%s%s%s%s%d%s%.2f", name, separador, type, separador, country, separador, num_album, separador, final);

        free((char*)name);
        free((char*)type);
        free((char*)country);

        return result;
    }

    return strdup("");
}
 
void* queryTwoNoCountry(SPOTILIXO data, int count, char* separador) {
    STATISTICS estatisticas = get_spotilixo_estatisticas(data);
    TOPARTISTS topartists = get_top_artists(estatisticas);

    if (count <= 0) return strdup("\n");

    int estimated_size = count * 128;
    char *output = malloc(estimated_size * sizeof(char));
    output[0] = '\0';

    GList *top_artists_copy = get_top_artists_list(topartists);

    GList *current = top_artists_copy;

    while (current != NULL && count > 0) {
        ARTIST artist = (ARTIST)current->data;

        if (artist != NULL) {
            char artist_info[128];

            const char* name = get_artist_name(artist);
            const char* type = get_artist_type(artist);
            char* discographyDuration = convert_seconds_to_time(get_artist_totalDuration(artist));
            const char* country = get_artist_country(artist);

            snprintf(artist_info, sizeof(artist_info), "%s%s%s%s%s%s%s\n", name, separador, type, separador, discographyDuration, separador, country);

            strcat(output, artist_info);

            free(discographyDuration);
            free((char*)name);
            free((char*)type);
            free((char*)country);
        }

        current = current->next;
        count--;
    }

    g_list_free(top_artists_copy);
    return output;
}

void* queryTwo(SPOTILIXO data, char** args, int N, char* separador) {
    if (N!=2) return queryTwoNoCountry(data, atoi(args[0]), separador);
    if (strcmp(args[0],"0")==0) return strdup("\n");
    char *artistNumber = args[0];
    int count = atoi(artistNumber);
    char *country_filter = args[1];

    int estimated_size = count * 128;
    char *output = malloc(estimated_size * sizeof(char));
    output[0] = '\0';

    if (!output) {
        perror("Erro ao alocar memória para output");
        return strdup("\n");
    }

    ARTISTS_T artistas = get_spotilixo_artistas(data);

    GList *artists_in_country = find_artists_by_country(artistas, country_filter);
    
    for (GList *l = artists_in_country; l != NULL && count; l = l->next) {
        ARTIST artist = (ARTIST)l->data;

        if (artist!=NULL){

            char artist_info[128];
            
            const char* name = get_artist_name(artist);
            const char* type = get_artist_type(artist);
            char* discographyDuration = convert_seconds_to_time(get_artist_totalDuration(artist));

            snprintf(artist_info, sizeof(artist_info), "%s%s%s%s%s%s%s\n", name, separador ,type, separador ,discographyDuration, separador ,country_filter);

            strcat(output, artist_info);

            free((char*)name);
            free(discographyDuration);
            free((char*)type);

            count--;
        }
    }
    g_list_free(artists_in_country);

    return output;
}

void* queryThree(SPOTILIXO data, char** args, int N, char* separador) {
    char* min_age_char = args[0];
    char* max_age_char = args[1];
    if (!min_age_char && !max_age_char) return strdup("\n");

    int min_age = atoi(min_age_char);
    int max_age = atoi(max_age_char);

    STATISTICS statistics = get_spotilixo_estatisticas(data);
    AGEDATA agedata = get_statistics_agedata(statistics);

    int estimated_size = 10 * 64;
    char *output = malloc(estimated_size * sizeof(char));
    output[0] = '\0';

    char result[128];

    GList* genre_list = sum_genre_likes(agedata, min_age, max_age);

    for (GList* l = genre_list; l != NULL; l = l->next) {
        GenreLikes gl = (GenreLikes)l->data;
        snprintf(result, sizeof(result), "%s%s%d\n", get_genreLikes_genre(gl),separador, get_genreLikes_likes(gl));
        strcat(output, result);
    }

    for (GList* l = genre_list; l != NULL; l = l->next) {
        GenreLikes gl = (GenreLikes)l->data;
        free_genreLikes(gl);
    }

    g_list_free(genre_list);

    return output;
}

void* queryFour(SPOTILIXO data, char** args, int N, char* separador) {

    STATISTICS estatisticas = get_spotilixo_estatisticas(data);
    ARTISTS_T artistas = get_spotilixo_artistas(data);
    LEADERBOARD leaderboard = get_statistics_leaderboard(estatisticas);

    int estimated_size = 10 * 64;
    char *output = malloc(estimated_size * sizeof(char));
    output[0] = '\0';

    char result[128];

    if (N==0){
        char ** info = get_most_played_artist(leaderboard);
        if (info[0]==NULL && info[1]==NULL) return strdup("\n");

        ARTIST artist = search_artist(artistas, info[0]);
        char * type = get_artist_type(artist);

        snprintf(result, sizeof(result), "%s%s%s%s%s\n", info[0], separador, type, separador, info[1]);
        strcat(output, result);

        free(type);
        free(info[1]);
        free(info);
    }

    if (N==2){
        int year1, mes1, dia1, year2, mes2, dia2;
        sscanf(args[0], "%d/%d/%d", &year1, &mes1, &dia1);
        sscanf(args[1], "%d/%d/%d", &year2, &mes2, &dia2);

        char ** info = get_most_played_artist_between_dates(leaderboard, year1, mes1, dia1, year2, mes2, dia2);
        if (info[0]==NULL && info[1]==NULL) {
            free(info);
            free(output);
            return strdup("\n");
        }

        ARTIST artist = search_artist(artistas, info[0]);
        char * type = get_artist_type(artist);

        snprintf(result, sizeof(result), "%s%s%s%s%s\n", info[0], separador, type, separador, info[1]);
        strcat(output, result);

        free(info[1]);
        free(info);
        free(type);
    }

    
    return output;
}

void* queryFive(SPOTILIXO data, char** args, int N, char* separador) {
    if (N!=2) return strdup("\n");

    USERS_T users = get_spotilixo_users(data);
    
    char* user_id = args[0];
    USER user = search_user(users, user_id);
    if (!user) return strdup("\n");
    char* number_users = args[1];
    if (atoi(number_users)==0) return strdup("\n");

    STATISTICS estatisticas = get_spotilixo_estatisticas(data);
    ALGORITHM algorithm = get_statistics_algorithm(estatisticas);

    char** results = process_recommended_users(algorithm, user_id, atoi(number_users));
    if (!results) return strdup("\n");

    int estimated_size = atoi(number_users) * 64;
    char *output = malloc(estimated_size * sizeof(char));
    output[0] = '\0';

    char result[128];

    for (int i = 0; i < atoi(number_users) && results[i]; i++) {
        snprintf(result, sizeof(result), "%s\n", results[i]);
        strcat(output, result);
    }

    free(results);

    return output;
}

void* querySix(SPOTILIXO data, char** args, int N, char* separador) {

    STATISTICS estatisticas = get_spotilixo_estatisticas(data);
    SUMMARY annualsummary = get_statistics_annualsummary(estatisticas);

    char * user_id = args[0];
    char * ano = args[1];

    int estimated_size = 10 * 64;
    char *output = malloc(estimated_size * sizeof(char));
    output[0] = '\0';

    char result[128];

    int listeningtime = get_annualsummary_listeningtime(annualsummary, user_id, ano);
    int numplays = get_annualsummary_numplays(annualsummary, user_id, ano);
    char* artist_id = get_annualsummary_mostplayed_artist(annualsummary, user_id, ano);
    char* mostplayedday = get_annualsummary_mostplayed_day(annualsummary, user_id, ano);
    char* mostplayedgenre = get_annualsummary_mostplayed_genre(annualsummary, user_id, ano);
    char* mostplayedalbum = get_annualsummary_mostplayed_album(annualsummary, user_id, ano);
    char* favouritehour = get_annualsummary_favouritehour(annualsummary, user_id, ano);
    char* time = convert_seconds_to_time(listeningtime);

    if (listeningtime!=-1 && numplays!=-1 && artist_id && mostplayedday && mostplayedgenre && mostplayedalbum && favouritehour){
        snprintf(result, sizeof(result), "%s%s%d%s%s%s%s%s%s%s%s%s%s\n", time, separador, numplays, separador, artist_id, separador, mostplayedday, separador, mostplayedgenre, separador, mostplayedalbum, separador, favouritehour);
        strcat(output, result);
    } else {
        if (artist_id) free(artist_id);
        if (mostplayedday) free(mostplayedday);
        if (mostplayedgenre) free(mostplayedgenre);
        if (mostplayedalbum) free(mostplayedalbum);
        if (favouritehour) free(favouritehour);
        if (time) free(time);
        free(output);
        return strdup("\n");
    }

    if (N == 3) {
        int i=0;
        GList* lista = get_annualsummary_favouriteartists(annualsummary, user_id, ano, atoi(args[2]));
        if (lista) {
            for (GList* iter = lista; iter != NULL && i<atoi(args[2]); iter = iter->next) {
                ARTISTSTATS entry = (ARTISTSTATS) iter->data;

                char *name = get_artiststats_id(entry);
                int numsongs = get_artiststats_numsongs(entry);
                int listeningtime_sub_artist = get_artiststats_listeningtime(entry);
                char *timeart = convert_seconds_to_time(listeningtime_sub_artist);

                char topartists[128];
                snprintf(topartists, sizeof(topartists), "%s%s%d%s%s\n", name, separador, numsongs, separador, timeart);

                strcat(output, topartists);
                free(timeart);
                free(name);
                i++;
            }
            free_artiststats_list(lista);
        }
    }

    free(artist_id);
    free(mostplayedday);
    free(mostplayedgenre);
    free(mostplayedalbum);
    free(favouritehour);
    free(time);

    return output;
}
