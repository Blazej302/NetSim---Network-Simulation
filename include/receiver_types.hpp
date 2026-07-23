//
// Created by blazej on 7/23/26.
//

#ifndef NETSIM_RECEIVER_TYPES_HPP
#define NETSIM_RECEIVER_TYPES_HPP

#include <map>
#include <memory>

#include "types.hpp"
#include "helpers.hpp"

class IPackageReceiver;

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



#endif //NETSIM_RECEIVER_TYPES_HPP
