#include "Timer.h"

Timer::Timer(bool a_run)
{
	if (a_run)
	{
		Reset();
	}
}

void Timer::Reset()
{
	m_start = hrClock_t::now();
}

std::chrono::duration<double> Timer::Elapsed() const
{
	auto timeNow = hrClock_t::now();

	return std::chrono::duration_cast<seconds_t>(timeNow - m_start);
}