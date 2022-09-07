#include "pch.h"
#include "Timers.h"

//std::vector<DX::StepTimer> Timers::sTimers(Timers::COUNT);
std::unordered_map<std::string, Timers::FixedTimer> Timers::sFixedTimers;
std::unordered_map<std::string, Timers::VariableTimer> Timers::sVariableTimers;

void Timers::Tick()
{
	for (auto& [key, val] : sFixedTimers)
		val.timer.Tick(val.callback);

	//for (auto& [key, val] : sVariableTimers)
	//{
	//	val.timer.Tick([]() {
	//		val.callback(val.timer);
	//	});
	//}
}

void Timers::Add(const FixedId& id, double seconds, FixedTimerCallback callback)
{
	FixedTimer fixed;
	fixed.callback = callback;
	fixed.timer.SetTargetElapsedSeconds(seconds);
	fixed.timer.SetFixedTimeStep(true);
	sFixedTimers.insert({ id.name, std::move(fixed) });
}

void Timers::Add(const VariableId& id, VariableTimerCallback callback)
{
	VariableTimer variable;
	variable.callback = callback;
	sVariableTimers.insert({ id.name, std::move(variable) });
}

void Timers::Remove(const FixedId& id)
{
	sFixedTimers.erase(id.name);
}

void Timers::Remove(const VariableId& id)
{
	sVariableTimers.erase(id.name);
}
