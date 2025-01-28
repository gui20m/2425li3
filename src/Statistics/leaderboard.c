#include "Statistics/leaderboard.h"
#include "Main/utils.h"
#include "Enteties/artists_t.h"

struct leaderboard {
    GHashTable* leaderboard; // year -> [week -> linked list top 10 artists]
    GHashTable* top10;
};

struct top10 {
    char* artista;
    int duracao;
};

static TOP10 create_top10_artist(char* artista, int duracao) {
    TOP10 new = malloc(sizeof(struct top10));
    if (!new) {
        fprintf(stderr, "Erro ao alocar memória para top10 artist.\n");
        return NULL;
    }
    new->artista = strdup(artista);
    new->duracao = duracao;
    return new;
}

static void free_top10(gpointer data) {
    TOP10 new = (TOP10)data;
    free(new->artista);
    free(new);
}

LEADERBOARD create_leaderboard(void){
    LEADERBOARD new = malloc(sizeof(struct leaderboard));
    if (!new){
        fprintf(stderr, "Falha ao alocar memória para leaderboard table.\n");
        return NULL;
    }

    new->leaderboard = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)free_weekdata);
    new->top10 = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)g_hash_table_destroy);

    return new;
}

void free_weekdata(gpointer data) {
    GHashTable* weekdata = (GHashTable*) data;
    g_hash_table_destroy(weekdata);
}

static GHashTable* search_yearData(LEADERBOARD table, char* key){
    return g_hash_table_lookup(table->leaderboard, key);
}

void free_leaderboard(LEADERBOARD table){
    g_hash_table_destroy(table->leaderboard);
    g_hash_table_destroy(table->top10);
    free(table);
}

static int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static int day_of_year(int year, int month, int day) {
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (is_leap_year(year)) {
        days_in_month[1] = 29;
    }
    int total_days = 0;
    for (int i = 0; i < month - 1; i++) {
        total_days += days_in_month[i];
    }
    total_days += day;
    return total_days;
}

static int calculate_day_of_week(int year, int month, int day) {
    if (month < 3) {
        month += 12;
        year -= 1;
    }
    int K = year % 100;
    int J = year / 100;
    int h = (day + (13 * (month + 1)) / 5 + K + (K / 4) + (J / 4) - (2 * J)) % 7;
    return (h + 6) % 7;
}

static int calculate_week_number(int year, int month, int day) {
    int doy = day_of_year(year, month, day);
    int first_sunday = 1+(7 - calculate_day_of_week(year, 1, 1)) % 7;

    if (doy < first_sunday) {
        return 0;
    }

    int week_number = (doy - first_sunday) / 7 + 1;

    if (week_number > 52) {
        int last_day_of_week = calculate_day_of_week(year, 12, 31);
        if (last_day_of_week == 6) {
            return 53;
        }
        return 52;
    }

    return week_number;
}

void weekly_duration_update(LEADERBOARD table, void* artists_table, char* artists_id, char* timestamp, int duration) {
    int year, mes, dia, hours, minutes, seconds;
    sscanf(timestamp, "%d/%d/%d %d:%d:%d", &year, &mes, &dia, &hours, &minutes, &seconds);

    char ano[5];
    snprintf(ano, sizeof(ano), "%d", year);
    char semana[4];
    snprintf(semana, sizeof(semana), "%d", calculate_week_number(year, mes, dia));

    GHashTable* yeardata = search_yearData(table, ano);
    if (!yeardata) {
        yeardata = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)g_hash_table_destroy);
        g_hash_table_insert(table->leaderboard, strdup(ano), yeardata);
    }

    GHashTable* weekdata = g_hash_table_lookup(yeardata, semana);
    if (!weekdata) {
        weekdata = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
        g_hash_table_insert(yeardata, strdup(semana), weekdata);
    }

    char** artistas = split_array(artists_id);
    for (int i = 0; artistas[i] != NULL; i++) {
        gpointer existing_duration = g_hash_table_lookup(weekdata, artistas[i]);
        int novo_valor = duration;
        if (existing_duration) {
            novo_valor += GPOINTER_TO_INT(existing_duration);
        }
        g_hash_table_insert(weekdata, strdup(artistas[i]), GINT_TO_POINTER(novo_valor));
    }
    free_tokens(artistas);
}

static void free_top10_glist(GList* list) {
    GList* temp;

    while (list != NULL) {
        temp = list->next;
        free_top10(list->data);
        g_list_free_1(list);
        list = temp;
    }
}

static int compare_duration(gconstpointer a, gconstpointer b) {
    TOP10 artist_a = (TOP10)a;
    TOP10 artist_b = (TOP10)b;

    int duration_difference = artist_b->duracao - artist_a->duracao;

    if (duration_difference != 0) {
        return duration_difference;
    }

    return strcmp(artist_a->artista, artist_b->artista);
}

void process_top10_weekly(LEADERBOARD data) {
    GHashTableIter year_iter;
    gpointer year_key, year_value;
    g_hash_table_iter_init(&year_iter, data->leaderboard);

    while (g_hash_table_iter_next(&year_iter, &year_key, &year_value)) {
        GHashTable* weekdata = (GHashTable*)year_value;

        GHashTable* top10_for_year = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)free_top10_glist);

        GHashTableIter week_iter;
        gpointer week_key, week_value;
        g_hash_table_iter_init(&week_iter, weekdata);

        while (g_hash_table_iter_next(&week_iter, &week_key, &week_value)) {
            GHashTable* artistdata = (GHashTable*)week_value;
            GList* artists_list = NULL;

            GHashTableIter artist_iter;
            gpointer artist_key, artist_value;
            g_hash_table_iter_init(&artist_iter, artistdata);

            while (g_hash_table_iter_next(&artist_iter, &artist_key, &artist_value)) {
                TOP10 user = create_top10_artist((char*)artist_key, GPOINTER_TO_INT(artist_value));
                artists_list = g_list_append(artists_list, user);
            }

            artists_list = g_list_sort(artists_list, compare_duration);

            GList* top10_artists = NULL;
            int count = 0;
            for (GList* l = artists_list; l != NULL && count < 10; l = l->next, count++) {
                top10_artists = g_list_append(top10_artists, l->data);
            }

            g_hash_table_insert(top10_for_year, strdup((char*)week_key), top10_artists);

            GList* current = artists_list;
            while (current != NULL) {
                GList* next = current->next;
                if (!g_list_find(top10_artists, current->data)) {
                    free_top10(current->data);
                }
                g_list_free_1(current);
                current = next;
            }
        }

        g_hash_table_insert(data->top10, strdup((char*)year_key), top10_for_year);
    }
}

char** get_most_played_artist(LEADERBOARD data) {
    GHashTable* top10_data = data->top10;
    char** result = malloc(2 * sizeof(char*));
    result[0] = NULL;
    result[1] = NULL;

    if (!top10_data) {
        return result;
    }

    GHashTable* artist_count = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, free);

    GHashTableIter year_iter;
    gpointer year_key, year_value;
    g_hash_table_iter_init(&year_iter, top10_data);

    while (g_hash_table_iter_next(&year_iter, &year_key, &year_value)) {
        GHashTable* weekdata = (GHashTable*)year_value;

        GHashTableIter week_iter;
        gpointer week_key, week_value;
        g_hash_table_iter_init(&week_iter, weekdata);

        while (g_hash_table_iter_next(&week_iter, &week_key, &week_value)) {
            GList* artists_list = (GList*)week_value;

            for (GList* l = artists_list; l != NULL; l = l->next) {
                TOP10 user = (TOP10)l->data;

                if (!user || !user->artista) {
                    continue;
                }

                int* count = g_hash_table_lookup(artist_count, user->artista);
                if (count) {
                    (*count)++;
                } else {
                    int* new_count = malloc(sizeof(int));
                    *new_count = 1;
                    g_hash_table_insert(artist_count, user->artista, new_count);
                }
            }
        }
    }

    char* most_played_artist = NULL;
    int most_played_count = 0;

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, artist_count);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        char* artist = (char*)key;
        int count = *((int*)value);

        if (most_played_artist == NULL || count > most_played_count || 
           (count == most_played_count && strcmp(artist, most_played_artist) < 0)) {
            most_played_artist = artist;
            most_played_count = count;
        }
    }

    g_hash_table_destroy(artist_count);

    if (most_played_artist) {
        result[0] = most_played_artist;
        result[1] = malloc(20 * sizeof(char));
        snprintf(result[1], 20, "%d", most_played_count);
    }

    return result;
}

char** get_most_played_artist_between_dates(LEADERBOARD data, int year1, int mes1, int dia1, int year2, int mes2, int dia2) {
    int semana1 = calculate_week_number(year1, mes1, dia1);
    int semana2 = calculate_week_number(year2, mes2, dia2);

    char* most_played_artist = NULL;
    int most_played_count = 0;

    GHashTable* artist_count = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, free);

    for (int year = year1; year <= year2; year++) {
        char year_str[5];
        snprintf(year_str, sizeof(year_str), "%d", year);
        GHashTable* year_data = g_hash_table_lookup(data->top10, year_str);

        if (!year_data) {
            continue;
        }

        int start_week = (year == year1) ? semana1 : 1;
        int end_week = (year == year2) ? semana2 : 52;

        for (int week = start_week; week <= end_week; week++) {
            char week_str[3];
            snprintf(week_str, sizeof(week_str), "%d", week);

            GList* artists_list = g_hash_table_lookup(year_data, week_str);
            if (artists_list) {
                for (GList* l = artists_list; l != NULL; l = l->next) {
                    TOP10 user = (TOP10)l->data;

                    if (!user || !user->artista) {
                        continue;
                    }

                    int* count = g_hash_table_lookup(artist_count, user->artista);
                    if (count) {
                        (*count)++;
                    } else {
                        int* new_count = malloc(sizeof(int));
                        *new_count = 1;
                        g_hash_table_insert(artist_count, user->artista, new_count);
                    }
                }
            }
        }
    }

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, artist_count);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        char* artist = (char*)key;
        int count = *((int*)value);

        if (most_played_artist == NULL || count > most_played_count || 
           (count == most_played_count && strcmp(artist, most_played_artist) < 0)) {
            most_played_artist = artist;
            most_played_count = count;
        }
    }

    g_hash_table_destroy(artist_count);

    char** result = malloc(2 * sizeof(char*));
    if (most_played_artist) {
        result[0] = most_played_artist;
        result[1] = malloc(20 * sizeof(char));
        snprintf(result[1], 20, "%d", most_played_count);
    } else {
        result[0] = NULL;
        result[1] = NULL;
    }

    return result;
}
