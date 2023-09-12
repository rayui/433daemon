#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
 
#define STATE_UNKNOWN 0
#define STATE_T0 1
#define STATE_T1 2 
#define STATE_T2 3
#define STATE_T3 4
#define STATE_OK 5
#define STATE_DONE 6

#define MAGIC_NUMBER 32

struct OokDecoder
{
  unsigned int id;
  unsigned int state;
  unsigned int total_bits;
  unsigned int short_min;
  unsigned int long_min;
  unsigned int long_max;
  unsigned int pos;
  unsigned int flip;
  unsigned int len;
  unsigned char *data;
};

struct OokDecoder *createOokDecoder(int id, int len, int short_min, int long_min, int long_max);
struct OokDecoder *destroyOokDecoder(struct OokDecoder *decoder);
bool nextPulseOok(struct OokDecoder *decoder, unsigned int width);
void resetOok(struct OokDecoder *decoder);
void manchesterOok(struct OokDecoder *decoder, bool value);
void sprintOok(struct OokDecoder *decoder);
void gotBitOok(struct OokDecoder *decoder, unsigned int value);
unsigned int decodeOok(struct OokDecoder *decoder, unsigned int width);
