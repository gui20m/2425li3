#include "Main/dataval.h"
#include "Main/utils.h"
#include "Enteties/artists.h"
#include <string.h>
#include <ctype.h>

int validate_date(char* date_string){

    int currentYear, currentMonth, currentDay;
    int birthYear, birthMonth, birthDay;

    if (strlen(date_string) != 10) return 0;

    sscanf(SYSTEM_DATE, "%d/%d/%d", &currentYear, &currentMonth, &currentDay);
    sscanf(date_string, "%d/%d/%d", &birthYear, &birthMonth, &birthDay);

    if (birthYear > currentYear) return 0;
    if (birthYear == currentYear && birthMonth > currentMonth) return 0;
    if (birthYear == currentYear && birthMonth == currentMonth && birthDay > currentDay) return 0;

    // validate years
    if(!isDigit(date_string[0]) ||
       !isDigit(date_string[1]) ||
       !isDigit(date_string[2]) ||
       !isDigit(date_string[3]) ||
       (date_string[4] != '/')  ||
       (date_string[7] != '/')) return 0;

    // validate months
    int aux;
    if(isDigit(date_string[5]) && isDigit(date_string[6]))
        aux = (date_string[5] - '0') * 10 + (date_string[6] - '0');
    else return 0;

    if (aux > 12 || aux < 1) return 0;

    // validate days
    if(isDigit(date_string[8]) && isDigit(date_string[9]))
        aux = (date_string[8] - '0') * 10 + (date_string[9] - '0');
    else return 0;

    if (aux > 31 || aux < 1) return 0;

    return 1;
}

int validate_email(char* email) {
    if (strlen(email) == 0) return 0;

    int i = 0;
    int email_length = strlen(email);

    while (email[i] != '@' && i < email_length) {
        if (!isLetter(email[i]) && !isDigit(email[i])) return 0;
        i++;
    }

    if (i < 1 || i == email_length) return 0;

    int j = i;
    i++;

    while (email[i] != '.' && i < email_length) {
        if (!isLetter(email[i])) return 0;
        i++;
    }

    if ((i - j) < 1 || i == email_length) return 0;

    int rstring_length = email_length - i - 1;
    if (rstring_length < 2 || rstring_length > 3) return 0;

    for (int k = i + 1; k < email_length; k++) {
        if (!isLetter(email[k])) return 0;
    }

    return 1;
}

int validate_subscription_type(char* str){
    return (strcmp(str,"normal")==0 || strcmp(str, "premium")==0);
}

int validate_array(char* str) {
    int length = strlen(str);

    if (length < 2) return 0;
    if (str[0] != '[' || str[length - 1] != ']') return 0;

    return 1;
}

int validate_id_constituent(char *str, char* type){

    return ((strcasecmp(type, "group")==0) || (strcasecmp(str, "[]")==0 && strcasecmp(type, "individual")==0));    
}

int validate_artists_id(char *input, void* artists_table){
    int len = strlen(input);
    if (len < 2 || input[0] != '[' || input[len - 1] != ']') {
        return 0;
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

            ARTIST artist = search_artist(artists_table, token);
            if (!artist) return 0;

            token_start = p + 1;
        }
    }
    
    return 1;
}

int validate_album_id(char* str, void* albums_table){

    ALBUM album = search_album(albums_table, str);
    if (!album) return 0;

    return 1;
}

int validate_duration(char* time_string) {
    if (time_string == NULL || strlen(time_string) != 8) return 0;
    if (time_string[2] != ':' || time_string[5] != ':') return 0;

    int aux;
    if (isDigit(time_string[0]) && isDigit(time_string[1]))
        aux = (time_string[0] - '0') * 10 + (time_string[1] - '0');
    else return 0;
    if (aux > 100 || aux < 0) return 0; //hours

    if (isDigit(time_string[3]) && isDigit(time_string[4]))
        aux = (time_string[3] - '0') * 10 + (time_string[4] - '0');
    else return 0;
    if (aux > 59 || aux < 0) return 0; //minutes

    if (isDigit(time_string[6]) && isDigit(time_string[7]))
        aux = (time_string[6] - '0') * 10 + (time_string[7] - '0');
    else return 0;
    if (aux > 59 || aux < 0) return 0; //seconds

    return 1;
}

int validate_liked_songs(char *input, MUSICS_T musics_table) {
    int len = strlen(input);
    if (len < 2 || input[0] != '[' || input[len - 1] != ']') {
        return 0;
    }

    const char *start = input + 1;
    const char *end = input + len - 2;

    const char *token_start = start;
    for (const char *p = start; p <= end; p++) {
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
                token[end_idx] = '\0';
                end_idx--;
            }

            MUSIC music = search_music(musics_table, token);
            if (!music) return 0;

            token_start = p + 1;
        }
    }

    return 1;
}

int validate_user(char* str, void*users){
    USER user = search_user(users, str);
    if(!user) return 0;

    return 1;
}

int validate_music(char* str, void*musics){
    MUSIC music = search_music(musics, str);
    if (!music) return 0;

    return 1;
}

int validate_plataform(char* str){
    return (strcasecmp(str, "desktop")==0 || strcasecmp(str, "mobile")==0);
}