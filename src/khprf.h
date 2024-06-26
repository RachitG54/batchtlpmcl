//SPDX-License-Identifier: MIT OR GPL-3.0-only
/*
 * Rachit Garg <rachg96@cs.utexas.edu>
 * 2023
 * */

#ifndef khprfdef
#define khprfdef

#include "btlpincludes.h"

class khprf {
public:
    vector<G1> g1;
    vector<G2> g2;
    int n;
    Fr key;
    G1 punckey;
// public:
	void setup(int sz);
	void setkey();
	void setkey(Fr &key2);
	void setpunckey(G1 &punckey2);
	string getkey();
	void clearkeys();
	void prfeval(GT &result, int i);
	void puncture(int i);
	void punceval(GT &result, int punci, int i);
	void print();
	// ll szbytes();
};

#endif // khprfdef