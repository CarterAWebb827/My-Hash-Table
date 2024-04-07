#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hash_table.h"
#include "prime.h"

static hash_table_item HT_DELETED_ITEM = {NULL, NULL}; // Initialize the deleted item

// Create a new hash table item
static hash_table_item *hash_table_item_new(const char *key, const char *value) {
    hash_table_item *item = malloc(sizeof(hash_table_item)); // Allocate memory for the item
    item->key = strdup(key); // Copy the key
    item->value = strdup(value); // Copy the value
    return item;
}

static hash_table *hash_table_new_size(const int base_size) {
    hash_table *table = xmalloc(sizeof(hash_table)); // Allocate memory for the table
    table->base_size = base_size; // Initialize the base size

    table->size = next_prime(table->base_size); // Initialize the size

    table->count = 0; // Initialize the count
    table->items = xcalloc((size_t)table->size, sizeof(hash_table_item *)); // Allocate memory for the items

    return table;
}

// Delete a hash table
hash_table *hash_table_new() {
    return hash_table_new_size(HT_INITIAL_BASE_SIZE); // Create a new hash table
}

// Delete a hash table item
void hash_table_item_delete(hash_table_item *item) {
    free(item->key); // Free the key
    free(item->value); // Free the value
    free(item); // Free the item
}

// Delete a hash table
void hash_table_delete(hash_table *table) {
    for (int i = 0; i < table->size; i++) { // Iterate over the items
        hash_table_item *item = table->items[i]; // Get the item
        if (item != NULL) { // If the item exists
            hash_table_item_delete(item); // Delete the item
        }
    }

    free(table->items); // Free the items
    free(table); // Free the table
}

// Hash a key
static int hash(const char *key, const int a, const int m) {
    long hash = 0; // Initialize the hash to 0
    const int len_s = strlen(key); // Get the length of the key
    for (int i = 0; i < len_s; i++) { // Iterate over the characters of the key
        hash += (long)pow(a, len_s - (i + 1)) * key[i]; // Calculate the hash
        hash = hash % m; // Calculate the hash
    }

    return (int)hash; // Return the hash
}

// Get the hash of a key
static int get_hash(const char *key, const int num_buckets, const int attempt) {
    const int hash_a = hash(key, HT_PRIME_1, num_buckets); // Hash the key
    const int hash_b = hash(key, HT_PRIME_2, num_buckets); // Hash the key
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets; // Return the hash
}

// Insert a key-value pair into a hash table
void hash_table_insert(hash_table *table, const char *key, const char *value) {
    const int load = table->count * 100 / table->size; // Calculate the load
    if (load > 70) { // If the load is greater than 70
        hash_table_resize_up(table); // Resize the table
    }

    hash_table_item *item = hash_table_item_new(key, value); // Create a new item
    int index = get_hash(item->key, table->size, 0); // Get the hash
    hash_table_item *current_item = table->items[index]; // Get the item at the index
    int i = 1; // Initialize i to 1

    while (current_item != NULL) { // While the item exists
        if (current_item != &HT_DELETED_ITEM) { // If the item is not the deleted item
            if (strcmp(current_item->key, key) == 0) { // If the keys match
                hash_table_item_delete(current_item); // Delete the item
                table->items[index] = item; // Set the item
                return;
            }
        }

        index = get_hash(item->key, table->size, i); // Get the hash
        current_item = table->items[index]; // Get the item at the index
        i++;
    }

    table->items[index] = item; // Set the item
    table->count++; // Increment the count
}

// Search for a key in a hash table
char *hash_table_search(hash_table *table, const char *key) {
    int index = get_hash(key, table->size, 0); // Get the hash
    hash_table_item *item = table->items[index]; // Get the item at the index
    int i = 1; // Initialize i to 1

    while (item != NULL) { // While the item exists
        if (item != &HT_DELETED_ITEM){
            if (strcmp(item->key, key) == 0) { // If the keys match
                return item->value; // Return the value
            }
        }

        index = get_hash(key, table->size, i); // Get the hash
        item = table->items[index]; // Get the item at the index
        i++;
    }

    return NULL; // Return NULL
}

// Delete a key-value pair from a hash table
void hash_table_delete_item(hash_table *table, const char *key) {
    const int load = table->count * 100 / table->size; // Calculate the load
    if (load < 10) { // If the load is less than 10
        hash_table_resize_down(table); // Resize the table
    }

    int index = get_hash(key, table->size, 0); // Get the hash
    hash_table_item *item = table->items[index]; // Get the item at the index
    int i = 1; // Initialize i to 1

    while (item != NULL) { // While the item exists
        if (item != &HT_DELETED_ITEM && strcmp(item->key, key) == 0) { // If the keys match
            hash_table_item_delete(item); // Delete the item
            table->items[index] = &HT_DELETED_ITEM; // Set the item to the deleted item
        }

        index = get_hash(key, table->size, i); // Get the hash
        item = table->items[index]; // Get the item at the index
        i++;
    }

    table->count--; // Decrement the count
}

// Resize a hash table
static void hash_table_resize(hash_table *table, const int base_size) {
    if (base_size < HT_INITIAL_BASE_SIZE) { // If the base size is less than the initial base size
        return;
    }

    hash_table *new_table = hash_table_new_size(base_size); // Create a new table
    for (int i = 0; i < table->size; i++) { // Iterate over the items
        hash_table_item *item = table->items[i]; // Get the item
        if (item != NULL && item != &HT_DELETED_ITEM) { // If the item exists and is not the deleted item
            hash_table_insert(new_table, item->key, item->value); // Insert the item
        }
    }

    table->base_size = new_table->base_size; // Set the base size
    table->count = new_table->count; // Set the count

    const int tmp_size = table->size; // Get the size
    table->size = new_table->size; // Set the size
    new_table->size = tmp_size; // Set the size

    hash_table_item **tmp_items = table->items; // Get the items
    table->items = new_table->items; // Set the items
    new_table->items = tmp_items; // Set the items

    hash_table_delete(new_table); // Delete the new table
}

static void hash_table_resize_up(hash_table *table) {
    const int new_size = table->base_size * 2; // Calculate the new size
    hash_table_resize(table, new_size); // Resize the table
}

static void hash_table_resize_down(hash_table *table) {
    const int new_size = table->base_size / 2; // Calculate the new size
    hash_table_resize(table, new_size); // Resize the table
}

// Allocate memory
void *xmalloc(size_t size) {
    void *result = malloc(size);
    if (!result) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    return result;
}

void *xcalloc(size_t count, size_t size) {
    void *result = calloc(count, size);
    if (!result) {
        perror("calloc failed");
        exit(EXIT_FAILURE);
    }
    return result;
}
