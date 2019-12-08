#include <sys/cdefs.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_table.h"

uint_fast32_t primes[16] = { 97,      2143,    12781,   73043,
	                         153191,  270961,  350699,  397027,
	                         440683,  506101,  665617,  855311,
	                         1250437, 1576177, 1921133, 399999949 };

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
