#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <err.h>
#include <sys/cdefs.h>
#include "intcode.h"

#define BUFLEN 200
#define VALUE  19690720


/*
first:	5866663
second:	4259
*/
int
main(int argc, char **argv)
{
	if (2 != argc) errx(1, "%s", "invalid input file");
	FILE *f = fopen(argv[1], "r");
	if (NULL == f) err(1, NULL);
	
	int_fast64_t *nums = malloc(BUFLEN * sizeof(int_fast64_t));
	if (NULL == nums) err(1, NULL);
	size_t len = 0; 
	for (size_t bufsize = BUFLEN; EOF != fscanf(f, "%lld,", nums + len); len++) {
		if (bufsize == len + 1) {
			bufsize *= 2;
			int_fast64_t *nums_new = realloc(nums, bufsize * sizeof(int_fast64_t));
			if (NULL == nums_new) err(1, NULL);
			nums = nums_new;
		}	
	}
	fclose(f);

	struct vm_state vm = {nums, len, 0, 0, { 0, 0, 0, 0 }, 0 };

	int_fast64_t *code = malloc(len * sizeof(int_fast64_t));
	uint_fast8_t i, j;
	for (i = 0; i <= 99; i++) {
		for (j = 0; j <= 99; j++) {
			memcpy(code, nums, len * sizeof(int_fast64_t));
			code[1] = i; code[2] = j;
			vm.band = code; vm.rip = 0;
			vm_simulate(&vm);
			if (VALUE == code[0]) {
				goto end;
			}
		}
	}

end:
	nums[1] = 12; nums[2] = 2;
	vm.band = nums; vm.rip = 0;
	vm_simulate(&vm);
	printf("first:\t%ld\n", nums[0]);
	printf("second:\t%u\n", 100 * i + j);
}
