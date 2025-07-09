// Demo and test harness for the hash table implementation.
// Exercises create, set (including collisions and resize), get,
// iteration, and destroy. Prints PASS/FAIL for each check and
// exits non-zero if any check fails.

#include "ht.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

#define CHECK(cond, msg) do { \
	if (cond) { \
		printf("PASS: %s\n", msg); \
	} else { \
		printf("FAIL: %s\n", msg); \
		failures++; \
	} \
} while (0)

int main(void) {
	ht* table = ht_create();
	CHECK(table != NULL, "ht_create returns a non-NULL table");
	CHECK(ht_length(table) == 0, "new table has length 0");

	// Basic set/get.
	static int one = 1, two = 2, three = 3;
	ht_set(table, "one", &one);
	ht_set(table, "two", &two);
	ht_set(table, "three", &three);
	CHECK(ht_length(table) == 3, "length is 3 after three inserts");
	CHECK(ht_get(table, "one") == &one, "get returns value for 'one'");
	CHECK(ht_get(table, "two") == &two, "get returns value for 'two'");
	CHECK(ht_get(table, "missing") == NULL, "get returns NULL for missing key");

	// Overwrite an existing key; length must not change.
	static int updated_one = 100;
	ht_set(table, "one", &updated_one);
	CHECK(ht_get(table, "one") == &updated_one, "set overwrites existing key's value");
	CHECK(ht_length(table) == 3, "length unchanged after overwrite");

	// Insert enough entries to force at least one resize (initial
	// capacity is 16, expansion triggers at half full), and to force
	// collisions since capacity is small relative to key count.
	enum { N = 500 };
	static int values[N];
	char key[32];
	for (int i = 0; i < N; i++) {
		values[i] = i;
		snprintf(key, sizeof(key), "key-%d", i);
		ht_set(table, key, &values[i]);
	}
	CHECK(ht_length(table) == (size_t)(N + 3), "length correct after bulk insert (resize exercised)");

	int lookup_ok = 1;
	for (int i = 0; i < N; i++) {
		snprintf(key, sizeof(key), "key-%d", i);
		int* v = (int*)ht_get(table, key);
		if (v == NULL || *v != i) {
			lookup_ok = 0;
			break;
		}
	}
	CHECK(lookup_ok, "all bulk-inserted keys retrievable after resize");

	// Iteration should visit exactly ht_length(table) entries, each key
	// unique, and each value matching what was stored.
	hti it = ht_iterator(table);
	size_t seen = 0;
	int iter_values_ok = 1;
	while (it_next(&it)) {
		seen++;
		if (strncmp(it.key, "key-", 4) == 0) {
			int idx = atoi(it.key + 4);
			int* v = (int*)it.value;
			if (idx < 0 || idx >= N || v != &values[idx]) {
				iter_values_ok = 0;
			}
		}
	}
	CHECK(seen == ht_length(table), "iterator visits exactly length() entries");
	CHECK(iter_values_ok, "iterator returns correct key/value pairs");

	ht_destroy(table);
	printf("\n%s\n", failures == 0 ? "ALL TESTS PASSED" : "SOME TESTS FAILED");
	return failures == 0 ? 0 : 1;
}
