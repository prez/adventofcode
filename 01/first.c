#include <stdio.h>

int
main(void)
{
	FILE *fp = fopen("input", "r");
	int res = 0;
	for (int c = 0; EOF != fscanf(fp, "%d\n", &c); ) {
		res += c/3-2;
	}
	fclose(fp);
	printf("%d\n", res);
}
