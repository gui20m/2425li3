#include "Main/output.h"
#include "Enteties/users.h"

#include <stdlib.h>
#include <stdio.h>

void output_query(FILE* output_file, void* output, int query_id) {

    if (output == NULL){
        return;
    }

    static output_query_func output_queries[] = {output_query1,output_query2, output_query3, output_query4, output_query5, output_query6};

    output_queries[query_id - 1](output_file, output);
}

void output_query1(FILE* file, void* output){
    fprintf(file, "%s\n", (char*)output);
}

void output_query2(FILE* file, void* output){
    fprintf(file, "%s", (char*)output);
}

void output_query3(FILE* file, void* output){
    if (strcmp(output, "")!=0){
        fprintf(file, "%s", (char*)output);
        return;
    }
    fprintf(file, "\n");
}

void output_query4(FILE* file, void* output){
    fprintf(file, "%s", (char*)output);
}

void output_query5(FILE* file, void* output){
    fprintf(file, "%s", (char*)output);
}

void output_query6(FILE* file, void* output){
    fprintf(file, "%s", (char*)output);
}