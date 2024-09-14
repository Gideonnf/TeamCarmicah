#pragma once
#include <bitset>
#include <cstdint>

// Keep track of entity ids
// use 16 bit int for now 
using Entity = unsigned int;

// For setting sizes of component arrays, define the max number of entities allowed
const unsigned int MAX_ENTITIES = 10000;

// use an 8 bit int since there wont be that many components created
using ComponentID = std::uint8_t;

// For now, set the max number of components to 32
const std::uint8_t MAX_COMPONENTS = 32;

// Keep track of entity signature using bitset
using Signature = std::bitset<MAX_COMPONENTS>;
