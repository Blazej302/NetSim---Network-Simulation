//
// Created by blazej on 7/24/26.
//
#include "Factory.hpp"
#include "types.hpp"
#include "Worker.hpp"

template <typename Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id)
{
    auto it = collection.find_by_id(id);
    if (it == collection.end()) return;

    IPackageReceiver* receiver_ptr = dynamic_cast<IPackageReceiver*>(&(*it));

    for (auto& ramp:ramps_)
    {
        ramp.receiver_preferences_.remove_receiver(receiver_ptr);
    }

    for (auto& worker : workers_) {
        worker.receiver_preferences_.remove_receiver(receiver_ptr);
    }

    collection.remove_by_id(id);

}

// Recursive DFS algorithm to check if the sender has access to the storage

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if (node_colors[sender] == NodeColor::VERIFIED) {
        return true;
    }
    node_colors[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().empty()) {
        throw std::logic_error("The sender has no defined recipients!");
    }

    bool has_other_receiver = false;

    for (const auto& [receiver_ptr, probability] : sender->receiver_preferences_.get_preferences()) {
        if (receiver_ptr->get_receiver_type() == ReceiverType::STOREHOUSE) {
            has_other_receiver = true;
        } else if (receiver_ptr->get_receiver_type() == ReceiverType::WORKER) {
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if (sendrecv_ptr == sender) {
                continue;
            }
            has_other_receiver = true;

            if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED) {
                has_reachable_storehouse(sendrecv_ptr, node_colors);
            }
        }
    }

    node_colors[sender] = NodeColor::VERIFIED;

    if (has_other_receiver) {
        return true;
    } else {
        throw std::logic_error("The sender does not have a valid output connection!");
    }
}

bool Factory::is_consistent() const {
    std::map<const PackageSender*, NodeColor> node_colors;

    for (const auto& ramp : ramps_) {
        node_colors[&ramp] = NodeColor::UNVISITED;
    }
    for (const auto& worker : workers_) {
        node_colors[&worker] = NodeColor::UNVISITED;
    }

    try {
        for (const auto& ramp : ramps_) {
            has_reachable_storehouse(&ramp, node_colors);
        }
    } catch (const std::logic_error&) {
        return false;
    }

    return true;
}

void Factory::do_deliveries(Time t) {
    for (auto& ramp : ramps_) {
        ramp.deliver_goods(t);
    }
}

void Factory::do_package_passing() {
    for (auto& ramp : ramps_) {
        ramp.send_package();
    }
    for (auto& worker : workers_) {
        worker.send_package();
    }
}

void Factory::do_work(Time t) {
    for (auto& worker : workers_) {
        worker.do_work(t);
    }
}

