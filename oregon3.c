#include "include/oregon3.h"
#include "include/output.h"

struct Oregon3Decoder *createOregon3Decoder(int id, int min_len, int max_len, int short_min, int long_min, int long_max) {
  struct Oregon3Decoder *decoder = malloc(sizeof(struct Oregon3Decoder));
  decoder->id = id;
  decoder->state = STATE_UNKNOWN;
  decoder->min_len = min_len;
  decoder->max_len = max_len;
  decoder->short_min = short_min;
  decoder->long_min = long_min;
  decoder->long_max = long_max;
  decoder->pos = 0;
  decoder->flip = false;
  decoder->data = malloc(sizeof(unsigned char) * decoder->max_len);
  return decoder;
}

struct Oregon3Decoder *destroyOregon3Decoder(struct Oregon3Decoder *decoder)
{
  free(decoder->data);
  free(decoder);
  return decoder;
}

bool nextPulseOregon3(struct Oregon3Decoder *decoder, unsigned int width) {
  if (decoder->state != STATE_DONE) {
    switch (decodeOregon3(decoder, width)) {
      case -1:
        resetOregon3(decoder);
        return false;
      case 1:
        decoder->state = STATE_DONE;
        return true;
    }
    return decoder->state == STATE_DONE;
  }
  return false;
}

void resetOregon3(struct Oregon3Decoder *decoder) {
  decoder->total_bits = 0;
  decoder->pos = 0;
  decoder->flip = 0;
  decoder->state = STATE_UNKNOWN;
}

void manchesterOregon3(struct Oregon3Decoder *decoder, bool value) {
  decoder->flip ^= value;
  gotBitOregon3(decoder, decoder->flip);
}

void sprintOregon3(struct Oregon3Decoder *decoder) {
  printHex(decoder->data, decoder->total_bits);
  resetOregon3(decoder);
}

void gotBitOregon3(struct Oregon3Decoder *decoder, unsigned int value) {
  decoder->data[decoder->pos] = (decoder->data[decoder->pos] >> 1) | (value ? 0x80 : 0);
  decoder->total_bits++;
  decoder->pos = decoder->total_bits >> 3;
  if (decoder->pos >= (decoder->max_len + 7) >> 3) {
    resetOregon3(decoder);
  }
  decoder->state = STATE_OK;
}

unsigned int decodeOregon3(struct Oregon3Decoder *decoder, unsigned int width) {
  bool w = false;

  if (width >= decoder->short_min && width < decoder->long_max) {
    w = width >= decoder->long_min;
    switch(decoder->state) {
      case STATE_UNKNOWN:
        if (!w) {
          decoder->flip++;
        } else if (decoder->flip > MAGIC_NUMBER) {
          decoder->flip = 1;
          manchesterOregon3(decoder, true);
        } else {
          return -1;
        }
        break;
      case STATE_OK:
        if (!w) {
          decoder->state = STATE_T0;
        } else {
          manchesterOregon3(decoder, true);
        }
        break;
      case STATE_T0:
        if (!w) {
          manchesterOregon3(decoder, false);
        } else {
          return -1;
        }
    }
  } else {
    return (decoder->total_bits >= decoder->min_len && decoder->total_bits < decoder->max_len  ) ? 1 : -1;   
  }

  return decoder->total_bits == decoder->max_len ? 1 : 0;
}

