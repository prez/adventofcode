#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/cdefs.h>

static inline void eq2s(uint_fast8_t *x) { *x = (*x== 2); }
static inline void ge2s(uint_fast8_t *x) { *x = (*x>= 2); }

uint_fast8_t __attribute_pure__
dubs(void (*f)(uint_fast8_t *), uint_fast64_t n)
{
	uint_fast8_t ret = 0;

	uint_fast8_t nums[10] = { 0 };
	for (uint_fast64_t div = 1; div < n; div *= 10) {
		nums[(n / div) % 10]++;
	}
	for (int i = 1; i < 10; i++) {
		(*f)(&nums[i]);
		ret = ret || (uint_fast8_t)nums[i];
	}

	return ret;
}

void
adjust(uint_fast64_t *n)
{
	uint_fast64_t div = 10;
	for (; !(*n % div); div *= 10) {}
	uint_fast64_t l_d = (*n % div) * 10 / div;
	for (div /= 100; div; div /= 10) {
		*n += l_d * div;
	}
}

void
pre_adjust(uint_fast64_t *n)
{
	uint_fast64_t c_d, div = 10, sum = 0;
	for (; *n / (div * 10); div *= 10) {}
	c_d = (*n / div) % 10;
	for (sum += c_d * div; (div /= 10); sum += c_d * div) {
		if ((*n / div) % 10 < c_d) {
			*n = sum;
			return;
		}
		c_d = (*n / div) % 10;
	}
}

int
main(int argc, char **argv)
{
	if (2 != argc) errx(1, "%s", "invalid input file");
	FILE *f = fopen(argv[1], "r");
	if (NULL == f) err(1, NULL);

	uint_fast64_t a, b, cnt1 = 0, cnt2 = 0;
	fscanf(f, "%llu-%llu\n", &a, &b);
	fclose(f);

	pre_adjust(&a);
	for (adjust(&a); a <= b; a++) {
		if (!(a % 10)) {
			adjust(&a);
		}
		if (dubs(ge2s, a)) {
			cnt1++;
			if (dubs(eq2s, a)) {
				cnt2++;
			}
		}
	}

	printf("first:\t%llu\n", --cnt1);
	printf("second:\t%llu\n", cnt2);
}
