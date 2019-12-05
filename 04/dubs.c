#include <stdio.h>
#include <stdint.h>
#include <sys/cdefs.h>

void eq2s(uint_fast8_t *x) { *x = (*x== 2); }
void ge2s(uint_fast8_t *x) { *x = (*x>= 2); }

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

/* Assuming n itself is increasing, return the next increasing number */
uint_fast64_t
next_increasing(uint_fast64_t n)
{
    if (n % 10 != 9)
        return n + 1;
    
    n = next_increasing(n / 10);
    return 10 * n + n % 10;
}

int
main(void)
{
	FILE *fp = fopen("evenbiggerboy", "r");
	uint_fast64_t a, b, cnt1 = 0, cnt2 = 0;
	fscanf(fp, "%llu-%llu\n", &a, &b);
	fclose(fp);
	
	//for (adjust(&a); a <= b; a++) {
	for (adjust(&a); a <= b; a = next_increasing(a)) {
		//if (!(a % 10)) {
		//	adjust(&a);
		//}
		if (dubs(ge2s, a)) {
			cnt1++;
			if (dubs(eq2s, a)) {
				cnt2++;
			}
		}
	}

	//printf("first:\t%llu\n", --cnt1);
	printf("first:\t%llu\n", cnt1);
	printf("second:\t%llu\n", cnt2);
}
