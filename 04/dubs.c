#include <stdio.h>
#include <stdint.h>
#include <sys/cdefs.h>

void
eq2s(uint_fast8_t *x) { *x = (*x== 2); }
void
ge2s(uint_fast8_t *x) { *x = (*x>= 2); }

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

uint_fast64_t __attribute_pure__
lt_ten_p_digits(uint_fast64_t n)
{
	uint_fast64_t ret;
	for (ret = 1; 0 != n; n /= 10, ret *= 10) {}
	return ret/10;
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
adjust_init(uint_fast64_t *n)
{
	uint_fast64_t div, sum = 0;
	uint_fast8_t d, l_d = 0, found = 0;
	
	for (div = lt_ten_p_digits(*n); div; div /= 10) {
		if (!found) {
			d = (*n / div) % 10;
		}
		if (d < l_d) {
			d = l_d;
			found = 1;
		}
		sum += d * div;
		l_d = d;
	}
	*n = sum;
}

int
main(void)
{
	FILE *fp = fopen("evenbiggerboy", "r");
	uint_fast64_t a, b, cnt1 = 0, cnt2 = 0;
	fscanf(fp, "%llu-%llu\n", &a, &b);
	fclose(fp);
	
	for (adjust_init(&a); a <= b; a++) {
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

	printf("first: %llu\n", --cnt1);
	printf("second: %llu\n", cnt2);
}
