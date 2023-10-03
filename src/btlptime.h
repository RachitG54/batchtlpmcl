#ifndef btlptime_H
#define btlptime_H

#include "btlpincludes.h"

class btlptime {
private:
    high_resolution_clock::time_point start = high_resolution_clock::now();
    high_resolution_clock::time_point stop = high_resolution_clock::now();
    microseconds duration = duration_cast<microseconds>(stop - start);
public:
    btlptime();
    void starttime();
    void donetime();
    void printtime(int cont);
	uint64_t getTime();
    uint64_t getTimefromstart(); 
	void resetTime();
	uint64_t getAndResetTime();
};

extern btlptime timerCRSgentime;
extern btlptime timerpairCRSgentime;
extern btlptime timerpuzzleCRSgentime;
extern btlptime timergentime;
extern btlptime timerpairgentime;
extern btlptime timerpuzzlegentime;
extern btlptime timersolvetime;
extern btlptime timerpairsolvetime;
extern btlptime timerpuzzlesolvetime;
extern btlptime timerbatchsolvetime;
extern btlptime timerpairbatchsolvetime;
extern btlptime timerpuzzlebatchsolvetime;
extern btlptime timergraphbatchsolvetime;
extern btlptime timerrsagentime;
extern btlptime timerrsaadjusttime;

#endif // btlptime_H