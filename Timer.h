#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

namespace Rendering {
	class Timer {
	public:
		Timer() : startTicks(0), endTicks(0) {
			QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
		}

		void Start() {
			startTicks = endTicks = GetTimeNow();
		}
		int Stop() {
			endTicks = GetTimeNow();
			return GetTicksElapsed();
		}
		void Restart() {
			Start();
		}
		int GetTicksElapsed() {
			return int(GetTimeNow() - startTicks);
		}
		int GetTicksPerMillisecond() { return (int)frequency; }
		double GetElapsedTime() {
			return double(GetTicksElapsed()) / double(GetTicksPerMillisecond());
		}
		bool IsStopped() { return endTicks != startTicks; }
	private:
		__int64 GetTimeNow() {
			__int64 time;
			QueryPerformanceCounter((LARGE_INTEGER*)&time);
			return time;
		}
		__int64 frequency; //frequency of timer
		__int64 startTicks; //start time
		__int64 endTicks; //end time
	};
}

#endif