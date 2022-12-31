#pragma once
#include <functional>

class TimerBase
{
public:
	inline bool IsExpired() const;
	inline bool IsRepeat() const;

	inline float Duration() const;
	inline float Elapsed() const;
	inline float Percentage() const;
	inline void Reset();

	const float mDuration;
	float mElapsed;
	bool mRepeat;

protected:
	TimerBase(float duration, bool repeat) :
		mElapsed(0.0f), mDuration(duration), mRepeat(repeat) {}
};

// Intended for simple "fire and forget" callbacks such as logging
using TimerCallback = std::function<void()>;
class Timer : public TimerBase
{
public:
	Timer(float duration, TimerCallback callback, bool repeat) :
		TimerBase(duration, repeat), mCallback(callback) {}

	inline void Tick(float dt);

private:
	TimerCallback mCallback;
};

// Intended for comprehensive frequency-based callbacks such as death sequences
class Process;
using ProcessCallback = std::function<void(const Process&)>;
class Process : public TimerBase
{
public:
	Process(float frequency, float duration, ProcessCallback callback, bool repeat) :
		TimerBase(duration, repeat), mCallback(callback), mFrequency(frequency), mAccumulator(0.0f) {}

	inline float Frequency() const;
	inline void Tick(float dt);

private:
	ProcessCallback mCallback;
	const float mFrequency;
	float mAccumulator;
};

inline bool TimerBase::IsExpired() const
{
	return mElapsed >= mDuration;
}

inline bool TimerBase::IsRepeat() const
{
	return mRepeat;
}

inline float TimerBase::Duration() const
{
	return mDuration;
}

inline float TimerBase::Elapsed() const
{
	return mElapsed;
}

inline float TimerBase::Percentage() const
{
	return mElapsed / mDuration;
}

inline void TimerBase::Reset()
{
	mElapsed = 0.0f;
}

inline float Process::Frequency() const
{
	return mFrequency;
}

inline void Timer::Tick(float dt)
{
	mElapsed += dt;
	if (IsExpired())
	{
		if (IsRepeat())
			Reset();
		mCallback();
	}
}

inline void Process::Tick(float dt)
{
	mElapsed += dt;
	mAccumulator += dt;

	if (mAccumulator > mFrequency && !IsExpired())
	{
		mAccumulator = 0.0;
		mCallback(*this);
	}

	if (IsExpired() && IsRepeat())
	{
		Reset();
	}
}
