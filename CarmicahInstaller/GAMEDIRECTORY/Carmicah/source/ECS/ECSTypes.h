/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			ECSTypes.h

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

 brief:			Contains keywords to be used for ECS systems

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef ECS_TYPES_H
#define ECS_TYPES_H
#include <bitset>
#include <cstdint>

namespace Carmicah
{
	// Keep track of entity ids
	// use 16 bit int for now 
	using Entity = unsigned int;

	// For setting sizes of component arrays, define the max number of entities allowed
	const unsigned int MAX_ENTITIES = 5000;

	// use an 8 bit int since there wont be that many components created
	using ComponentID = std::uint8_t;

	// For now, set the max number of components to 32
	const std::uint8_t MAX_COMPONENTS = 32;

	const std::uint8_t MAX_LAYERS = 32;

	// Keep track of entity signature using bitset
	using Signature = std::bitset<MAX_COMPONENTS>;
}
#endif