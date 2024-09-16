# Definiciones de S-box y P-box
sbox = [14, 4, 13, 1, 2, 11, 7, 8, 9, 6, 10, 12, 5, 3, 15, 0]
pbox = [2, 0, 6, 4, 7, 5, 1, 3]

# Crear S-box inversa
inverse_sbox = [0] * 16
for i in range(16):
    inverse_sbox[sbox[i]] = i

# Crear P-box inversa
inverse_pbox = [0] * 8
for i in range(8):
    inverse_pbox[pbox[i]] = i

# Clave
key_string = "1234"  # La misma clave utilizada para cifrar
key = 0
for i, char in enumerate(reversed(key_string)):
    key |= (ord(char) & 0xFF) << (8 * i)

# String cifrado
encrypted_string = "�����5�S��e�Ԅ"  # Aquí coloca el string cifrado que generaste

# String descifrado resultante
decrypted_string = ""

for i in range(0, len(encrypted_string), 4):
    # Convertir 4 chars a un bloque de 32 bits
    encrypted_block = 0
    for j in range(4):
        encrypted_block |= (ord(encrypted_string[i + j]) & 0xFF) << (8 * j)

    # Paso 1: Invertir P-box y S-box
    block = 0
    for j in range(4):
        encrypted_byte = (encrypted_block >> (8 * j)) & 0xFF
        # Invertir P-box
        after_pbox = 0
        for k in range(8):
            bit = (encrypted_byte >> k) & 1
            after_pbox |= bit << inverse_pbox[k]
        # Invertir S-box
        left = (after_pbox >> 4) & 0xF
        right = after_pbox & 0xF
        decrypted_byte = (inverse_sbox[left] << 4) | inverse_sbox[right]
        block |= (decrypted_byte << (8 * j))
    
    # Paso 2: XOR con la clave
    block ^= key

    # Convertir el bloque descifrado a caracteres
    for j in range(4):
        decrypted_char = chr((block >> (8 * j)) & 0xFF)
        decrypted_string += decrypted_char

print("Decrypted:", decrypted_string)

