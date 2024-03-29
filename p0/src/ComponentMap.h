#pragma once
#include "Entity.h"
#include <cassert>
#include <vector>
#include <unordered_map>

extern std::unordered_map<Entity, uint32_t> gLookup;

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

		gLookup[entity] |= Component::Hash();
		mLookup[entity] = mComponents.size();
		mComponents.push_back(Component());
		mEntities.push_back(entity);

		return mComponents.back();
	}

	void Remove(Entity entity)
	{
		if (HasComponent(entity))
		{
			const size_t index = mLookup.at(entity);
			if (index < mComponents.size() - 1)
			{
				mComponents.at(index) = std::move(mComponents.back());
				mEntities.at(index) = mEntities.back();
				mLookup.at(mEntities.at(index)) = index;
			}

			mEntities.pop_back();
			mComponents.pop_back();
			mLookup.erase(entity);
			gLookup.at(entity) &= ~Component::Hash();
		}
	}

	size_t Count() const
	{
		assert(mEntities.size() == mComponents.size());
		return mComponents.size();
	}

	bool HasComponent(const Entity entity) const
	{
		assert(gLookup.find(entity) != gLookup.end());
		return (gLookup.at(entity) & Component::Hash()) == Component::Hash();
	}

	const Component& operator[](size_t index) const {
		return mComponents.at(index);
	}

	Component& operator[](size_t index) {
		return mComponents.at(index);
	}

	const Entity GetEntity(size_t index) const {
		return mEntities.at(index);
	}

	Entity& GetEntity(size_t index) {
		return mEntities.at(index);
	}

	const Component* Get(const Entity entity) const
	{
		return HasComponent(entity) ? &mComponents.at(mLookup.at(entity)) : nullptr;
	}

	Component* Get(Entity entity)
	{
		return HasComponent(entity) ? &mComponents.at(mLookup.at(entity)) : nullptr;
	}

private:
	std::vector<Entity> mEntities;
	std::vector<Component> mComponents;
	std::unordered_map<Entity, size_t> mLookup;
};
