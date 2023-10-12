//SPDX-License-Identifier: MIT
/*
 * Rachit Garg
 * 2023
 * */

#include "btlp.h"

void btlp::initialize (int number) {
	n = number;
	// LHP_init_puzzle ( &dest_puzzle ) ;
	puzzle_array = (LHP_puzzle_t*) malloc ( sizeof ( LHP_puzzle_t ) * n ) ;
}

void btlp::setuptlp(uint64_t t) {
	T = t;
	LHP_init_param ( &param ) ;
	LHP_PSetup ( &param , SEC_PARAM , T) ;
	SUCCESS ( "Setup" ) ;
}

void btlp::gentlp() {
	LHP_init_puzzle ( &puzzle ) ;
	LHP_PGen ( &puzzle , &param , str) ;
	SUCCESS ( "Gen" ) ;
}


// Experiment 3 : Measure time for PSolve
//Solves generated puzzle
void btlp::solvetlp ()
{
	mpz_init ( solution .s ) ;
	LHP_PSolve ( &param , &puzzle , &solution ) ;
	mpz_t num ;
	mpz_init_set_ui ( num , 0 ) ;
	int len = str.size();
	for( int i = 0 ; i < len ; i++ ) {
		mpz_mul_ui ( num , num , 1 << 8 ) ;
		mpz_add_ui ( num , num , (uint8_t)str[i] ) ;
	}
	if ( mpz_cmp ( num , solution .s ) == 0 ) {
		SUCCESS ( "Solver" ) ;
	}
	else {
		FAILED ( "Solver" ) ;
	}
	mpz_clear (num) ;
}

// Experiment 4 : Measure time for PEval
// Batches into a destination puzzle, then solves the homomorphic computation to check
void btlp::batchtlp ()
{
	// Preparing for Experiment 4
	mpz_t num ;
	mpz_init_set_ui ( num , 0 ) ;
	int len = str.size();
	for( int i = 0 ; i < len ; i++ ) {
		mpz_mul_ui ( num , num , 1 << 8 ) ;
		mpz_add_ui ( num , num , (uint8_t)str[i] ) ;
	}
	LHP_init_puzzle ( &dest_puzzle ) ;
	for ( int i = 0 ; i < n ; i ++ ) {
		LHP_init_puzzle ( puzzle_array + i ) ;
		LHP_PGen ( puzzle_array + i , &param , str) ;
	}
	LHP_PEval ( &param , puzzle_array , n , &dest_puzzle ) ;
	LHP_PSolve ( &param , &dest_puzzle , &solution ) ;
	mpz_mul_ui ( num , num , n ) ;
	if ( mpz_cmp ( num , solution .s ) == 0 ) {
		SUCCESS ( "Batch" ) ;
	}
	else {
		FAILED ( "Batch" ) ;
	}
	mpz_clear (num) ;
}
// Cleans the tlp
void btlp::cleantlp ()
{
	for ( int i = 0 ; i < n ; i ++ ) {
		LHP_clear_puzzle ( puzzle_array + i ) ;
	}
	LHP_clear_param ( &param ) ;
	LHP_clear_puzzle ( &puzzle ) ;
	LHP_clear_puzzle ( &dest_puzzle ) ;
	free ( puzzle_array ) ;
}