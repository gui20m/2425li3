#include "Statistics/annualsummary.h"
#include "Enteties/musics_t.h"
#include "Enteties/musics.h"
#include <string.h>

struct summary {
    GHashTable* annualsummary; // year -> [userid -> USERSTATS]
    GHashTable* favouriteartists; // year -> [artistid -> ARTISTSTATS]
};

struct userstats {
    int listeningtime;
    int num_songs;
    GHashTable* playedartists; // [artists -> num_plays]
    GHashTable* favouriteday; // [day -> num_plays]
    GHashTable* playedgenres; // [genre -> num_plays]
    GHashTable* playedalbums; // [album -> num_plays]
    GHashTable* favouritehour; // [hour -> num_plays]
};

static USERSTATS create_userstats(void){
    USERSTATS new = malloc(sizeof(struct userstats));

    new->listeningtime = 0;
    new->num_songs = 0;
    new->playedartists = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    new->favouriteday = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    new->playedgenres = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    new->playedalbums = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    new->favouritehour = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);

    return new;
}

static void free_userstats(gpointer data){
    USERSTATS info = (USERSTATS) data;
    g_hash_table_destroy(info->playedartists);
    g_hash_table_destroy(info->favouriteday);
    g_hash_table_destroy(info->playedgenres);
    g_hash_table_destroy(info->playedalbums);
    g_hash_table_destroy(info->favouritehour);
    free(info);
}

int get_userstats_listeningtime(USERSTATS data){
    return data->listeningtime;
}

int get_userstats_num_songs(USERSTATS data){
    return data->num_songs;
}

struct artiststats {
    int songnum;
    int listeningtime;
    char* artist_id;
};

char* get_artiststats_id(ARTISTSTATS data){
    return strdup(data->artist_id);
}

int get_artiststats_listeningtime(ARTISTSTATS data){
    return data->listeningtime;
}

int get_artiststats_numsongs(ARTISTSTATS data){
    return data->songnum;
}

static ARTISTSTATS create_artiststats(){
    ARTISTSTATS new = malloc(sizeof(struct artiststats));
    new->listeningtime = 0;
    new->songnum = 0;
    new->artist_id=NULL;

    return new;
}

static void free_artiststats(gpointer data){
    ARTISTSTATS info = (ARTISTSTATS) data;
    free(info->artist_id);
    free(info);
}

SUMMARY create_annualsummary(void){
    SUMMARY new = malloc(sizeof(struct summary));
    if (!new){
        fprintf(stderr, "Falha ao alocar memória para SUMMARY table.\n");
        return NULL;
    }

    new->annualsummary = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)g_hash_table_destroy);
    new->favouriteartists = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)g_hash_table_destroy);

    return new;
}

static GHashTable* search_year_favouriteartists(SUMMARY table, char* key){
    return g_hash_table_lookup(table->favouriteartists, key);
}

int get_annualsummary_listeningtime(SUMMARY summary, char* user_id, char* ano) {
    if (!summary || !user_id || !ano) {
        return -1;
    }
    GHashTable* year_summary = g_hash_table_lookup(summary->annualsummary, ano);
    if (!year_summary) {
        return -1;
    }
    USERSTATS userstats = g_hash_table_lookup(year_summary, user_id);
    if (!userstats) {
        return -1;
    }
    return userstats->listeningtime;
}

int get_annualsummary_numplays(SUMMARY summary, char* user_id, char* ano) {
    if (!summary || !user_id || !ano) {
        return -1;
    }
    GHashTable* year_summary = g_hash_table_lookup(summary->annualsummary, ano);
    if (!year_summary) {
        return -1;
    }
    USERSTATS userstats = g_hash_table_lookup(year_summary, user_id);
    if (!userstats) {
        return -1;
    }
    return userstats->num_songs;
}

char* get_annualsummary_mostplayed_artist(SUMMARY summary, char* user_id, char* ano) {
    if (!summary || !user_id || !ano) {
        return NULL;
    }
    GHashTable* year_summary = g_hash_table_lookup(summary->annualsummary, ano);
    if (!year_summary) {
        return NULL;
    }
    USERSTATS userstats = g_hash_table_lookup(year_summary, user_id);
    if (!userstats) {
        return NULL;
    }
    GHashTable* user_artists = userstats->playedartists;
    if (!user_artists) {
        return NULL;
    }
    const char* most_played_artist = NULL;
    int max_plays = -1;
    GList* keys = g_hash_table_get_keys(user_artists);
    for (GList* iter = keys; iter != NULL; iter = iter->next) {
        const char* artist = (const char*) iter->data;
        int num_plays = GPOINTER_TO_INT(g_hash_table_lookup(user_artists, artist));

        if (num_plays > max_plays || (num_plays == max_plays && strcmp(artist, most_played_artist) < 0)) {
            most_played_artist = artist;
            max_plays = num_plays;
        }
    }
    g_list_free(keys);

    return (most_played_artist ? strdup(most_played_artist) : NULL);
}

static int compare_artist_entries(gconstpointer a, gconstpointer b) {
    ARTISTSTATS entry_a = (ARTISTSTATS)a;
    ARTISTSTATS entry_b = (ARTISTSTATS)b;

    int dif = entry_b->listeningtime - entry_a->listeningtime;

    if (dif !=0) return dif;

    return strcmp(entry_b->artist_id, entry_a->artist_id)<0;
}

GList* get_annualsummary_favouriteartists(SUMMARY data, char* user_id, char* ano, int n) {
    GHashTable* yeardata = g_hash_table_lookup(data->favouriteartists, ano);
    if (!yeardata) {
        return NULL;
    }

    GHashTable* user_favouriteartists = g_hash_table_lookup(yeardata, user_id);
    if (!user_favouriteartists) {
        return NULL;
    }

    GList* artist_list = NULL;

    GHashTableIter iter;
    gpointer artist_name, artist_data;
    g_hash_table_iter_init(&iter, user_favouriteartists);

    while (g_hash_table_iter_next(&iter, &artist_name, &artist_data)) {
        ARTISTSTATS artiststats = (ARTISTSTATS)artist_data;

        ARTISTSTATS entry = create_artiststats();
        entry->artist_id = strdup(artiststats->artist_id);
        entry->listeningtime = artiststats->listeningtime;
        entry->songnum = artiststats->songnum;

        artist_list = g_list_prepend(artist_list, entry);
    }

    artist_list = g_list_sort(artist_list, compare_artist_entries);

    if (g_list_length(artist_list) > n) {
        GList* temp = g_list_nth(artist_list, n);
        g_list_free_full(temp->next, free);
        temp->next = NULL;
    }

    return artist_list;
}

static int compare_dates(const char* date1, const char* date2) {
    return strcmp(date2, date1);
}

char* get_annualsummary_mostplayed_day(SUMMARY summary, char* user_id, char* ano) {
    if (!summary || !user_id || !ano) {
        return NULL;
    }

    GHashTable* year_summary = g_hash_table_lookup(summary->annualsummary, ano);
    if (!year_summary) {
        return NULL;
    }

    USERSTATS userstats = g_hash_table_lookup(year_summary, user_id);
    if (!userstats) {
        return NULL;
    }
    GHashTable* user_days = userstats->favouriteday;
    if (!user_days) {
        return NULL;
    }

    const char* most_played_day = NULL;
    int max_plays = -1;

    GList* keys = g_hash_table_get_keys(user_days);
    for (GList* iter = keys; iter != NULL; iter = iter->next) {
        const char* day = (const char*) iter->data;
        int num_plays = GPOINTER_TO_INT(g_hash_table_lookup(user_days, day));
        if (num_plays > max_plays || (num_plays == max_plays && compare_dates(day, most_played_day) < 0)) {
            most_played_day = day;
            max_plays = num_plays;
        }
    }
    g_list_free(keys);

    return (most_played_day ? strdup(most_played_day) : NULL);
}

static int compare_genres(const char* genre1, const char* genre2) {
    return strcmp(genre2, genre1);
}

char* get_annualsummary_mostplayed_genre(SUMMARY summary, char* user_id, char* ano) {
    if (!summary || !user_id || !ano) {
        return NULL;
    }
    GHashTable* year_summary = g_hash_table_lookup(summary->annualsummary, ano);
    if (!year_summary) {
        return NULL;
    }
    USERSTATS userstats = g_hash_table_lookup(year_summary, user_id);
    if (!userstats) {
        return NULL;
    }

    GHashTable* user_genres = userstats->playedgenres;
    if (!user_genres) {
        return NULL;
    }

    const char* most_played_genre = NULL;
    int max_plays = -1;

    GList* keys = g_hash_table_get_keys(user_genres);
    for (GList* iter = keys; iter != NULL; iter = iter->next) {
        const char* genre = (const char*) iter->data;
        int num_plays = GPOINTER_TO_INT(g_hash_table_lookup(user_genres, genre));
        if (num_plays > max_plays || (num_plays == max_plays && compare_genres(genre, most_played_genre) < 0)) {
            most_played_genre = genre;
            max_plays = num_plays;
        }
    }
    g_list_free(keys);

    return (most_played_genre ? strdup(most_played_genre) : NULL);
}

static int compare_albums(const char* album1, const char* album2) {
    return strcmp(album1, album2); // Comparação para obter o álbum mais tocado
}

char* get_annualsummary_mostplayed_album(SUMMARY summary, char* user_id, char* ano) {
    if (!summary || !user_id || !ano) {
        return NULL;
    }
    GHashTable* year_summary = g_hash_table_lookup(summary->annualsummary, ano);
    if (!year_summary) {
        return NULL;
    }
    USERSTATS userstats = g_hash_table_lookup(year_summary, user_id);
    if (!userstats) {
        return NULL;
    }

    GHashTable* user_albums = userstats->playedalbums;
    if (!user_albums) {
        return NULL;
    }

    const char* most_played_album = NULL;
    int max_plays = -1;

    GList* keys = g_hash_table_get_keys(user_albums);
    for (GList* iter = keys; iter != NULL; iter = iter->next) {
        const char* album = (const char*) iter->data;
        int num_plays = GPOINTER_TO_INT(g_hash_table_lookup(user_albums, album));
        if (num_plays > max_plays || (num_plays == max_plays && compare_albums(album, most_played_album) < 0)) {
            most_played_album = album;
            max_plays = num_plays;
        }
    }
    g_list_free(keys);

    return (most_played_album ? strdup(most_played_album) : NULL);
}

char* get_annualsummary_favouritehour(SUMMARY summary, char* user_id, char* ano) {
    if (!summary || !user_id || !ano) {
        return NULL;
    }
    GHashTable* year_summary = g_hash_table_lookup(summary->annualsummary, ano);
    if (!year_summary) {
        return NULL;
    }
    USERSTATS userstats = g_hash_table_lookup(year_summary, user_id);
    if (!userstats) {
        return NULL;
    }

    GHashTable* user_hours = userstats->favouritehour;
    if (!user_hours) {
        return NULL;
    }

    const char* most_played_hour = NULL;
    int max_plays = -1;

    GList* keys = g_hash_table_get_keys(user_hours);
    for (GList* iter = keys; iter != NULL; iter = iter->next) {
        const char* hour = (const char*) iter->data;
        int num_plays = GPOINTER_TO_INT(g_hash_table_lookup(user_hours, iter->data));
        if (num_plays > max_plays || (num_plays == max_plays && strcmp(hour, most_played_hour) < 0)) {
            most_played_hour = hour;
            max_plays = num_plays;
        }
    }
    g_list_free(keys);

    return (most_played_hour ? strdup(most_played_hour) : NULL);
}

void free_annualsummary(SUMMARY data){
    g_hash_table_destroy(data->annualsummary);
    g_hash_table_destroy(data->favouriteartists);
    free(data);
}

static void update_hash_table(GHashTable* table, char* key, int value) {
    gpointer current_value = g_hash_table_lookup(table, key);
    if (current_value) {
        value += GPOINTER_TO_INT(current_value);
        g_hash_table_replace(table, strdup(key), GINT_TO_POINTER(value));
    } else {
        g_hash_table_insert(table, strdup(key), GINT_TO_POINTER(value));
    }
}

static void process_favouriteartists(SUMMARY data, char* user_id, char* music_id, char* ano, void* musics, int duration) {
    MUSIC music = search_music(musics, music_id);
    char* artists = get_music_artist(music);
    char** subartists = split_array(artists);

    GHashTable* yeardata = search_year_favouriteartists(data, ano);
    if (!yeardata) {
        yeardata = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)g_hash_table_destroy);
        g_hash_table_insert(data->favouriteartists, strdup(ano), yeardata);
    }

    GHashTable* user_favouriteartists = g_hash_table_lookup(yeardata, user_id);
    if (!user_favouriteartists) {
        user_favouriteartists = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)free_artiststats);
        g_hash_table_insert(yeardata, strdup(user_id), user_favouriteartists);
    }

    for (int i = 0; subartists[i] != NULL; i++) {
        ARTISTSTATS artiststats = g_hash_table_lookup(user_favouriteartists, subartists[i]);
        if (!artiststats) {
            artiststats = create_artiststats();
            artiststats->artist_id = strdup(subartists[i]);
            g_hash_table_insert(user_favouriteartists, strdup(subartists[i]), artiststats);
        }
        artiststats->listeningtime += duration;
        artiststats->songnum++;
    }

    free_tokens(subartists);
    free(artists);
}


void process_annualsummary(SUMMARY data, char* user_id, char* music_id, char* timestamp, int duration, void* musics, void* artistas) {
    int year, mes, dia, hours, minutes, seconds;
    sscanf(timestamp, "%d/%d/%d %d:%d:%d", &year, &mes, &dia, &hours, &minutes, &seconds);

    char ano[5];
    snprintf(ano, sizeof(ano), "%d", year);

    char date[11];
    snprintf(date, sizeof(date), "%02d/%02d/%02d", year, mes, dia);

    char hora[3];
    snprintf(hora, sizeof(hora), "%02d", hours);

    GHashTable* yeardata = g_hash_table_lookup(data->annualsummary, ano);
    if (!yeardata && !interactive_mode) return;
    if (interactive_mode && !yeardata) {
        yeardata = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)free_userstats);
        g_hash_table_insert(data->annualsummary, strdup(ano), yeardata);
    }

    USERSTATS userstats = g_hash_table_lookup(yeardata, user_id);
    if (!userstats && !interactive_mode) return;
    if (interactive_mode && !userstats){
        userstats = create_userstats();
        g_hash_table_insert(yeardata, strdup(user_id), userstats);
    }

    userstats->listeningtime += duration;
    userstats->num_songs++;

    MUSIC music = search_music(musics, music_id);

    // Update artists
    char* artists = get_music_artist(music);
    char** subartists = split_array(artists);
    for (int i = 0; subartists[i] != NULL; i++) {
        update_hash_table(userstats->playedartists, subartists[i], duration);
    }
    free_tokens(subartists);
    free(artists);

    // Update days played
    update_hash_table(userstats->favouriteday, date, 1);

    // Update genres played
    char* genre = get_music_genre(music);
    update_hash_table(userstats->playedgenres, genre, duration);
    free(genre);

    // Update played albums
    char* album_id = get_music_album(music);
    update_hash_table(userstats->playedalbums, album_id, duration);
    free(album_id);

    // Update hour plays
    update_hash_table(userstats->favouritehour, hora, duration);

    process_favouriteartists(data, user_id, music_id, ano, musics, duration);
}


void free_artiststats_list(GList* artiststats_list) {
    GList* iterator = artiststats_list;

    while (iterator != NULL) {
        struct artiststats* current_artiststats = (struct artiststats*)iterator->data;
        
        free(current_artiststats->artist_id);

        free(current_artiststats);

        iterator = iterator->next;
    }
    g_list_free(artiststats_list);
}


void pre_process_users(SUMMARY data, char* user_id, char* ano) {
    GHashTable* yeardata = g_hash_table_lookup(data->annualsummary, ano);
    if (!yeardata){
        yeardata = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)free_userstats);
        g_hash_table_insert(data->annualsummary, strdup(ano), yeardata);
    }
    USERSTATS userstats = g_hash_table_lookup(yeardata, user_id);
    if (!userstats){
        userstats = create_userstats();
        g_hash_table_insert(yeardata, strdup(user_id), userstats);
    }
}
