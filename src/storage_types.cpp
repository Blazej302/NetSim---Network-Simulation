//
// Created by blazej on 7/23/26.
//

#include "storage_types.hpp"

void PackageQueue::push(Package&& p)
{
    container_.push_back(std::move(p));
}

Package PackageQueue::pop()
{
    if (empty())
    {
        throw std::out_of_range("Attempted to pop from an empty queue!");
    }

    if (queue_type_ == PackageQueueType::FIFO)
    {
        Package p = std::move(container_.front());
        container_.pop_front();
        return p;
    }
    else
    {
        Package p = std::move(container_.back());
        container_.pop_back();
        return p;
    }
}
