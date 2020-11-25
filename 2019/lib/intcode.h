#ifndef _INTCODE_H_
#define _INTCODE_H_

#include <stdint.h>
#include <stdarg.h>

#define USE_HASHMAP 0
#if USE_HASHMAP
#include "hash_map.h"
#endif

struct vm_state {
	int_fast64_t *band;
	size_t band_len;
	size_t rip;
	int_fast64_t base;
	uint_fast8_t crr_mode[4];
	struct q_instance *feed;
	
};	

void vm_simulate(struct vm_state *vm);
void vm_destroy(struct vm_state *vm);
struct vm_state *vm_init(struct vm_state *vm, char *mode, ...);

#endif /* _INTCODE_H_ */
