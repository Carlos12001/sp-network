#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Definiciones de S-box y P-box
int sbox[16] = {14, 4, 13, 1, 2, 11, 7, 8, 9, 6, 10, 12, 5, 3, 15, 0};
int pbox[8] = {2, 0, 6, 4, 7, 5, 1, 3};

// Clave de 32 bits específica a partir de la cadena "abcd"
char key_string[] = "1234";
uint32_t key = 0;  // La clave de 32 bits

// String de entrada
char input_string[] = "hello world!!";
int len = 0;

// Resultado cifrado (suponiendo que no excedemos el límite de entrada)
uint32_t result[64];  // Suficiente para 256 bytes de texto de entrada

void set_data_input() {
  // Preparar la clave de 32 bits
  for (int i = 0; i < 4; i++) {
    int sub_val = 3 - i;
    int shift_val = 8 * sub_val;
    unsigned int shifted = (unsigned char)key_string[i] << shift_val;
    key |= shifted;
  }

  // Asegurar que la entrada es un múltiplo de 4 caracteres
  len = strlen(input_string);
  printf("len:%i\n", len);
  while (len % 4 != 0) {
    input_string[len++] = '\0';  // Padding con caracteres nulos
  }

  // Cifrado
  printf("Original:\n%s\n", input_string);
}

void print_result() {
  // Imprimir el resultado almacenado en memoria como caracteres
  printf("Encrypted result as characters:\n");
  for (int i = 0; i < len / 4; i++) {
    uint32_t block = result[i];
    for (int j = 0; j < 4; j++) {
      unsigned char encrypted_char = (block >> (8 * (3 - j))) & 0xFF;
      printf("%c", encrypted_char);
    }
  }

  printf("\n============  END   ============\n");

  // Guardar el resultado en un archivo binario
  FILE *file = fopen("out.bin", "wb");
  if (file == NULL) {
    perror("Error al abrir el archivo");
    return;
  }

  fwrite(result, sizeof(uint32_t), len / 4, file);
  fclose(file);
}

int main() {
  set_data_input();  // Initialize input data but we don't need to do it on
                     // assembly
  // Cifrar
  for (int i = 0; i < len; i += 4) {
    // Convertir 4 chars a un bloque de 32 bits usando memcpy
    uint32_t block;
    memcpy(&block, &input_string[i], sizeof(block));  // LDR 32 bits from memory

    // Paso 1: XOR con la clave
    block ^= key;

    // Paso 2 y 3: Aplicar S-box y P-box a cada byte del bloque
    uint32_t encrypted_block = 0;
    for (int j = 0; j < 4; j++) {
      int sub_val = 3 - j;
      int shift_amount = sub_val << 3;
      unsigned int shifted_block = block >> shift_amount;
      unsigned char byte = shifted_block & 0xFF;

      unsigned char temp0 = byte >> 4;
      unsigned char temp1 = byte & 0x0F;
      unsigned char left = sbox[temp0];
      unsigned char right = sbox[temp1];
      unsigned char shifted_left = left << 4;
      unsigned char combined = shifted_left | right;

      unsigned char encrypted_byte = 0;
      for (int k = 0; k < 8; k++) {
        unsigned int shifted_combined = combined >> k;
        unsigned char bit = shifted_combined & 1;
        int pbox_shift = pbox[k];
        unsigned int shifted_bit = bit << pbox_shift;
        encrypted_byte |= shifted_bit;
      }
      int encrypted_shift = sub_val << 3;
      unsigned int shifted_encrypted_byte = encrypted_byte << encrypted_shift;
      encrypted_block |= shifted_encrypted_byte;
    }
    // Almacenar el resultado cifrado (Aqui lo almacenaria en memoria STR 32
    // bits TO memory)
    result[i / 4] = encrypted_block;  // Usar el mismo índice dividido por 4
  }

  print_result();  // To show the result in the console
  return 0;
}
