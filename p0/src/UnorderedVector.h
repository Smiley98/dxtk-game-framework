#pragma once
#include <vector>
#include <unordered_map>

namespace
{
	uint32_t gId = 0;
}

struct Test
{
	int a;
	float b;
};

class UnorderedVector
{
public:

	inline uint32_t Add(int a, float b)
	{
		mObjects.push_back({ a, b });
		mForward[++gId] = mObjects.size() - 1;
		mBackward[mObjects.size() - 1] = gId;
		return gId;
	}

	inline Test& Get(uint32_t id)
	{
		return mObjects[mForward[id]];
	}

	inline void Remove(uint32_t id)
	{
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
			mObjects[index] = mObjects.back();

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

	inline std::vector<Test> Objects()
	{
		return mObjects;
	}

private:
	std::vector<Test> mObjects;
	std::unordered_map<uint32_t, size_t> mForward;
	std::unordered_map<size_t, uint32_t> mBackward;
};
