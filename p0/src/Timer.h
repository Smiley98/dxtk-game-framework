#pragma once
#include <functional>

// Callback should be a simple "fire and forget" function.
// Prefer processes over repeating timers for frequency-based updates.
using TimerCallback = std::function<void()>;

class Timer
{
public:
	Timer(float duration, TimerCallback callback, bool repeat) :
		mDuration(duration), mCallback(callback), mRepeat(repeat), mElapsed(0.0f) {}

	inline bool IsExpired() const;
	inline bool IsRepeat() const;

	inline float Duration() const;
	inline float Elapsed() const;
	inline float Percentage() const;

	inline void Tick(float dt);
	inline void Reset();

	// Neat ideas but also makes things confusing, especially with repeat functionality.
	//inline void Complete(); -- fires callback and makes elapsed > duration
	//inline void Expire(); -- makes elapsed > duration but doesn't fire callback

private:
	TimerCallback mCallback;
	bool mRepeat;

	float mElapsed;
	const float mDuration;
};

inline bool Timer::IsExpired() const
{
	return mElapsed >= mDuration;
}

inline bool Timer::IsRepeat() const
{
	return mRepeat;
}

inline float Timer::Duration() const
{
	return mDuration;
}

inline float Timer::Elapsed() const
{
	return mElapsed;
}

inline float Timer::Percentage() const
{
	return mElapsed / mDuration;
}

inline void Timer::Tick(float dt)
{
	mElapsed += dt;
	if (IsExpired())
	{
		mCallback();
		if (IsRepeat())
			Reset();
	}
}

inline void Timer::Reset()
{
	mElapsed = 0.0f;
}
