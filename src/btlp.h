//SPDX-License-Identifier: MIT
/*
 * Rachit Garg
 * 2023
 * */

#ifndef btlpdef
#define btlpdef

#include "btlpincludes.h"
#include "lhp.h"
#include "util.h"
#include "khprf.h"

using namespace std;

class btlp {
private:
	int n;
	uint64_t T;
	khprf prf;
	LHP_param_t param ;
	LHP_puzzle_t puzzle ;
	LHP_puzzle_t *puzzle_array ;
	LHP_puzzle_t dest_puzzle ;
	LHP_puzzle_sol_t solution ;
	string str = "1111";
	// unsigned char* str = (unsigned char*)"111111111111111111111"
	// "11111111111111111111111111111"
	// "11111111111111111111111111111"
	// "111111111111111111111" ;
	// int len = 100 ;

public:
	void initialize(int number);
	void setuptlp(uint64_t t);
	void gentlp();
	void solvetlp();
	void batchtlp();
	void cleantlp();
};

#endif // btlpdef