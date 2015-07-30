#include "dynarr.h"

#include <stdio.h>
#include <time.h>

#define TESTS 1000 * 1000 * 50

void test_r_array();
void test_b_array();

int main() {
    test_r_array();

    test_b_array();

    return 0;
}

void test_r_array() {
    r_array_t arr;
    r_array_new(&arr, sizeof(int));

    clock_t start = clock();
    for (int i = 0; i < TESTS; i++) {
        r_array_append(&arr, &i);
    }
    clock_t end = clock();
    printf("[R] clock cycles: %lu\n", end - start);
    printf("[R] arr len: %d\n", r_array_len(&arr));
}

void test_b_array() {
    b_array_t arr;
    b_array_new(&arr, sizeof(int));

    clock_t start = clock();
    for (int i = 0; i < TESTS; i++) {
        b_array_append(&arr, &i);
    }
    clock_t end = clock();
    printf("[B] clock cycles: %lu\n", end - start);
    printf("[B] arr len: %d\n", b_array_len(&arr));
}
