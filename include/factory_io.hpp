//
// Created by blazej on 7/26/26.
//

#ifndef NETSIM_FACTORY_IO_HPP
#define NETSIM_FACTORY_IO_HPP

#include <istream>
#include <ostream>
#include "Factory.hpp"

// loading and save structure of factory
Factory load_factory_structure(std::istream& is);
void save_factory_structure(const Factory& factory, std::ostream& os);

// generating reports
void generate_structure_report(const Factory& factory, std::ostream& os);
void generate_simulation_turn_report(const Factory& factory, std::ostream& os, Time turn);

#endif //NETSIM_FACTORY_IO_HPP
