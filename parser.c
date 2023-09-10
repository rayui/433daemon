#include <stdio.h>
#include <stdint.h>
#include "include/parser.h"
#include "include/output.h"

struct Parser *createParser(int id, int len, int short_min, int long_min, int long_max) {
  struct Parser *parser = malloc(sizeof(struct Parser));
  parser->id = id;
  parser->len = len;
  parser->short_min = short_min;
  parser->long_min = long_min;
  parser->long_max = long_max;
  parser->last_pulse = INVALID_PULSE;
  parser->data = 0;
  parser->pos = 0;
  parser->flip = false;
  return parser;
}

struct Parser *destroyParser(struct Parser *parser) {
  free(parser);
  return parser;
} 

bool isLastPulseValid(struct Parser *parser) {
  return parser->last_pulse != INVALID_PULSE;
}

void resetParser(struct Parser *parser) {
  parser->data = 0;
  parser->pos = 0;
  parser->flip = false;
}

bool isParserReady(struct Parser *parser, int pulse_type) {
  return parser->flip && parser->last_pulse ^ pulse_type;
}

void commitBit(struct Parser *parser) {
 	parser->data += (parser->last_pulse << parser->pos);
  parser->pos++;
  parser->flip = false;
}

bool isParserDone(struct Parser *parser) {
  return parser->data != 0 && parser->pos == parser->len;
}
     
void printBits(struct Parser *parser) {
  printHex(parser->data, parser->len);
}

void flipParser(struct Parser *parser) {
  parser->flip = true;
}

void setPulse(struct Parser *parser, int pulse_type) {
  parser->last_pulse = pulse_type;
}

int getPulseType(struct Parser *parser, int w) {
  if (w < parser->short_min) {
    return INVALID_PULSE;
  } else if (w < parser->long_min) {
    return SHORT_PULSE;
  } else if (w <= parser->long_max) {
    return LONG_PULSE;
  }

  return INVALID_PULSE;
}

void decodePulse(struct Parser *parser, int width) {
  int pulse_type = getPulseType(parser, width);

  if (!isLastPulseValid(parser)) {
    resetParser(parser);
	} else if (isParserReady(parser, pulse_type)) {
    commitBit(parser);
	  if (isParserDone(parser)) {
      printBits(parser);
		}
  } else {
    flipParser(parser);
  }

  setPulse(parser, pulse_type);
}


