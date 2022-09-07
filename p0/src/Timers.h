#pragma once
#include "StepTimer.h"
#include <functional>
#include <unordered_map>
#include <string>
// If timers are known we can store them in a vector
// Could also use my MappedVector class, but its not as versatile as stl and ticking doesn't need to be cache-friendly

using FixedTimerCallback = std::function<void()>;
using VariableTimerCallback = std::function<void(const DX::StepTimer& timer)>;

struct FixedId
{
	std::string name;
};

struct VariableId
{
	std::string name;
};

class Timers
{
public:
	static void Tick();

	static void Add(const FixedId& id, double seconds, FixedTimerCallback callback);
	static void Add(const VariableId& id, VariableTimerCallback callback);
	static void Remove(const FixedId& id);
	static void Remove(const VariableId& id);

private:
	struct FixedTimer
	{
		FixedTimerCallback callback;
		DX::StepTimer timer;
	};

	struct VariableTimer
	{
		VariableTimerCallback callback;
		DX::StepTimer timer;
	};

	static std::unordered_map<std::string, FixedTimer> sFixedTimers;
	static std::unordered_map<std::string, VariableTimer> sVariableTimers;
};
