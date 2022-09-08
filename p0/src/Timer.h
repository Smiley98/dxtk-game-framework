#pragma once
#include <functional>

using TimerCallback = std::function<void()>;

class Timer
{
public:
	Timer(float duration, TimerCallback callback, bool repeat) :
		mDuration(duration), mCallback(callback), mRepeat(repeat), mElapsed(0.0f) {}

	inline bool IsExpired() const;
	inline bool IsRepeat() const;
	inline float Duration() const;

	inline void Update(float dt);
	inline void Reset();

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

inline void Timer::Update(float dt)
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

// unordered_map doesn't require a default constructor so we can keep Timer immutable!
/*
inline void SetDuration(float duration);
inline void SetRepeat(bool repeat);
inline void SetCallback(TimerCallback callback);

inline void Timer::SetDuration(float duration)
{
	mDuration = duration;
}

inline void Timer::SetRepeat(bool repeat)
{
	mRepeat = repeat;
}

inline void Timer::SetCallback(TimerCallback callback)
{
	mCallback = callback;
}
*/