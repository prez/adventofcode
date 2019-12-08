#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define WIDTH	25
#define HEIGHT	6

int
main(void)
{
	FILE *f = fopen("input", "r");
	
	uint_fast8_t decoded[WIDTH*HEIGHT];
	for (size_t i = 0; i < WIDTH*HEIGHT; decoded[i] = 2, i++) {}
	
	uint_fast32_t offs, mins[3], counts[3] = { 0 }, pos = 0;
	for (int_fast32_t c; ; pos++) {
		c = getc(f);
		if ((EOF == c) || ('\n' == c)) break;
		offs = pos % (WIDTH*HEIGHT);
		if (!offs) {
			if (pos && (counts[0] < mins[0])) {
				memcpy(mins, counts, sizeof(counts));
			}
			counts[0] = 0; counts[1] = 0; counts[2] = 0;
		}
		if (2 == decoded[offs]) decoded[offs] = c-'0';
		counts[c-'0']++;
	}
	fclose(f);

	printf("first:\t%lu\n", mins[1] * mins[2]);
	printf("%s\n", "second:");
	for (size_t i = 0; i < WIDTH*HEIGHT; i++) {
		switch (decoded[i]) {
		case 0: printf(" "); break;
		case 1: printf("█"); break;
		}
		if (0 == (i+1) % WIDTH) printf("\n");
	}
}