#ifndef _H_
#define OS_H_
#include <inttypes.h>
extern void serialInit();
extern void serialWrite(const char message[]);
extern void serialWrite64(uint64_t number);
extern void serialWrite32(uint32_t number);
extern void serialWrite16(uint16_t number);
extern void serialWrite8(uint8_t number);

#endif