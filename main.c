#include <gpiod.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "include/parser.h"
#include "include/oregon3.h"
#include "include/gpioline.h"

int main(int argc, char **argv)
{
	struct gpiod_chip *chip;
  struct GPIOLine *gpioLine;
  struct Parser *parser;
  struct Oregon3Decoder *decoder;
	
  unsigned int line_num = 27;
  unsigned int min_msg_len = 32; 
  unsigned int max_msg_len = 32; 
  unsigned int short_pulse_min = 0;
  unsigned int long_pulse_min = 0;
  unsigned int long_pulse_max = 0;
  unsigned int protocol = 0;

  if( argc != 9 ) {
    printf("Arguments are: chipname gpio_pin min_message_len(bits) max_message_len(bits) short_pulse_min(us) long_pulse_min(us) long_pulse_max(us) protocol(pulse=0|oregon3=1)\ne.g. %s gpiochip0 27 24 120 190 400 750 0\n", argv[0]);
    return 0;
  }

  line_num = atoi(argv[2]);
  min_msg_len = atoi(argv[3]);
  max_msg_len = atoi(argv[4]);
  short_pulse_min = atoi(argv[5]) * 1000;
  long_pulse_min = atoi(argv[6]) * 1000;
  long_pulse_max = atoi(argv[7]) * 1000;
  protocol = atoi(argv[8]);

  parser = createParser(0, min_msg_len, max_msg_len, short_pulse_min, long_pulse_min, long_pulse_max);
  decoder = createOregon3Decoder(1, min_msg_len, max_msg_len, short_pulse_min, long_pulse_min, long_pulse_max);
  
  chip = gpiod_chip_open_by_name(argv[1]);
	if (!chip) {
		perror("Open chip failed\n");
		goto end;
	}

  gpioLine = createGPIOLine(line_num, chip);
  if (isGPIOErr(gpioLine))
  {
    goto close_gpio;
  }

  requestEvents(gpioLine);
  if (isGPIOErr(gpioLine))
  {
    goto close_gpio;
  }

  while (!isGPIOErr(gpioLine))
  {
    if (isEventReady(gpioLine))
    {
      while (updateTick(gpioLine))
      {
        if (protocol == 0) {
          decodePulse(parser, getLastWidth(gpioLine));
        } else {
          if (nextPulseOregon3(decoder, getLastWidth(gpioLine))) {
            sprintOregon3(decoder);
          }
        }
        setLastTick(gpioLine);
      }
    }
  }

close_gpio:
  destroyGPIOLine(gpioLine);
close_chip:
  gpiod_chip_close(chip);
end:
  destroyParser(parser);
  destroyOregon3Decoder(decoder);
  return -1;
}
