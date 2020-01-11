#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <err.h>
#include <sys/cdefs.h>

#define BUFLEN 20000

uint_fast32_t
update_max(uint_fast32_t current)
{
	static uint_fast32_t max = 0;
	if (current && (current > max)) {
		max = current;
	}
	return max;
}

uint_fast8_t
none_repeating(int_fast32_t *arr)
{
	uint_fast8_t ret = 1;
	for (int i = 0; i < 5; i++) {
		for (int j = i+1; j < 5; j++) {
			if (arr[i] == arr[j])
				return 0;
		}
	}
	return ret;
}

/*
first:	212460
*/
int
main(int argc, char **argv)
{
	if (2 != argc) errx(1, "%s", "invalid input file");
	FILE *f = fopen(argv[1], "r");
	if (NULL == f) err(1, NULL);

	int_fast32_t *nums = malloc(BUFLEN * sizeof(int_fast32_t));
	if (NULL == nums) err(1, NULL);

	size_t len = 0; 
	for (size_t bufsize = BUFLEN; EOF != fscanf(f, "%ld,", nums + len); len++) {
		if (bufsize == len+1) {
			bufsize *= 50;
			xrealloc(&nums, bufsize);
		}	
	}
	fclose(f);

	int_fast32_t initial = 0;
	int st = 0, en = 5;
	for (int i = st; i < en; i++) {
	for (int j = st; j < en; j++) {
	for (int k = st; k < en; k++) {
	for (int l = st; l < en; l++) {
	for (int m = st; m < en; m++)	
	{
		int_fast32_t phase[5] = { i, j, k, l, m };

		// feed should be double arg dependent on prev ?
		/* band, band_len, rip, base, crr_mode[4], feed */		
		struct vm_state *a = vm_init(NULL, "xalrbcf", nums, len, 0, 0, { 0, 0, 0, 0 }, phase[0], 0 ...);	
		struct vm_state *b = vm_init(NULL, "xalrbcf", nums, len, 0, 0, { 0, 0, 0, 0 }, phase[1], 0 ...);	
		struct vm_state *c = vm_init(NULL, "xalrbcf", nums, len, 0, 0, { 0, 0, 0, 0 }, phase[2], 0 ...);	
		struct vm_state *d = vm_init(NULL, "xalrbcf", nums, len, 0, 0, { 0, 0, 0, 0 }, phase[3], 0 ...);	
		struct vm_state *d = vm_init(NULL, "xalrbcf", nums, len, 0, 0, { 0, 0, 0, 0 }, phase[4], 0 ...);	

		// ret of last: int_fast32_t meme = ;				
		vm_simulate(a);
		vm_simulate(b);
		vm_simulate(c);
		vm_simulate(d);
		vm_simulate(e);
		
		vm_destroy(a);
		vm_destroy(b);
		vm_destroy(c);
		vm_destroy(d);
		vm_destroy(e);

		if (none_repeating(phase))
			update_max(meme);
//			printf("%ld\t%ld\t%ld\t%ld\t%ld\t%ld\n", meme, phase[0], phase[1], phase[2], phase[3], phase[4]);
	}}}}}
	printf("first:\t%ld\n", update_max(0));
}
