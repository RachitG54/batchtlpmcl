#ifndef classuncobtlpdef
#define classuncobtlpdef

#include "btlpincludes.h"
#include "lhp.h"
#include "util.h"
#include "khprf.h"
#include "classbtlp.h"

class classuncobtlp {
private:
public:
	int deg;
	// uint64_t T;
	// vi slotvec;
	vector<classbtlp> btlpvec;
	// vector<khprf> prfvec;
	// vector<LHP_puzzle_t*> puzzle_ptr_vec;
	// LHP_puzzle_t puzzle;
	// LHP_puzzle_sol_t solution;
	// vector<GT> ctpad_vec;
	void initialize(int d);
	ll szbytes();
	// classbtlp(int nValue, int slotValue, uint64_t TValue, const khprf& prfValue, const LHP_puzzle_t& puzzleValue, const LHP_puzzle_sol_t& solutionValue, const string& ctpadValue);
	// ~classbtlp();
	// functions for size that I will use later
};

#endif // classuncobtlpdef