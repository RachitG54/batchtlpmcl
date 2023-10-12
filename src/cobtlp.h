//SPDX-License-Identifier: MIT
/*
 * Rachit Garg
 * 2023
 * */

#ifndef cobtlpdef
#define cobtlpdef

#include "btlpincludes.h"
#include "classbtlp.h"
#include "lhp.h"
#include "util.h"
#include "puzzle.h"
#include "khprf.h"
#include "btlptime.h"

using namespace std;

class cobtlp {
private:
public:
	int n;
	uint64_t T;
	khprf prf;
	LHP_param_t param ;
	vector<classbtlp> btlparray;

	void initialize(int number, uint64_t t, int genornot = 0);
	// void setuptlp(int number, int i, uint64_t t);
	void gentlp(int i, GT &gtelt);
	void gentlp(GT &gtelt, classbtlp &tlpinst);
	// int gettlpidx(int i);
	void solvetlp(int idx, GT &result);
	void solvetlp(GT &result, classbtlp &tlpinst);
	//Assuming we take in an index of values for setS
	void batchsolvetlp(vi &setS, vector<GT> &result);
	void batchsolvetlp(vector<GT> &result, vector<classbtlp> &batcharray);
	void cleantlp(int i);
	void cleantlp(vector<classbtlp> &batcharray);
	void cleantlp();

	ll crsszbytes();
	// void copypp();
};

#endif // cobtlpdef