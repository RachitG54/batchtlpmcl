/*
 * Adithya Bhat <bhat24@purdue.edu>
 * 2019
 * */


#include "btlpincludes.h"
#include "params.h"
#include "puzzle.h"
#include "util.h"
#include "btlptime.h"

#ifndef __LHP_HEADER__
#define __LHP_HEADER__

void LHP_PSetup ( LHP_param_t* params , // Parameters to fill
	uint64_t lambda , uint64_t T, int genornot = 0) ;

// void LHP_PGen ( LHP_puzzle_t* puzzle , LHP_param_t* pp , unsigned char* , size_t ) ;
void LHP_PGen ( LHP_puzzle_t* puzzle , LHP_param_t* pp , string str);

void LHP_PGen_seeded ( LHP_puzzle_t* puzzle , 
		LHP_param_t* pp , 
		unsigned char* str , size_t s_size , 
		unsigned char* seed, size_t seed_len 
	) ;

void LHP_PSolve ( LHP_param_t* pp , LHP_puzzle_t* Z , LHP_puzzle_sol_t* ) ;

// Setup variable number of arguments
void LHP_PEval ( LHP_param_t* pp , LHP_puzzle_t* , size_t , LHP_puzzle_t* ) ;
// void LHP_PEval ( LHP_param_t* pp , vector<classbtlp> &btlparray, size_t num_puzzles , LHP_puzzle_t* dest_puzzle );

#endif
