#include "pch.h"
#include "Timers.h"

//std::vector<DX::StepTimer> Timers::sTimers(Timers::COUNT);
std::unordered_map<std::string, Timers::Timer> Timers::sTimerCallbacks;

void Timers::Tick()
{
	for (auto& [key, val] : sTimerCallbacks)
	{
		val.step.Tick(val.callback);
	}
}

void Timers::Add(const std::string& name, double seconds, TimerCallback callback)
{
	Timer timer;
	timer.callback = callback;
	timer.step.SetTargetElapsedSeconds(seconds);
	timer.step.SetFixedTimeStep(true);
	// fixed is needed due to internal timer logic; if variable, tick as fast as possible and write delta to timer
	sTimerCallbacks.insert({ name, std::move(timer) });
}

void Timers::Remove(const std::string& name)
{
	sTimerCallbacks.erase(name);
}
