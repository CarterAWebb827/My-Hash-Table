#include <time.h>

#include "hash_table.h"

// To compile and run:
// gcc -o main main.c hash_table.c prime.c -lm

int main () {
    hash_table *table = hash_table_new(); // Create a new hash table
    
    // Calculate the time taken to insert a key-value pair
    clock_t start = clock(); // Start the clock
    hash_table_insert(table, "key1", "value1"); // Insert a key-value pair
    clock_t end = clock(); // End the clock

    double time_taken = ((double)end - start) / CLOCKS_PER_SEC; // Calculate the time taken
    printf("Time taken to insert a key-value pair: %f\n", time_taken); // Print the time taken


    hash_table_insert(table, "key2", "value2"); // Insert a key-value pair


    clock_t start2 = clock(); // Start the clock
    char *value = hash_table_search(table, "key1"); // Search for a key
    clock_t end2 = clock(); // End the clock

    double time_taken2 = ((double)end2 - start2) / CLOCKS_PER_SEC; // Calculate the time taken
    printf("Time taken to search for a key: %f\n", time_taken2); // Print the time taken


    printf("%s\n", value); // Print the value


    clock_t start3 = clock(); // Start the clock
    hash_table_delete_item(table, "key1"); // Delete a key-value pair
    clock_t end3 = clock(); // End the clock

    double time_taken3 = ((double)end3 - start3) / CLOCKS_PER_SEC; // Calculate the time taken
    printf("Time taken to delete a key-value pair: %f\n", time_taken3); // Print the time taken

    hash_table_delete(table); // Delete the hash table
}