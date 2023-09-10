#include <gpiod.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif

#define SHORT_PULSE_MIN 70
#define LONG_PULSE_MIN 281 
#define LONG_PULSE_MAX 575 

#define INVALID_PULSE -1 
#define SHORT_PULSE 0
#define LONG_PULSE 1

#define BIT_FALSE 0 
#define BIT_TRUE 1
#define BIT_INVALID 2

#define BUFF_LEN 24 

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 
	

int getPulseType(int w)
{
  if (w < SHORT_PULSE_MIN) {
    return INVALID_PULSE;
  } else if (w < LONG_PULSE_MIN) {
    return SHORT_PULSE;
  } else if (w <= LONG_PULSE_MAX) {
    return LONG_PULSE;
  }

  return INVALID_PULSE;
}

int main(int argc, char **argv)
{
	char *chipname = "gpiochip0";
	unsigned int line_num = 27;	// GPIO Pin #25
	struct timespec ts = { 1, 0 };
	struct gpiod_line_event event;
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	int ret;
	unsigned long long last_tick = 0;
	unsigned long long tick = 0;
  unsigned long data = 0;
	int pulse_type = INVALID_PULSE;
	int last_pulse = INVALID_PULSE;
	int pos = 0;
  bool flip = false;

	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed\n");
		ret = -1;
		goto end;
	}

	line = gpiod_chip_get_line(chip, line_num);
	if (!line) {
		perror("Get line failed\n");
		ret = -1;
		goto close_chip;
	}

	ret = gpiod_line_request_both_edges_events(line, CONSUMER);
	if (ret < 0) {
		perror("Request event notification failed\n");
		ret = -1;
		goto release_line;
	}

	while (true) {
		ret = gpiod_line_event_wait(line, &ts);
		if (ret < 0) {
			perror("Wait event notification failed\n");
			ret = -1;
			goto release_line;
		} else if (ret == 0) {
			printf("Wait event notification on line #%u timeout\n", line_num);
			continue;
		}

		ret = gpiod_line_event_read(line, &event);
		if (ret < 0) {
			perror("Read last event notification failed\n");
			ret = -1;
			goto release_line;
		}

		tick = event.ts.tv_sec * 1000000 + event.ts.tv_nsec / 1000;
		pulse_type = getPulseType(tick - last_tick);

		if (last_pulse == INVALID_PULSE) {
      data = 0;
      pos = 0;
      flip = false;
		} else if (flip && last_pulse ^ pulse_type) {
    	data += (last_pulse << pos);
  		pos++;
      flip = false;
		  if (pos == BUFF_LEN && data & 0x6 == 6) {
     //    printf(BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(data & 0xFF), BYTE_TO_BINARY((data >> 8) &0xFF), BYTE_TO_BINARY((data >> 16) & 0xFF));
         printf("%02x%02x%02x\n", (data >> 2) & 0xFF, (data >> 10) & 0xFF, (data >> 18) & 0xFF);
         fflush(stdout);
		 	}
	  } else {
      flip = true;
    }

		last_tick = tick;
		last_pulse = pulse_type;
	}

	ret = 0;

release_line:
	gpiod_line_release(line);
close_chip:
	gpiod_chip_close(chip);
end:
	return ret;
}
