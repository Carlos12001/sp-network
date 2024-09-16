# Implementation of SP Networks

This repository contains an implementation of the Substitution-Permutation Network (SP Network) algorithm in both C and Python. The SP Network is a cryptographic primitive used in the construction of block ciphers, playing a crucial role in achieving confusion and diffusion, the two main properties of secure encryption algorithms.

## Overview

The SP Network algorithm is implemented in two main parts:

- **Encryption (`sp-network.c`)**: This program takes an input string, applies a series of substitution and permutation operations defined by S-boxes and P-boxes, and produces an encrypted output. The encrypted data is then stored in a binary file.
- **Decryption (`not-sp-network.c`)**: This program reads the encrypted data from the binary file, reverses the substitution and permutation operations, and outputs the original input string.

## Features

- **C Implementation**: The core cryptographic functions are implemented in C for efficiency and lower-level control.
- **Python Version**: A Python implementation is also provided for ease of understanding and experimentation.

## Usage

### Compilation and Execution

A `Makefile` is provided to automate the compilation and execution process. You can use the following commands:

- To compile and run the encryption program:

  ```bash
  make encrypt
  ```

  This will compile `sp-network.c`, execute the generated binary, and remove the binary after execution.

- To compile and run the decryption program:

  ```bash
  make decrypt
  ```

  This will compile `not-sp-network.c`, execute the generated binary, and remove the binary after execution.

- To compile and run both programs in sequence:

  ```bash
  make
  ```

- To clean up executable files:

  ```bash
  make clean
  ```

### Example Output

After running the encryption and decryption programs, you should see output similar to the following:

```bash
len:13
Original:
hello world!!
Encrypted result as characters:
�5S�eԄ
============  END   ============
```

```bash
Decrypted result:
hello world!!
```

## Requirements

- **C Compiler**: `gcc` is used in the provided `Makefile`, but any C compiler should work.
- **Python**: If using the Python implementation.
- **Make**: To use the `Makefile` for automated compilation.
