//Emmanuel Torres
//Tecnologico de Monterrey
//Campus Puebla
//Octubre 2024
extern void intoa(int N, char *str) {
    int i = 0;
  
    // Save the copy of the number for sign
    int sign = N;

    // If the number is negative, make it positive
    if (N < 0)
        N = -N;

    // Extract digits from the number and add them to the
    // string
    while (N > 0) {
      
        // Convert integer digit to character and store
      	// it in the str
        str[i++] = N % 10 + '0';
      	N /= 10;
    } 

    // If the number was negative, add a minus sign to the
    // string
    if (sign < 0) {
        str[i++] = '-';
    }

    // Null-terminate the string
    str[i] = '\0';

    // Reverse the string to get the correct order
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}

void gray_img(char mask[10], char path[80]) {
    FILE *image, *outputImage;
    char add_char[80] = "./img_res/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");

    image = fopen(path, "rb");
    if (image == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo %s\n", path);
        return;
    }

    outputImage = fopen(add_char, "wb");
    if (outputImage == NULL) {
        fprintf(stderr, "Error: No se pudo crear el archivo %s\n", add_char);
        fclose(image);
        return;
    }

    unsigned char xx[54];
    for (int i = 0; i < 54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage);
    }

    long ancho = (long)xx[20] * 65536 + (long)xx[19] * 256 + (long)xx[18];
    long alto = (long)xx[24] * 65536 + (long)xx[23] * 256 + (long)xx[22];

    unsigned char* arr_in = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    if (arr_in == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para arr_in.\n");
        fclose(image);
        fclose(outputImage);
        return;
    }

    int j = 0;
    unsigned char r, g, b, pixel;
    while (j < ancho * alto && !feof(image)) {
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);
        if (feof(image)) break;
        pixel = 0.21 * r + 0.72 * g + 0.07 * b;
        arr_in[j] = pixel;
        j++;
    }

    // Escribir imagen de salida en escala de grises
    for (int i = 0; i < ancho * alto; i++) {
        fputc(arr_in[i], outputImage); // Blue
        fputc(arr_in[i], outputImage); // Green
        fputc(arr_in[i], outputImage); // Red
    }

    // Escritura en archivo de registro
    FILE *outputLog = fopen("output_log.txt", "a");
    if (outputLog == NULL) {
        fprintf(stderr, "Error: No se pudo crear o abrir el archivo de registro.\n");
        free(arr_in);
        fclose(image);
        fclose(outputImage);
        return;
    }

    fprintf(outputLog, "Función: %s\n", "gray_img");
    fprintf(outputLog, "Localidades totales leídas: %ld\n", ancho * alto);
    fprintf(outputLog, "Localidades totales escritas: %ld\n", ancho * alto);
    fprintf(outputLog, "-------------------------------------\n");
    fclose(outputLog);

    free(arr_in);
    fclose(image);
    fclose(outputImage);
}


void inv_img_grey_horizontal(char mask[10], char path[80]) {
    FILE *image, *outputImage;
    char add_char[80] = "./img_res/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");

    image = fopen(path, "rb");
    if (image == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo %s\n", path);
        return;
    }

    outputImage = fopen(add_char, "wb");
    if (outputImage == NULL) {
        fprintf(stderr, "Error: No se pudo crear el archivo %s\n", add_char);
        fclose(image);
        return;
    }

    // Leer cabecera
    unsigned char xx[54];
    for (int i = 0; i < 54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage);
    }

    long tam = (long)xx[4] * 65536 + (long)xx[3] * 256 + (long)xx[2];
    long bpp = (long)xx[29] * 256 + (long)xx[28];
    long ancho = (long)xx[20] * 65536 + (long)xx[19] * 256 + (long)xx[18];
    long alto = (long)xx[24] * 65536 + (long)xx[23] * 256 + (long)xx[22];

    // Asignar memoria para los píxeles
    unsigned char* arr_in = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    if (arr_in == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para arr_in.\n");
        fclose(image);
        fclose(outputImage);
        return;
    }

    // Leer píxeles
    int j = 0;
    unsigned char r, g, b, pixel;
    while (j < ancho * alto && !feof(image)) {
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);
        if (feof(image)) break;
        pixel = 0.21 * r + 0.72 * g + 0.07 * b;
        arr_in[j] = pixel;
        j++;
    }

    // Escribir píxeles invertidos
    for (int i = 0; i < ancho * alto; i++) {
        fputc(arr_in[(ancho * alto) - i - 1], outputImage);
        fputc(arr_in[(ancho * alto) - i - 1], outputImage);
        fputc(arr_in[(ancho * alto) - i - 1], outputImage);
    }

    // Escritura en archivo de registro
    FILE *outputLog = fopen("output_log.txt", "a");
    if (outputLog == NULL) {
        fprintf(stderr, "Error: No se pudo crear o abrir el archivo de registro.\n");
        free(arr_in);
        fclose(image);
        fclose(outputImage);
        return;
    }

    fprintf(outputLog, "Función: %s\n", "invert_img_grey_horizontal");
    fprintf(outputLog, "Localidades totales leídas: %ld\n", ancho * alto);
    fprintf(outputLog, "Localidades totales escritas: %ld\n", ancho * alto);
    fprintf(outputLog, "-------------------------------------\n");
    fclose(outputLog);

    free(arr_in);
    fclose(image);
    fclose(outputImage);
}

void inv_img_grey_vertical(char mask[10], char path[80]) {
    FILE *image, *outputImage;
    char add_char[80] = "./img_res/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");

    image = fopen(path, "rb");
    if (image == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo %s\n", path);
        return;
    }

    outputImage = fopen(add_char, "wb");
    if (outputImage == NULL) {
        fprintf(stderr, "Error: No se pudo crear el archivo %s\n", add_char);
        fclose(image);
        return;
    }

    // Leer cabecera
    unsigned char xx[54];
    for (int i = 0; i < 54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage);
    }

    long tam = (long)xx[4] * 65536 + (long)xx[3] * 256 + (long)xx[2];
    long bpp = (long)xx[29] * 256 + (long)xx[28];
    long ancho = (long)xx[20] * 65536 + (long)xx[19] * 256 + (long)xx[18];
    long alto = (long)xx[24] * 65536 + (long)xx[23] * 256 + (long)xx[22];

    // Asignar memoria para los píxeles
    unsigned char* arr_in = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    if (arr_in == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para arr_in.\n");
        fclose(image);
        fclose(outputImage);
        return;
    }

    // Leer píxeles
    int j = 0;
    unsigned char r, g, b, pixel;
    while (j < ancho * alto && !feof(image)) {
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);
        if (feof(image)) break;
        pixel = 0.21 * r + 0.72 * g + 0.07 * b;
        arr_in[j] = pixel;
        j++;
    }

    // Inversión Vertical 
    for(int y = 0; y < alto; y++) {
        for(int x = 0; x < ancho; x++) {
            int original_pos = y * ancho + x;
            int flipped_pos = y * ancho + (ancho - 1 - x);  // Cálculo clave
            
            fputc(arr_in[flipped_pos], outputImage);
            fputc(arr_in[flipped_pos], outputImage);
            fputc(arr_in[flipped_pos], outputImage);
        }
    }

    // Escritura en archivo de registro
    FILE *outputLog = fopen("output_log.txt", "a");
    if (outputLog == NULL) {
        fprintf(stderr, "Error: No se pudo crear o abrir el archivo de registro.\n");
        free(arr_in);
        fclose(image);
        fclose(outputImage);
        return;
    }

    fprintf(outputLog, "Función: %s\n", "inv_img_grey_vertical");
    fprintf(outputLog, "Localidades totales leídas: %ld\n", ancho * alto);
    fprintf(outputLog, "Localidades totales escritas: %ld\n", ancho * alto);
    fprintf(outputLog, "-------------------------------------\n");
    fclose(outputLog);

    free(arr_in);
    fclose(image);
    fclose(outputImage);
}

void inv_img_color_horizontal(char mask[10], char path[80]){

    FILE *image, *outputImage, *lecturas, *fptr;    
    // char PATH_0[120] = "/Users/emmanueltorres/Documents/image_proces_c/c_openmp/img/";
    char add_char[80] = "./img_res/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, tam1;
    long ancho, tam, bpp;
    long alto;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    for(int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    tam1 = tam;

    unsigned char* arr_in_b = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
    unsigned char* arr_in_g = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
    unsigned char* arr_in_r = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
   
    j = 0;

    while(!feof(image)){
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);
        
        arr_in_b[j] = b;
        arr_in_g[j] = g;
        arr_in_r[j] = r;
        j++;
    }

    // Escribir píxeles invertidos
    for (int i = 0; i < ancho * alto; i++) {
        fputc(arr_in_b[(ancho * alto) - i - 1], outputImage);
        fputc(arr_in_g[(ancho * alto) - i - 1], outputImage);
        fputc(arr_in_r[(ancho * alto) - i - 1], outputImage);
    }

    // Escritura en archivo de registro
    FILE *outputLog = fopen("output_log.txt", "a");
    if (outputLog == NULL) {
        fprintf(stderr, "Error: No se pudo crear o abrir el archivo de registro.\n");
        fclose(image);
        fclose(outputImage);
        return;
    }

    fprintf(outputLog, "Función: %s\n", "inv_img_color_horizontal");
    fprintf(outputLog, "Localidades totales leídas: %ld\n", ancho * alto);
    fprintf(outputLog, "Localidades totales escritas: %ld\n", ancho * alto);
    fprintf(outputLog, "-------------------------------------\n");
    fclose(outputLog);
    
    free(arr_in_b);
    free(arr_in_g);
    free(arr_in_r);
    fclose(image);
    fclose(outputImage);

}

void inv_img_color_vertical(char mask[10], char path[80]){

    FILE *image, *outputImage, *lecturas, *fptr;    
    // char PATH_0[120] = "/Users/emmanueltorres/Documents/image_proces_c/c_openmp/img/";
    char add_char[80] = "./img_res/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    image = fopen(path,"rb");          //Original Image
    outputImage = fopen(add_char,"wb");

	//Definition of variables
	int i, j, tam1;
    long ancho, tam, bpp;
    long alto;
    unsigned char r, g, b, pixel;               //Pixel

    unsigned char xx[54];
    for(int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    bpp = (long)xx[29]*256+(long)xx[28];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    tam1 = tam;

    unsigned char* arr_in_b = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
    unsigned char* arr_in_g = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
    unsigned char* arr_in_r = (unsigned char*)malloc(ancho*alto*sizeof(unsigned char));
   
    j = 0;

    while(!feof(image)){
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);
        
        arr_in_b[j] = b;
        arr_in_g[j] = g;
        arr_in_r[j] = r;
        j++;
    }
    
    // Inversión Vertical 
    for(int y = 0; y < alto; y++) {
        for(int x = 0; x < ancho; x++) {
            int original_pos = y * ancho + x;
            int flipped_pos = y * ancho + (ancho - 1 - x);  // Cálculo clave
            
            fputc(arr_in_b[flipped_pos], outputImage);
            fputc(arr_in_g[flipped_pos], outputImage);
            fputc(arr_in_r[flipped_pos], outputImage);
        }
    }


    // Escritura en archivo de registro
    FILE *outputLog = fopen("output_log.txt", "a");
    if (outputLog == NULL) {
        fprintf(stderr, "Error: No se pudo crear o abrir el archivo de registro.\n");
        fclose(image);
        fclose(outputImage);
        return;
    }

    fprintf(outputLog, "Función: %s\n", "inv_img_color_vertical");
    fprintf(outputLog, "Localidades totales leídas: %ld\n", ancho * alto);
    fprintf(outputLog, "Localidades totales escritas: %ld\n", ancho * alto);
    fprintf(outputLog, "-------------------------------------\n");
    fclose(outputLog);

    free(arr_in_b);
    free(arr_in_g);
    free(arr_in_r);
    fclose(image);
    fclose(outputImage);

}

void desenfoque(const char* input_path, const char* name_output, int kernel_size) {
    FILE *image, *outputImage;
    char output_path[100] = "./img_res/";
    strcat(output_path, name_output);
    strcat(output_path, ".bmp");

    image = fopen(input_path, "rb");
    outputImage = fopen(output_path, "wb");

    if (!image || !outputImage) {
        printf("Error abriendo archivos.\n");
        return;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);
    fwrite(header, sizeof(unsigned char), 54, outputImage);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int row_padded = (width * 3 + 3) & (~3);

    unsigned char** input_rows = (unsigned char**)malloc(height * sizeof(unsigned char*));
    unsigned char** output_rows = (unsigned char**)malloc(height * sizeof(unsigned char*));
    unsigned char** temp_rows = (unsigned char**)malloc(height * sizeof(unsigned char*));

    for (int i = 0; i < height; i++) {
        input_rows[i] = (unsigned char*)malloc(row_padded);
        output_rows[i] = (unsigned char*)malloc(row_padded);
        temp_rows[i] = (unsigned char*)malloc(row_padded);
        fread(input_rows[i], sizeof(unsigned char), row_padded, image);
    }

    int k = kernel_size / 2;

    // Paso intermedio: desenfoque horizontal
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int sumB = 0, sumG = 0, sumR = 0, count = 0;

            for (int dx = -k; dx <= k; dx++) {
                int nx = x + dx;
                if (nx >= 0 && nx < width) {
                    int idx = nx * 3;
                    sumB += input_rows[y][idx + 0];
                    sumG += input_rows[y][idx + 1];
                    sumR += input_rows[y][idx + 2];
                    count++;
                }
            }

            int index = x * 3;
            temp_rows[y][index + 0] = sumB / count;
            temp_rows[y][index + 1] = sumG / count;
            temp_rows[y][index + 2] = sumR / count;
        }

        // Copiar padding sin modificar
        for (int p = width * 3; p < row_padded; p++) {
            temp_rows[y][p] = input_rows[y][p];
        }
    }

    // Paso final: desenfoque vertical
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int sumB = 0, sumG = 0, sumR = 0, count = 0;

            for (int dy = -k; dy <= k; dy++) {
                int ny = y + dy;
                if (ny >= 0 && ny < height) {
                    int idx = x * 3;
                    sumB += temp_rows[ny][idx + 0];
                    sumG += temp_rows[ny][idx + 1];
                    sumR += temp_rows[ny][idx + 2];
                    count++;
                }
            }

            int index = x * 3;
            output_rows[y][index + 0] = sumB / count;
            output_rows[y][index + 1] = sumG / count;
            output_rows[y][index + 2] = sumR / count;
        }

        // Copiar padding sin modificar
        for (int p = width * 3; p < row_padded; p++) {
            output_rows[y][p] = temp_rows[y][p];
        }
    }

    // Escritura final
    for (int i = 0; i < height; i++) {
        fwrite(output_rows[i], sizeof(unsigned char), row_padded, outputImage);
        free(input_rows[i]);
        free(temp_rows[i]);
        free(output_rows[i]);
    }

    // Escritura en archivo de registro
    FILE *outputLog = fopen("output_log.txt", "a");
    if (outputLog == NULL) {
        fprintf(stderr, "Error: No se pudo crear o abrir el archivo de registro.\n");
        fclose(image);
        fclose(outputImage);
        return;
    }

    fprintf(outputLog, "Función: %s\n", "desenfoque");
    fprintf(outputLog, "Localidades totales leídas: %d\n", width * height);
    fprintf(outputLog, "Localidades totales escritas: %d\n", width * height);
    fprintf(outputLog, "-------------------------------------\n");
    fclose(outputLog);

    free(input_rows);
    free(temp_rows);
    free(output_rows);
    fclose(image);
    fclose(outputImage);
}