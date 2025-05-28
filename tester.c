#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "selec_proc.h"
#include <omp.h>

#define NUM_IMAGES 100
#define OUTPUT_DIR ""
// Define los rangos para dividir 100 imágenes en 3 bloques
#define BLOCK1_START 0
#define BLOCK1_END 32

#define BLOCK2_START 33
#define BLOCK2_END 65

#define BLOCK3_START 66
#define BLOCK3_END 99


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
        }
    }
    fclose(file);
    return 1;
}

int main() {

    // Vaciar el archivo progress.txt al inicio
    FILE *progress = fopen("progress.txt", "w");
    if (progress) {
        fclose(progress);
    }

    printf("Iniciando el procesamiento de imágenes...\n");

    Config cfg = {0};
    if (!parse_config(&cfg, "config.txt")) {
        printf("No se pudo abrir config.txt\n");
        return 1;
    }
    //printf("Config: grayscale=%d, flip_h=%d, flip_v=%d, gray_flip_h=%d, gray_flip_v=%d, blur=%d, folder_path=%s, blur_value=%d\n",
        //cfg.grayscale, cfg.flip_h, cfg.flip_v, cfg.gray_flip_h, cfg.gray_flip_v, cfg.blur, cfg.folder_path, cfg.blur_value);

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
            if (cfg.gray_flip_h){
                {
                    for (int i = BLOCK1_START; i <= BLOCK1_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp", cfg.folder_path, i);
                        sprintf(out_file, "inv_grey_horizontal_%d.bmp", i);
                        inv_img_grey_horizontal(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }}
            #pragma omp section
                if (cfg.gray_flip_h){
                    for (int i = BLOCK2_START; i <= BLOCK2_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_grey_horizontal_%d.bmp", i);
                        inv_img_grey_horizontal(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }}
            #pragma omp section
                if (cfg.gray_flip_h){
                    for (int i = BLOCK3_START; i <= BLOCK3_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_grey_horizontal_%d.bmp", i);
                        inv_img_grey_horizontal(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }

            #pragma omp section
                if (cfg.gray_flip_v){
                    for (int i = BLOCK1_START; i <= BLOCK1_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_grey_vertical_%d.bmp", i);
                        inv_img_grey_vertical(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }
            #pragma omp section
                if (cfg.gray_flip_v){
                    for (int i = BLOCK2_START; i <= BLOCK2_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_grey_vertical_%d.bmp", i);
                        inv_img_grey_vertical(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }
            #pragma omp section
                if (cfg.gray_flip_v){
                    for (int i = BLOCK3_START; i <= BLOCK3_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_grey_vertical_%d.bmp", i);
                        inv_img_grey_vertical(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }
            #pragma omp section
                if(cfg.flip_h){
                    for (int i = BLOCK1_START; i <= BLOCK1_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_color_horizontal_%d.bmp", i);
                        inv_img_color_horizontal(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }
            #pragma omp section
                if(cfg.flip_h){
                    for (int i = BLOCK2_START; i <= BLOCK2_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_color_horizontal_%d.bmp", i);
                        inv_img_color_horizontal(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }
            #pragma omp section
                if(cfg.flip_h){
                    for (int i = BLOCK3_START; i <= BLOCK3_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_color_horizontal_%d.bmp", i);
                        inv_img_color_horizontal(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }
            #pragma omp section
                if (cfg.flip_v){
                    for (int i = BLOCK1_START; i <= BLOCK1_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_color_vertical_%d.bmp", i);
                        inv_img_color_vertical(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }
            #pragma omp section
                if (cfg.flip_v){
                    for (int i = BLOCK2_START; i <= BLOCK2_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_color_vertical_%d.bmp", i);
                        inv_img_color_vertical(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }
            #pragma omp section
                if (cfg.flip_v){
                    for (int i = BLOCK3_START; i <= BLOCK3_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "inv_color_vertical_%d.bmp", i);
                        inv_img_color_vertical(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }
            #pragma omp section
                if (cfg.grayscale){
                    for (int i = BLOCK1_START; i <= BLOCK1_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "grey_%d.bmp", i);
                        gray_img(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }
            #pragma omp section
                if (cfg.grayscale){
                    for (int i = BLOCK2_START; i <= BLOCK2_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "grey_%d.bmp", i);
                        gray_img(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }
            #pragma omp section
                if (cfg.grayscale){
                    for (int i = BLOCK3_START; i <= BLOCK3_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "grey_%d.bmp", i);
                        gray_img(out_file, in_file);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }
            #pragma omp section
                if (cfg.blur){
                    for (int i = BLOCK1_START; i <= BLOCK1_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "desenfoque_%d.bmp", i);
                        desenfoque(in_file, out_file, cfg.blur_value);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }
            #pragma omp section
                if (cfg.blur){
                    for (int i = BLOCK2_START; i <= BLOCK2_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "desenfoque_%d.bmp", i);
                        desenfoque(in_file, out_file, cfg.blur_value);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
                    }
                }
            #pragma omp section
                if (cfg.blur){
                    for (int i = BLOCK3_START; i <= BLOCK3_END; i++) {
                        char out_file[500], in_file[500];
                        sprintf(in_file, "%s/%d.bmp",cfg.folder_path, i);
                        sprintf(out_file, "desenfoque_%d.bmp", i);
                        desenfoque(in_file, out_file, cfg.blur_value);

                        // Escribir un '1' en progress.txt después de procesar cada imagen
                        FILE *progress = fopen("progress.txt", "a");
                        if (progress) {
                            fputc('1', progress);
                            fclose(progress);
                        }
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

    // Calcular el número total de bytes procesados
    long total_bytes = NUM_IMAGES * 54; // 54 bytes de cabecera por imagen
    for (int i = 0; i < NUM_IMAGES; i++) {
        total_bytes += 4928 * 3264 * 3; // Ancho * Alto * 3 bytes por píxel (RGB)
    }

    // Calcular la tasa de bytes promedio
    double avg_byte_rate = total_bytes / total_time;
    printf("Tasa de bytes promedio: %.2f bytes/segundo\n", avg_byte_rate);


    return 0;
}