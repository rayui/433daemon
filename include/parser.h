#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define INVALID_PULSE -1
#define SHORT_PULSE 0
#define LONG_PULSE 1

struct Parser
{
  int id;
  char *data;
  int pos;
  int min_len;
  int max_len;
  int last_pulse;
  int short_min;
  int long_min;
  int long_max;
  bool flip;
};

struct Parser *createParser(int id, int min_len, int max_len, int short_min, int long_min, int long_max);
struct Parser *destroyParser(struct Parser *parser);
void resetParser(struct Parser *parser);
void commitBit(struct Parser *parser);
int getPulseType(struct Parser *parser, unsigned int w);
void decodePulse(struct Parser *parser, unsigned int width);
