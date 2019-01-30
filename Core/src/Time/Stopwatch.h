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
		Stopwatch()
			: m_started(false) 
			, m_paused(false)
		{ }

		float GetStartTime() { return m_started ? ClockToSec( m_laps[0] ) : 0.f; }
		
		void Start()
		{
			if (!m_paused)
			{
				m_laps.push_back( std::clock() );
			}
			else
			{
				// Shift all the times up so the next lap doesn't include stopped time
				std::clock_t timeShift = std::clock() - m_laps.back();
				for (std::clock_t time : m_laps)
				{
					time += timeShift;
				}
			}
			m_paused = false;
			m_started = true;
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

		void Stop() 
		{ 
			m_paused = true;
			// Count a stop as a lap, we'll need this later
			m_laps.push_back( std::clock() );
		}
		
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
		bool m_paused;

	};
}