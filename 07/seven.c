#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <err.h>
#include <sys/cdefs.h>

#define BUFLEN 20000

int_fast32_t __attribute_pure__
fetch_arg(int_fast32_t *arr, size_t rip, uint_fast8_t *param_mode, size_t param_nr)
{
	switch (param_mode[param_nr]) {
		case 0:
			return arr[arr[rip+param_nr]]; 
		case 1:
			return arr[rip+param_nr];
		default:
			errx(1, "%s\n", "invalid mode");
	}
}

void
xrealloc(int_fast32_t **arr, size_t new_sz)
{
	int_fast32_t *arr_new = realloc(*arr, new_sz * sizeof(int_fast32_t));
	if (NULL == arr_new) {
		err(1, NULL);
	}
	*arr = arr_new;
}

void
write_at_addr(int_fast32_t *arr, size_t pos, size_t len, int_fast32_t val)
{
	//if (arr[pos] >= len) {
//		xrealloc(&arr, arr[pos]);
//	}
	arr[arr[pos]] = val;
}

int_fast32_t
simulate(int_fast32_t in1, int_fast32_t in2, int_fast32_t *arr, size_t len)
{
	int_fast32_t ret = 0;
	size_t in_nr = 0;
	int_fast32_t in[2] = { in1, in2 };
	for (size_t rip = 0; rip < len; ) {
		if (99 == arr[rip]) return ret;

		size_t opcode = arr[rip] % 100;
		uint_fast8_t param_mode[4] = { 2,                  (arr[rip]/100)%10,
		                               (arr[rip]/1000)%10, (arr[rip]/10000)%10 };
		
		switch (opcode) {
		case 1:
			write_at_addr(arr, rip+3, len, (fetch_arg(arr, rip, param_mode, 1) +
			                                fetch_arg(arr, rip, param_mode, 2)));
			rip += 4;
			break;
		case 2:
			write_at_addr(arr, rip+3, len, (fetch_arg(arr, rip, param_mode, 1) *
			                                fetch_arg(arr, rip, param_mode, 2)));
			rip += 4;
			break;
		case 3:
			write_at_addr(arr, rip+1, len, in[in_nr]);
			in_nr++;
			rip += 2;
			break;
		case 4:
			if (fetch_arg(arr, rip, param_mode, 1)) {
				ret = fetch_arg(arr, rip, param_mode, 1);
			}
			rip += 2;
			break;
		case 5:
			if (fetch_arg(arr, rip, param_mode, 1)) {
				rip = fetch_arg(arr, rip, param_mode, 2);
			}
			else rip += 3;
			break;
		case 6:
			if (!fetch_arg(arr, rip, param_mode, 1)) {
				rip = fetch_arg(arr, rip, param_mode, 2);
			}
			else rip += 3;
			break;
		case 7:
			write_at_addr(arr, rip+3, len, (fetch_arg(arr, rip, param_mode, 1) <
		                                    fetch_arg(arr, rip, param_mode, 2)));
			rip += 4;
			break;
		case 8:
			write_at_addr(arr, rip+3, len, (fetch_arg(arr, rip, param_mode, 1) ==
			                                fetch_arg(arr, rip, param_mode, 2)));
			rip += 4;
			break;
		default:
			errx(1, "%s %lu\n", "invalid opcode", opcode);
		}
	}
	return ret;
}

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

int
main(int argc, char **argv)
{
	if (2 != argc) errx(1, "%s", "invalid input file");
	FILE *f = fopen(argv[1], "r");
	if (NULL == f) err(1, NULL);

	int_fast32_t *nums = malloc(BUFLEN * sizeof(int_fast32_t));
	if (NULL == nums) {
		err(1, NULL);
	}
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
		//int_fast32_t phase[5] = { 1, 0, 4, 3, 2 };
	
		int_fast32_t *nums_1 = malloc(len * sizeof(int_fast32_t));
		memcpy(nums_1, nums, len * sizeof(int_fast32_t));
		int_fast32_t *nums_2 = malloc(len * sizeof(int_fast32_t));
		memcpy(nums_2, nums, len * sizeof(int_fast32_t));
		int_fast32_t *nums_3 = malloc(len * sizeof(int_fast32_t));
		memcpy(nums_3, nums, len * sizeof(int_fast32_t));
		int_fast32_t *nums_4 = malloc(len * sizeof(int_fast32_t));
		memcpy(nums_4, nums, len * sizeof(int_fast32_t));
		int_fast32_t *nums_5 = malloc(len * sizeof(int_fast32_t));
		memcpy(nums_5, nums, len * sizeof(int_fast32_t));
	
		
		int_fast32_t meme = simulate(phase[4], simulate(phase[3], simulate(phase[2], simulate(phase[1], simulate(phase[0], 0, nums_1, len), nums_2, len), nums_3, len), nums_4, len), nums_5, len);
	
		
		if (none_repeating(phase))
			update_max(meme);
//			printf("%ld\t%ld\t%ld\t%ld\t%ld\t%ld\n", meme, phase[0], phase[1], phase[2], phase[3], phase[4]);
	}}}}}
	printf("first:\t%ld\n", update_max(0));
}
