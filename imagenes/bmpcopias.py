import shutil

# Path to the original BMP file
source_file = 'prueba1.bmp'  # Replace with your actual file path

# Output file name prefix
output_prefix = 'output_'

# Duplicate the file 100 times
for i in range(100):
    output_filename = f'{i}.bmp'
    shutil.copy(source_file, output_filename)

print("100 copies created successfully.")
