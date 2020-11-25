#include <err.h>
#include <stdio.h>
#include <string.h>
#include "hash_table.h"

#define BUFLEN 4096

uint_fast32_t
walk_up(struct ht_instance *ht, char *start)
{
	if (strcmp(start, "COM")) {
		return 1 + walk_up(ht, ht_query(ht, start));
	}
	else return 0;
}

int_fast32_t
insert_up(struct ht_instance *ht, struct ht_instance *ht_p, char *start)
{
	if (0 == strcmp(start, "COM")) {
		return 0;
	}
	else if (ht_insert(ht_p, start, "")) {
		return -1 + insert_up(ht, ht_p, ht_query(ht, start));
	}
	else {
		return 1 + insert_up(ht, ht_p, ht_query(ht, start));
	}
}

int
main(int argc, char **argv)
{
	if (2 != argc) errx(1, "%s", "invalid input file");
	FILE *f = fopen(argv[1], "r");
	if (NULL == f) err(1, NULL);
	
	struct ht_instance *ht = ht_new();
	for (char input[BUFLEN], c[4095], p[4095]; fgets(input, sizeof(input), f); ) {
		sscanf(input, "%4094[^)]) %4094[^\n]", p, c);
		ht_insert(ht, c, p);
	}
	fclose(f);

	uint_fast32_t sum = 0;
	for (uint_fast32_t i = 0, total = 0; total < ht->count; i++) {
		if (NULL != ht->items[i]) {
			sum += walk_up(ht, ht->items[i]->key);
			total++;
		}
	}

	struct ht_instance *p_ht = ht_new();
		
	printf("first:\t%lu\n", sum);
	printf("second:\t%ld\n", insert_up(ht, p_ht, "SAN") + insert_up(ht, p_ht, "YOU") - 2);
}
