import os
from PIL import Image

# Ruta de la carpeta con las imágenes
input_folder = "./pngs/"
output_folder = "./bmps/"

# Crea la carpeta de destino si no existe
os.makedirs(output_folder, exist_ok=True)

# Itera sobre cada archivo en la carpeta de origen
for filename in os.listdir(input_folder):
    if filename.lower().endswith(('.png', '.jpg', '.jpeg')):  # extensiones de imágenes comunes
        img_path = os.path.join(input_folder, filename)
        img = Image.open(img_path).convert("RGBA")  # 32 bits (RGBA)

        # Guarda la imagen como BMP
        output_path = os.path.join(output_folder, os.path.splitext(filename)[0] + ".bmp")
        img.save(output_path, format="BMP")
        print(f"Convertido: {filename} → {output_path}")

print("Conversión completa.")