#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "selec_proc.h"
#include <omp.h>
#include <mpi.h>

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

typedef struct {
    int grayscale;
    int flip_h;
    int flip_v;
    int gray_flip_h;
    int gray_flip_v;
    int blur;
    char folder_path[256];
    int blur_value;
    int total_imgs;
} Config;

int parse_config(Config *cfg, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char key[64], value[256];
        if (sscanf(line, "%63[^=]=%255[^\n]", key, value) == 2) {
            if (strcmp(key, "grayscale") == 0) cfg->grayscale = atoi(value);
            else if (strcmp(key, "flip_h") == 0) cfg->flip_h = atoi(value);
            else if (strcmp(key, "flip_v") == 0) cfg->flip_v = atoi(value);
            else if (strcmp(key, "gray_flip_h") == 0) cfg->gray_flip_h = atoi(value);
            else if (strcmp(key, "gray_flip_v") == 0) cfg->gray_flip_v = atoi(value);
            else if (strcmp(key, "blur") == 0) cfg->blur = atoi(value);
            else if (strcmp(key, "folder_path") == 0) strncpy(cfg->folder_path, value, sizeof(cfg->folder_path)-1);
            else if (strcmp(key, "blur_value") == 0) cfg->blur_value = atoi(value);
            else if (strcmp(key, "total_imgs") == 0) cfg->total_imgs = atoi(value);
        }
    }
    fclose(file);
    return 1;
}

int main(int argc, char** argv) {
    int myrank, nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    // Vaciar el archivo progress.txt al inicio
    FILE *progress0 = fopen("progress0.txt", "w");
    if (progress0) {
        fclose(progress0);
    }

    FILE *progress1 = fopen("progress1.txt", "w");
    if (progress1) {
        fclose(progress1);
    }

    FILE *progress2 = fopen("progress2.txt", "w");
    if (progress2) {
        fclose(progress2);
    }

    printf("Iniciando el procesamiento de imágenes...\n");

    Config cfg = {0};
    if (!parse_config(&cfg, "config.txt")) {
        printf("No se pudo abrir config.txt\n");
        return 1;
    }

    // Calcular los bloques dinámicamente según total_imgs
    int total_imgs = cfg.total_imgs;
    int block_size = total_imgs / 3;
    int remainder = total_imgs % 3;

    int block1_start = 0;
    int block1_end = block_size - 1 + (remainder > 0 ? 1 : 0);

    int block2_start = block1_end + 1;
    int block2_end = block2_start + block_size - 1 + (remainder > 1 ? 1 : 0);

    int block3_start = block2_end + 1;
    int block3_end = total_imgs - 1;

    // Ahora puedes usar block1_start, block1_end, block2_start, block2_end, block3_start, block3_end en tus ciclos

    // Crear la carpeta "img_res/" si no existe
    struct stat st = {0};
    if (stat("img_res", &st) == -1) {
        if (mkdir("img_res", 0700) != 0) {
            fprintf(stderr, "Error: No se pudo crear la carpeta 'img_res/'.\n");
            return 1;
        }
        //printf("Carpeta 'img_res/' creada exitosamente.\n");
    } else {
       // printf("Carpeta 'img_res/' ya existe.\n");
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
    omp_set_num_threads(18);  
    //printf("cfg.folder_path: %s\n", cfg.folder_path);
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            if (cfg.gray_flip_h && myrank == 0) {
                {
                    for (int i = block1_start; i <= block1_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp", cfg.folder_path, i);
                        sprintf(out_file, "inv_grey_horizontal_%d.bmp", i);
                        inv_img_grey_horizontal(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress0 = fopen("progress0.txt", "a");
                        if (progress0) {
                            fputc('1', progress0);
                            fclose(progress0);
                        }
                        printf("Procesada imagen: %s, %d\n", out_file, myrank);
                    }
                }}
            #pragma omp section
                if (cfg.gray_flip_h && myrank == 1){
                    for (int i = block2_start; i <= block2_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_grey_horizontal_%d.bmp", i);
                        inv_img_grey_horizontal(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress1 = fopen("progress1.txt", "a");
                        if (progress1) {
                            fputc('1', progress1);
                            fclose(progress1);
                        }
                        printf("Procesada imagen: %s, %d\n", out_file, myrank);
                    }}
            #pragma omp section
                if (cfg.gray_flip_h && myrank == 2){
                    for (int i = block3_start; i <= block3_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_grey_horizontal_%d.bmp", i);
                        inv_img_grey_horizontal(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress2 = fopen("progress2.txt", "a");
                        if (progress2) {
                            fputc('1', progress2);
                            fclose(progress2);
                        }
                        printf("Procesada imagen: %s, %d\n", out_file, myrank);
                    }
                }

            #pragma omp section
                if (cfg.gray_flip_v && myrank == 0){
                    for (int i = block1_start; i <= block1_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_grey_vertical_%d.bmp", i);
                        inv_img_grey_vertical(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress0 = fopen("progress0.txt", "a");
                        if (progress0) {
                            fputc('1', progress0);
                            fclose(progress0);
                        }
                    }
                }
            #pragma omp section
                if (cfg.gray_flip_v && myrank == 1){
                    for (int i = block2_start; i <= block2_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_grey_vertical_%d.bmp", i);
                        inv_img_grey_vertical(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress1 = fopen("progress1.txt", "a");
                        if (progress1) {
                            fputc('1', progress1);
                            fclose(progress1);
                        }
                    }
                }
            #pragma omp section
                if (cfg.gray_flip_v && myrank == 2){
                    for (int i = block3_start; i <= block3_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_grey_vertical_%d.bmp", i);
                        inv_img_grey_vertical(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress2 = fopen("progress2.txt", "a");
                        if (progress2) {
                            fputc('1', progress2);
                            fclose(progress2);
                        }
                    }
                }
            #pragma omp section
                if(cfg.flip_h && myrank == 0){
                    for (int i = block1_start; i <= block1_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_color_horizontal_%d.bmp", i);
                        inv_img_color_horizontal(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress0 = fopen("progress0.txt", "a");
                        if (progress0) {
                            fputc('1', progress0);
                            fclose(progress0);
                        }
                    }
                }
            #pragma omp section
                if(cfg.flip_h && myrank == 1){
                    for (int i = block2_start; i <= block2_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_color_horizontal_%d.bmp", i);
                        inv_img_color_horizontal(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress1 = fopen("progress1.txt", "a");
                        if (progress1) {
                            fputc('1', progress1);
                            fclose(progress1);
                        }
                    }
                }
            #pragma omp section
                if(cfg.flip_h && myrank == 2){
                    for (int i = block3_start; i <= block3_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_color_horizontal_%d.bmp", i);
                        inv_img_color_horizontal(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress2 = fopen("progress2.txt", "a");
                        if (progress2) {
                            fputc('1', progress2);
                            fclose(progress2);
                        }
                    }
                }
            #pragma omp section
                if (cfg.flip_v && myrank == 0){
                    for (int i = block1_start; i <= block1_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_color_vertical_%d.bmp", i);
                        inv_img_color_vertical(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress0 = fopen("progress0.txt", "a");
                        if (progress0) {
                            fputc('1', progress0);
                            fclose(progress0);
                        }
                    }
                }
            #pragma omp section
                if (cfg.flip_v && myrank == 1){
                    for (int i = block2_start; i <= block2_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_color_vertical_%d.bmp", i);
                        inv_img_color_vertical(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress1 = fopen("progress1.txt", "a");
                        if (progress1) {
                            fputc('1', progress1);
                            fclose(progress1);
                        }
                    }
                }
            #pragma omp section
                if (cfg.flip_v && myrank == 2){
                    for (int i = block3_start; i <= block3_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_color_vertical_%d.bmp", i);
                        inv_img_color_vertical(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress2 = fopen("progress2.txt", "a");
                        if (progress2) {
                            fputc('1', progress2);
                            fclose(progress2);
                        }
                    }
                }
            #pragma omp section
                if (cfg.grayscale && myrank == 0){
                    for (int i = block1_start; i <= block1_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "grey_%d.bmp", i);
                        gray_img(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress0 = fopen("progress0.txt", "a");
                        if (progress0) {
                            fputc('1', progress0);
                            fclose(progress0);
                        }
                    }
                }
            #pragma omp section
                if (cfg.grayscale && myrank == 1){
                    for (int i = block2_start; i <= block2_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "grey_%d.bmp", i);
                        gray_img(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress1 = fopen("progress1.txt", "a");
                        if (progress1) {
                            fputc('1', progress1);
                            fclose(progress1);
                        }
                    }
                }
            #pragma omp section
                if (cfg.grayscale && myrank == 2){
                    for (int i = block3_start; i <= block3_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "grey_%d.bmp", i);
                        gray_img(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress2 = fopen("progress2.txt", "a");
                        if (progress2) {
                            fputc('1', progress2);
                            fclose(progress2);
                        }
                    }
                }
            #pragma omp section
                if (cfg.blur && myrank == 0){
                    for (int i = block1_start; i <= block1_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "desenfoque_%d.bmp", i);
                        desenfoque(in_file, out_file, cfg.blur_value);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress0 = fopen("progress0.txt", "a");
                        if (progress0) {
                            fputc('1', progress0);
                            fclose(progress0);
                        }
                    }
                }
            #pragma omp section
                if (cfg.blur && myrank == 1){
                    for (int i = block2_start; i <= block2_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "desenfoque_%d.bmp", i);
                        desenfoque(in_file, out_file, cfg.blur_value);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress1 = fopen("progress1.txt", "a");
                        if (progress1) {
                            fputc('1', progress1);
                            fclose(progress1);
                        }
                    }
                }
            #pragma omp section
                if (cfg.blur && myrank == 2){
                    for (int i = block3_start; i <= block3_end; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "desenfoque_%d.bmp", i);
                        desenfoque(in_file, out_file, cfg.blur_value);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress2 = fopen("progress2.txt", "a");
                        if (progress2) {
                            fputc('1', progress2);
                            fclose(progress2);
                        }
                    }
                }
            }
        }
    
    
    end_time = omp_get_wtime();
    total_time = end_time - start_time;

    printf("Procesamiento completado. Resultados en %s/\n", OUTPUT_DIR);

    printf("Tiempo de ejecución: %f segundos\n", total_time);

    long total_operations = cfg.total_imgs * 54; // 54 bytes de cabecera por imagen
    for (int i = 0; i < cfg.total_imgs; i++) {
        //  imagen ancho * alto píxeles
        long ancho = 4928;
        long alto = 3264;
        total_operations += ancho * alto * 3; // 3 operaciones por píxel (RGB)
    }

    long total_instructions = total_operations * 20; // 20 instrucciones de ensamblador por operación
    printf("Instrucciones totales ejecutadas: %ld\n", total_instructions);

    double mips = total_instructions / (total_time * 1e6);
    printf("MIPS ejecutados: %.2f\n", mips);

    // Calcular el número total de bytes procesados
    long total_bytes = cfg.total_imgs * 54; // 54 bytes de cabecera por imagen
    for (int i = 0; i < cfg.total_imgs; i++) {
        total_bytes += 4928 * 3264 * 3; // Ancho * Alto * 3 bytes por píxel (RGB)
    }

    // Calcular la tasa de bytes promedio
    double avg_byte_rate = total_bytes / total_time;
    printf("Tasa de bytes promedio: %.2f bytes/segundo\n", avg_byte_rate);

    MPI_Finalize(); // <-- Agrega esta línea antes de terminar main

    return 0;
}