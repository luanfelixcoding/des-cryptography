#include <stdio.h>
#include <string.h>
#include "functions.h"

int main()
{
    key keyBit;

    //* Declaracao das variaveis

    // Um vetor para armazenar a entrada hexadecimal
    unsigned int hex_input[8], i = 0, k = 0, rodada = 0, sboxInt = 0;

    // Um vetor de 7 bytes para armazenar a chave apos a aplicacao da permutacao inicial PC1
    unsigned char chavePosPC1[7] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0'};

    // Um vetor para armazenar o texto claro (os dados que estao sendo criptografados)
    unsigned char plaintext[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};

    // Um vetor para armazenar o texto claro ap�s a permuta��o inicial (IP)
    unsigned char plaintext_permutado[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};

    // Um vetor para armazenar o texto cifrado (resultado da criptografia)
    unsigned char cyphertext[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};

    // Um vetor que pode ser usado para armazenar o resultado final ap�s a troca das partes esquerda e direita do bloco de texto
    unsigned char swapFinal[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};

    // Um vetor para armazenar a chave original de 64 bits. No entanto, apenas 56 bits s�o usados efetivamente ap�s a permuta��o
    unsigned char chave[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};

    // Um vetor que armazena a chave de 56 bits apos a permuta��o PC1
    unsigned char chave56bits[7] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0'};

    // Vetores que armazenam as partes esquerda
    unsigned char left[4] = {'\0', '\0', '\0', '\0'};

    // Vetores que armazenam as partes direita
    unsigned char right[4] = {'\0', '\0', '\0', '\0'};

    // Um vetor para armazenar a nova parte esquerda ap�s a opera��o de combina��o entre as partes esquerda e direita
    unsigned char novoLeft[4] = {'\0', '\0', '\0', '\0'};

    // Um vetor que armazena a parte direita expandida de 32 bits para 48 bits usando a matriz de expansao 'ex'
    unsigned char right_expandido[6] = {'\0', '\0', '\0', '\0', '\0', '\0'};

    // Um vetor para armazenar a subchave de 48 bits que � gerada a partir da chave original em cada rodada
    unsigned char subchave[6] = {'\0', '\0', '\0', '\0', '\0', '\0'};

    // Um vetor que armazena o resultado da opera��o XOR entre a parte direita expandida e a subchave
    unsigned char textoXORchave48bits[6] = {'\0', '\0', '\0', '\0', '\0', '\0'};

    // Um vetor para armazenar o resultado da opera��o S-Box (4 bytes, 32 bits ap�s a aplica��o das S-Boxes
    unsigned char sboxKey[4] = {'\0', '\0', '\0', '\0'};

    // Um vetor para armazenar o resultado da permuta��o final aplicada ap�s a opera��o S-Box
    unsigned char saidaP[4] = {'\0', '\0', '\0', '\0'};

    // char48toint Uma vari�vel para armazenar o resultado da convers�o de um vetor de 48 bits para um inteiro
    unsigned char aux = 0;
    unsigned long long int char48ToInt = 0, bit64 = 0;

    // LEITURA DO TEXTO PLANO
    char letra;
    printf("Escreva um bloco de 8 digitos para ser criptografado\n");
    for(i = 0; i < 8; i++) {
        scanf("%c", &letra);
        hex_input[i] = (unsigned int) letra;
        plaintext[i] = (unsigned char) hex_input[i];
    }

    // Permutação inicial do plaintext IP
    printf("\nIP\n");
    limpaBits(plaintext_permutado, sizeof (plaintext_permutado));
    permutacaoGenerica(plaintext, plaintext_permutado, (sizeof(ip)/sizeof(ip[0])), ip);
    imprimeHexadecimal(plaintext_permutado, sizeof (plaintext_permutado));

    // Divide o plaintext permutado em 2 partes 32bits cada
    divideTexto(plaintext_permutado, left, right);


    // LEITURA DA CHAVE
    printf("\nCHAVE\n");
    for(i = 0; i < 8; i++) {
        scanf("%X", &hex_input[i]);
        chave[i] = (unsigned char) hex_input[i];
    }

    // Primeira permutacao da chave
    limpaBits(chavePosPC1, sizeof (chavePosPC1));
    permutacaoGenerica(chave, chavePosPC1, (sizeof(pc1)/sizeof(pc1[0])), pc1);
    printf("\nPC1 - SELECIONA CHAVE\n");
    imprimeHexadecimal(chavePosPC1, sizeof (chavePosPC1));

    // Divisao da chave em duas parte 28 bits cada
    keyBit = divideChave(chavePosPC1);


    // ================================ Inicio das rodadas =================================

    for (rodada = 0; rodada < 16; rodada++) {

        // Gera chave da rodada e imprime (saida da permuted choice 2)
        keyBit = leftShiftCircular(keyBit, rodada);
        juntaChaveEsqDi(keyBit, chave56bits);
        limpaBits(subchave, sizeof (subchave));
        permutacaoGenerica(chave56bits, subchave, (sizeof(pc2)/sizeof(pc2[0])), pc2);
        printf("\nCHAVE DE ROUND %d\n", rodada+1);
        printf("Deslocamento: ");
        imprimeHexadecimal(chave56bits, sizeof (chave56bits));
        printf("PC2: ");
        imprimeHexadecimal(subchave, sizeof (subchave));
        imprimeHexadecimal(subchave, sizeof (subchave));

        // Manipulacoes com texto (Funcao F) e imprime Round
        printf("\nROUND %d\n", rodada+1);
        imprimeHexadecimal(plaintext_permutado, sizeof (plaintext_permutado));

        // Copia metade direita do texto para ser usado na proxima rodada como lado esquerdo
        for(k = 0; k < 4; k++) novoLeft[k] = right[k];

        // Expande parte direita do texto com matriz E
        limpaBits(right_expandido, sizeof (right_expandido));
        permutacaoGenerica(right, right_expandido, (sizeof(ex)/sizeof(ex[0])), ex);
        printf("Expansao: ");
        imprimeHexadecimal(right_expandido, sizeof (right_expandido));

        // XOR da chave permutada com texto expandido (Funcao F)
        for (i = 0; i < 6; i++) textoXORchave48bits[i] = right_expandido[i] ^ subchave[i];
        printf("Add Key: ");
        imprimeHexadecimal(textoXORchave48bits, sizeof (textoXORchave48bits));

        // S-BOX encoding.
        // Primeiro converte vetor de char para inteiro para manipular 6bits inves de 8bits
        char48ToInt = 0;
        sboxInt = 0;
        for (i = 0; i < 6; i++){
            bit64 = textoXORchave48bits[i];
            char48ToInt = char48ToInt | bit64;
            if(i != 5) char48ToInt = char48ToInt << 8;
        }
        aux = (char48ToInt >> 42) & 63;
        aux = (unsigned char) sBoxEncoding(aux, s1);
        sboxInt = sboxInt | aux;
        sboxInt = (sboxInt << 4);

        aux = (char48ToInt >> 36) & 63;
        aux = (unsigned char) sBoxEncoding(aux, s2);
        sboxInt = sboxInt | aux;
        sboxInt = (sboxInt << 4);

        aux = (char48ToInt >> 30) & 63;
        aux = (unsigned char) sBoxEncoding(aux, s3);
        sboxInt = sboxInt | aux;
        sboxInt = (sboxInt << 4);

        aux = (char48ToInt >> 24) & 63;
        aux = (unsigned char) sBoxEncoding(aux, s4);
        sboxInt = sboxInt | aux;
        sboxInt = (sboxInt << 4);

        aux = (char48ToInt >> 18) & 63;
        aux = (unsigned char) sBoxEncoding(aux, s5);
        sboxInt = sboxInt | aux;
        sboxInt = (sboxInt << 4);

        aux = (char48ToInt >> 12) & 63;
        aux = (unsigned char) sBoxEncoding(aux, s6);
        sboxInt = sboxInt | aux;
        sboxInt = (sboxInt << 4);

        aux = (char48ToInt >> 6) & 63;
        aux = (unsigned char) sBoxEncoding(aux, s7);
        sboxInt = sboxInt | aux;
        sboxInt = (sboxInt << 4);

        aux = (char48ToInt >> 0) & 63;
        aux = (unsigned char) sBoxEncoding(aux, s8);
        sboxInt = sboxInt | aux;

        // Converte a saida das S-BOX de inteiro para char
        converteIntParaChar(sboxInt, sboxKey, sizeof (sboxKey));

        // Imprime saida da S-BOX
        printf("S-Box: ");
        imprimeHexadecimal(sboxKey, sizeof (sboxKey));

        // Permuta a saida da S-BOX com a matriz P
        limpaBits(saidaP, sizeof (saidaP));
        permutacaoGenerica(sboxKey, saidaP, (sizeof(p)/sizeof(p[0])), p);
        printf("Permuta: ");
        imprimeHexadecimal(saidaP, sizeof (saidaP));

        // XOR entre a saida da Funcao F(saidaP) e a parte esquerda do texto e imprime.
        for (k = 0; k < 4; k++) right[k] = left[k] ^ saidaP[k];
        printf("Add Left: ");
        imprimeHexadecimal(right, sizeof (right));

        // Novo L usado como L na proxima rodada
        for (k = 0; k < 4; k++) left[k] = novoLeft[k];

        // Imprime o texto resultante do final da rodada
        juntaTextoLeftRight(plaintext_permutado, novoLeft, right);
        imprimeHexadecimal(plaintext_permutado, sizeof (plaintext_permutado));

    }

    // Swap final
    printf("\nSwap: ");
    juntaTextoLeftRight(swapFinal, right, novoLeft);
    imprimeHexadecimal(swapFinal, sizeof (swapFinal));

    // Permutacao inversa
    limpaBits(cyphertext, sizeof (cyphertext));
    permutacaoGenerica(swapFinal, cyphertext, (sizeof(ipI)/sizeof(ipI[0])), ipI);
    printf("\nIP Inverso: ");
    imprimeHexadecimal(cyphertext, sizeof (cyphertext));

    return 0;
}