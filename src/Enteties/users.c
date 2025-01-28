#include "Enteties/users.h"
#include "Main/dataval.h"
#include "Enteties/spotilixo.h"
#include "Enteties/musics.h"
#include <glib.h>
#include <string.h>
#include <stdio.h>
#include "Main/utils.h"
#include <ctype.h>

struct user{
    char *email;
    char *first_name;
    char *last_name;
    int age;
    char *country;
    char *subscription_type;
    char *liked_songs_id;
};

static USER create_user(void) {
    USER user = malloc(sizeof(struct user));

    user->email = NULL;
    user->first_name = NULL;
    user->last_name = NULL;
    user->age = 0;
    user->country = NULL;
    user->subscription_type = NULL;
    user->liked_songs_id = NULL;
    return user;
}

static void set_user_email(USER user, char *email){
    user->email = strdup(email);
}

static void set_user_first_name(USER user, char *first_name){
    user->first_name = strdup(first_name);
}

static void set_user_last_name(USER user, char *last_name){
    user->last_name = strdup(last_name);
}

static void set_user_age(USER user, int age){
    user->age = age;
}

static void set_user_country(USER user, char *country){
    user->country = strdup(country);
}

static void set_user_subscription_type(USER user, char *subscription_type){
    user->subscription_type = strdup(subscription_type);
}

static void set_user_liked_songs_id(USER user, char *liked_songs_id){
    user->liked_songs_id = strdup(liked_songs_id);
}

char* get_user_email(USER user){
    return strdup(user->email);
}

char* get_user_first_name(USER user){
    return strdup(user->first_name);
}

char* get_user_last_name(USER user){
    return strdup(user->last_name);
}

char* get_user_country(USER user){
    return strdup(user->country);
}

int get_user_age(USER user){
    return user->age;
}

char* get_user_subscription_type(USER user){
    return strdup(user->subscription_type);
}

char* get_user_liked_songs_id(USER user){
    return strdup(user->liked_songs_id);
}

void free_user(USER user) {
    free(user->email);
    free(user->first_name);
    free(user->last_name);
    free(user->country);
    free(user->subscription_type);
    free(user->liked_songs_id);

    free(user);
}

static int verify_user(char** fields, MUSICS_T musics_table){
    if (!(fields[0]) || !(fields[1]) || !(fields[2]) ||
        !(fields[3]) || !(fields[4]) || !(fields[5]) ||
        !(fields[6])|| !(fields[7])) return 0;

    if(!validate_date(fields[4])) return 0;

    if(!validate_email(fields[1])) return 0;

    if(!validate_subscription_type(fields[6])) return 0;

    if(!validate_array(fields[7])) return 0;

    if(!validate_liked_songs(fields[7], musics_table)) return 0;

    return 1;
}

static void process_user_statistics(char *input, int age, void* musicas, void* agedata) {
    int len = strlen(input);
    if (len < 2 || input[0] != '[' || input[len - 1] != ']') {
        return;
    }

    const char* start = input + 1;
    const char* end = input + len - 2;

    char age_key[4];
    snprintf(age_key, sizeof(age_key), "%d", age);

    GHashTable* informacao = search_ageMapGenre(agedata, age_key);
    if (!informacao) {
        informacao = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
        insert_ageMapGenre(informacao, agedata, strdup(age_key));
    }

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

            char *token_ptr = token;
            while (*token_ptr && isspace((unsigned char)*token_ptr)) {
                token_ptr++;
            }
            if (token_ptr != token) {
                memmove(token, token_ptr, strlen(token_ptr) + 1);
            }

            int end_idx = strlen(token) - 1;
            while (end_idx >= 0 && isspace((unsigned char)token[end_idx])) {
                token[end_idx--] = '\0';
            }

            MUSIC music = search_music(musicas, token);
            if (!music) {
                continue; 
            }

            char* genre = get_music_genre(music);
            int* likes = g_hash_table_lookup(informacao, genre);

            if (!likes) {
                int* likes_ptr = g_new(int, 1);
                *likes_ptr = 1;
                g_hash_table_insert(informacao, strdup(genre), likes_ptr);
            } else {
                (*likes)++;

            }

            free((char*)genre);

            token_start = p + 1;
        }
    }

    return;
}


int build_user(char **user_fields, void *data){
    USERS_T users = get_spotilixo_users(data);
    MUSICS_T musicas = get_spotilixo_musicas(data);

    for (int i = 0; i < 8; i++) {
        remove_char(user_fields[i]);
    }

    if (!verify_user(user_fields, musicas)) return 0;

    USER user = create_user();
    int age = calculate_age(SYSTEM_DATE, user_fields[4]);

    set_user_email(user, user_fields[1]);
    set_user_first_name(user,user_fields[2]);
    set_user_last_name(user,user_fields[3]);
    set_user_age(user, age);
    set_user_country(user,user_fields[5]);
    set_user_subscription_type(user,user_fields[6]);
    set_user_liked_songs_id(user,user_fields[7]);

    insert_user(user,users,user_fields[0]);

    STATISTICS estatisticas = get_spotilixo_estatisticas(data);
    AGEDATA agedata = get_statistics_agedata(estatisticas);

    process_user_statistics(user_fields[7], age, musicas, agedata);

    return 1;
}