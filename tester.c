#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "selec_proc.h"
#include <omp.h>

#define NUM_IMAGES 50
#define INPUT_DIR "imagenes"    // Carpeta de entrada
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
    double start_time, end_time;

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
                    desenfoque(out_file, in_file, 55);
                }
            }
        }
    }
    /*
    #pragma omp parallel for schedule(dynamic)  // Balanceo dinámico
    for (int i = 0; i < NUM_IMAGES; i++) {
        char filename_in[256];
        char filename_out_gray[256];
        char filename_out_blur[256];
        
        // Generar nombres (ej: "images/image_001.bmp")
        sprintf(filename_in, "%s/%d.bmp", INPUT_DIR, i);
        
        // Verificar si la imagen existe
        if (!file_exists(filename_in)) {
            printf("[Hilo %d] ¡Advertencia: %s no existe!\n", omp_get_thread_num(), filename_in);
            continue;  // Saltar si no existe
        }

        // Nombres de archivos de salida
        sprintf(filename_out_gray, "%s/gray_%03d.bmp", OUTPUT_DIR, i);
        sprintf(filename_out_blur, "%s/blur_%03d.bmp", OUTPUT_DIR, i);

        printf("[Hilo %d] Procesando %s\n", omp_get_thread_num(), filename_in);

        // Procesamiento (manejar errores)
        gray_img(filename_out_gray, filename_in);   
        inv_img_color_horizontal(filename_out_gray, filename_in);   
        inv_img_color_vertical(filename_out_gray, filename_in);  
        inv_img_grey_horizontal(filename_out_gray, filename_in);  
        inv_img_grey_vertical(filename_out_gray, filename_in);    // Conversión a grises
        desenfoque(filename_out_blur, filename_in, 5); // Desenfoque
    }

    */
    
    //printf("Procesamiento completado. Resultados en %s/\n", OUTPUT_DIR);
    printf("Procesamiento completado. Resultados en %s/\n", OUTPUT_DIR);
    
    end_time = omp_get_wtime();

    printf("Tiempo de ejecución: %f segundos\n", end_time - start_time);
    return 0;
}