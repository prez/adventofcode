#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <stdint.h>

#define HT_LOAD_FACTOR 50

struct ht_entry {
	char *key;
	char *val;
};

struct ht_instance {
	uint_fast32_t size;
	uint_fast32_t count;
	struct ht_entry **items;
};

struct ht_instance *ht_new(void);
void ht_del_instance(struct ht_instance *ht);
uint_fast8_t ht_insert(struct ht_instance *ht, char *key, char *val);
char *ht_query (struct ht_instance *ht, char *key);

#endif /* _HASH_TABLE_H_ */
