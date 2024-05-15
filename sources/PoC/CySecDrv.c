/*
 * CySecDrv.c
 *  Description: Security module
 *
 *  Created on: May 10, 2024
 *  Author: Daniel Calaver
 */

/*#################################*/
/*         Include-Files           */
/*#################################*/
#include "CySecDrv.h"


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

/*#################################*/
/*         Local defines           */
/*#################################*/
#define H0 0x6a09e667f3bcc908ULL
#define H1 0xbb67ae8584caa73bULL
#define H2 0x3c6ef372fe94f82bULL
#define H3 0xa54ff53a5f1d36f1ULL
#define H4 0x510e527fade682d1ULL
#define H5 0x9b05688c2b3e6c1fULL
#define H6 0x1f83d9abfb41bd6bULL
#define H7 0x5be0cd19137e2179ULL

const unsigned long long CySecStaticHash[] =
{
    0xd3f3abf79fa8b7b0ull,
    0xcdb70a5737cf9b2dull,
    0x79ec83a9c322599aull,
    0x30404165174f1cffull,
    0x84bc0bc775564fa0ull,
    0xc57bf59502d82092ull,
    0xd10248857838212eull,
    0xd0a29bc8eadad371ull
};

teCySecDrvStatus current_status = UNDEFINED;

/*#################################*/
/*        Local data types         */
/*#################################*/

/*#################################*/
/*        Global ROM data          */
/*#################################*/

/*#################################*/
/*        Global RAM data          */
/*#################################*/


/*#################################*/
/*        Local ROM data           */
/*#################################*/
const uint64_t k[80] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL,
    0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
    0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL,
    0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
    0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL,
    0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
    0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL,
    0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
    0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL,
    0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
    0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL,
    0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL,
    0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
    0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL,
    0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

uint8_t const paddingFor128Bytes[128] = {
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00
};


/*#################################*/
/*        Local RAM data           */
/*#################################*/
uint64_t h_values[8];
uint8_t chunk[128];
FILE *file;
bool lastChunk = false;

/*#################################*/
/*    Local function declaration   */
/*#################################*/
/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
uint64_t right_rotate(uint64_t x, int n) {
	uint64_t right_shift = (x >> n);
	uint64_t left_shift = (x << (64-n));
    return right_shift  | left_shift;
}

/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
void initialize_H_Values() {
    h_values[0] = H0;
    h_values[1] = H1;
    h_values[2] = H2;
    h_values[3] = H3;
    h_values[4] = H4;
    h_values[5] = H5;
    h_values[6] = H6;
    h_values[7] = H7;
    return;
}

/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
//! Byte swap unsigned int
uint32_t swap_uint32( uint32_t val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | (val >> 16);
}


/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
void getNextChunk() {
    static size_t read_bytes = 0;
    if (lastChunk) {
        fseek(file, 0, SEEK_SET);
        lastChunk = false;
    }
    read_bytes = fread(chunk, 1, 128, file);
    if (read_bytes < 128) {
        for (int i = read_bytes; i < 128; i++) {
            chunk[i] = paddingFor128Bytes[i];
        }
        lastChunk = true;
    }
    for (int i = 0; i < 128; i++) {
        printf("%02x", chunk[i]);
    }
    return;
}

/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
void processChunk(uint8_t* const block) {
    uint64_t words[80];
    uint64_t a = h_values[0];
    uint64_t b = h_values[1];
    uint64_t c = h_values[2];
    uint64_t d = h_values[3];
    uint64_t e = h_values[4];
    uint64_t f = h_values[5];
    uint64_t g = h_values[6];
    uint64_t h = h_values[7];
    for (int i = 0; i < 16; i++) {
        int offset = i * 8;
        words[i] =  ((uint64_t)(uint8_t)block[offset] << 56)     |
                    ((uint64_t)(uint8_t)block[offset + 1] << 48) |
                    ((uint64_t)(uint8_t)block[offset + 2] << 40) |
                    ((uint64_t)(uint8_t)block[offset + 3] << 32) |
                    ((uint64_t)(uint8_t)block[offset + 4] << 24) |
                    ((uint64_t)(uint8_t)block[offset + 5] << 16) |
                    ((uint64_t)(uint8_t)block[offset + 6] << 8)  |
                     (uint64_t)(uint8_t)block[offset + 7];
    }
    for (int i = 16; i < 80; i++) {
        uint64_t s0 = right_rotate(words[i - 15], 1) ^ right_rotate(words[i - 15], 8) ^ (words[i - 15] >> 7);
        uint64_t s1 = right_rotate(words[i - 2], 19) ^ right_rotate(words[i - 2], 61) ^ (words[i - 2] >> 6);
        words[i] = (words[i - 16] + s0 + words[i - 7] + s1) & 0xFFFFFFFFFFFFFFFF;
    }
    for (int i = 0; i < 80; i++) {
        uint64_t S1 = right_rotate(e, 14) ^ right_rotate(e, 18) ^ right_rotate(e, 41);
        uint64_t ch = (e & f) ^ (~e & g);
        uint64_t temp1 = h + S1 + ch + k[i] + words[i];
        uint64_t S0 = right_rotate(a, 28) ^ right_rotate(a, 34) ^ right_rotate(a, 39);
        uint64_t maj = (a & b) ^ (a & c) ^ (b & c);
        uint64_t temp2 = S0 + maj;
        
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }
    h_values[0] += a;
    h_values[1] += b;
    h_values[2] += c;
    h_values[3] += d;
    h_values[4] += e;
    h_values[5] += f;
    h_values[6] += g;
    h_values[7] += h;
}

/*#################################*/
/*  Global function implementation */
/*#################################*/


/*#################################*/
/*  Local function implementation  */
/*#################################*/
teCySecDrvStatus CySecDrvGetSecurityState () {
	return current_status;
}

void CySecDrvInit() {
    printf("Entering CySecDrvInit\n");
	uint32_t i;
	current_status = UNDEFINED;
	initialize_H_Values();
    file = fopen("E:\\Workspace\\workspace\\sha-512\\doc\\x2000-x27FF-memory.bin", "rb");
    while (current_status == UNDEFINED) {
        CySecDrvMain();
    }
}

int coutn = 0;
void CySecDrvMain() {
    getNextChunk();
    processChunk(chunk);
    coutn++;
    if (lastChunk) {
        printf("coutn: %d\n", coutn);
        coutn = 0;
        // printf("\nSHA-512 Hash: %016llx%016llx%016llx%016llx%016llx%016llx%016llx%016llx\n",
        // h_values[0], h_values[1], h_values[2], h_values[3], h_values[4], h_values[5], h_values[6], h_values[7]);
        current_status = SECURED;
        uint8_t i;
        for(i=0; i<8; i++) {
            if (h_values[i] != CySecStaticHash[i]) {
                current_status = NOT_SECURED;
                break;
            }
        }
        initialize_H_Values();
    }
}

	// serialWrite("Before reading memory \n\r");
	// uint8_t *address = (uint8_t *)0x6000;
	// uint8_t ucTest[2048];
	// for (i=0; i<2048; i++) {
	//	ucTest[i] = pgm_read_byte(address[i]);
	// }
	// serialWrite("After reading memory \n\r");
	// serialWrite("Before calculating hash \n\r");


//	for (i=0; i<8; i++){
		// sprintf(print_h, "%016llx", h_values[i]);
		// serialWrite("LOOP!\n\r");
		//serialWrite64(h_values[i]);
		//serialWrite("\n\r");
		//if (h_values[i] != CySecStaticHash[i]) {
//			current_status = NOT_SECURED;
			//break;
		//}
	//}