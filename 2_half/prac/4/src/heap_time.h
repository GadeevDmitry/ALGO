#ifndef HEAP_TIME_H
#define HEAP_TIME_H

//--------------------------------------------------------------------------------------------------------------------------------
// RUN
//--------------------------------------------------------------------------------------------------------------------------------

bool   run_frame               (int *const arr_original, const int arr_size, FILE *const out_stream);

double run_bin_heap            (int *const arr, const int arr_size);
double run_kth_heap            (int *const arr, const int arr_size);
double run_kth_optimized_heap  (int *const arr, const int arr_size);

double run_sort(void *const hp, int *const arr, const int arr_size, bool (*sort_func) (void *, int *, const int));

//--------------------------------------------------------------------------------------------------------------------------------
// TEST
//--------------------------------------------------------------------------------------------------------------------------------

int *gen_test(const int n);

//--------------------------------------------------------------------------------------------------------------------------------
// plot
//--------------------------------------------------------------------------------------------------------------------------------

bool  make_gpi      (const int x_min, const int x_max);
FILE *open_data_file();
void  save_data_frame(const double *const data_frame, const int frame_size, const int elem_num, FILE *const dat_stream);

#endif //HEAP_TIME_H