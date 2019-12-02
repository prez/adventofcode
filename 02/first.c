#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BUFLEN 200

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


	nums[1] = 12;
	nums[2] = 2;
	
	for (size_t i = 0; i < len; i += 4) {
		if (99 == nums[i]) {
			goto end;
		}
		if (nums[i+3] >= len) {
			uint_fast32_t *nums_new = realloc(nums, nums[i+3] * sizeof(uint_fast32_t));
			if (NULL == nums_new) {
				exit(1);
			}
			nums = nums_new;
		}
		switch (nums[i]) {
		case 1:
			nums[nums[i+3]] = nums[nums[i+1]] + nums[nums[i+2]];
			break;
		case 2:
			nums[nums[i+3]] = nums[nums[i+1]] * nums[nums[i+2]];
			break;
		}
	}

end:
	printf("%lu\n", nums[0]);
}
