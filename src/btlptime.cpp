#include "btlptime.h"

btlptime timerCRSgentime;
btlptime timerpairCRSgentime;
btlptime timerpuzzleCRSgentime;
btlptime timergentime;
btlptime timerpairgentime;
btlptime timerpuzzlegentime;
btlptime timersolvetime;
btlptime timerpairsolvetime;
btlptime timerpuzzlesolvetime;
btlptime timerbatchsolvetime;
btlptime timerpairbatchsolvetime;
btlptime timerpuzzlebatchsolvetime;
btlptime timergraphbatchsolvetime;

btlptime::btlptime() {
	start = high_resolution_clock::now();
	duration = duration_cast<microseconds>(start - start);
}
void btlptime::starttime() {
	start = high_resolution_clock::now();
}

void btlptime::donetime() {
    stop = high_resolution_clock::now();
    duration += duration_cast<microseconds>(stop - start);
}
void btlptime::printtime(int cont) {
    cout << (duration).count() << " microseconds" << endl;
	if(cont != 1)duration = duration_cast<microseconds>(start - start);
}

uint64_t btlptime::getTime(){
	return (duration).count();
}

uint64_t btlptime::getTimefromstart() {
	high_resolution_clock::time_point now = high_resolution_clock::now();
    return (duration_cast<seconds>(now - start)).count();
}

void btlptime::resetTime(){
	duration = duration_cast<microseconds>(start - start);
}

uint64_t btlptime::getAndResetTime(){
	microseconds temp = duration;
	duration = duration_cast<microseconds>(start - start);
	start = high_resolution_clock::now();
	return (temp).count();
}
