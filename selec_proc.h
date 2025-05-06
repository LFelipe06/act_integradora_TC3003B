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
    printf("%s\n", add_char);

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

    printf("Tamaño archivo: %li\n", tam);
    printf("Bits por pixel: %li\n", bpp);
    printf("Largo img: %li\n", alto);
    printf("Ancho img: %li\n", ancho);

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

    // Escribir píxeles (esta es la parte que debes modificar)
    for (int i = 0; i < ancho * alto; i++) {
        fputc(arr_in[i], outputImage); // Blue
        fputc(arr_in[i], outputImage); // Green
        fputc(arr_in[i], outputImage); // Red
    }

    free(arr_in);
    fclose(image);
    fclose(outputImage);
}

void inv_img_grey_horizontal(char mask[10], char path[80]) {
    FILE *image, *outputImage;
    char add_char[80] = "./img_res/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    printf("%s\n", add_char);

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

    printf("Tamaño archivo: %li\n", tam);
    printf("Bits por pixel: %li\n", bpp);
    printf("Largo img: %li\n", alto);
    printf("Ancho img: %li\n", ancho);

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

    free(arr_in);
    fclose(image);
    fclose(outputImage);
}

void inv_img_grey_vertical(char mask[10], char path[80]) {
    FILE *image, *outputImage;
    char add_char[80] = "./img_res/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    printf("%s\n", add_char);

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

    printf("Tamaño archivo: %li\n", tam);
    printf("Bits por pixel: %li\n", bpp);
    printf("Largo img: %li\n", alto);
    printf("Ancho img: %li\n", ancho);

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
    printf("%s\n", add_char);
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
    printf("tamano archivo %li\n", tam);
    tam1 = tam;
    printf("bits por pixel %li\n", bpp);
    printf("largo img %li\n",alto);
    printf("ancho img %li\n",ancho);

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

    printf("lectura de datos: %d\n", j * 3);
    printf("elementos faltantes: %d\n", tam1 - (j * 3));
    printf("\n");


    // Escribir píxeles invertidos
    for (int i = 0; i < ancho * alto; i++) {
        fputc(arr_in_b[(ancho * alto) - i - 1], outputImage);
        fputc(arr_in_g[(ancho * alto) - i - 1], outputImage);
        fputc(arr_in_r[(ancho * alto) - i - 1], outputImage);
    }
    
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
    printf("%s\n", add_char);
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
    printf("tamano archivo %li\n", tam);
    tam1 = tam;
    printf("bits por pixel %li\n", bpp);
    printf("largo img %li\n",alto);
    printf("ancho img %li\n",ancho);

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

    printf("lectura de datos: %d\n", j * 3);
    printf("elementos faltantes: %d\n", tam1 - (j * 3));
    printf("\n");
    
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
    free(arr_in_b);
    free(arr_in_g);
    free(arr_in_r);
    fclose(image);
    fclose(outputImage);

}

void desenfoque(char mask[10], char path[80], int kernel_size) {
    if (kernel_size % 2 == 0 || kernel_size < 3) {
        fprintf(stderr, "Error: kernel_size debe ser impar y >= 3\n");
        return;
    }

    FILE *image, *outputImage;
    char add_char[80] = "./img_res/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    printf("%s\n", add_char);

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
    unsigned char header[54];
    for (int i = 0; i < 54; i++) {
        header[i] = fgetc(image);
        fputc(header[i], outputImage);
    }

    long tam = (long)header[4] * 65536 + (long)header[3] * 256 + (long)header[2];
    long bpp = (long)header[29] * 256 + (long)header[28];
    long ancho = (long)header[20] * 65536 + (long)header[19] * 256 + (long)header[18];
    long alto = (long)header[24] * 65536 + (long)header[23] * 256 + (long)header[22];

    printf("\n==========================\n");
    printf("Tamaño archivo: %li\n", tam);
    printf("Bits por pixel: %li\n", bpp);
    printf("Largo img: %li\n", alto);
    printf("Ancho img: %li\n", ancho);

    // Asignar memoria para los píxeles RGB
    unsigned char* r_in = (unsigned char*)malloc(ancho * alto);
    unsigned char* g_in = (unsigned char*)malloc(ancho * alto);
    unsigned char* b_in = (unsigned char*)malloc(ancho * alto);

    if (!r_in || !g_in || !b_in) {
        fprintf(stderr, "Error: No se pudo asignar memoria para canales de entrada.\n");
        fclose(image);
        fclose(outputImage);
        return;
    }

    // Leer píxeles RGB
    for (int i = 0; i < ancho * alto; i++) {
        b_in[i] = fgetc(image);
        g_in[i] = fgetc(image);
        r_in[i] = fgetc(image);
    }

    // Aplicar blur por canal
    unsigned char *r_out = malloc(ancho * alto);
    unsigned char *g_out = malloc(ancho * alto);
    unsigned char *b_out = malloc(ancho * alto);

    int k = kernel_size / 2;

    for (int y = 0; y < alto; y++) {
        // printf("Procesando fila %d de %li...\n", y, alto);
        for (int x = 0; x < ancho; x++) {
            int r_sum = 0, g_sum = 0, b_sum = 0, count = 0;

            for (int dy = -k; dy <= k; dy++) {
                for (int dx = -k; dx <= k; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < ancho && ny >= 0 && ny < alto) {
                        int idx = ny * ancho + nx;
                        r_sum += r_in[idx];
                        g_sum += g_in[idx];
                        b_sum += b_in[idx];
                        count++;
                    }
                }
            }

            int idx = y * ancho + x;
            r_out[idx] = r_sum / count;
            g_out[idx] = g_sum / count;
            b_out[idx] = b_sum / count;
        }
    }

    // Escribir imagen de salida RGB
    for (int i = 0; i < ancho * alto; i++) {
        fputc(b_out[i], outputImage);
        fputc(g_out[i], outputImage);
        fputc(r_out[i], outputImage);
    }

    free(r_in); free(g_in); free(b_in);
    free(r_out); free(g_out); free(b_out);
    fclose(image);
    fclose(outputImage);
}
