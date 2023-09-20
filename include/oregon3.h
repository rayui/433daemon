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

struct Oregon3Decoder
{
  unsigned int id;
  unsigned int state;
  unsigned int total_bits;
  unsigned int short_min;
  unsigned int long_min;
  unsigned int long_max;
  unsigned int pos;
  unsigned int flip;
  unsigned int min_len;
  unsigned int max_len;
  unsigned char *data;
};

struct Oregon3Decoder *createOregon3Decoder(int id, int min_len, int max_len, int short_min, int long_min, int long_max);
struct Oregon3Decoder *destroyOregon3Decoder(struct Oregon3Decoder *decoder);
bool nextPulseOregon3(struct Oregon3Decoder *decoder, unsigned int width);
void resetOregon3(struct Oregon3Decoder *decoder);
void manchesterOregon3(struct Oregon3Decoder *decoder, bool value);
void sprintOregon3(struct Oregon3Decoder *decoder);
void gotBitOregon3(struct Oregon3Decoder *decoder, unsigned int value);
unsigned int decodeOregon3(struct Oregon3Decoder *decoder, unsigned int width);
