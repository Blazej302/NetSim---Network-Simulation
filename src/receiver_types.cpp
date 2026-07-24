//
// Created by blazej on 7/23/26.
//

#include "receiver_types.hpp"
#include "receiver_types.hpp"

void ReceiverPreferences::add_receiver(IPackageReceiver* r)
{
    if (preferences_.empty())
    {
        preferences_[r] = 1.0;
    }

    if (preferences_.contains(r))
    {
        return;
    }

    preferences_[r] = 0.0;

    double new_probability = 1.0 / static_cast<double>(preferences_.size());
    for (auto& [receiver, probability] : preferences_)
    {
        probability = new_probability;
    }

}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r)
{
    if (!preferences_.contains(r))
    {
        return;
    }

    preferences_.erase(r);

    if (preferences_.empty())
    {
        return;
    }

    double new_probability = 1.0 / static_cast<double>(preferences_.size());

    for (auto& [receiver, probability]: preferences_)
    {
        probability = new_probability;
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver()
{
    if (preferences_.empty())
    {
        return nullptr;
    }

    double p = pg_();
    // we use distribution function to choose receiver
    double cumulative_probability = 0.0;
    for (const auto& [receiver, probability]:preferences_)
    {
        cumulative_probability += probability;
        if (p <= cumulative_probability)
        {
            return receiver;
        }
    }

    return preferences_.rbegin()->first;
}

void PackageSender::push_package(Package&& p)
{
    sending_buffer_ = std::move(p);
}

void PackageSender::send_package()
{
    if (!sending_buffer_.has_value())
    {
        return;
    }

    IPackageReceiver* receiver = receiver_preferences_.choose_receiver();

    if (receiver != nullptr)
    {
        receiver->receive_package(std::move(sending_buffer_.value())); //std::move(*sending_buffer_)
        sending_buffer_.reset();
    }

}
