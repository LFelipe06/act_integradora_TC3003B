#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "selec_proc.h"
#include <omp.h>

int main() {
    double start_time, end_time;

    start_time = omp_get_wtime();
    /*
    // Crear la carpeta ./img si no existe
    struct stat st = {0};
    if (stat("./img_res", &st) == -1) {
        mkdir("./img_res", 0700);
    }

    char folder_path[] = "./bmps/";
    char file_name[100];
    char mask[20];

    for (int i = 0; i <= 9; i++) {
        snprintf(file_name, sizeof(file_name), "%s00000%d.bmp", folder_path, i);
        snprintf(mask, sizeof(mask), "inv_%d", i);

        FILE* test_file = fopen(file_name, "rb");
        if (test_file == NULL) {
            printf("Error: No se pudo abrir el archivo %s\n", file_name);
            continue; 
        }
        fclose(test_file);

        // Procesar la imagen en escala de grises
        inv_img(mask, file_name);

        // Procesar la imagen en color invertido
        snprintf(mask, sizeof(mask), "inv_color_%d", i);
        inv_img_color(mask, file_name);
    }*/

    omp_set_num_threads(5);
    
    #pragma omp parallel
    {
        #pragma omp single
        {
            printf("Creando tareas paralelas con %d hilos disponibles...\n", omp_get_num_threads());
            
            #pragma omp task
            {
                printf("[Tarea 1] Inversión gris horizontal\n");
                inv_img_grey_horizontal("inv_grey_horizontal_1", "sample1.bmp");
            }
            
            #pragma omp task
            {
                printf("[Tarea 2] Inversión gris vertical\n");
                inv_img_grey_vertical("inv_grey_vertical_1", "sample1.bmp");
            }
            
            #pragma omp task
            {
                printf("[Tarea 3] Inversión color horizontal\n");
                inv_img_color_horizontal("inv_color_horizontal_1", "sample1.bmp");
            }
            
            #pragma omp task
            {
                printf("[Tarea 4] Inversión color vertical\n");
                inv_img_color_vertical("inv_color_vertical_1", "sample1.bmp");
            }
            
            #pragma omp task
            {
                printf("[Tarea 5] Conversión a grises\n");
                gray_img("grey_1", "sample1.bmp");
            }
        }
    } // Barrera implícita - todas las tareas completan aquí
    
    printf("Procesamiento de imágenes completado\n");

    end_time = omp_get_wtime();

    printf("Tiempo de ejecución: %f segundos\n", end_time - start_time);
    return 0;
}