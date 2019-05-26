#pragma once
#include <chrono>
class Clock
{
public:
	Clock();
	double getMilisecondsElapsed();
	void Restart();
	bool Stop();
	bool Start();
	~Clock();
private:
	bool isrunning = false;

	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> stop;
};

