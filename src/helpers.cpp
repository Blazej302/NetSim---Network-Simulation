//
// Created by blazej on 7/23/26.
//

#include "helpers.hpp"

#include <cstdlib>
#include <random>
#include <functional>

std::random_device rd;
std::mt19937 rng(rd());

double default_probability_generator() {
    return std::generate_canonical<double, 10>(rng);
}

std::function<double()> probability_generator = default_probability_generator;