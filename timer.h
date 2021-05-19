#ifndef TIMER_HEAD_H_2021_05_18
#define TIMER_HEAD_H_2021_05_18

#include <chrono>

class Timer {
private:
	
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<clock_t> m_pBegin;

public:
	Timer() : m_pBegin(clock_t::now()) {}

	void reset() {
		m_pBegin = clock_t::now();
	}

	double elapsed() const {
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_pBegin).count();
	}
}; 

#endif