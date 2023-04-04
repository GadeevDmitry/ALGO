#ifndef HEAP_TIME_H
#define HEAP_TIME_H

//--------------------------------------------------------------------------------------------------------------------------------
// RUN
//--------------------------------------------------------------------------------------------------------------------------------

bool   run_frame   (int *const arr_original, const int arr_size, FILE *const out_stream);
double run_bin_heap(int *const arr         , const int arr_size);
double run_k_heap  (int *const arr         , const int arr_size);

//--------------------------------------------------------------------------------------------------------------------------------
// TEST
//--------------------------------------------------------------------------------------------------------------------------------

void test_heap_sort (int *const arr, const int arr_size, bool is_binary);
void dump_wrong_test(const int *arr_original, const int *arr_sorted, const int arr_size);
int *gen_test       (const int n);

//--------------------------------------------------------------------------------------------------------------------------------
// plot
//--------------------------------------------------------------------------------------------------------------------------------

bool  make_gpi      (const int x_min, const int x_max);
FILE *open_data_file();
void  save_data_frame(const double *const data_frame, const int frame_size, const int elem_num, FILE *const dat_stream);

#endif //HEAP_TIME_H