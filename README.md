# C Hash Table

A small hash table implementation in C, built by following Ben Hoyt's tutorial [Build Your Own Hash Table in C](https://benhoyt.com/writings/hash-table-in-c/). This project exists to understand how hash tables actually work under the hood: hashing, open addressing, collision resolution, and manual memory management.

This is a derivative and learning-based reimplementation of Ben Hoyt's work. All credit for the original design belongs to him. Ben released his tutorial code into the public domain, and this adaptation is shared in that same spirit.

## Features

- Open addressing with linear probing for collision resolution.
- Automatic resize when the table is more than half full.
- String keys (copied via `strdup`) and `void*` values, so it can store anything.
- An iterator for walking every key-value pair.
- FNV-1a hashing for a good, fast distribution.

## File structure

| File        | Description |
|-------------|-------------|
| `ht.c`      | Implementation of the hash table (`ht`) and its iterator (`hti`) |
| `ht.h`      | Public interface for using the hash table |
| `main.c`    | Test/demo program exercising create, set, get, resize, iteration, and destroy |
| `Makefile`  | Builds and runs the test program |
| `README.md` | This file |

## Building and running

Requires a C compiler (`cc`/`gcc`/`clang`) and `make`.

```sh
make
```

This compiles `ht.c` and `main.c` into a `test` binary and runs it. Each check prints `PASS` or `FAIL`, and the program exits non-zero if anything fails. Example output:

```
PASS: ht_create returns a non-NULL table
PASS: length is 3 after three inserts
PASS: get returns value for 'one'
...
ALL TESTS PASSED
```

To clean up the build artifact:

```sh
make clean
```

The test suite has also been checked with AddressSanitizer and UndefinedBehaviorSanitizer (`-fsanitize=address,undefined`) and comes back clean, no leaks, no use-after-free, no undefined behavior.

## How it works

- `ht_create` allocates and initializes an empty hash table.
- `ht_set` inserts a key-value pair, expanding the table first if it's more than half full.
- `ht_get` looks up a key and returns the corresponding value, or `NULL` if it isn't present.
- `ht_destroy` frees all allocated memory, including the copied keys.
- `ht_iterator` / `it_next` walk through every stored item.

Internally the table is a flat array of `ht_entry` structs, each holding a key and a value. Inserting a key hashes it with FNV-1a to pick a starting slot, then probes forward linearly until it finds a matching key or an empty slot.

## Example usage

```c
ht* table = ht_create();
ht_set(table, "name", "Mateo");
ht_set(table, "language", "C");

printf("%s\n", (char*)ht_get(table, "name"));  // Output: Mateo

ht_destroy(table);
```

To iterate:

```c
hti it = ht_iterator(table);
while (it_next(&it)) {
    printf("%s => %s\n", it.key, (char*)it.value);
}
```

## Credit

This project is a derivative and learning-based reimplementation of:

Ben Hoyt, [Build Your Own Hash Table in C](https://benhoyt.com/writings/hash-table-in-c/)

Ben released his original work into the public domain. This adaptation is shared in that same spirit, to learn from and to help others learn low-level programming.

## License

Released under the MIT License, with credit to Ben Hoyt for the original tutorial and design.
