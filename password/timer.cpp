//
//  timer.cpp
//  password-mac
//
//  Created by Sanjay Madhav on 12/26/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "timer.h"

Timer::Timer()
{
#ifdef _WIN32
	// Windows
	QueryPerformanceFrequency(&mFreq);
	QueryPerformanceCounter(&mStart);
#else
	// Mac
#endif
}

void Timer::start()
{
#ifdef _WIN32
	// Windows
	QueryPerformanceFrequency(&mFreq);
	QueryPerformanceCounter(&mStart);
#else
	// Mac
	mStart = std::chrono::high_resolution_clock::now();
#endif
}

double Timer::getElapsed()
{
#ifdef _WIN32
	// Windows
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
	double elapsed = static_cast<double>(end.QuadPart - mStart.QuadPart) / mFreq.QuadPart;
	return elapsed;
#else
	// Mac
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - mStart).count();
	return duration / 1000000000.0;
#endif
}
