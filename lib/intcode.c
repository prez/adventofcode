#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include <sys/cdefs.h>
#include "intcode.h"

static void vm_write(struct vm_state *vm, size_t param_nr, int_fast64_t val);
static int_fast64_t vm_fetch(struct vm_state *vm, size_t param_nr);
static void vm_realloc(struct vm_state *vm, size_t access_pos);

static int_fast64_t
vm_fetch(struct vm_state *vm, size_t param_nr)
{
	switch (vm->crr_mode[param_nr]) {
		case 0:
			return vm->band[vm->band[vm->rip+param_nr]]; 
		case 1:
			return vm->band[vm->rip+param_nr];
		case 2:
			return vm->band[vm->base+vm->band[vm->rip+param_nr]];
		default:
			errx(1, "%s\n", "invalid mode");
	}
}

static void
vm_write(struct vm_state *vm, size_t param_nr, int_fast64_t val)
{
	vm_realloc(vm, vm->rip+param_nr);
	size_t pos;
	switch (vm->crr_mode[param_nr]) {
		case 0:
			pos = vm->band[vm->rip+param_nr]; 
			break;
		case 1:
			pos = vm->rip+param_nr;
			break;
		case 2:
			pos = vm->base+vm->band[vm->rip+param_nr];
			break;
		default:
			errx(1, "%s\n", "invalid mode");
	}
	vm_realloc(vm, pos);
	vm->band[pos] = val;
}

static void
vm_realloc(struct vm_state *vm, size_t access_pos)
{
	if (access_pos >= vm->band_len) {
		access_pos += 10;
		int_fast64_t *band_new = realloc(vm->band, access_pos * sizeof(int_fast64_t));
		if (NULL == band_new) err(1, NULL);
		vm->band = band_new;
		for (size_t i = vm->band_len; i < access_pos; i++) { 
			printf("debug: realloc request due to out-of-band access:\n");
			printf("overwriting %lld with 0\n", vm->band[i]);
			vm->band[i] = 0;
		} 
	}
}

struct vm_state *
vm_init(struct vm_state *vm, char *mode, ...)
{
	va_list ap;
	int xflg = 0; /* modify the band by default */

	if (NULL == vm) {
		if (NULL == strchr(mode, 'a')) {
			errx(1, "%s\n", "error: cant create a new vm without band");
		}
		vm = malloc(sizeof(struct vm_state));
	}

	va_start(ap, mode);
	for (; *mode; ) {
		switch (*mode++) {
		case 'x': /* dont modify band */
			xflg = 1;
			break;
		case 'a': /* band */
			vm->band = va_arg(ap, int_fast64_t *);
			break;
		case 'l': /* len */
			vm->band_len = va_arg(ap, size_t);
			break;
		case 'r': /* rip */
			vm->rip = va_arg(ap, size_t);
			break;
		case 'b': /* base */
			vm->base = va_arg(ap, int_fast64_t);
			break;
		case 'c': /* crr_mode */
			vm->crr_mode = va_arg(ap, int_fast8_t *);
			break;
		case 'f': /* feed */
			vm->feed = va_arg(ap, int_fast64_t);
			break;
		}
	}	
	va_end(ap);

	if (NULL == vm->band) {
		errx(1, "%s\n", "FUCK: NULL-band after init / before memcpy");
	}

	if (xflg) {
		int_fast64_t *new_band = malloc(vm->band_len * sizeof(int_fast64_t));
		if (NULL == new_band) err(1, NULL);
		memcpy(new_band, vm->band, vm->band_len * sizeof(int_fast64_t));
		vm->band = new_band;
	}

	return vm;
}

void vm_destroy(struct vm_state *vm)
{
	free(vm->band);
	free(vm);
}

void
vm_simulate(struct vm_state *vm)
{
	for (uint_fast8_t opcode; vm->rip < vm->band_len; ) {
		opcode = vm->band[vm->rip] % 100;
		for (int_fast64_t i = 1; i < 4; i++) {
			vm->crr_mode[i] = (vm->band[vm->rip] / (10 * (int_fast64_t)pow(10, i))) % 10;
		}		
		/*
		printf("rip: %lu, \top: %lu,\tn: %ld,\ti: %ld,\tarr[100]: %ld,\t[rip+1]: %ld,\t[rip+2]: %ld,\t[rip+3]: %ld\n",
		       vm->rip, opcode, vm->band[0], vm->band[1], vm->band[100], vm->band[vm->rip+1], vm->band[vm->rip+2], vm->band[vm->rip+3]);
		*/		
		switch (opcode) {
		case 99:
			/* cleanup routine */
			return;
		case 1:
			vm_write(vm, 3, vm_fetch(vm, 1) + vm_fetch(vm, 2));
			vm->rip += 4;
			break;
		case 2:
			vm_write(vm, 3, vm_fetch(vm, 1) * vm_fetch(vm, 2));
			vm->rip += 4;
			break;
		case 3:
			vm_write(vm, 1, vm->feed);
			vm->rip += 2;
			break;
		case 4:
	//		if (vm_fetch(vm, 1)) {
				printf("%ld\n", vm_fetch(vm, 1));
	//		}
			vm->rip += 2;
			break;
		case 5:
			if (vm_fetch(vm, 1)) vm->rip = vm_fetch(vm, 2);
			else vm->rip += 3;
			break;
		case 6:
			if (!vm_fetch(vm, 1)) vm->rip = vm_fetch(vm, 2);
			else vm->rip += 3;
			break;
		case 7:
			vm_write(vm, 3, vm_fetch(vm, 1) < vm_fetch(vm, 2));
			vm->rip += 4;
			break;
		case 8:
			vm_write(vm, 3, vm_fetch(vm, 1) == vm_fetch(vm, 2));
			vm->rip += 4;
			break;
		case 9:
			vm->base += vm_fetch(vm, 1);
			vm->rip += 2;
			break;
		default:
			errx(1, "%s %u\n", "invalid opcode", opcode);
		}
	}
}
