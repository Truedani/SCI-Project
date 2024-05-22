#include "MySerial.h"
#if defined(DEBUG) && (DEBUG == true)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include <inttypes.h>
// #include <util/delay.h>

#define CPU_FRQ 8000000
#define BAUD	9600
#define BRC ((CPU_FRQ/16/BAUD) -1)

#define BUFFER_SIZE 128


void serialInit() {
	UBRRH = (BRC >> 8);
	UBRRL = BRC;
	UCSRB = (1 << TXEN);
	UCSRC = (1 << UCSZ1) | (1 << UCSZ0);
}

void serialWrite(const char message[]) {
	unsigned int i;
	for (i=0; i<strlen(message); i++) {
		UDR = message[i];
		while(!(UCSRA & (1 << UDRE)));
	}
}

uint64_t swap_uint64( uint64_t val )
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
}

void serialWrite64(uint64_t number) {
	number = swap_uint64(number);
	uint8_t* bytePtr = (uint8_t*)&number;
	uint8_t i;
	uint8_t current_byte;
	char a, b;
	for (i=0; i<sizeof(uint64_t); i++) {
		current_byte = *(bytePtr + i);
		b = (*(bytePtr + i)) & 0x0F;
		a = ((*(bytePtr + i)) >> 4) & 0x0F;
		if ((a >= 0) && (a <= 9))
			a += '0';
		else
			a = a - 10 + 'A';

		if ((b < 10) && (b > -1))
			b = b + '0';
		else
			b = b + 'A' - 10;
		UDR = a;
		while(!(UCSRA & (1 << UDRE)));
		UDR = b;
		while(!(UCSRA & (1 << UDRE)));
	}
}

void serialWrite32(uint32_t number) {
	// number = swap_uint32(number);
	number = ((number << 8) & 0xFF00FF00 ) | ((number >> 8) & 0xFF00FF ); 
	number = (number << 16) | (number >> 16);
	uint8_t* bytePtr = (uint8_t*)&number;
	uint8_t i;
	uint8_t current_byte;
	char a, b;
	for (i=0; i<sizeof(uint32_t); i++) {
		current_byte = *(bytePtr + i);
		b = (*(bytePtr + i)) & 0x0F;
		a = ((*(bytePtr + i)) >> 4) & 0x0F;
		if ((a >= 0) && (a <= 9))
			a += '0';
		else
			a = a - 10 + 'A';

		if ((b < 10) && (b > -1))
			b = b + '0';
		else
			b = b + 'A' - 10;
		UDR = a;
		while(!(UCSRA & (1 << UDRE)));
		UDR = b;
		while(!(UCSRA & (1 << UDRE)));
	}
}

void serialWrite16(uint16_t number) {
	// number = swap_uint32(number);
	number = (number >> 8) | (number << 8);
	uint8_t* bytePtr = (uint8_t*)&number;
	uint8_t i;
	uint8_t current_byte;
	char a, b;
	for (i=0; i<sizeof(uint16_t); i++) {
		current_byte = *(bytePtr + i);
		b = (*(bytePtr + i)) & 0x0F;
		a = ((*(bytePtr + i)) >> 4) & 0x0F;
		if ((a >= 0) && (a <= 9))
			a += '0';
		else
			a = a - 10 + 'A';

		if ((b < 10) && (b > -1))
			b = b + '0';
		else
			b = b + 'A' - 10;
		UDR = a;
		while(!(UCSRA & (1 << UDRE)));
		UDR = b;
		while(!(UCSRA & (1 << UDRE)));
	}
}

void serialWrite8(uint8_t number) {
	uint8_t* bytePtr = (uint8_t*)&number;
	char a, b;
	b = (*(bytePtr)) & 0x0F;
	a = ((*(bytePtr)) >> 4) & 0x0F;
	if (a < 10) a = a + '0';
	else a = a + 'A' - 10;
	if (b < 10) b = b + '0';
	else b = b + 'A' - 10;
	UDR = a;
	while(!(UCSRA & (1 << UDRE)));
	UDR = b;
	while(!(UCSRA & (1 << UDRE)));
}
#endif