#ifdef _WIN32
       #include <windows.h>
#else
       #include <sys/time.h>
#endif

#ifndef hr_timer
#include "hr_time.h"
#define hr_timer
#endif

#ifdef _WIN32
double CStopWatch::LIToSecs( LARGE_INTEGER & L) {
	return ((double)L.QuadPart /(double)frequency.QuadPart);
}

CStopWatch::CStopWatch(){
	timer.start.QuadPart=0;
	timer.stop.QuadPart=0;	
	QueryPerformanceFrequency( &frequency );
}

void CStopWatch::startTimer( ) {
    QueryPerformanceCounter(&timer.start);
}

void CStopWatch::stopTimer( ) {
    QueryPerformanceCounter(&timer.stop);
}


double CStopWatch::getElapsedTime() {
	LARGE_INTEGER time;
	time.QuadPart = timer.stop.QuadPart - timer.start.QuadPart;
    return LIToSecs( time) ;
}
#else
CStopWatch::CStopWatch(){
//	timer.start=0;
//	timer.stop=0;
}

void CStopWatch::startTimer( ) {
//    QueryPerformanceCounter(&timer.start);
    gettimeofday(&timer.start,&timer.tz);
}

void CStopWatch::stopTimer( ) {
//    QueryPerformanceCounter(&timer.stop);
    gettimeofday(&timer.stop,&timer.tz);
}


double CStopWatch::getElapsedTime() {
//	LARGE_INTEGER time;
//	time.QuadPart = timer.stop.QuadPart - timer.start.QuadPart;
//    return LIToSecs( time) ;
        double t1, t2;

        t1 =  (double)timer.start.tv_sec + (double)timer.start.tv_usec/(1000*1000);
        t2 =  (double)timer.stop.tv_sec + (double)timer.stop.tv_usec/(1000*1000);
        return t2-t1;

}

#endif
