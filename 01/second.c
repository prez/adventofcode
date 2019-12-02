#include <stdio.h>

int
main(void)
{
	FILE *fp = fopen("input", "r");
	unsigned int res = 0;
	for (unsigned int c; EOF != fscanf(fp, "%u\n", &c);) {
		for (; (c = c / 3 - 2) > 0; res += c) {}
	}
	fclose(fp);
	printf("%u\n", res);
}
