#ifdef _WIN32
       #include <windows.h>
#else
       #include <sys/time.h>
#endif

typedef struct {
#ifdef _WIN32
    LARGE_INTEGER start;
    LARGE_INTEGER stop;
#else
     timeval start;
     timeval stop;
     struct timezone tz;
#endif
} stopWatch;

class CStopWatch {

private:
	stopWatch timer;
#ifdef _WIN32	
	LARGE_INTEGER frequency;
	double LIToSecs( LARGE_INTEGER & L);
#endif
public:
	CStopWatch();
	void startTimer( );
	void stopTimer( );
	double getElapsedTime();
};
