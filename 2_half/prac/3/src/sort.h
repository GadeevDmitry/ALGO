#ifndef SORT_H
#define SORT_H

//================================================================================================================================
// FUNCTION DECLARATION
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// sort
//--------------------------------------------------------------------------------------------------------------------------------

void bubble_sort(int *const arr, const int n);
void insert_sort(int *const arr, const int n);
void choose_sort(int *const arr, const int n);
//--------------------------------------------------------------------------------------------------------------------------------
void quick_sort                    (int *const arr, const int n, int (*get_pivot) (int *, int));
int  quick_sort_frame              (int *const arr, const int n, int (*get_pivot) (int *, int));

void      sort_by_median_of_three  (int *const arr, const int n);
int  get_pivot_by_median_of_three  (int *const arr, const int n);

void      sort_by_central          (int *const arr, const int n);
int  get_pivot_by_central          (int *const arr, const int n);

void      sort_by_rand             (int *const arr, const int n);
int  get_pivot_by_rand             (int *const arr, const int n);

void      sort_by_median_of_medians(int *const arr, const int n);
int  get_pivot_by_median_of_medians(int *const arr, const int n);
int  kth_element                   (int *const arr, const int n, const int k);

//--------------------------------------------------------------------------------------------------------------------------------
// swap
//--------------------------------------------------------------------------------------------------------------------------------

void int_swap(int *const a, int *const b);

#endif //SORT_H