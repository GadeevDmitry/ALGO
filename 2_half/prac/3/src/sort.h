#ifndef SORT_H
#define SORT_H

#include <stdlib.h>

//--------------------------------------------------------------------------------------------------------------------------------
// quadratic
//--------------------------------------------------------------------------------------------------------------------------------

void sort_insert(int *const arr, const size_t size);
void sort_bubble(int *const arr, const size_t size);
void sort_choose(int *const arr, const size_t size);

//--------------------------------------------------------------------------------------------------------------------------------
// logarithmic
//--------------------------------------------------------------------------------------------------------------------------------

void sort_by_median_of_three  (int *const arr, const size_t size);
void sort_by_central          (int *const arr, const size_t size);
void sort_by_rand             (int *const arr, const size_t size);
void sort_by_median_of_medians(int *const arr, const size_t size);
void sort_merge               (int *const arr, const size_t size);

#endif //SORT_H
