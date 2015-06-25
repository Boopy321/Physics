#pragma once
#include <chrono>

class Timer
{
	typedef std::chrono::high_resolution_clock hrClock_t;
	typedef std::chrono::duration<double> seconds_t;
private:
	hrClock_t::time_point m_start;

public:
	Timer(bool a_run);

	void Reset();

	seconds_t Elapsed() const;
};