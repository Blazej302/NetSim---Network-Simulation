//
// Created by blazej on 7/24/26.
//

#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP

#include "Worker.hpp"
#include "Ramp.hpp"
#include "Storehouse.hpp"

#include <list>
#include <algorithm>
#include <stdexcept>
#include <map>

#include "receiver_types.hpp"

template <typename Node>
class NodeCollection
{
public:
    using container_t = std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node&& node){collection_.push_back(std::move(node));}

    iterator find_by_id(ElementID id)
    {
        return std::find_if(collection_.begin(), collection_.end(), [id](const auto& elem) {return elem.get_id() == id;});
    }

    const_iterator find_by_id(ElementID id) const
    {
        return std::find_if(collection_.cbegin(), collection_.cend(), [id](const auto& elem) {return elem.get_id() == id;});
    }

    void remove_by_id(ElementID id)
    {
        auto it = find_by_id(id);
        if (it != collection_.end())
        {
            collection_.erase(it);
        }
    }

    iterator begin() { return collection_.begin(); }
    iterator end() { return collection_.end(); }
    const_iterator begin() const { return collection_.cbegin(); }
    const_iterator end() const { return collection_.cend(); }
    const_iterator cbegin() const { return collection_.cbegin(); }
    const_iterator cend() const { return collection_.cend(); }

private:
    container_t collection_;
};

class Factory
{
public:
    void add_ramp(Ramp&& ramp) { ramps_.add(std::move(ramp)); }
    void add_worker(Worker&& worker) { workers_.add(std::move(worker)); }
    void add_storehouse(Storehouse&& storehouse) { storehouses_.add(std::move(storehouse)); }

    void remove_ramp(ElementID id) { ramps_.remove_by_id(id); }
    void remove_worker(ElementID id) { remove_receiver(workers_, id); }
    void remove_storehouse(ElementID id) { remove_receiver(storehouses_, id); }

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return ramps_.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return ramps_.find_by_id(id); }
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return workers_.find_by_id(id); }
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return workers_.find_by_id(id); }
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return storehouses_.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return storehouses_.find_by_id(id); }

    NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return ramps_.cbegin(); }
    NodeCollection<Ramp>::const_iterator ramp_cend() const { return ramps_.cend(); }
    NodeCollection<Worker>::const_iterator worker_cbegin() const { return workers_.cbegin(); }
    NodeCollection<Worker>::const_iterator worker_cend() const { return workers_.cend(); }
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return storehouses_.cbegin(); }
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return storehouses_.cend(); }

    bool is_consistent() const;
    void do_deliveries(Time t);
    void do_package_passing();
    void do_work(Time t);

private:
    template <typename Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);

    NodeCollection<Ramp> ramps_;
    NodeCollection<Worker> workers_;
    NodeCollection<Storehouse> storehouses_;
};

#endif //NETSIM_FACTORY_HPP
