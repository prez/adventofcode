#include <stdio.h>

int
main(void)
{
	FILE *fp = fopen("input", "r");
	int res = 0, other = 0;
	for (int c; EOF != fscanf(fp, "%d\n", &c);) {
		res += c / 3 - 2;
		for (; (c = c / 3 - 2) > 0; other += c) {}
	}
	fclose(fp);
	printf("first:\t%d\n", res);
	printf("second:\t%d\n", other);
}
