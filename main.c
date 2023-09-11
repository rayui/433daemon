#include <gpiod.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "include/parser.h"
#include "include/gpioline.h"

int main(int argc, char **argv)
{
	struct gpiod_chip *chip;
  struct GPIOLine *gpioLine;
  struct Parser *parser;
	
  unsigned int line_num = 27;
  unsigned int buff_len = 8;
  unsigned int short_pulse_min = 0;
  unsigned int long_pulse_min = 0;
  unsigned int long_pulse_max = 0;

  if( argc != 7 ) {
    printf("Arguments are: chipname gpio_pin buffer_len(bytes, max 8) short_pulse_min(us) long_pulse_min(us) long_pulse_max(us)\ne.g. %s gpiochip0 27 3 190 400 750\n", argv[0]);
    return 0;
  }

  line_num = atoi(argv[2]);
  buff_len = atoi(argv[3]);
  short_pulse_min = atoi(argv[4]);
  long_pulse_min = atoi(argv[5]);
  long_pulse_max = atoi(argv[6]);

  parser = createParser(0, buff_len * 8, short_pulse_min, long_pulse_min, long_pulse_max);
	
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

  while (updateTick(gpioLine))
  {
    if (!isGPIOWaiting(gpioLine))
    {
      decodePulse(parser, getLastWidth(gpioLine));
    }
  }

close_gpio:
  destroyGPIOLine(gpioLine);
close_chip:
  gpiod_chip_close(chip);
end:
  destroyParser(parser);
  return -1;
}
