#ifndef BASE_SYSTEM_H
#define BASE_SYSTEM_H
#include <set>
#include <ECSTypes.h>
#include <vector>



class BaseSystem
{
public:
	// contain a set that holds what entities it holds
	std::set<Entity> mEntitiesSet;
	// Keep track of the system's signature
	Signature mSignature;
	// Holds a vector of what components it uses. tbh idk if itll be used for anything but just keeping it ig
	std::vector<const char*> componentNames;

	//// Initialize the components that the system needs
	//virtual void InitComponents(std::vector<const char*> componentList)
	//{
	//	// Loop through the vector
	//	for (auto const& component : componentList)
	//	{
	//		componentNames.push_back(component);
	//		// Set the signature for all the component it has
	//		mSignature.set(ComponentManager::GetInstance()->GetComponentID(component), true);
	//	}
	//}

	//~BaseSystem()
	//{

	//}
};

#endif