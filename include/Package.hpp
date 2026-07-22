//
// Created by blazej on 7/22/26.
//

#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include "types.hpp"

#include <set>

class Package
{
private:
    ElementID id_;
    inline static std::set<ElementID> assigned_IDs{};
    inline static std::set<ElementID> freed_IDs{};

public:
    Package();
    explicit Package(ElementID id);

    Package(const Package&) = delete;
    Package& operator = (const Package&) = delete;

    Package(Package&& other) noexcept;
    Package& operator = (Package&& other) noexcept;

    ~Package();

    ElementID get_id() const {return id_;}

};

#endif //NETSIM_PACKAGE_HPP
