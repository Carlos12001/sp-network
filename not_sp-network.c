#include <stdint.h>
#include <stdio.h>
#include <string.h>

// S-box y P-box inversas predefinidas
int inverse_sbox[16] = {15, 3, 4, 13, 1, 12, 9, 6, 7, 8, 10, 5, 11, 2, 0, 14};
int inverse_pbox[8] = {1, 6, 0, 7, 3, 5, 2, 4};

// Clave de 32 bits específica a partir de la cadena "abcd"
char key_string[] = "1234";
uint32_t key = 0;

// String cifrado almacenado
uint32_t result[64];  // Suficiente para 256 bytes de texto de entrada

// Longitud del texto cifrado
int len = 0;

// String descifrado resultante
char decrypted_string[256] = {0};  // Suficiente para 256 bytes

void set_data_input() {
  // Preparar la clave de 32 bits
  for (int i = 0; i < 4; i++) {
    int sub_val = 3 - i;
    int shift_val = 8 * sub_val;
    unsigned int shifted = (unsigned char)key_string[i] << shift_val;
    key |= shifted;
  }

  // Leer el resultado cifrado del archivo binario
  FILE *file = fopen("out.bin", "rb");
  if (file == NULL) {
    perror("Error al abrir el archivo");
    return;
  }

  // Obtener el tamaño del archivo
  fseek(file, 0, SEEK_END);
  len = ftell(file);
  rewind(file);

  // Leer el contenido cifrado
  fread(result, sizeof(uint32_t), len / sizeof(uint32_t), file);
  fclose(file);
}

void print_result() {
  // Imprimir el resultado descifrado
  printf("Decrypted result:\n%s\n", decrypted_string);
}

int main() {
  set_data_input();  // Inicializar la entrada de datos, NO necesario en ASM

  // Descifrado
  for (int i = 0; i < len / sizeof(uint32_t); i++) {
    uint32_t encrypted_block = result[i];
    uint32_t block = 0;

    // Paso 1: Invertir P-box y S-box
    for (int j = 0; j < 4; j++) {
      int sub_val = 3 - j;
      int shift_amount = sub_val << 3;
      unsigned int shifted_block = encrypted_block >> shift_amount;
      unsigned char encrypted_byte = shifted_block & 0xFF;

      // Invertir P-box
      unsigned char after_pbox = 0;
      for (int k = 0; k < 8; k++) {
        unsigned int shifted_combined = encrypted_byte >> k;
        unsigned char bit = shifted_combined & 1;
        int pbox_shift = inverse_pbox[k];
        unsigned int shifted_bit = bit << pbox_shift;
        after_pbox |= shifted_bit;
      }

      // Invertir S-box
      unsigned char temp0 = after_pbox >> 4;
      unsigned char temp1 = after_pbox & 0x0F;
      unsigned char left = inverse_sbox[temp0];
      unsigned char right = inverse_sbox[temp1];
      unsigned char shifted_left = left << 4;
      unsigned char decrypted_byte = shifted_left | right;

      int decrypted_shift = sub_val << 3;
      unsigned int shifted_decrypted_byte = decrypted_byte << decrypted_shift;
      block |= shifted_decrypted_byte;
    }

    // Paso 2: XOR con la clave
    block ^= key;

    // Convertir el bloque descifrado a caracteres
    memcpy(&decrypted_string[i * 4], &block,
           sizeof(block));  // STR 32 bits to memory
  }

  print_result();

  return 0;
}
