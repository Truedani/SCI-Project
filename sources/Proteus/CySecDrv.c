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
// TODO: remove serial in final version
#include "MySerial.h"

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <util/delay.h>
#include "LedDrv.h"

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

#define MAX_ROUNDS 80
#define WORDS_SIZE 80 // message schedule array
#define CHUNK_SIZE 16
// with 1 round and 2 words per init finishes in 10295 ms in less then 2.5ms per 5ms cycle
// with 1 round and 3 words per init finishes in 9190  ms
// with 2 rounds and 2 words per init finishes in 6900ms but the load on round gets to about 3ms
#define ROUND_BATCH_SIZE 1
#define WORDS_BATCH_SIZE 3
#define PRINT true		// TODO: remove PRINT in final version
#define F_CPU 8000000UL // TODO: remove F_CPU in final version 

/*#################################*/
/*        Local data types         */
/*#################################*/
typedef enum
{
	NEXT_CHUNK = 0u,
	RESET_HASH = 1u,
	INIT_WORDS = 2u, // message schedule array
	PROCESS_ROUND = 3u,
	FINISHED = 4u
} teHashState;

/*#################################*/
/*        Global ROM data          */
/*#################################*/

/*#################################*/
/*        Global RAM data          */
/*#################################*/

/*#################################*/
/*        Local ROM data           */
/*#################################*/
const uint64_t k[80] PROGMEM = {
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

const uint64_t CySecStaticHash[] PROGMEM =
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


/*#################################*/
/*        Local RAM data           */
/*#################################*/
teCySecDrvStatus current_status = UNDEFINED;
uint64_t h_values[8];
uint8_t chunk[CHUNK_SIZE*8];
bool lastChunk = false;

/*#################################*/
/*    Local function declaration   */
/*#################################*/
/* Function name: readU64FromProgmem
	Description: reads 64bit variable from flash memory
	Function parameters: 
		addr - pointer to the flash address
	Function output:
		64bit unsigned int with the value read
*/
uint64_t readU64FromProgmem(const uint64_t* addr) {
    uint32_t lower = pgm_read_dword((const uint32_t*)addr);
    uint32_t upper = pgm_read_dword((const uint32_t*)addr + 1);
    return ((uint64_t)upper << 32) | lower;
}

/* Function name: right_rotate
   Description: rotates a 64bit number with n positions
   Function parameters:
		x - variable to rotate
		n - rotate factor 
	Function output:
		returns the rotated value
*/
uint64_t right_rotate(uint64_t x, int n) {
	uint64_t right_shift = (x >> n);
	uint64_t left_shift = (x << (64-n));
    return right_shift  | left_shift;
}

/* Function name: initialize_H_Values
   Description: resets H values for new hash computation
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

/* Function name: getNextChunk
   Description:	Updates the next 128byte chunk to be processed from flash memory
*/
void getNextChunk() {
	static uint16_t count=0;
	uint16_t i;
    if (lastChunk) {
        count = 0;
        lastChunk = false;
    }
    if (count == CHUNK_SIZE) {
		chunk[0] = 0x80;
		for(i=1; i<126; i++) chunk[i] = 0x0;
		chunk[126] = 0x40;
		chunk[127] = 0x00;
        lastChunk = true;
    } else {
		for (i=count*128; i<count*128+128; i++) {
			chunk[i-count*128] = pgm_read_byte((const void *)(0x4800 +i));
		}
		count++;
	}
    return;
}

/* Function name: wordsInit
   Description: initialize words list for processRounds
*/
uint64_t words[WORDS_SIZE];
teHashState wordsInit() {
    static uint16_t i = 0;
	uint16_t last_i = i;
	for(i=last_i; (i<(last_i+WORDS_BATCH_SIZE)) && (i <  WORDS_SIZE); i++)
		if (i < CHUNK_SIZE){
			uint16_t offset = i * 8;
			words[i] = ((uint64_t)chunk[offset]     << 56) |
					   ((uint64_t)chunk[offset + 1] << 48) |
					   ((uint64_t)chunk[offset + 2] << 40) |
					   ((uint64_t)chunk[offset + 3] << 32) |
					   ((uint64_t)chunk[offset + 4] << 24) |
					   ((uint64_t)chunk[offset + 5] << 16) |
					   ((uint64_t)chunk[offset + 6] << 8)  |
						(uint64_t)chunk[offset + 7];
		}
		else {
			uint64_t s0 = right_rotate(words[i - 15], 1) ^ right_rotate(words[i - 15], 8) ^ (words[i - 15] >> 7);
			uint64_t s1 = right_rotate(words[i - 2], 19) ^ right_rotate(words[i - 2], 61) ^ (words[i - 2] >> 6);
			words[i] = (words[i - 16] + s0 + words[i - 7] + s1) & 0xFFFFFFFFFFFFFFFFULL;
		}
	teHashState returnValue;
	if (i == WORDS_SIZE)  {
		returnValue = PROCESS_ROUND;
		i = 0;
	}
	else
		returnValue = INIT_WORDS;
	return returnValue;
}

/* Function name: processRounds
   Description: processes ROUND_BATCH_SIZE/MAX_ROUNDS rounds from the current chunk buffer
*/
teHashState processRounds() {
    static uint16_t i = 0;
    static uint64_t a, b, c, d, e, f, g, h;
    if (i == 0) {
        a = h_values[0];
        b = h_values[1];
        c = h_values[2];
        d = h_values[3];
        e = h_values[4];
        f = h_values[5];
        g = h_values[6];
        h = h_values[7];
    }
    uint16_t last_i = i;
    for (i = last_i; (i < (last_i+ROUND_BATCH_SIZE)) && (i < MAX_ROUNDS); ++i) {
        uint64_t S1 = right_rotate(e, 14) ^ right_rotate(e, 18) ^ right_rotate(e, 41);
        uint64_t ch = (e & f) ^ (~e & g);
        uint64_t temp1 = h + S1 + ch + readU64FromProgmem(&k[i]) + words[i];
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
    teHashState return_value = PROCESS_ROUND;
    if (i == MAX_ROUNDS) {
        i = 0;
        h_values[0] += a;
        h_values[1] += b;
        h_values[2] += c;
        h_values[3] += d;
        h_values[4] += e;
        h_values[5] += f;
        h_values[6] += g;
        h_values[7] += h;
		if (lastChunk) return_value = FINISHED;
		else return_value = NEXT_CHUNK;
    }
    return return_value;
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
	serialInit();
	if(PRINT) serialWrite("Security Init!\n\r");
	current_status = UNDEFINED;
	initialize_H_Values();
    while (current_status == UNDEFINED) {
         CySecDrvMain();
    }
}

void CySecDrvMain() {
	static uint64_t count = 0;
	count ++;
	static teHashState hash_state = NEXT_CHUNK;
	switch(hash_state){
        case NEXT_CHUNK:
			if(lastChunk) hash_state = RESET_HASH;
			else hash_state = INIT_WORDS;
			getNextChunk();
			break;
		case RESET_HASH:
            initialize_H_Values();
			if(PRINT) { serialWrite("Count: "); serialWrite64(count); serialWrite("\n\r\n\r"); }
			count = 0;
			hash_state = INIT_WORDS;
			break;
		case INIT_WORDS:
			// LedDrvSetLedState(LED_LEFT, ON);
			hash_state = wordsInit();
			// hash_state = PROCESS_ROUND;
			// LedDrvSetLedState(LED_LEFT, OFF);
			break;
		case PROCESS_ROUND:
			hash_state = processRounds();
			break;
		case FINISHED:
            current_status = SECURED;
            uint16_t i;
            printf("Hash: \n");
            for(i=0; i<8; i++) {
				if(PRINT){ 
					serialWrite64(h_values[i]);
					serialWrite("\n\r");
				}
                if (h_values[i] != readU64FromProgmem(&CySecStaticHash[i])) {
                    current_status = NOT_SECURED;
                    break;
                }
            }
			if(PRINT) if (SECURED == current_status) serialWrite("SECURED\n\r\n\r"); else serialWrite("NOT SECURED\n\r\n\r");
			hash_state = NEXT_CHUNK;
			break;
		}

}

