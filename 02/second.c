#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BUFLEN 200
#define VALUE  19690720

/* int __attribute__ ((pure)) */
uint_fast32_t
simulate(uint_fast8_t noun, uint_fast8_t verb, uint_fast32_t *arr, size_t len)
{
	arr[1] = noun;
	arr[2] = verb;

	for (size_t i = 0; i < len; i += 4) {
		if (99 == arr[i]) {
			goto end;
		}
		if (arr[i+3] >= len) {
			uint_fast32_t *arr_new = realloc(arr, arr[i+3] * sizeof(uint_fast32_t));
			if (NULL == arr_new) {
				exit(1);
			}
			arr = arr_new;
		}
		switch (arr[i]) {
		case 1:
			arr[arr[i+3]] = arr[arr[i+1]] + arr[arr[i+2]];
			break;
		case 2:
			arr[arr[i+3]] = arr[arr[i+1]] * arr[arr[i+2]];
			break;
		}
	}

end:
	return arr[0];
}

int
main(void)
{
	FILE *f = fopen("bigboy", "r");
	uint_fast32_t *nums = malloc(BUFLEN * sizeof(uint_fast32_t));
	if (NULL == nums) {
		return 1;
	}
	size_t len = 0; 
	for (size_t bufsize = BUFLEN; EOF != fscanf(f, "%lu,", nums + len); len++) {
		if (bufsize == len+1) {
			bufsize *= 50;
			uint_fast32_t *nums_new = realloc(nums, bufsize * sizeof(uint_fast32_t));
			if (NULL == nums_new) {
				return 1;
			}
			nums = nums_new;
		}
	
	}
	fclose(f);

	uint_fast32_t *code = malloc(len * sizeof(uint_fast32_t));
	for (uint_fast8_t i = 0; i <= 99; i++) {
		for (uint_fast8_t j = 0; j <= 99; j++) {
			memcpy(code, nums, len * sizeof(uint_fast32_t));
			if (VALUE == simulate(i, j, code, len)) {
				printf("%u\n", 100 * i + j);
				return 0;
			}
		}
	}
}
