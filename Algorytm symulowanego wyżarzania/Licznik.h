#pragma once

#include <windows.h>

class Licznik
{
public:

	double PCFreq;
	__int64 CounterStart;

	Licznik();
	~Licznik();
	void StartCounter();
	double GetCounter();
};

