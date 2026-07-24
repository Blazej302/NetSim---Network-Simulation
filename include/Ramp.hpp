//
// Created by blazej on 7/24/26.
//

#ifndef NETSIM_RAMP_HPP
#define NETSIM_RAMP_HPP

#include "types.hpp"
#include "Package.hpp"
#include "storage_types.hpp"
#include "receiver_types.hpp"

class Ramp : public PackageSender
{
public:
    Ramp(ElementID id, TimeOffset delivery_interval): id_(id), delivery_interval_(delivery_interval){}

    void delivery_goods(Time t);

    TimeOffset get_delivery_interval() const {return delivery_interval_;}
    ElementID get_id() const {return id_;}
private:
    ElementID id_;
    TimeOffset delivery_interval_;
};

#endif //NETSIM_RAMP_HPP
