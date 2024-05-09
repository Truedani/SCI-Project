#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


// write right_rotate function used in sha-512 algorithm
unsigned long long int right_rotate(unsigned long long int x, int n) {
    return (x >> n) | ((x << (64 - n)) & 0xFFFFFFFFFFFFFFFF);
}

enum INITIAL_H { 
    H0 = 0x6a09e667f3bcc908, H1 = 0xbb67ae8584caa73b, H2 = 0x3c6ef372fe94f82b, H3 = 0xa54ff53a5f1d36f1,
    H4 = 0x510e527fade682d1, H5 = 0x9b05688c2b3e6c1f, H6 = 0x1f83d9abfb41bd6b, H7 = 0x5be0cd19137e2179
};

enum INITIAL_K {
    K0 = 0x428a2f98d728ae22,  K1  = 0x7137449123ef65cd, K2  = 0xb5c0fbcfec4d3b2f, K3  = 0xe9b5dba58189dbbc, K4  = 0x3956c25bf348b538,
    K5 = 0x59f111f1b605d019,  K6  = 0x923f82a4af194f9b, K7  = 0xab1c5ed5da6d8118, K8  = 0xd807aa98a3030242, K9  = 0x12835b0145706fbe,
    K10 = 0x243185be4ee4b28c, K11 = 0x550c7dc3d5ffb4e2, K12 = 0x72be5d74f27b896f, K13 = 0x80deb1fe3b1696b1, K14 = 0x9bdc06a725c71235,
    K15 = 0xc19bf174cf692694, K16 = 0xe49b69c19ef14ad2, K17 = 0xefbe4786384f25e3, K18 = 0x0fc19dc68b8cd5b5, K19 = 0x240ca1cc77ac9c65,
    K20 = 0x2de92c6f592b0275, K21 = 0x4a7484aa6ea6e483, K22 = 0x5cb0a9dcbd41fbd4, K23 = 0x76f988da831153b5, K24 = 0x983e5152ee66dfab,
    K25 = 0xa831c66d2db43210, K26 = 0xb00327c898fb213f, K27 = 0xbf597fc7beef0ee4, K28 = 0xc6e00bf33da88fc2, K29 = 0xd5a79147930aa725,
    K30 = 0x06ca6351e003826f, K31 = 0x142929670a0e6e70, K32 = 0x27b70a8546d22ffc, K33 = 0x2e1b21385c26c926, K34 = 0x4d2c6dfc5ac42aed,
    K35 = 0x53380d139d95b3df, K36 = 0x650a73548baf63de, K37 = 0x766a0abb3c77b2a8, K38 = 0x81c2c92e47edaee6, K39 = 0x92722c851482353b,
    K40 = 0xa2bfe8a14cf10364, K41 = 0xa81a664bbc423001, K42 = 0xc24b8b70d0f89791, K43 = 0xc76c51a30654be30, K44 = 0xd192e819d6ef5218,
    K45 = 0xd69906245565a910, K46 = 0xf40e35855771202a, K47 = 0x106aa07032bbd1b8, K48 = 0x19a4c116b8d2d0c8, K49 = 0x1e376c085141ab53,
    K50 = 0x2748774cdf8eeb99, K51 = 0x34b0bcb5e19b48a8, K52 = 0x391c0cb3c5c95a63, K53 = 0x4ed8aa4ae3418acb, K54 = 0x5b9cca4f7763e373,
    K55 = 0x682e6ff3d6b2b8a3, K56 = 0x748f82ee5defb2fc, K57 = 0x78a5636f43172f60, K58 = 0x84c87814a1f0ab72, K59 = 0x8cc702081a6439ec,
    K60 = 0x90befffa23631e28, K61 = 0xa4506cebde82bde9, K62 = 0xbef9a3f7b2c67915, K63 = 0xc67178f2e372532b, K64 = 0xca273eceea26619c,
    K65 = 0xd186b8c721c0c207, K66 = 0xeada7dd6cde0eb1e, K67 = 0xf57d4f7fee6ed178, K68 = 0x06f067aa72176fba, K69 = 0x0a637dc5a2c898a6,
    K70 = 0x113f9804bef90dae, K71 = 0x1b710b35131c471b, K72 = 0x28db77f523047d84, K73 = 0x32caab7b40c72493, K74 = 0x3c9ebe0a15c9bebc,
    K75 = 0x431d67c49c100d4c, K76 = 0x4cc5d4becb3e42b6, K77 = 0x597f299cfc657e2a, K78 = 0x5fcb6fab3ad6faec, K79 = 0x6c44198c4a475817
};

typedef struct H_Values {
    uint64_t h0;
    uint64_t h1;
    uint64_t h2;
    uint64_t h3;
    uint64_t h4;
    uint64_t h5;
    uint64_t h6;
    uint64_t h7; 
}H_Values;

// Initialize array of round constants: (first 64 bits of the fractional parts of the cube roots of the first 80 primes 2..409):
const uint64_t k[80] = {
    K0,  K1,  K2,  K3,  K4,  K5,  K6,  K7,  K8,  K9,  K10, K11, K12, K13, K14, K15, K16, K17, K18, K19,
    K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K30, K31, K32, K33, K34, K35, K36, K37, K38, K39,
    K40, K41, K42, K43, K44, K45, K46, K47, K48, K49, K50, K51, K52, K53, K54, K55, K56, K57, K58, K59,
    K60, K61, K62, K63, K64, K65, K66, K67, K68, K69, K70, K71, K72, K73, K74, K75, K76, K77, K78, K79
};

H_Values initialize_H_Values() {
    H_Values h_values;
    h_values.h0 = H0;
    h_values.h1 = H1;
    h_values.h2 = H2;
    h_values.h3 = H3;
    h_values.h4 = H4;
    h_values.h5 = H5;
    h_values.h6 = H6;
    h_values.h7 = H7;
    return h_values;
}

void padding(uint8_t *message, uint32_t length) {
    // Calculate the number of bytes needed for padding
    // 1024 bits = 128 bytes, length is in bytes. 16 bytes are reserved for the length of the message in bits
    // however we only plan to support messages up to 2kb, so we can use 4 bytes for length, see later
    int padding_bytes = 128 - (length % 128) - 16;

    // First bit should be 1 from left, since the smallest cell is 1 byte, it means that first byte after msg must be 0x80
    message[length] = 0x80;

    // Append padding zeros
    memset(message + length + 1, 0, padding_bytes - 1 + 16 - 4);
    // Append the length of the message in bits
    uint32_t length_bits = __builtin_bswap32(length * 8);
    uint8_t length_bits_array[4] = {length_bits & 0xFF, (length_bits >> 8) & 0xFF, (length_bits >> 16) & 0xFF, (length_bits >> 24) & 0xFF};
    // printf("length_bits = %02x%02x%02x%02x\n",  length_bits_array[0], length_bits_array[1], length_bits_array[2], length_bits_array[3]);
    
    memcpy(message + length + padding_bytes  + 16 - 4, &length_bits, sizeof(uint32_t));
    message[length + padding_bytes] |= length_bits;

    // printf("Padded message in hex: ");
    // printf("\n\n%d\n\n", length+padding_bytes+16);
    // for (int i=0; i<length+padding_bytes+16; i++) {
    //     printf("%02x", (uint8_t) message[i]);
    // }
    // printf("\n");
}


void process_block(char *block, H_Values *h_values) {
    unsigned long long int words[80];
    // Initialize the working variables
    unsigned long long int a = h_values->h0;
    unsigned long long int b = h_values->h1;
    unsigned long long int c = h_values->h2;
    unsigned long long int d = h_values->h3;
    unsigned long long int e = h_values->h4;
    unsigned long long int f = h_values->h5;
    unsigned long long int g = h_values->h6;
    unsigned long long int h = h_values->h7;
    
    // Break the block into 16 64-bit words
    // printf("Breaking chunk into 16 64-bit words\n");
    for (int i = 0; i < 16; i++) {
        int offset = i * 8;
        words[i] = ((unsigned long long int)(unsigned char)block[offset] << 56) |
                ((unsigned long long int)(unsigned char)block[offset + 1] << 48) |
                ((unsigned long long int)(unsigned char)block[offset + 2] << 40) |
                ((unsigned long long int)(unsigned char)block[offset + 3] << 32) |
                ((unsigned long long int)(unsigned char)block[offset + 4] << 24) |
                ((unsigned long long int)(unsigned char)block[offset + 5] << 16) |
                ((unsigned long long int)(unsigned char)block[offset + 6] << 8) |
                (unsigned long long int)(unsigned char)block[offset + 7];
        // printf("words[%d]: %llx\n", i, words[i]);
    }
    
    // Extend the 16 64-bit words into 79 64-bit words
    // printf("Extending 16 64-bit words into 79 64-bit words\n");
    for (int i = 16; i < 80; i++) {
        unsigned long long int s0 = right_rotate(words[i - 15], 1) ^ right_rotate(words[i - 15], 8) ^ (words[i - 15] >> 7);
        unsigned long long int s1 = right_rotate(words[i - 2], 19) ^ right_rotate(words[i - 2], 61) ^ (words[i - 2] >> 6);
        words[i] = (words[i - 16] + s0 + words[i - 7] + s1) & 0xFFFFFFFFFFFFFFFF;
        // printf("words[%d]: %llx\n", i, words[i]);
    }
    
    // Perform the main hash computation
    for (int i = 0; i < 80; i++) {
        unsigned long long int S1 = right_rotate(e, 14) ^ right_rotate(e, 18) ^ right_rotate(e, 41);
        unsigned long long int ch = (e & f) ^ (~e & g);
        unsigned long long int temp1 = h + S1 + ch + k[i] + words[i];
        unsigned long long int S0 = right_rotate(a, 28) ^ right_rotate(a, 34) ^ right_rotate(a, 39);
        unsigned long long int maj = (a & b) ^ (a & c) ^ (b & c);
        unsigned long long int temp2 = S0 + maj;
        
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
        // printf("a: %llx, b: %llx, c: %llx, d: %llx, e: %llx, f: %llx, g: %llx, h: %llx\n", a, b, c, d, e, f, g, h);
        // if ((a + h_values->h0) == 0xcf83e1357eefb8bd) {
        //     printf("D:LJADLKSJADLKJSDKLAJ i = %d\n", i);
        // }
    }
    
    // Update the hash values
    h_values->h0 += a;
    h_values->h1 += b;
    h_values->h2 += c;
    h_values->h3 += d;
    h_values->h4 += e;
    h_values->h5 += f;
    h_values->h6 += g;
    h_values->h7 += h;
    
}

H_Values process_message(uint8_t *message, uint32_t length) {
    // Initialize the hash values
    H_Values h_values = initialize_H_Values();
    
    // Pad the message
    padding(message, length);
    uint32_t block_count = 1 + (length) / 64;  // Adjusted for bits to bytes + padding
    printf("block_count: %d\n", block_count);
    // Process the message in 1024-bit blocks 
    for (uint32_t i = 0; i < block_count; i++) {
        // Process the block
        printf("Processing block %d\n", i);
        process_block(message + i * 64, &h_values);
    }
    return h_values;
}

void main() {
    // initialize h-values
    // uint8_t message[3000] = "Ana are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere fAna are multe mere f";
    uint8_t message[3000] = "416e61206172652013qwerasdfzxcvtyuighjkbnma416e61206172652013qwe";
    uint32_t length = strlen("416e61206172652013qwerasdfzxcvtyuighjkbnma416e61206172652013qwe");
    printf("length = %d\n", length);
    // H_Values h_values = process_message(message, 2000);
    H_Values h_values = process_message(message, length);
    printf("SHA-512 Hash: %llx%llx%llx%llx%llx%llx%llx%llx\n", h_values.h0, h_values.h1, h_values.h2, h_values.h3, h_values.h4, h_values.h5, h_values.h6, h_values.h7);
    return;
}