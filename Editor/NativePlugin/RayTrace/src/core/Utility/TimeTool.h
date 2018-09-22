#pragma once




#ifdef WIN32

#include <Windows.h>
typedef LARGE_INTEGER PTime;

inline PTime getPerformanceTime(){
	PTime nLast;
	QueryPerformanceCounter(&nLast);
	return nLast;
}
inline float getDiffTime(const PTime& timeNow,const PTime& timeLast){
	LARGE_INTEGER nFreq;
	QueryPerformanceFrequency(&nFreq);
	return float(timeLast.QuadPart - timeNow.QuadPart)/nFreq.QuadPart;
}
#else
#include <time.h>
typedef signed long long PTime;
inline PTime getPerformanceTime(){
	PTime nLast;
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	return tp.tv_sec * 1000000 + tp.tv_nsec/1000;
}
inline float getDiffTime(const PTime& timeNow, const PTime& timeLast){
	return float(timeLast - timeNow) / 1000000.0f;
}


#endif
