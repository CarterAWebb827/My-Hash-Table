#include <math.h>

#include "prime.h"

/// <summary>
/// Return whether a number is prime
/// if it is prime, return 1
/// if it is not prime, return 0
/// if we cannot determine, return -1
/// </summary>

int is_a_prime(const int x) {
    if (x < 2) {
        return -1;
    }

    if (x < 4) {
        return 1;
    }

    if ((x % 2) == 0) {
        return 0;
    }

    for (int i = 3; i <= floor(sqrt((double)x)); i += 2) {
        if ((x % i) == 0) {
            return 0;
        }
    }

    return 1;
}

/// <summary>
/// Return the next prime number after x
/// </summary>

int next_prime(int x) {
    while (is_a_prime(x) != 1) {
        x++;
    }

    return x;
}