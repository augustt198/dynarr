// resize-based dynamic array
typedef struct {
    int cap_size;
    int log_size;
    int elem_size;
    
    void *base_ptr;
} r_array_t;
 
void r_array_new    (r_array_t *arr, int elem_size);
void r_array_append (r_array_t *arr, void *elem);
void r_array_get    (r_array_t *arr, int idx, void *dst);
int  r_array_len    (r_array_t *arr);


// bucket-based dynamic array
typedef struct {
    // current bucket capacity
    int curr_bucket_cap;
    // how much of the current bucket is filled
    int curr_bucket_filled;

    int buckets_used;
    int buckets_cap;
    
    int elem_size;

     // the logical total length of the array
    int log_len;

    void **buckets;
} b_array_t;

void b_array_new    (b_array_t *arr, int elem_size);
void b_array_append (b_array_t *arr, void *elem);
void b_array_get    (b_array_t *arr, int idx, void *dst);
int  b_array_len    (b_array_t *arr);
