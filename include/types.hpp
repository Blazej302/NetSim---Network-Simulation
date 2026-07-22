//
// Created by blazej on 7/22/26.
//

#ifndef NETSIM_TYPES_HPP
#define NETSIM_TYPES_HPP

#include <cstddef>

using ElementID = std::size_t;
using Time = std::size_t;
using TimeOffset = std::size_t;

enum class PackageQueueType
{
    FIFO,
    LIFO
};

#endif //NETSIM_TYPES_HPP
