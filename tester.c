#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "selec_proc.h"
#include <omp.h>

#define NUM_IMAGES 100
#define OUTPUT_DIR ""

// Función para verificar si un archivo existe
int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

int main() {

    // Crear la carpeta "img_res/" si no existe
    struct stat st = {0};
    if (stat("img_res", &st) == -1) {
        if (mkdir("img_res", 0700) != 0) {
            fprintf(stderr, "Error: No se pudo crear la carpeta 'img_res/'.\n");
            return 1;
        }
        printf("Carpeta 'img_res/' creada exitosamente.\n");
    } else {
        printf("Carpeta 'img_res/' ya existe.\n");
    }

    // Abrir el archivo de registro en modo "w" para vaciarlo
    FILE *outputLog = fopen("output_log.txt", "w");
    if (outputLog == NULL) {
        fprintf(stderr, "Error: No se pudo crear o abrir el archivo de registro.\n");
        return 1;
    }
    fclose(outputLog); // Cerrar el archivo inmediatamente después de vaciarlo

    double start_time, end_time, total_time;

    start_time = omp_get_wtime();
    omp_set_num_threads(200);  

    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                printf("[Tarea 1] Inversión gris horizontal\n");
                for (int i = 0; i < NUM_IMAGES; i++) {
                    char out_file[50], in_file[50];
                    sprintf(in_file, "imagenes/%d.bmp", i);
                    sprintf(out_file, "inv_grey_horizontal_%d.bmp", i);
                    inv_img_grey_horizontal(out_file, in_file);
                }
            }

            #pragma omp section
            {
                printf("[Tarea 2] Inversión gris vertical\n");
                for (int i = 0; i < NUM_IMAGES; i++) {
                    char out_file[50], in_file[50];
                    sprintf(in_file, "imagenes/%d.bmp", i);
                    sprintf(out_file, "inv_grey_vertical_%d.bmp", i);
                    inv_img_grey_vertical(out_file, in_file);
                }
            }

            #pragma omp section
            {
                printf("[Tarea 3] Inversión color horizontal\n");
                for (int i = 0; i < NUM_IMAGES; i++) {
                    char out_file[50], in_file[50];
                    sprintf(in_file, "imagenes/%d.bmp", i);
                    sprintf(out_file, "inv_color_horizontal_%d.bmp", i);
                    inv_img_color_horizontal(out_file, in_file);
                }
            }

            #pragma omp section
            {
                printf("[Tarea 4] Inversión color vertical\n");
                for (int i = 0; i < NUM_IMAGES; i++) {
                    char out_file[50], in_file[50];
                    sprintf(in_file, "imagenes/%d.bmp", i);
                    sprintf(out_file, "inv_color_vertical_%d.bmp", i);
                    inv_img_color_vertical(out_file, in_file);
                }
            }

            #pragma omp section
            {
                printf("[Tarea 5] Conversión a grises\n");
                for (int i = 0; i < NUM_IMAGES; i++) {
                    char out_file[50], in_file[50];
                    sprintf(in_file, "imagenes/%d.bmp", i);
                    sprintf(out_file, "grey_%d.bmp", i);
                    gray_img(out_file, in_file);
                }
            }

            #pragma omp section
            {
                printf("[Tarea 6] Desenfoque\n");
                for (int i = 0; i < NUM_IMAGES; i++) {
                    char out_file[50], in_file[50];
                    sprintf(in_file, "imagenes/%d.bmp", i);
                    sprintf(out_file, "desenfoque_%d.bmp", i);
                    desenfoque(in_file, out_file, 155);
                }
            }
        }
    }
    
    
    end_time = omp_get_wtime();
    total_time = end_time - start_time;

    printf("Procesamiento completado. Resultados en %s/\n", OUTPUT_DIR);

    printf("Tiempo de ejecución: %f segundos\n", total_time);

    long total_operations = NUM_IMAGES * 54; // 54 bytes de cabecera por imagen
    for (int i = 0; i < NUM_IMAGES; i++) {
        //  imagen ancho * alto píxeles
        long ancho = 4928;
        long alto = 3264;
        total_operations += ancho * alto * 3; // 3 operaciones por píxel (RGB)
    }

    long total_instructions = total_operations * 20; // 20 instrucciones de ensamblador por operación
    printf("Instrucciones totales ejecutadas: %ld\n", total_instructions);

    double mips = total_instructions / (total_time * 1e6);
    printf("MIPS ejecutados: %.2f\n", mips);


    return 0;
}