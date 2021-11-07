
#pragma once

#include <vector>

    template<typename T>
    void ensureSize(std::vector<T>& v, size_t i, T def = T())
    {
        if (v.size() <= i)
        {
            v.resize(i + 1, def);
        }
    }

    template<typename T>
    bool isIndexValid(const std::vector<T>& v, size_t i)
    {
        return i < v.size();
    }
