#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define SHORT_PULSE_MIN 70
#define LONG_PULSE_MIN 281 
#define LONG_PULSE_MAX 575 

#define INVALID_PULSE -1 
#define SHORT_PULSE 0
#define LONG_PULSE 1

struct Parser {
  int id;
  unsigned long data;
  int pos;
  int len;
  int last_pulse;
  int short_min;
  int long_min;
  int long_max;
  bool flip;
};

struct Parser *createParser(int id, int len, int short_min, int long_min, int long_max);
struct Parser *destroyParser(struct Parser *parser);
bool isLastPulseValid(struct Parser *parser);
void resetParser(struct Parser *parser);
bool isParserReady(struct Parser *parser, int pulse_type);
void commitBit(struct Parser *parser);
bool isParserDone(struct Parser *parser); 
void printBits(struct Parser *parser); 
void flipParser(struct Parser *parser); 
void setPulse(struct Parser *parser, int pulse_type); 
int getPulseType(struct Parser *parser, int w);
void decodePulse(struct Parser *parser, int width); 

