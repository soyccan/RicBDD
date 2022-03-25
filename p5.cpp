/* Problem 5
 * Implemented *BMD of function X^2
 * Along with sqrt(X) algorithm
 */

#include <vector>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdint>

using namespace std;

//
// Linux kernel style bit functions
//
// #define ilog2(n) ((n) < 2 ? 0 : 63 - __builtin_clzll(n))

static inline __attribute__((const)) bool is_power_of_2(unsigned long n)
{
    return (n != 0 && ((n & (n - 1)) == 0));
}

// #define allset(n) (n == ((typeof(n))(uint64_t)(int64_t)-1))

// bool dominate(uint64_t x, uint64_t y, unsigned n)
// {
//     uint64_t mask = (1UL << n) - 1;
//     return allset((x & mask) | (~y & mask));
// }

// template <typename T>
// T evaluate(uint64_t x, uint64_t nargs, const vector<T> &tree, uint64_t idx = 1)
// {
//     uint64_t level = ilog2(idx);
//     uint64_t v = (x >> (nargs - level - 1)) & 1;
//     // printf("l=%d n=%d i=%d x=%d v=%d tree[%d]=%d\n", level, nargs, idx, x, v, idx - (1 << nargs), tree[idx - (1 << nargs)]);
//     if (level >= nargs)
//         return tree[idx - (1 << nargs)];
//     T ret = 0;
//     ret += evaluate(x, nargs, tree, idx * 2);
//     if (v)
//     {
//         ret += evaluate(x, nargs, tree, idx * 2 + 1);
//     }
//     return ret;
// }

template <typename T>
T evaluate(unsigned x, const vector<T> &bmd)
{
    assert(is_power_of_2(bmd.size()));

    T res = 0;
    for (unsigned i = 0; i < bmd.size(); i++)
    {
        unsigned dom = x | ~i; // if x dominates i
        if (dom == (unsigned)-1)
        {
            res += bmd[i];
        }
    }
    return res;
}

template <typename T>
unsigned inverse(T y, const vector<T> &bmd)
{
    assert(is_power_of_2(bmd.size()));

    // binary search: find max x subject to f(x) <= y
    unsigned l = 0, r = bmd.size() - 1;
    while (l < r)
    {
        unsigned x = (l + r + 1) >> 1;
        if (evaluate(x, bmd) <= y)
        {
            l = x;
        }
        else
        {
            r = x - 1;
        }
    }
    assert(l == r);
    return l;
}

int main()
{
    // *BMD of X^2 where X is 3-bit
    vector<int> bmd = {0, 1, 4, 4, 16, 8, 16, 0};
    for (int y = 0; y < 70; y++)
    {
        cout << y << ' ' << inverse(y, bmd) << endl;
    }
}