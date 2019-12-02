#include <stdio.h>

int
main(void)
{
	FILE *fp = fopen("input", "r");
	unsigned int res = 0;
	for (unsigned int c; EOF != fscanf(fp, "%u\n", &c);) {
		res += c / 3 - 2;
	}
	fclose(fp);
	printf("%u\n", res);
}
