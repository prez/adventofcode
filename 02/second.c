#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFLEN 10000
#define VALUE 19690720

int
simulate(int in1, int in2, int *nums, int len)
{
	int arr[BUFLEN];
	memcpy(arr, nums, BUFLEN);

	arr[1] = in1;
	arr[2] = in2;
	
	for (int i = 0; i < len; i+=4) {
		switch (arr[i]) {
		case 99:
			goto end;
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
	char line[BUFLEN+1];
	FILE *fd = fopen("input", "r");
	if (NULL == fgets(line, BUFLEN, fd)) {
		return 1;
	}

	int nums[BUFLEN];
	int len = 0;
	for (char *s = strtok(line, ",\n"); NULL != s; s = strtok(NULL, ",\n"), len++) {
		nums[len] = atoi(s);
	}

	for (int i = 0; i <= 99; i++) {
		for (int j = 0; j <= 99; j++) {
			if (VALUE == simulate(i, j, nums, len)) {
				printf("%d\n", 100*i + j);
			}
		}
	}
}
