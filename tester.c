#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "selec_proc.h"

int main() {

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
    }

    return 0;
}