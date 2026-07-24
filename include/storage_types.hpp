//
// Created by blazej on 7/23/26.
//

#ifndef NETSIM_STORAGETYPES_HPP
#define NETSIM_STORAGETYPES_HPP

#include <list>
#include <cstddef>
#include <stdexcept>

#include "Package.hpp"
#include "types.hpp"

class IPackageStockpile
{
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&& p) = 0;

    virtual std::size_t size() const = 0;
    virtual bool empty() const = 0;

    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

    virtual ~IPackageStockpile() = default;

};

class IPackageQueue:public IPackageStockpile
{
public:
    virtual PackageQueueType get_queue_type() const = 0;
    virtual Package pop() = 0;
    virtual ~IPackageQueue() override = default;
};

class PackageQueue:public IPackageQueue
{
public:
    explicit PackageQueue(PackageQueueType queue_type):queue_type_(queue_type){}

    PackageQueueType get_queue_type() const override {return queue_type_;}
    Package pop() override;

    void push(Package&& p) override;
    std::size_t size() const override {return container_.size();}
    bool empty() const override {return container_.empty();}

    const_iterator begin() const override {return container_.begin();}
    const_iterator end() const override {return container_.end();}
    const_iterator cbegin() const override {return container_.cbegin();}
    const_iterator cend() const override {return container_.cend();}

private:
    PackageQueueType queue_type_;
    std::list<Package> container_;

};

#endif //NETSIM_STORAGETYPES_HPP
