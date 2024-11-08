# C Project: Des Cryptography

## Description
This project implements a simplified version of the DES (Data Encryption Standard) algorithm in C. It takes an 8-character plaintext input and a 64-bit key to perform encryption through multiple rounds of permutations, substitutions, and key mixing. The program demonstrates key components of the DES algorithm, including initial and final permutations, round-based key generation, S-Box transformations, and XOR operations.
- **main.c**: It's the primary entry point for the DES encryption process. It handles:
    - Reading plaintext input from the user.
    - Performing an initial permutation on the plaintext.
    - Dividing the data into left and right segments for iterative round processing.
    - Reading and permuting the key for round-based subkey generation.
    - Executing 16 rounds of the DES encryption process, including expansion, key mixing, and substitution using S-Boxes.
    - Applying the final permutation to produce the ciphertext output.

- **functions.h**: This file defines functions and structures necessary for performing the DES (Data Encryption Standard) algorithm, including key generation, initial and final permutations, expansion permutations, and S-Box operations. It uses bitwise operations to achieve these cryptographic functions.

## Features 
1. Initial and Final Permutations: The plaintext undergoes an initial permutation (IP) and a final permutation (IP inverse) as required by the DES specification.

2. Key Schedule for Round Keys: The program generates 16 subkeys using left shifts and predefined permutations, mimicking the DES key scheduling process.

3. 16-Round DES Encryption: The data is processed over 16 rounds, incorporating expansion, key mixing, S-Box substitution, and final permutation to yield the encrypted output.

4. Custom Key Structure: The key struct stores 28-bit left and right halves of a key.

5. Initial and Inverse Permutations: Functions for the initial permutation (ip) and the inverse permutation (ipI) are implemented as 8x8 matrices.

6. Expansion Permutation: The ex matrix expands 32-bit blocks to 48 bits.

7. Key Permutations: The pc1 matrix transforms a 64-bit key to 56 bits, and pc2 transforms the 56-bit key into 48-bit subkeys.

8. S-Boxes: Eight substitution boxes (s1 to s8) perform non-linear substitution to increase security.

## Prerequisites
To build and run this project, you will need:
- A C compiler, such as `gcc`.
- A Unix-based system or a compatible development environment.

## Limitations
- The code provided focuses on the core operations needed for DES but does not constitute a complete DES encryption or decryption process.

- Make sure to handle edge cases and integrate proper input handling in a production environment.

- The input only support 8 characters

## Build and Run
Follow these steps to compile and run the program:

### Compilation
```bash
gcc main.c functions.c -o program
./program
