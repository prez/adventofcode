#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <stdint.h>

#define HT_OFFS_1 1000
#define HT_OFFS_2 500

struct ht_entry {
	uint_fast32_t x;
	uint_fast32_t y;
	uint_fast32_t cnt;
};

struct ht_instance {
	uint_fast32_t size;
	uint_fast32_t count;
	struct ht_entry **items;
};

void ht_insert(struct ht_instance *ht, uint_fast32_t key1, uint_fast32_t key2, uint_fast32_t val);
uint_fast32_t ht_query(struct ht_instance *ht, uint_fast32_t key1, uint_fast32_t key2);
struct ht_instance *ht_new(void);
void ht_del_instance(struct ht_instance *ht);

extern uint_fast32_t primes[16];

#endif /* _HASH_TABLE_H_ */
