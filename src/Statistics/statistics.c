#include "Statistics/statistics.h"
#include "Statistics/agedata.h"

struct statistics {
    AGEDATA agedata;
    TOPARTISTS top_artists;
    INDIVIDUAL_ALBUM ind_album;
    VIEWS views;
    LEADERBOARD leaderboard;
    SUMMARY annualsummary;
    ALGORITHM algorithm;
};

STATISTICS create_statistics(void){
    STATISTICS new = malloc(sizeof(struct statistics));
    if (!new){
        fprintf(stderr, "Falha ao alocar memória para statistics table.\n");
        return NULL;
    }

    new->agedata = create_agedata();
    new->top_artists = create_top_artists();
    new->ind_album = create_individualalbum();
    new->views = create_views();
    new->leaderboard = create_leaderboard();
    new->annualsummary = create_annualsummary();
    new->algorithm = create_algorithm();
    return new;
}

void* get_top_artists(STATISTICS table) {
    return table->top_artists;
}

AGEDATA get_statistics_agedata(STATISTICS table) {
    return table->agedata;
}

INDIVIDUAL_ALBUM get_statistics_ind_album(STATISTICS table){
    return table->ind_album;
}

void* get_statistics_views(STATISTICS table){
    return table->views;
}

LEADERBOARD get_statistics_leaderboard(STATISTICS table){
    return table->leaderboard;
}

SUMMARY get_statistics_annualsummary(STATISTICS table){
    return table->annualsummary;
}

ALGORITHM get_statistics_algorithm(STATISTICS table){
    return table->algorithm;
}

void free_statistics(STATISTICS table){
    free_agedata(table->agedata);
    free_top_artists(table->top_artists);
    free_individualalbum(table->ind_album);
    free_views(table->views);
    free_leaderboard(table->leaderboard);
    free_annualsummary(table->annualsummary);
    free_algorithm(table->algorithm);
    free(table);
}