#include "dynarr.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#define INIT_CAPACITY 16

void r_array_new(r_array_t *arr, int elem_size) {
    arr->cap_size  = INIT_CAPACITY;
    arr->log_size  = 0;
    arr->elem_size = elem_size;
    
    arr->base_ptr = malloc(elem_size * INIT_CAPACITY);
}

void r_array_append(r_array_t *arr, void *elem) {
    if (arr->log_size < arr->cap_size) {
        enough_cap:
        memcpy(
            (char*) arr->base_ptr + (arr->elem_size * arr->log_size),
            elem,
            arr->elem_size
        );

        arr->log_size += 1;
    } else {
        int new_cap    = arr->cap_size * 2;
        void *new_ptr  = malloc(new_cap * arr->elem_size);

        memcpy(
            new_ptr,
            arr->base_ptr,
            arr->cap_size * arr->elem_size
        );

        free(arr->base_ptr);

        arr->cap_size = new_cap;
        arr->base_ptr = new_ptr;
        
        goto enough_cap;
    }
}

int r_array_get(r_array_t *arr, int idx, void *dst) {
    if (idx < 0 || idx >= arr->log_size)
        return -1;

    memcpy(
        dst,
        (char*) arr->base_ptr + (arr->elem_size * idx),
        arr->elem_size
    );

    return 0;
}

int r_array_len(r_array_t *arr) {
    return arr->log_size;
}


void b_array_new(b_array_t *arr, int elem_size) {
    arr->curr_bucket_cap     = INIT_CAPACITY;
    arr->curr_bucket_filled  = 0;
    arr->buckets_used        = 1;
    arr->buckets_cap         = INIT_CAPACITY;
    arr->log_len             = 0;
    arr->elem_size           = elem_size;
    
    arr->buckets      = malloc(sizeof(void*) * INIT_CAPACITY);
    void *bucket      = malloc(elem_size * INIT_CAPACITY);
    arr->buckets[0]   = bucket;

    arr->cache_b_start_idx = 0;
    arr->cache_b_end_idx   = 16;
    arr->cache_b_current   = 0;
}

void b_array_append(b_array_t *arr, void *elem) {
    if (arr->curr_bucket_filled < arr->curr_bucket_cap) {
        enough_bucket_cap:
        memcpy(
            (char*) arr->buckets[arr->buckets_used - 1] + (arr->curr_bucket_filled * arr->elem_size),
            elem,
            arr->elem_size
        );
        arr->curr_bucket_filled  += 1;
        arr->log_len             += 1;
    } else { // not enough space in bucket
        if (arr->buckets_used == arr->buckets_cap) {
            // not enough space to add another bucket
            int new_buckets_cap = arr->buckets_cap * 2;
            void **new_ptr      = malloc(sizeof(void*) * new_buckets_cap);
            
            memcpy(new_ptr, arr->buckets, sizeof(void*) * arr->buckets_cap);

            arr->buckets_cap = new_buckets_cap;
            arr->buckets     = new_ptr;
        }
        int new_cap                     = arr->curr_bucket_cap * 2;
        void *bucket                    = malloc(arr->elem_size * new_cap);
        arr->buckets[arr->buckets_used] = bucket;
        arr->buckets_used               += 1;
        arr->curr_bucket_cap            = new_cap;
        arr->curr_bucket_filled         = 0;

        goto enough_bucket_cap;
    } 
}

int b_array_get(b_array_t *arr, int idx, void *dst) {
    if (idx < 0 || idx >= arr->log_len)
        return -1;

    int bucket_idx, offset;
    if (idx < 16) {
        bucket_idx = 0;
        offset     = idx;
    } else if (idx >= arr->cache_b_start_idx &&
               idx <  arr->cache_b_end_idx) {
        bucket_idx = arr->cache_b_current;
        offset     = idx - arr->cache_b_start_idx;
    } else {
        bucket_idx    = ceil(log2(idx + 16 + 1) - 5);
        int start_idx = (1 << (bucket_idx + 4)) - 16;
        offset = idx - start_idx;
        
        arr->cache_b_start_idx = start_idx;
        arr->cache_b_end_idx   = (1 << (bucket_idx + 5)) - 16;
        arr->cache_b_current   = bucket_idx;
    }

    memcpy(
        dst,
        (char*) arr->buckets[bucket_idx] + (offset * arr->elem_size),
        arr->elem_size
    );

    return 0;
}

int b_array_len(b_array_t *arr) {
    return arr->log_len;
}
