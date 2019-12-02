#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFLEN 10000

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

	nums[1] = 12;
	nums[2] = 2;
	
	for (int i = 0; i < len; i+=4) {
		switch (nums[i]) {
		case 99:
			goto end;
		case 1:
			nums[nums[i+3]] = nums[nums[i+1]] + nums[nums[i+2]];
			break;
		case 2:
			nums[nums[i+3]] = nums[nums[i+1]] * nums[nums[i+2]];
			break;
		}
	}

end:
	printf("%d\n", nums[0]);
}
