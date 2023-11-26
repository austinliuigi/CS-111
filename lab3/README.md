# Hash Hash Hash

In this lab, we make a thread-safe hash table.

## Building
```shell
make
```

## Running
```shell
./hash-table-tester [-t NTHREADS] [-s SIZE]
```
- `NTHREADS` - number of threads to create
- `SIZE` - number of hash table entries

## First Implementation
I added a mutex in `struct hash_table_v1`, resulting in one mutex per hash table.
This makes it so that a user can create multiple hash tables, each with their own lock.

Upon creation of a hash table (via `hash_table_v1_create`), the mutex for
the created hash table gets initialized.

Upon destruction of a hash table (via `hash_table_v1_destroy`), the mutex for
the destroyed hash table gets destroyed.

In the `hash_table_v1_add_entry` function, I lock the mutex for the hash table,
disallowing other threads to add any entries to this hash table until the
current thread finishes adding its entry.

### Performance
```shell
$ ./hash-table-tester -t 4 -s 75000
Generation: 60,687 usec
Hash table base: 1,327,761 usec
  - 0 missing
Hash table v1: 1,569,944 usec
  - 0 missing
```
Version 1 is a little slower than the base version. As it locks the hash table
each time a thread needs to add an entry, causing extra context switching
overhead with no real gain in performance.

## Second Implementation
I added a mutex in `struct hash_table_entry`, resulting in `HASH_TABLE_CAPACITY`
mutexes per hash table.

Upon creation of a hash table, the mutex for each entry is initialized.

Upon destruction of a hash table, the mutex for each entry is destroyed.

In the `hash_table_v2_add_entry` function, I lock the mutex for the head of the
linked list where the entry should be located. Unlike v1, this only blocks other
threads trying to add an entry to the same linked list rather than blocking all
other threads trying to add entries to this hash table (regardless of whether
or not it's in the same linked list).

### Performance
```shell
$ ./hash-table-tester -t 4 -s 75000
Generation: 60,687 usec
Hash table base: 1,327,761 usec
  - 0 missing
Hash table v1: 1,569,944 usec
  - 0 missing
Hash table v2: 496,620 usec
  - 0 missing
```

The v2 hash table implementation is around 2.5 times faster than the base
implementation (running on 4 cores). This is because v2 takes advantage of
the extra threads, allowing them to run in parallel on multiple cores as
long as they aren't adding to the same entry in the hash table.

## Cleaning up
```shell
make clean
```
