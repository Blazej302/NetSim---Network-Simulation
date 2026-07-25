//
// Created by blazej on 7/24/26.
//

#ifndef NETSIM_WORKER_HPP
#define NETSIM_WORKER_HPP

#include <memory>

#include "types.hpp"
#include "Package.hpp"
#include "storage_types.hpp"
#include "receiver_types.hpp"

class Worker : public PackageSender, public IPackageReceiver
{
public:
    Worker(ElementID id, TimeOffset processing_duration, std::unique_ptr<IPackageQueue> q)
        : PackageSender(), id_(id), processing_duration_(processing_duration), q_(std::move(q)) {}

    void do_work(Time t);

    void receive_package(Package&& p) override {q_->push(std::move(p));}
    ElementID get_id() const override { return id_; }
    ReceiverType get_receiver_type() const override { return ReceiverType::WORKER; }

    IPackageStockpile::const_iterator begin() const override { return q_->begin(); }
    IPackageStockpile::const_iterator end() const override { return q_->end(); }
    IPackageStockpile::const_iterator cbegin() const override { return q_->cbegin(); }
    IPackageStockpile::const_iterator cend() const override { return q_->cend(); }

    TimeOffset get_processing_duration() const { return processing_duration_; }
    Time get_package_processing_start_time() const { return processing_start_time_; }
    const std::optional<Package>& get_processing_buffer() const { return processing_buffer_; }

private:
    ElementID id_;
    TimeOffset processing_duration_;
    std::unique_ptr<IPackageQueue> q_; // queue for packages witch are waiting for process

    std::optional<Package> processing_buffer_ = std::nullopt;
    Time processing_start_time_;
};

#endif //NETSIM_WORKER_HPP
