#pragma once
#include <ctime>
#include <vector>
#include <string>
namespace Time
{
	static inline float ClockToSec( std::clock_t clock ) { return clock / (float)CLOCKS_PER_SEC; }
	class Stopwatch
	{
	public:
		Stopwatch(): m_started(false) { }

		float GetStartTime() { return m_started ? ClockToSec( m_laps[0] ) : 0.f; }
		
		void Start()
		{
			if (!m_started)
			{
				m_laps.push_back( std::clock() );
				m_started = true;
			}
		}

		float Lap()
		{
			if (m_started)
			{
				size_t i = m_laps.size();
				m_laps.push_back( std::clock() );
				return ClockToSec( m_laps[i] - m_laps[i - 1] );
			}
			else
			{
				return 0.f;
			}
		}

		// TODO: Allow to restart
		float Stop() { m_started = false; m_laps.clear(); }
		
		float GetLap( uint32_t i )
		{
			if (i >= m_laps.size())
			{
				return 0.f;
			}
			else
			{
				return ClockToSec( m_laps[i]);
			}
		}

		float TotalTime()
		{
			return m_started ? ClockToSec( m_laps.back() ) - GetStartTime() : 0.f;
		}

	private:
		std::vector<std::clock_t> m_laps;
		bool m_started;

	};
}