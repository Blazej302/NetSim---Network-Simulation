//
// Created by blazej on 7/24/26.
//

#include "Ramp.hpp"

void Ramp::delivery_goods(Time t)
{
    if ((t-1) % delivery_interval_ == 0)
    {
        push_package(Package());
    }
}
