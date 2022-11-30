#pragma once
#include "Entity.h"
#include <cassert>
#include <vector>
#include <unordered_map>

// No idea why memory is getting corrupted...........
//namespace
//{
//	// global map of entities to component hashes
//	std::unordered_map<Entity, uint32_t> gLookup;
//}

template<typename Component>
class ComponentMap
{
public:
	Component& Add(Entity entity)
	{
		assert(entity != INVALID_ENTITY);
		assert(mLookup.find(entity) == mLookup.end());
		assert(mEntities.size() == mComponents.size());
		assert(mLookup.size() == mComponents.size());

		//gLookup[entity] |= Component::Hash();
		mLookup[entity] = mComponents.size();
		mComponents.push_back(Component());
		mEntities.push_back(entity);

		return mComponents.back();
	}

	void Remove(Entity entity)
	{
		auto it = mLookup.find(entity);
		if (it != mLookup.end())
		{
			const size_t index = it->second;
			const Entity entity = mEntities[index];

			if (index < mComponents.size() - 1)
			{
				mComponents[index] = std::move(mComponents.back());
				mEntities[index] = mEntities.back();
				mLookup[mEntities[index]] = index;
			}

			mComponents.pop_back();
			mEntities.pop_back();
			mLookup.erase(entity);
		}

		//auto hash = gLookup.find(entity);
		//if (hash != gLookup.end())
		//{
		//	gLookup.at(it->first) &= ~Component::Hash();
		//}
	}

	const Component& operator[](size_t index) const {
		return mComponents[index];
	}

	Component& operator[](size_t index) {
		return mComponents[index];
	}

	const Entity GetEntity(size_t index) const {
		return mEntities[index];
	}

	Entity& GetEntity(size_t index) {
		return mEntities[index];
	}

	const Component* GetComponent(const Entity entity) const
	{
		auto it = mLookup.find(entity);
		return it != mLookup.end() ? &mComponents[it->second] : nullptr;
	}

	Component* GetComponent(Entity entity)
	{
		auto it = mLookup.find(entity);
		return it != mLookup.end() ? &mComponents[it->second] : nullptr;
	}

	// Try not to expose these
	//const std::vector<Entity>& Entities() const
	//{
	//	return mEntities;
	//}
	//
	//const std::vector<Component>& Components() const
	//{
	//	return mComponents;
	//}

	size_t Count() const
	{
		assert(mEntities.size() == mComponents.size());
		return mComponents.size();
	}

	bool Contains(Entity entity) const
	{
		return mLookup.find(entity) != mLookup.end();
	}

private:
	std::vector<Entity> mEntities;
	std::vector<Component> mComponents;
	std::unordered_map<Entity, size_t> mLookup;
};
