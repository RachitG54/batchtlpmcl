//SPDX-License-Identifier: MIT
/*
 * Rachit Garg
 * 2023
 * */

#include "classuncobtlp.h"
void classuncobtlp::initialize(int d) {
	deg = d;
	btlpvec.resize(deg);
	// T = t;
	// prfvec.resize(d);
	// puzzle_ptr_vec.resize(d);
	// REP(i,0,d-1) {
	// 	puzzle_ptr_vec[i] = NULL;
	// }
	// ctpad_vec.resize(d);
	// slotvec.resize(d);
	// fill(all(slotvec),-1);
}


ll classuncobtlp::szbytes() {
	ll sz = 0;
	// cout << "deg is "<<deg<<"\n";
	REP(i,0,deg-1) {
		sz += btlpvec[i].szbytes();
	}
	return sz;
}