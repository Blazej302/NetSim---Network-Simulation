//
// Created by blazej on 7/24/26.
//

#ifndef NETSIM_STOREHOUSE_HPP
#define NETSIM_STOREHOUSE_HPP

#include <map>

#include "receiver_types.hpp"
#include "storage_types.hpp"
#include "types.hpp"

class Storehouse : public IPackageReceiver
{

public:
    explicit Storehouse(ElementID id,
                        std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO))
        : id_(id), d_(std::move(d)) {}

    void receive_package(Package&& p) override {d_->push(std::move(p));}
    ElementID get_id() const override {return id_;}

    IPackageStockpile::const_iterator begin() const override { return d_->begin(); }
    IPackageStockpile::const_iterator end() const override { return d_->end(); }
    IPackageStockpile::const_iterator cbegin() const override { return d_->cbegin(); }
    IPackageStockpile::const_iterator cend() const override { return d_->cend(); }


private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_; //type of storage/pile in storehouse
};

#endif //NETSIM_STOREHOUSE_HPP
