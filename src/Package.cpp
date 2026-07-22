//
// Created by blazej on 7/22/26.
//

#include <stdexcept>

#include "Package.hpp"

Package::Package()
{
    if (!freed_IDs.empty())
    {
        id_ = *freed_IDs.begin();
        freed_IDs.erase(freed_IDs.begin());
    }else if (!assigned_IDs.empty())
    {
        id_ = *assigned_IDs.rbegin() + 1;
    }else
    {
        id_ = 1;
    }
    assigned_IDs.insert(id_);
}

Package::Package(ElementID id): id_(id)
{
    if (assigned_IDs.contains(id))
    {
        throw std::invalid_argument("Package with this ID already exists!");
    }
    assigned_IDs.insert(id);

    freed_IDs.erase(id);
}

Package::Package(Package&& other) noexcept: id_(other.id_)
{
    other.id_ = 0;
}

Package& Package::operator=(Package&& other) noexcept
{
    if (this != &other)
    {
        if (id_ != 0)
        {
            assigned_IDs.erase(id_);
            freed_IDs.insert(id_);
        }
        id_ = other.id_;
        other.id_ = 0;
    }
    return *this;
}

Package::~Package()
{
    if (id_ != 0)
    {
        assigned_IDs.erase(id_);
        freed_IDs.insert(id_);
    }
}
