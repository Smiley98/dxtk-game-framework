#pragma once
#include "StepTimer.h"
#include <functional>
#include <unordered_map>
#include <string>
//#include <vector>

using TimerCallback = std::function<void()>;

class Timers
{
public:
	//enum Type
	//{
	//	LOG,
	//	COUNT
	//};

	static void Tick();
	static void Add(const std::string& name, double seconds, TimerCallback callback);
	static void Remove(const std::string& name);

private:
	struct Timer
	{
		TimerCallback callback;
		DX::StepTimer step;
	};
	static std::unordered_map<std::string, Timer> sTimerCallbacks;
};
