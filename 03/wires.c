#include <sys/cdefs.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "hash_table.h"

#define BUFSIZE 2000000
#define X_ORIGIN 0
#define Y_ORIGIN 1000

uint_fast32_t
update_min(uint_fast32_t current, uint_fast8_t ind)
{
	static uint_fast32_t min[2] = { 4294967295, 4294967295 };
	if (current && (current < min[ind])) {
		min[ind] = current;
	}
	return min[ind];
}

uint_fast32_t __attribute_pure__
manhattan_dist(uint_fast32_t x, uint_fast32_t y, uint_fast32_t x_o,
               uint_fast32_t y_o)
{
	uint_fast32_t i = (x > x_o) ? (x - x_o) : (x_o - x);
	uint_fast32_t j = (y > y_o) ? (y - y_o) : (y_o - y);
	return i + j;
}

int
main(int argc, char **argv)
{
	if (2 != argc) errx(1, "%s", "invalid input file");
	FILE *f = fopen(argv[1], "r");
	if (NULL == f) err(1, NULL);

	char wrk[BUFSIZE];
	struct ht_instance *hm = ht_new();
	for (uint_fast32_t x, y, second_run = 0; fgets(wrk, sizeof(wrk), f); second_run++) {
		uint_fast32_t res, steps_taken = 0;
		x = X_ORIGIN;
		y = Y_ORIGIN;
		for (char *ch = strtok(wrk, ",\n"); NULL != ch; ch = strtok(NULL, ",\n")) {
			char dir = ch[0];
			uint_fast32_t mov = (uint_fast32_t)strtoul(ch + 1, NULL, 10);

			switch (dir) {
			case 'R':
				for (uint_fast32_t i = 1; i <= mov; i++) {
					if (!second_run) {
						ht_insert(hm, x + i, y, steps_taken + i);
					}
					else if ((res = ht_query(hm, x + i, y))) {
						update_min(manhattan_dist(x + i, y, X_ORIGIN, Y_ORIGIN), 0);
						update_min(steps_taken + i + res, 1);
					}
				}
				x += mov;
				break;
			case 'L':
				for (uint_fast32_t i = 1; i <= mov; i++) {
					if (!second_run) {
						ht_insert(hm, x - i, y, steps_taken + i);
					}
					else if ((res = ht_query(hm, x - i, y))) {
						update_min(manhattan_dist(x - i, y, X_ORIGIN, Y_ORIGIN), 0);
						update_min(steps_taken + i + res, 1);
					}
				}
				x -= mov;
				break;
			case 'U':
				for (uint_fast32_t i = 1; i <= mov; i++) {
					if (!second_run) {
						ht_insert(hm, x, y + i, steps_taken + i);
					}
					else if ((res = ht_query(hm, x, y + i))) {
						update_min(manhattan_dist(x, y + i, X_ORIGIN, Y_ORIGIN), 0);
						update_min(steps_taken + i + res, 1);
					}
				}
				y += mov;
				break;
			case 'D':
				for (uint_fast32_t i = 1; i <= mov; i++) {
					if (!second_run) {
						ht_insert(hm, x, y - i, steps_taken + i);
					}
					else if ((res = ht_query(hm, x, y - i))) {
						update_min(manhattan_dist(x, y - i, X_ORIGIN, Y_ORIGIN), 0);
						update_min(steps_taken + i + res, 1);
					}
				}
				y -= mov;
				break;
			}
			steps_taken += mov;
		}
	}
	fclose(f);

	printf("first:\t%lu\n", update_min(0, 0));
	printf("second:\t%lu\n", update_min(0, 1));
}
