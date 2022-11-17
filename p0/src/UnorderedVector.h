#pragma once
#include <vector>
#include <unordered_map>

using Id = uint32_t;

namespace
{
	Id gId = 0;
}

template<typename T>
class UnorderedVector
{
public:
	inline Id Add(const T& object)
	{
		mObjects.push_back(object);
		mForward[++gId] = mObjects.size() - 1;
		mBackward[mObjects.size() - 1] = gId;
		return gId;
	}

	// Apparently I don't understand x-values...
	//inline Id Add(T&& object)
	//{
	//	mObjects.push_back(object);
	//	mForward[++gId] = mObjects.size() - 1;
	//	mBackward[mObjects.size() - 1] = gId;
	//	return gId;
	//}

	inline T* Get(Id id)
	{
		assert(mForward.find(id) != mForward.end());
		return &mObjects[mForward[id]];
	}

	inline void Remove(Id id)
	{
		assert(mForward.find(id) != mForward.end());

		// Handle self-removal
		if (mForward[id] == mObjects.size() - 1)
		{
			mObjects.pop_back();
			mForward.erase(id);
			mBackward.erase(mObjects.size());
		}
		else
		{
			// Overwrite object to be removed with last object 
			size_t index = mForward[id];
			mObjects[index] = std::move(mObjects.back());

			// Store last object's key and remove from maps
			mObjects.pop_back();
			mForward.erase(id);
			id = mBackward[mObjects.size()];
			mBackward.erase(mObjects.size());

			// Update maps
			mForward[id] = index;
			mBackward[index] = id;
		}
	}

	inline std::vector<T>& Objects()
	{
		return mObjects;
	}

private:
	std::vector<T> mObjects;
	std::unordered_map<Id, size_t> mForward;
	std::unordered_map<size_t, Id> mBackward;
};
