#include <stdlib.h>

#define HT_PRIME_1 151
#define HT_PRIME_2 163
#define HT_INITIAL_BASE_SIZE 53

// We use a typedef to define a struct that represents a key-value pair.
// This struct is used to store the key-value pairs in the hash table.
typedef struct {
    char *key;
    char *value;
} hash_table_item;

typedef struct {
    int size;       // The current number of items in the table
    int base_size;  // The initial size of the table
    int count;      // The total number of items that can be stored in the table
    hash_table_item **items;
} hash_table;

// Function declarations
void hash_table_insert(hash_table *table, const char *key, const char *value);
char *hash_table_search(hash_table *table, const char *key);
void hash_table_delete(hash_table *table);
static void hash_table_resize(hash_table *table, const int base_size);
static void hash_table_resize_up(hash_table *table);
static void hash_table_resize_down(hash_table *table);

void *xmalloc(size_t size);
void *xcalloc(size_t count, size_t size);