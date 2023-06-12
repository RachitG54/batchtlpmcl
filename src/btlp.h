#ifndef btlpdef
#define btlpdef

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <gmp.h>
#include "lhp.h"
#include "util.h"
#include "khprf.h"

using namespace std;

class btlp {
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
	void initialize();
	void setuptlp(uint64_t t);
};

#endif // btlpdef