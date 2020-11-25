#include <err.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
	if (2 != argc) errx(1, "%s", "invalid input file");
	FILE *f = fopen(argv[1], "r");
	if (NULL == f) err(1, NULL);
		
	int res1 = 0, res2 = 0;
	for (int c; EOF != fscanf(f, "%d\n", &c);) {
		res1 += c / 3 - 2;
		for (; (c = c / 3 - 2) > 0; res2 += c) {}
	}
	fclose(f);
	printf("first:\t%d\n", res1);
	printf("second:\t%d\n", res2);
}
