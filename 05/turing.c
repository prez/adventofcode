#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <err.h>
#include <sys/cdefs.h>

#define BUFLEN 10
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
write_at_addr(int_fast32_t **arr, size_t pos, size_t *len, int_fast32_t val)
{
	if ((*arr)[pos] >= *len) {

		printf("got request to store %ld in %lu, but array is %lu size. reallocating\n",
			       val, (*arr)[pos], *len);

		xrealloc(arr, (*arr)[pos]*+1);
		*len = (*arr)[pos]+1;
	}
	(*arr)[*len-1] = val;
	printf("succ\n");
	
}

void
simulate(int_fast32_t in, int_fast32_t *arr, size_t len)
{
	for (size_t rip = 0; rip < len; ) {
		if (99 == arr[rip]) return;

		size_t opcode = arr[rip] % 100;
		uint_fast8_t param_mode[4] = { 2,                  (arr[rip]/100)%10,
		                               (arr[rip]/1000)%10, (arr[rip]/10000)%10 };

		arr[100] = 2;
		/*
		printf("rip: %lu, \top: %lu,\tn: %ld,\ti: %ld,\tarr[100]: %ld,\t[rip+1]: %ld,\t[rip+2]: %ld,\t[rip+3]: %ld\n",
		       rip, opcode, arr[0], arr[1], arr[100], arr[rip+1], arr[rip+2], arr[rip+3]);
*/
		switch (opcode) {
		case 1:
			write_at_addr(&arr, rip+3, &len, (fetch_arg(arr, rip, param_mode, 1) +
			                                fetch_arg(arr, rip, param_mode, 2)));
			rip += 4;
			break;
		case 2:
			write_at_addr(&arr, rip+3, &len, (fetch_arg(arr, rip, param_mode, 1) *
			                                fetch_arg(arr, rip, param_mode, 2)));
			rip += 4;
			break;
		case 3:
			write_at_addr(&arr, rip+1, &len, in);
			rip += 2;
			break;
		case 4:
			//if (fetch_arg(arr, rip, param_mode, 1)) {
				printf("OUT: %ld\n", fetch_arg(arr, rip, param_mode, 1));
			//}
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
			printf("using op 7\n");
			write_at_addr(&arr, rip+3, &len, (fetch_arg(arr, rip, param_mode, 1) <
		                                    fetch_arg(arr, rip, param_mode, 2)));
			rip += 4;
			break;
		case 8:
			printf("using op 8\n");
			write_at_addr(&arr, rip+3, &len, (fetch_arg(arr, rip, param_mode, 1) ==
			                                fetch_arg(arr, rip, param_mode, 2)));
			rip += 4;
			break;
		case 9:
			printf("%c", fetch_arg(arr, rip, param_mode, 1));
			rip += 2;
			break;
		default:
			errx(1, "%s %lu\n", "invalid opcode", opcode);
		}
	}
}

int
main(void)
{
	FILE *f = fopen("bigboy", "r");
	int_fast32_t *nums = malloc(BUFLEN * sizeof(int_fast32_t));
	if (NULL == nums) {
		err(1, NULL);
	}
	size_t bufsize, len = 0; 
	for (bufsize = BUFLEN; EOF != fscanf(f, "%ld,", nums + len); len++) {
		if (bufsize == len+1) {
			bufsize *= 50;
			xrealloc(&nums, bufsize);
		}	
	}
	len = bufsize;
	fclose(f);

	int_fast32_t *nums_2 = malloc(len * sizeof(int_fast32_t));
	memcpy(nums_2, nums, len * sizeof(int_fast32_t));

	printf("%s", "first:\t"); simulate(1, nums, len);
	//printf("%s", "second:\t"); simulate(5, nums_2, len);
}
