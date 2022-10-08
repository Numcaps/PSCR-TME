#ifndef UTILS_
#define UTILS_
#include <cstddef>

namespace pr
{
    template <typename iterator>
    size_t count(iterator begin, iterator end)
    {
        size_t cpt;
        for (cpt = 0; begin != end; ++cpt, ++begin)
        {
        }
        return cpt;
    }

    template <typename iterator, typename T>
    size_t count_if_equal(iterator begin, iterator end, const T &val)
    {
        size_t cpt = 0;
        while (begin != end)
        {
            if (*begin == val)
            {
                ++cpt;
            }

            ++begin;
        }
        return cpt;
    }
}
#endif