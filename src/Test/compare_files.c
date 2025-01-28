#include "Test/compare_files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glib.h>

void compare_files(char* expectedOutput, int n, GList* lista) {

    struct timespec start, end;
    int i = 1, query_num;

    int countQuery1=0,countQuery2=0,countQuery3=0,countQuery4=0,countQuery5=0,countQuery6=0,
        countQuery1errors=0, countQuery2errors=0, countQuery3errors=0,countQuery4errors=0,countQuery5errors=0,countQuery6errors=0;
    double time1=0, time2=0, time3=0, time4=0, time5=0, time6=0;

    char result1[4096] = "";
    char result2[4096] = "";
    char result3[4096] = "";
    char result4[4096] = "";
    char result5[4096] = "";
    char result6[4096] = "";

    while (i<n) {

        clock_gettime(CLOCK_REALTIME, &start);

        query_num = GPOINTER_TO_INT(lista->data);

        char pathF[256];
        sprintf(pathF, "resultados/command%d_output.txt", i);

        char pathE[256];
        sprintf(pathE, "%s/command%d_output.txt", expectedOutput, i);

        FILE *file1 = fopen(pathE, "r");
        FILE *file2 = fopen(pathF, "r");

        if (file1 == NULL || file2 == NULL) {
            perror("Error opening files for comparison");
            return;
        }

        char *line1 = NULL;
        char *line2 = NULL;
        size_t len1 = 0;
        size_t len2 = 0;
        ssize_t read1, read2;
        int line_number = 1;

        char result_temp[512];

        while (1) {
            read1 = getline(&line1, &len1, file1);
            read2 = getline(&line2, &len2, file2);

            // Compare lines
            if (read1 != read2 || (read1 != -1 && read2 != -1 && strcmp(line1, line2) != 0)) {
                snprintf(result_temp, sizeof(result_temp), "    Descrepância na query %d: linha %d de \"resultados/command%d_output.txt\"\n",i,line_number,i);
                if (query_num==1){
                    strcat(result1, result_temp);
                    countQuery1errors++;
                }
                if (query_num==2){
                    strcat(result2, result_temp);
                    countQuery2errors++;
                }
                if (query_num==3){
                    strcat(result3, result_temp);
                    countQuery3errors++;
                }
                if (query_num==4){
                    strcat(result4, result_temp);
                    countQuery4errors++;
                }
                if (query_num==5){
                    strcat(result5, result_temp);
                    countQuery5errors++;
                }
                if (query_num==6){
                    strcat(result6, result_temp);
                    countQuery6errors++;
                }

                break;
            }

            if (read1 == -1 && read2 == -1) {
                break;
            }


            line_number++;
        }

        clock_gettime(CLOCK_REALTIME, &end);

        if (query_num==1){
            countQuery1++;
            time1 += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ 1e9;
        }
        if (query_num==2){
            countQuery2++;
            time2 += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ 1e9;
            }
        if (query_num==3){
            countQuery3++;
            time3 += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ 1e9;
        }
        if (query_num==4){
            countQuery4++;
            time4 += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ 1e9;
        }
        if (query_num==5){
            countQuery5++;
            time5 += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ 1e9;
        }
        if (query_num==6){
            countQuery6++;
            time6 += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ 1e9;
        }

        lista=lista->next;

        free(line1);
        free(line2);
        fclose(file1);
        fclose(file2);
        i++;
    }

    printf("Q1: %d de %d testes ok!\n", countQuery1-countQuery1errors, countQuery1);
    printf("%s", result1);  // Imprime todas as discrepâncias acumuladas para Q1
    printf("Q2: %d de %d testes ok!\n", countQuery2-countQuery2errors, countQuery2);
    printf("%s", result2);  // Imprime todas as discrepâncias acumuladas para Q2
    printf("Q3: %d de %d testes ok!\n", countQuery3-countQuery3errors, countQuery3);
    printf("%s", result3);
    printf("Q4: %d de %d testes ok!\n", countQuery4-countQuery4errors, countQuery4);
    printf("%s", result4);
    printf("Q5: %d de %d testes ok!\n", countQuery5-countQuery5errors, countQuery5);
    printf("%s", result5);
    printf("Q6: %d de %d testes ok!\n", countQuery6-countQuery6errors, countQuery6);
    printf("%s", result6);

    printf("Tempos de execução:\n  Q1: %.2fms\n  Q2: %.2fms\n  Q3: %.2fms\n  Q4: %.2fms\n  Q5: %.2fms\n  Q6: %.2fms\n", time1*1000, time2*1000, time3*1000, time4*1000, time5*1000, time6*1000);

}