#ifndef uncobtlpdef
#define uncobtlpdef

#include "btlpincludes.h"
#include "lhp.h"
#include "util.h"
#include "khprf.h"

#define SEC_PARAM 1024

using namespace std;

class uncobtlp {
private:
	int n;
	uint64_t T;
	khprf pp;
	LHP_param_t param ;
	LHP_puzzle_t puzzle ;
	LHP_puzzle_t *puzzle_array ;
	LHP_puzzle_t dest_puzzle ;
	LHP_puzzle_sol_t solution ;
	unsigned char* str = (unsigned char*)"111111111111111111111"
	"11111111111111111111111111111"
	"11111111111111111111111111111"
	"111111111111111111111" ;
	int len = 100 ;

public:
	void initialize(int number);
	void setuptlp(uint64_t t);
	void gentlp();
	void solvetlp();
	void batchtlp();
	void cleantlp();
};

#endif // btlpdef