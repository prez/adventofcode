#include "hash_table.h"

#include <sys/cdefs.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

uint_fast32_t primes[16] = { 97,      2143,    12781,   73043,
	                         153191,  270961,  350699,  397027,
	                         440683,  506101,  665617,  855311,
	                         1250437, 1576177, 1921133, 399999949 };

static struct ht_entry *
ht_new_entry(uint_fast32_t key1, uint_fast32_t key2, uint_fast32_t val);
static void
ht_del_entry(struct ht_entry *e);
static inline uint_fast32_t
ht_hash(uint_fast32_t key1, uint_fast32_t key2, uint_fast32_t bucket_size,
        uint_fast32_t p);
static uint_fast32_t
ht_calc_hash(uint_fast32_t key1, uint_fast32_t key2, uint_fast32_t bucket_size,
             uint_fast32_t attempt);
static void
ht_resize(struct ht_instance *ht);

static struct ht_entry *
ht_new_entry(uint_fast32_t key1, uint_fast32_t key2, uint_fast32_t val)
{
	struct ht_entry *e = malloc(sizeof(struct ht_entry));
	if (NULL == e) {
		err(1, NULL);
	}
	e->x = key1;
	e->y = key2;
	e->cnt = val;
	return e;
}

static void
ht_del_entry(struct ht_entry *e)
{
	free(e);
}

static inline uint_fast32_t __attribute_pure__
ht_hash(uint_fast32_t key1, uint_fast32_t key2, uint_fast32_t bucket_size,
        uint_fast32_t p)
{
	uint_fast32_t h = key1 * p + key2;
	h %= bucket_size;
	return (uint_fast32_t)h;
}

static uint_fast32_t __attribute_pure__
ht_calc_hash(uint_fast32_t key1, uint_fast32_t key2, uint_fast32_t bucket_size,
             uint_fast32_t attempt)
{
	uint_fast32_t hash_1 = ht_hash(key1, key2, bucket_size, HT_OFFS_1);
	uint_fast32_t hash_2 = ht_hash(key1, key2, bucket_size, HT_OFFS_2);
	if (0 == hash_2 % bucket_size) {
		hash_2 = 1;
	}
	return (hash_1 + attempt * (hash_2)) % bucket_size;
}

struct ht_instance *
ht_new(void)
{
	static uint8_t sz = 0;
	struct ht_instance *ht = malloc(sizeof(struct ht_instance));
	if (NULL == ht) {
		err(1, NULL);
	}
	ht->size = primes[sz++];
	ht->count = 0;
	ht->items = calloc(ht->size, sizeof(struct ht_entry *));
	if (NULL == ht->items) {
		err(1, NULL);
	}
	return ht;
}

static void
ht_resize(struct ht_instance *ht)
{
	struct ht_instance *ht_next = ht_new();
	for (uint_fast32_t i = 0; i < ht->size; i++) {
		struct ht_entry *e = ht->items[i];
		if (NULL != e) {
			ht_insert(ht_next, e->x, e->y, e->cnt);
		}
	}

	ht->count = ht_next->count;
	uint_fast32_t tmp_size = ht->size;
	ht->size = ht_next->size;
	ht_next->size = tmp_size;
	struct ht_entry **tmp_items = ht->items;
	ht->items = ht_next->items;
	ht_next->items = tmp_items;
	ht_del_instance(ht_next);
}

void
ht_del_instance(struct ht_instance *ht)
{
	for (uint_fast32_t i = 0; i < ht->size; i++) {
		struct ht_entry *e = ht->items[i];
		if (NULL != e) {
			ht_del_entry(e);
		}
	}
	free(ht->items);
	free(ht);
}

void
ht_insert(struct ht_instance *ht, uint_fast32_t key1, uint_fast32_t key2,
          uint_fast32_t val)
{
	if ((ht->count * 100 / ht->size) > 70) {
		ht_resize(ht);
	}

	struct ht_entry *new = ht_new_entry(key1, key2, val);

	uint_fast32_t ind, atmpt = 0;
	for (struct ht_entry *taken; !atmpt || (NULL != taken); atmpt++) {
		if (atmpt && (key1 == taken->x) && (key2 == taken->y)) {
			ht_del_entry(taken);
			ht->items[ind] = new;
			return;
		}
		ind = ht_calc_hash(key1, key2, ht->size, atmpt);
		taken = ht->items[ind];
	}

	ht->items[ind] = new;
	ht->count++;
}

/* returns val for this key, or 0 if not present */
uint_fast32_t
ht_query(struct ht_instance *ht, uint_fast32_t key1, uint_fast32_t key2)
{
	uint_fast32_t ind, atmpt = 0;
	for (struct ht_entry *taken; !atmpt || (NULL != taken); atmpt++) {
		if (atmpt && (key1 == taken->x) && (key2 == taken->y)) {
			return taken->cnt;
		}
		ind = ht_calc_hash(key1, key2, ht->size, atmpt);
		taken = ht->items[ind];
	}

	return 0;
}
