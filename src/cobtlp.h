#ifndef cobtlpdef
#define cobtlpdef

#include "btlpincludes.h"
#include "classbtlp.h"
#include "lhp.h"
#include "util.h"
#include "puzzle.h"
#include "khprf.h"

using namespace std;

class cobtlp {
private:
	int n;
	uint64_t T;
	khprf prf;
	LHP_param_t param ;
	vector<classbtlp> btlparray;

public:
	void initialize(int number, uint64_t t);
	// void setuptlp(int number, int i, uint64_t t);
	void gentlp(int i, GT &gtelt);
	// int gettlpidx(int i);
	void solvetlp(int idx, GT &result);
	//Assuming we take in an index of values for setS
	void batchsolvetlp(vi &setS, vector<GT> &result);
	void cleantlp(int i);
	void cleantlp();
	// void copypp();
};

#endif // cobtlpdef