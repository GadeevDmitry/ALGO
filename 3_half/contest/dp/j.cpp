#include <algorithm>
#include <iostream>
#include <cstring>
#include <vector>

#define INPUT_ERROR 1

//==================================================================================================

static bool input(
    char *s1, size_t *s1_len,
    char *s2, size_t *s2_len);

static bool str_scanf(char *s);

static void fprintf_arr(FILE *stream, const std::vector<int> &arr);

static size_t get_lcs(
    const char *s1, const size_t s1_len,
    const char *s2, const size_t s2_len,

    std::vector<int> &ans1,
    std::vector<int> &ans2);

//==================================================================================================

int main()
{
    char s1[1001] = ""; size_t s1_len = 0;
    char s2[1001] = ""; size_t s2_len = 0;
    if (!input(s1, &s1_len, s2, &s2_len))
        return INPUT_ERROR;

    std::vector<int> ans1;
    std::vector<int> ans2;
    size_t lcs_len = get_lcs(s1, s1_len, s2, s2_len, ans1, ans2);

    printf("%lu\n", ans1.size());
    fprintf_arr(stdout, ans1);
    fprintf_arr(stdout, ans2);
}

//--------------------------------------------------------------------------------------------------

static bool input(
    char *s1, size_t *s1_len,
    char *s2, size_t *s2_len)
{
    if (!str_scanf(s1)) return false;
    if (!str_scanf(s2)) return false;

    *s1_len = strlen(s1);
    *s2_len = strlen(s2);

    return true;
}

//--------------------------------------------------------------------------------------------------

static bool str_scanf(char *s)
{
    int len = 0;
    int res = scanf("%1000s%n", s, &len);

    return res == 1 && len <= 1000;
}

//--------------------------------------------------------------------------------------------------

static void fprintf_arr(FILE *stream, const std::vector<int> &arr)
{
    for (auto elem : arr)
        fprintf(stream, "%d ", elem);
    fprintf(stream, "\n");
}

//--------------------------------------------------------------------------------------------------

static size_t get_lcs(
    const char *s1, const size_t s1_len,
    const char *s2, const size_t s2_len,

    std::vector<int> &ans1,
    std::vector<int> &ans2)
{
    int lcs[s1_len + 1][s2_len + 1];

    for (int i = 0; i <= s1_len; ++i)
        for (int j = 0; j <= s2_len; ++j)
            lcs[i][j] = 0;

    for (int i = 1; i <= s1_len; ++i)
    {
        for (int j = 1; j <= s2_len; ++j)
        {
            lcs[i][j] = std::max(lcs[i - 1][j], lcs[i][j - 1]);

            if (s1[i - 1] == s2[j - 1])
                lcs[i][j] = std::max(lcs[i][j], lcs[i - 1][j - 1] + 1);
        }
    }

    int p1 = s1_len;
    int p2 = s2_len;

    while (p1 != 0 && p2 != 0)
    {
        if ((lcs[p1][p2] == lcs[p1 - 1][p2 - 1] + 1) && (s1[p1 - 1] == s2[p2 - 1]))
        {
            ans1.push_back(p1);
            ans2.push_back(p2);

            --p1;
            --p2;
        }
        else
        {
            if (lcs[p1][p2] == lcs[p1 - 1][p2]) --p1;
            else                                --p2;
        }
    }

    std::reverse(ans1.begin(), ans1.end());
    std::reverse(ans2.begin(), ans2.end());

    return lcs[s1_len][s2_len];
}
