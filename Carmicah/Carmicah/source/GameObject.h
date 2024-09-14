#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "ECSTypes.h"

class GameObject
{
private:
	Entity mID;
	std::string mName;
public:
	GameObject();
	GameObject(const Entity& id);
	~GameObject();

	void Destroy();

	Entity GetID();
	void SetID(const Entity& id);

	std::string GetName();
	void SetName(const std::string& name);

	template<typename T>
	void AddComponent(const T& component);

	template<typename T>
	void RemoveComponent(const T& component);

	template<typename T>
	T& GetComponent();
	
};


#endif