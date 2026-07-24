//
// Created by blazej on 7/23/26.
//

#ifndef NETSIM_RECEIVER_TYPES_HPP
#define NETSIM_RECEIVER_TYPES_HPP

#include <map>
#include <memory>
#include <optional>

#include "types.hpp"
#include "helpers.hpp"
#include "Package.hpp"
#include "storage_types.hpp"

class IPackageReceiver
{
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;

    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;

    virtual ~IPackageReceiver() = default;
};

class ReceiverPreferences
{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator):pg_(std::move(pg)){}

    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);

    IPackageReceiver* choose_receiver();

    const preferences_t& get_preferences() const {return preferences_;}

    const_iterator begin() const {return preferences_.begin();}
    const_iterator end() const {return preferences_.end();}
    const_iterator cbegin() const {return preferences_.cbegin();}
    const_iterator cend() const {return preferences_.cend();}

private:
    preferences_t preferences_;
    ProbabilityGenerator pg_;
};

class PackageSender
{
public:
    ReceiverPreferences receiver_preferences_;

    PackageSender() = default;

    PackageSender(PackageSender&&) = default;
    PackageSender& operator = (PackageSender&&) = default;

    PackageSender(const PackageSender&) = delete;
    PackageSender& operator = (const PackageSender&) = delete;

    virtual ~PackageSender() = default;

    void push_package(Package&& p);
    void send_package();

    const std::optional<Package>& get_sending_buffer() const {return sending_buffer_;}
protected:
    void reset_buffer() {sending_buffer_.reset();}
private:
    std::optional<Package> sending_buffer_ = std::nullopt;
};

#endif //NETSIM_RECEIVER_TYPES_HPP
