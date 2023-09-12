#include "include/ook.h"
#include "include/output.h"

struct OokDecoder *createOokDecoder(int id, int len, int short_min, int long_min, int long_max) {
  struct OokDecoder *decoder = malloc(sizeof(struct OokDecoder));
  decoder->id = id;
  decoder->state = STATE_UNKNOWN;
  decoder->len = len;
  decoder->short_min = short_min;
  decoder->long_min = long_min;
  decoder->long_max = long_max;
  decoder->pos = 0;
  decoder->flip = false;
  decoder->data = malloc(sizeof(unsigned char) * decoder->len * 8);
  return decoder;
}

struct OokDecoder *destroyOokDecoder(struct OokDecoder *decoder)
{
  free(decoder->data);
  free(decoder);
  return decoder;
}

bool nextPulseOok(struct OokDecoder *decoder, unsigned int width) {
  if (decoder->state != STATE_DONE) {
    switch (decodeOok(decoder, width)) {
      case -1:
        resetOok(decoder);
        break;
      case 1:
        decoder->state = STATE_DONE;
        break;
    }
    return decoder->state == STATE_DONE;
  }
  return false;
}

void resetOok(struct OokDecoder *decoder) {
  decoder->total_bits = 0;
  decoder->pos = 0;
  decoder->flip = 0;
  decoder->state = STATE_UNKNOWN;
  free(decoder->data);
  decoder->data = malloc(sizeof(unsigned char) * decoder->len * 8);
}

void manchesterOok(struct OokDecoder *decoder, bool value) {
  decoder->flip ^= value;
  gotBitOok(decoder, decoder->flip);
}

void sprintOok(struct OokDecoder *decoder) {
  printHex(decoder->data, decoder->len);
  resetOok(decoder);
}

void gotBitOok(struct OokDecoder *decoder, unsigned int value) {
  decoder->data[decoder->pos] = (decoder->data[decoder->pos] >> 1) | (value ? 0x80 : 0);
  decoder->total_bits++;
  decoder->pos = decoder->total_bits >> 3;
  if (decoder->pos >= decoder->len) {
    resetOok(decoder);
  }
  decoder->state = STATE_OK;
}

unsigned int decodeOok(struct OokDecoder *decoder, unsigned int width) {
  bool w = false;

  if (width >= decoder->short_min && width < decoder->long_max) {
    w = width >= decoder->long_min;
    switch(decoder->state) {
      case STATE_UNKNOWN:
        if (!w) {
          decoder->flip++;
        } else if (decoder->flip > MAGIC_NUMBER) {
          decoder->flip = 1;
          manchesterOok(decoder, true);
        } else {
          return -1;
        }
        break;
      case STATE_OK:
        if (!w) {
          decoder->state = STATE_T0;
        } else {
          manchesterOok(decoder, true);
        }
        break;
      case STATE_T0:
        if (!w) {
          manchesterOok(decoder, false);
        } else {
          return -1;
        }
    }
  } else {
    return -1;
  }

  return decoder->total_bits == decoder->len * 8 - 1 ? 1 : 0;
}

