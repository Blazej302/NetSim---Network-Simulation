//
// Created by blazej on 7/22/26.
//

#ifndef NETSIM_TYPES_HPP
#define NETSIM_TYPES_HPP

#include <cstddef>
#include <functional>

using ElementID = std::size_t;
using Time = std::size_t;
using TimeOffset = std::size_t;
using ProbabilityGenerator = std::function<double()>;

enum class PackageQueueType
{
    FIFO,
    LIFO
};

enum class ReceiverType
{
    WORKER,
    STOREHOUSE
};

enum class NodeColor
{
    UNVISITED,
    VISITED,
    VERIFIED
};

#endif //NETSIM_TYPES_HPP
