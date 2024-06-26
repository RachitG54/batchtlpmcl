//SPDX-License-Identifier: MIT OR GPL-3.0-only
/*
 * Rachit Garg <rachg96@cs.utexas.edu>
 * 2023
 * */

#include "classbtlp.h"
classbtlp::classbtlp() {
	puzzle_ptr = NULL;
	slot = -1;
}

ll classbtlp::szbytes() {
	ll sz = 0;
	sz += 4; // for slot
	sz += 8; // for time

	sz += getmclbytes(prf.punckey);

	sz += getmclbytes(ctpad);

	sz += getmpzbytes(puzzle_ptr->u);
	sz += getmpzbytes(puzzle_ptr->v);
	
	// cout << getmclbytes(prf.punckey)<<" ";
	// cout << getmclbytes(ctpad)<<" ";
	// cout << getmpzbytes(puzzle_ptr->u)<<" ";
	// cout << getmpzbytes(puzzle_ptr->v)<<"\n";

	// cout << "size is "<<sz<<"\n";

	return sz;
}

// classbtlp::classbtlp(int nValue, int slotValue, uint64_t TValue, const khprf& prfValue, const LHP_puzzle_t& puzzleValue, const LHP_puzzle_sol_t& solutionValue, const std::string& ctpadValue) {
//     n=nValue; slot = slotValue; T = TValue; prf = prfValue; puzzle = puzzleValue; solution=solutionValue; ctpad = ctpadValue;
// }

// classbtlp::~classbtlp() {
//     LHP_clear_puzzle (&puzzle);
// }