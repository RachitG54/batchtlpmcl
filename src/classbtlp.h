//SPDX-License-Identifier: MIT
/*
 * Rachit Garg
 * 2023
 * */

#ifndef classbtlpdef
#define classbtlpdef

#include "btlpincludes.h"
#include "lhp.h"
#include "util.h"
#include "khprf.h"

class classbtlp {
private:
public:
	int n;
	int slot;
	uint64_t T;
	khprf prf;
	LHP_puzzle_t* puzzle_ptr;
	// LHP_puzzle_t puzzle;
	LHP_puzzle_sol_t solution;
	GT ctpad;
	ll szbytes();
	classbtlp();
	// classbtlp(int nValue, int slotValue, uint64_t TValue, const khprf& prfValue, const LHP_puzzle_t& puzzleValue, const LHP_puzzle_sol_t& solutionValue, const string& ctpadValue);
	// ~classbtlp();
	// functions for size that I will use later
};

#endif // cobtlpdef