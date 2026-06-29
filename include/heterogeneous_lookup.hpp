#pragma once

#include <functional>
#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess
{
    bool operator()(const std::string_view lhs, const std::string_view rhs) const
    {
        return lhs < rhs;
    }
};

struct TransparentStringEqual
{
    bool operator()(const std::string_view lhs, const std::string_view rhs) const
    {
        return lhs == rhs;
    }
};

struct TransparentStringHash
{
    std::size_t operator()(const std::string_view value) const
    {
        return std::hash<std::string_view>{}(value);
    }
};

}  // namespace bookdb
