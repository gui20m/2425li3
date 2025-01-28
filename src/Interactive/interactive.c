#include "Interactive/interactive.h"
#include "Interactive/home.h"

struct settings {
    char* datasetPath;
    char* inputsPath;
    char* outputPath;
    char** args;
    char* n_artistas;
    int argsN;
    SPOTILIXO data; 
};

SETTINGS create_settings(void){
    SETTINGS new = malloc(sizeof(struct settings));
    new->datasetPath = "./";
    new->inputsPath = "./";
    new->outputPath = "/resultados";
    new->args = NULL;
    new->n_artistas=NULL;
    new->argsN = 0;
    new->data = NULL;

    return new;
}

void set_datasetPath(SETTINGS setts, char* path){
    setts->datasetPath = strdup(path);
}

void set_nArtists(SETTINGS setts, char* n){
    setts->n_artistas = strdup(n);
}

void set_argSETTINGS(SETTINGS setts, char** args, int n) {
    if (!setts->args) {
        setts->args = malloc(sizeof(char*) * n);
        if (!setts->args) {
            perror("malloc failed in set_argSETTINGS");
            return;
        }
        setts->argsN = n;
    } else {
        if (n > setts->argsN) {
            setts->args = realloc(setts->args, sizeof(char*) * n);
            if (!setts->args) {
                perror("realloc failed in set_argSETTINGS");
                return;
            }
            setts->argsN = n;
        }
    }
    for (int i = 0; i < n; i++) {
        if (args[i]) {
            setts->args[i] = strdup(args[i]);
            if (!setts->args[i]) {
                perror("strdup failed in set_argSETTINGS");
                return;
            }
        } else {
            setts->args[i] = NULL;
        }
    }
}

void set_inputsPath(SETTINGS setts, char* path){
    setts->inputsPath = strdup(path);
}

void set_outputPath(SETTINGS setts, char* path){
    setts->outputPath = strdup(path);
}

void set_manager(SETTINGS setts, SPOTILIXO data){
    setts->data = data;
}

char* get_datasetPath(SETTINGS setts){
    return strdup(setts->datasetPath);
}

char* get_nArtists(SETTINGS setts){
 return strdup(setts->n_artistas);
}

char* get_inputsPath(SETTINGS setts){
    return strdup(setts->inputsPath);
}

char* get_outputPath(SETTINGS setts){
    return strdup(setts->outputPath);
}

char** get_args(SETTINGS setts) {
    return setts->args;
}

int get_argsN(SETTINGS setts) {
    return setts->argsN;
}

SPOTILIXO get_manager(SETTINGS setts){
    return setts->data;
}

void free_settings(SETTINGS setts){
    if (setts->data) free_spotilixo(setts->data);
    free(setts);
}

void interactive(void){
    SETTINGS settings = create_settings();

    home(settings);

    free_settings(settings);
}