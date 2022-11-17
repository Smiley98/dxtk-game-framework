#pragma once
#include "Entity.h"
#include <cassert>
#include <vector>
#include <unordered_map>

template<typename Component>
class ComponentCollection
{
public:
	Component& Add(Entity entity)
	{
		assert(entity != INVALID_ENTITY);
		assert(mLookup.find(entity) == mLookup.end());
		assert(mEntities.size() == mComponents.size());
		assert(mLookup.size() == mComponents.size());

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
	}

	Component& operator[](size_t index) {
		return mComponents[index];
	}

	Entity GetEntity(size_t index) const {
		return mEntities[index];
	}

	Component* GetComponent(Entity entity)
	{
		// This lookup isn't costly because we won't have many components.
		auto it = mLookup.find(entity);
		return it != mLookup.end() ? &mComponents[it->second] : nullptr;
	}

	size_t Count()
	{
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
