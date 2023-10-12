//SPDX-License-Identifier: MIT
/*
 * Rachit Garg
 * 2023
 * */

#ifndef uncobtlpdef
#define uncobtlpdef

#include "btlpincludes.h"
#include "lhp.h"
#include "util.h"
#include "khprf.h"
#include "cobtlp.h"
#include "classuncobtlp.h"
#include "graph.h"
#include "btlptime.h"

using namespace std;

class uncobtlp {
private:

public:
	cobtlp cobtlpinst;
	uint64_t T;
	int n_left;
	int n_right;
	int deg;
	// vector<classuncobtlp> uncobtlparray;
	void initialize(int noleft, int noright, int deg, uint64_t t, int genornot = 0);
	void gentlp(GT &gtelt,classuncobtlp &newtlp);
	void solvetlp(GT &result,classuncobtlp &newtlp);
	void batchsolvetlp(vector<GT> &result, vector<classuncobtlp> &batcharray);

	void storetlp(string &str);
	void loadtlp(string &str);

	ll crsszbytes();
	// int gettlpidx(int i);
	// void solvetlp(int idx, GT &result);
	// //Assuming we take in an index of values for setS
	// void batchsolvetlp(vi &setS, vector<GT> &result);
	// void cleantlp(int i);
	// void cleantlp();
};

#endif // unbtlpdef