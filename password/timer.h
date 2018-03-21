//
//  timer.h
//  password-mac
//
//  Janky cross-platform Timer class, because
//  (surprise!) Visual Studio 2013 implementation of
//  high resolution clock has a several ms resolution...
//
//  Created by Sanjay Madhav on 12/26/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once

#ifdef _WIN32
// Windows
#include <windows.h>
#undef max
#else
// Mac
#include <chrono>
#endif

class Timer
{
public:
	// Construct the timer
	Timer();
	
	// Start the timer
	void start();
	
	// Get the amount of elapsed time, in seconds since start was called
	double getElapsed();
	
private:
#ifdef _WIN32
	LARGE_INTEGER mFreq;
	LARGE_INTEGER mStart;
#else
	std::chrono::high_resolution_clock::time_point mStart;
#endif
};

