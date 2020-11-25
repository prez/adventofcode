#include <sys/cdefs.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_table.h"

/* for i in $(seq 1 32); do primes "$(echo "97*2^${i}"| bc)" | head -n 1; don */
uint_fast32_t primes[32] = { 197,           389,            787,            1553,
	                         3109,          6211,           12421,          24841,
	                         49667,         99347,          198659,         397337,
	                         794641,        1589249,        3178499,        6357011,
	                         12713989,      25427981,       50855947,       101711873,
	                         203423749,     406847521,      813695039,      1627389971,
	                         3254779921,    6509559827,     13019119649,    26038239263,
	                         52076478479,   104152956931,   208305913873,   416611827761 };

static struct ht_entry *ht_new_entry(char *k, char *v);
static void ht_del_entry(struct ht_entry *e);
static inline unsigned long int djb2(char *str);
static uint_fast32_t ht_hash(char *key, uint_fast32_t bucket_size, uint_fast32_t attempt);
static void ht_resize(struct ht_instance *ht);
static struct ht_instance *ht_new_sized(uint8_t p_sz_ind);

static struct ht_entry *
ht_new_entry(char *k, char *v)
{
	struct ht_entry *e = malloc(sizeof(struct ht_entry));
	if (NULL == e) {
		err(1, NULL);
	}
	e->key = strdup(k);
	e->val = strdup(v);
	return e;
}

static void
ht_del_entry(struct ht_entry *e)
{
	free(e->key);
	free(e->val);
	free(e);
}

static inline unsigned long int __attribute_pure__
djb2(char *str)
{
	unsigned long int hash = 5381;
	for (int c; (c = *str++); ) {
	    hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

static uint_fast32_t
ht_hash(char *key, uint_fast32_t bucket_size, uint_fast32_t attempt)
{
	unsigned long int h = djb2(key);
	return (h + attempt * attempt) % bucket_size;
}

static void
ht_resize(struct ht_instance *ht)
{
	static uint8_t sz = 0;
	struct ht_instance *ht_next = ht_new_sized(++sz);
	for (uint_fast32_t i = 0; i < ht->size; i++) {
		struct ht_entry *e = ht->items[i];
		if (NULL != e) {
			ht_insert(ht_next, e->key, e->val);
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

static struct ht_instance *
ht_new_sized(uint8_t p_sz_ind)
{
	struct ht_instance *ht = malloc(sizeof(struct ht_instance));
	if (NULL == ht) {
		err(1, NULL);
	}
	ht->size = primes[p_sz_ind];
	ht->count = 0;
	ht->items = calloc(ht->size, sizeof(struct ht_entry *));
	if (NULL == ht->items) {
		err(1, NULL);
	}
	return ht;
}

struct ht_instance *
ht_new(void)
{
	return ht_new_sized(0);
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

uint_fast8_t
ht_insert(struct ht_instance *ht, char *key, char *val)
{
	if ((ht->count * 100 / ht->size) > HT_LOAD_FACTOR) {
		ht_resize(ht);
	}

	struct ht_entry *new = ht_new_entry(key, val);

	uint_fast32_t ind, atmpt = 0;
	for (struct ht_entry *taken; !atmpt || (NULL != taken); atmpt++) {
		if (atmpt && (0 == strcmp(key, taken->key))) {
			ht_del_entry(taken);
			ht->items[ind] = new;
			return 1;
		}
		ind = ht_hash(key, ht->size, atmpt);
		taken = ht->items[ind];
	}

	ht->items[ind] = new;
	ht->count++;
	return 0;
}

char *
ht_query(struct ht_instance *ht, char *key)
{
	uint_fast32_t ind, atmpt = 0;
	for (struct ht_entry *taken; !atmpt || (NULL != taken); atmpt++) {
		if (atmpt && (0 == strcmp(key, taken->key))) {
			return taken->val;
		}
		ind = ht_hash(key, ht->size, atmpt);
		taken = ht->items[ind];
	}
	return NULL;
}
