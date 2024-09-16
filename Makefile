# Definimos el compilador y las opciones de compilación
CC = gcc
CFLAGS = -Wall

# Targets y sus dependencias
all: encrypt decrypt

# Compilación y ejecución del cifrado
encrypt: sp-network.c
	$(CC) $(CFLAGS) sp-network.c -o sp-network
	./sp-network
	rm sp-network

# Compilación y ejecución del descifrado
decrypt: not-sp-network.c
	$(CC) $(CFLAGS) not-sp-network.c -o not-sp-network
	./not-sp-network
	rm not-sp-network

# Target de limpieza
clean:
	rm -f sp-network not-sp-network
