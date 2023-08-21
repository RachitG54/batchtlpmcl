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
	void solvetlp(int i);
	void batchsolvetlp();
	void cleantlp(int i);
	void cleantlp();
	// void copypp();
};

#endif // cobtlpdef