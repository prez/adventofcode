#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <err.h>
#include <sys/cdefs.h>

#define BUFLEN 2000000

int_fast64_t __attribute_pure__
fetch_arg(int_fast64_t *arr, size_t rip, uint_fast8_t *param_mode, size_t param_nr, size_t base)
{
	switch (param_mode[param_nr]) {
		case 0:
			return arr[arr[rip+param_nr]]; 
		case 1:
			return arr[rip+param_nr];
		case 2:
			return arr[base+arr[rip+param_nr]];
		default:
			errx(1, "%s\n", "invalid mode");
	}
}

void
xrealloc(int_fast64_t **arr, size_t new_sz)
{
	int_fast64_t *arr_new = realloc(*arr, new_sz * sizeof(int_fast64_t));
	if (NULL == arr_new) {
		err(1, NULL);
	}
	*arr = arr_new;
}

//write_at_addr(arr, rip+1, len, in, 0)
void
write_at_addr(int_fast64_t *arr, size_t pos, size_t len, int_fast64_t val, int_fast64_t param_mode_base)
{
/*
	if ((*arr)[pos] >= *len) {

		printf("got request to store %ld in %lu, but array is %lu size. reallocating\n",
			       val, (*arr)[pos], *len);

		xrealloc(arr, (*arr)[pos]*+1);
		*len = (*arr)[pos]+1;
	}
	(*arr)[*len-1] = val;
	printf("succ\n");
*/	
	if (arr[pos] >= len) {
		//printf("got request beyond band size\n");
	}
	switch (param_mode_base) {
	case 0:
		arr[arr[pos]] = val;
		break;
	default:
		arr[param_mode_base + arr[pos]] = val;
	}
}

void
simulate(int_fast64_t in, int_fast64_t *arr, size_t len)
{
	int_fast64_t base = 0;
	for (size_t rip = 0; rip < len; ) {
		if (99 == arr[rip]) return;

		size_t opcode = arr[rip] % 100;
		uint_fast8_t param_mode[4] = { 2,                  (arr[rip]/100)%10,
		                               (arr[rip]/1000)%10, (arr[rip]/10000)%10 };

		
		/*
		printf("rip: %lu, \top: %lu,\tn: %ld,\ti: %ld,\tarr[100]: %ld,\t[rip+1]: %ld,\t[rip+2]: %ld,\t[rip+3]: %ld\n",
		       rip, opcode, arr[0], arr[1], arr[100], arr[rip+1], arr[rip+2], arr[rip+3]);
*/
		switch (opcode) {
		case 1:
			//printf("rip: %lu, \top: %lu,\tn: %ld,\ti: %ld,\tarr[100]: %ld,\t[rip+1]: %ld,\t[rip+2]: %ld,\t[rip+3]: %ld\n",
			//		       rip, opcode, arr[0], arr[1], arr[100], arr[rip+1], arr[rip+2], arr[rip+3]);
			switch (param_mode[3]) {
			case 0:
				write_at_addr(arr, rip+3, len, (fetch_arg(arr, rip, param_mode, 1, base) +
							                                fetch_arg(arr, rip, param_mode, 2, base)), 0);
				break;
			case 2:
				write_at_addr(arr, rip+3, len, (fetch_arg(arr, rip, param_mode, 1, base) +
			                                fetch_arg(arr, rip, param_mode, 2, base)), base);				break;
			}
			rip += 4;
			break;
		case 2:
			//printf("rip: %lu, \top: %lu,\tn: %ld,\ti: %ld,\tarr[100]: %ld,\t[rip+1]: %ld,\t[rip+2]: %ld,\t[rip+3]: %ld\n",
			//				       rip, opcode, arr[0], arr[1], arr[100], arr[rip+1], arr[rip+2], arr[rip+3]);
			switch (param_mode[3]) {
			case 0:
				write_at_addr(arr, rip+3, len, (fetch_arg(arr, rip, param_mode, 1, base) *
							                                fetch_arg(arr, rip, param_mode, 2, base)), 0);
				break;
			case 2:
				write_at_addr(arr, rip+3, len, (fetch_arg(arr, rip, param_mode, 1, base) *
			                                fetch_arg(arr, rip, param_mode, 2, base)), base);				break;
			}
			rip += 4;
			break;
		case 3:
			//printf("rip: %lu, \top: %lu,\tn: %ld,\ti: %ld,\tarr[100]: %ld,\t[rip+1]: %ld,\t[rip+2]: %ld,\t[rip+3]: %ld\n",
			//					       rip, opcode, arr[0], arr[1], arr[100], arr[rip+1], arr[rip+2], arr[rip+3]);
			switch (param_mode[1]) {
			case 0:
				write_at_addr(arr, rip+1, len, in, 0);
				break;
			case 2:
				write_at_addr(arr, rip+1, len, in, base);
				break;
			}
			rip += 2;
			break;
		case 4:
			//printf("rip: %lu, \top: %lu,\tn: %ld,\ti: %ld,\tarr[100]: %ld,\t[rip+1]: %ld,\t[rip+2]: %ld,\t[rip+3]: %ld\n",
			//					       rip, opcode, arr[0], arr[1], arr[100], arr[rip+1], arr[rip+2], arr[rip+3]);
			//if (fetch_arg(arr, rip, param_mode, 1, base)) {
				printf("%ld\n", fetch_arg(arr, rip, param_mode, 1, base));
			//}
			rip += 2;
			break;
		case 5:
			//printf("rip: %lu, \top: %lu,\tn: %ld,\ti: %ld,\tarr[100]: %ld,\t[rip+1]: %ld,\t[rip+2]: %ld,\t[rip+3]: %ld\n",
			//					       rip, opcode, arr[0], arr[1], arr[100], arr[rip+1], arr[rip+2], arr[rip+3]);
			//printf("using jump if true\n");
			if (fetch_arg(arr, rip, param_mode, 1, base)) {
				rip = fetch_arg(arr, rip, param_mode, 2, base);
			}
			else rip += 3;
			break;
		case 6:
			//printf("rip: %lu, \top: %lu,\tn: %ld,\ti: %ld,\tarr[100]: %ld,\t[rip+1]: %ld,\t[rip+2]: %ld,\t[rip+3]: %ld\n",
			//					       rip, opcode, arr[0], arr[1], arr[100], arr[rip+1], arr[rip+2], arr[rip+3]);
			//printf("using jump if false\n");
			if (!fetch_arg(arr, rip, param_mode, 1, base)) {
				rip = fetch_arg(arr, rip, param_mode, 2, base);
			}
			else rip += 3;
			break;
		case 7:
			//printf("rip: %lu, \top: %lu,\tn: %ld,\ti: %ld,\tarr[100]: %ld,\t[rip+1]: %ld,\t[rip+2]: %ld,\t[rip+3]: %ld\n",
			//					       rip, opcode, arr[0], arr[1], arr[100], arr[rip+1], arr[rip+2], arr[rip+3]);
			//printf("using op 7 to write %u\n", (fetch_arg(arr, rip, param_mode, 1, base) <
			//		                                    fetch_arg(arr, rip, param_mode, 2, base)));
			switch (param_mode[3]) {
			case 0:
				write_at_addr(arr, rip+3, len, (fetch_arg(arr, rip, param_mode, 1, base) <
							                                		                                    fetch_arg(arr, rip, param_mode, 2, base)), 0);
				break;
			case 2:
				write_at_addr(arr, rip+3, len, (fetch_arg(arr, rip, param_mode, 1, base) <
						                                    fetch_arg(arr, rip, param_mode, 2, base)), base);
			break;
			}
			rip += 4;
			break;
		case 8:
			//printf("rip: %lu, \top: %lu,\tn: %ld,\ti: %ld,\tarr[101]: %ld,\t[rip+1]: %ld,\t[rip+2]: %ld,\t[rip+3]: %ld\n",
			//					       rip, opcode, arr[0], arr[1], arr[101], arr[rip+1], arr[rip+2], arr[rip+3]);
			//printf("using op 8 to write %u\n", (fetch_arg(arr, rip, param_mode, 1, base) ==
			//			                                fetch_arg(arr, rip, param_mode, 2, base)));
			switch (param_mode[3]) {
			case 0:
				write_at_addr(arr, rip+3, len, (fetch_arg(arr, rip, param_mode, 1, base) ==
							                                		                                    fetch_arg(arr, rip, param_mode, 2, base)), 0);
				break;
			case 2:
				write_at_addr(arr, rip+3, len, (fetch_arg(arr, rip, param_mode, 1, base) ==
						                                    fetch_arg(arr, rip, param_mode, 2, base)), base);
			break;
			}
			rip += 4;
			break;
		case 9:
			//printf("rip: %lu, \top: %lu,\tn: %ld,\ti: %ld,\tarr[100]: %ld,\t[rip+1]: %ld,\t[rip+2]: %ld,\t[rip+3]: %ld\n",
			//					       rip, opcode, arr[0], arr[1], arr[100], arr[rip+1], arr[rip+2], arr[rip+3]);
			//	printf("adjusting relative base\n");
			base += fetch_arg(arr, rip, param_mode, 1, base);
			rip += 2;
			break;
		default:
			errx(1, "%s %lu\n", "invalid opcode", opcode);
		}
	}
}

int
main(int argc, char **argv)
{
	if (2 != argc) errx(1, "%s", "invalid input file");
	FILE *f = fopen(argv[1], "r");
	if (NULL == f) err(1, NULL);

	int_fast64_t *nums = calloc(BUFLEN, sizeof(int_fast64_t));
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

	int_fast64_t *nums_2 = calloc(len, sizeof(int_fast64_t));
	memcpy(nums_2, nums, len * sizeof(int_fast64_t));

	printf("%s", "first:\t"); simulate(1, nums, len);
	printf("%s", "second:\t"); simulate(2, nums_2, len);
}
