#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//#include "../../../lib/logs/log.h"

int binary_search(const int *beg, const int *end, int target);

int main()
{
    int n = 0;
    if (scanf("%d", &n) != 1)
    {
        //log_error("Expected integer input\n");
        return 0;
    }

    int *arr = (int *) calloc((size_t) n, sizeof(int));
    for (int i = 0; i < n; ++i)
    {
        if (scanf("%d", arr + i) != 1)
        {
            //log_error("i = %d: expected integer input\n", i);
            break;
        }
    }

    int q = 0;
    if (scanf("%d", &q) != 1)
    {
        //log_error("Expected integer input\n");
        return 0;
    }

    while (q--)
    {
        int beg    = 0;
        int end    = 0;
        int target = 0;
        if (scanf("%d %d %d", &beg, &end, &target) != 3)
        {
            //log_error("q = %d: expected three integers input\n");
            break;
        }

        int ans = binary_search(arr+beg, arr+end, target);

        if (ans) { printf("YES\n"); }
        else     { printf("NO\n");  }
    }

    free(arr);
}

int binary_search(const int *beg, const int *end, int target)
{
    //log_verify(beg != nullptr, 0);
    //log_verify(end != nullptr, 0);
    //log_verify(beg <  end    , 0);

    int l = 0;
    int r = end - beg;

    while (r - l > 1)
    {
        int mid = (r + l) / 2;

        if (beg[mid] <= target) l = mid;
        else                    r = mid;
    }

    return beg[l] == target;
}