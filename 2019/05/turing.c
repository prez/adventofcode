#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <err.h>
#include <sys/cdefs.h>
#include "intcode.h"

#define BUFLEN 200
/*
first:  0...0 7286649
second: 15724522
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
	//fscanf(f, "%lld", &nums[len]); //why?
	fclose(f);

	int_fast64_t *nums_2 = malloc(len * sizeof(int_fast64_t));
	memcpy(nums_2, nums, len * sizeof(int_fast64_t));

	struct vm_state vm = { nums, len, 0, 0, { 0, 0, 0, 0 }, 1 };
	printf("%s", "first:\t");
	vm_simulate(&vm);
	
	vm.band = nums_2; vm.rip = 0; vm.feed = 5;
	printf("%s", "second:\t");
	vm_simulate(&vm);
}
