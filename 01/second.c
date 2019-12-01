#include <stdio.h>

int
main(void)
{
	FILE *fp = fopen("input", "r");
	int res = 0;
	for (int c = 0; EOF != fscanf(fp, "%d\n", &c); ) {
		for (; (c = c/3-2) > 0; res += c) {}
	}
	fclose(fp);
	printf("%d\n", res);
}
