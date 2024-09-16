# Definiciones de S-box y P-box
sbox = [14, 4, 13, 1, 2, 11, 7, 8, 9, 6, 10, 12, 5, 3, 15, 0]
pbox = [2, 0, 6, 4, 7, 5, 1, 3]

# Recibir un string para la clave (debe ser exactamente 4 caracteres)
key_string = "1234"  # Aquí puedes cambiar por cualquier string de 4 caracteres

# Convertir el string en una clave de 32 bits
key = 0
for i, char in enumerate(reversed(key_string)):
    key |= (ord(char) & 0xFF) << (8 * i)

# String de entrada
input_string = "hello world!!"

# Asegurarse de que la entrada sea un múltiplo de 4 caracteres (32 bits)
while len(input_string) % 4 != 0:
    input_string += '\x00'  # Padding con caracteres nulos

# String cifrado resultante
encrypted_string = ""

for i in range(0, len(input_string), 4):
    # Convertir 4 chars a un bloque de 32 bits
    block = 0
    for j in range(4):
        block |= (ord(input_string[i + j]) & 0xFF) << (8 * j)
    
    # Paso 1: XOR con la clave
    block ^= key
    
    # Paso 2 y 3: Aplicar S-box y P-box a cada byte
    encrypted_block = 0
    for j in range(4):
        byte = (block >> (8 * j)) & 0xFF
        # Aplicar S-box
        left = byte >> 4
        right = byte & 0x0F
        after_sbox = (sbox[left] << 4) | sbox[right]
        # Aplicar P-box
        encrypted_byte = 0
        for k in range(8):
            bit = (after_sbox >> k) & 1
            encrypted_byte |= bit << pbox[k]
        encrypted_block |= (encrypted_byte << (8 * j))
    
    # Convertir el bloque cifrado a caracteres
    for j in range(4):
        encrypted_char = chr((encrypted_block >> (8 * j)) & 0xFF)
        encrypted_string += encrypted_char

print("Original:", input_string)
print("Encrypted:", encrypted_string)
# Guardar el resultado cifrado en un archivo binario
with open("out.bin", "wb") as file:
    file.write(encrypted_string.encode('latin1'))