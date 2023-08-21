#include "cobtlp.h"

void cobtlp::initialize(int number, uint64_t t) {
	n = number;
	T = t;
	
	LHP_init_param ( &param ) ;
	LHP_PSetup ( &param , SEC_PARAM , T) ;
	SUCCESS ( "LHP initialized" ) ;
	prf.setup(n);
	btlparray.resize(n+1);
	SUCCESS ( "PRF initialized" ) ;
	// SUCCESS ( "initialized" ) ;
}

// void cobtlp::setuptlp(int i) {
// 	slot = i;
// 	if(slot > n) {
// 		handleErrors("Setting incorrect slot number.");
// 	}
// 	LHP_init_param ( &param ) ;
// 	LHP_PSetup ( &param , SEC_PARAM , T) ;
// 	prf.setup(n);
// 	SUCCESS ( "Setup" ) ;
// }

void cobtlp::gentlp(int i, GT &gtelt) {
	if(i > n) {
		handleErrors("Setting incorrect slot number.");
	}

    prf.setkey();
    prf.puncture(i);
    string keystr = prf.getkey();
    // prf.clearkey();

    btlparray[i].puzzle_ptr = new LHP_puzzle_t;
	LHP_init_puzzle ( btlparray[i].puzzle_ptr) ;
	/*
	Check encoding is possible or not, it is for our current implementation because l is one and we're mapping Z_p* to
	an element in the time lock puzzle.
	*/

	// cout<<"keystr is "<<keystr<<"\n";

	// unsigned char* keybytes = (unsigned char*)keystr.c_str();
	// size_t keybyteslen = keystr.size()+1;

	// LHP_PGen ( btlparray[i].puzzle_ptr , &param , keybytes , keybyteslen) ;

	LHP_PGen ( btlparray[i].puzzle_ptr , &param , keystr) ;

	GT evalGT;
	prf.prfeval(evalGT, i);

	GT::mul(btlparray[i].ctpad,evalGT,gtelt);
	// cout << result <<"\n";
	SUCCESS ( "Gen" ) ;
}


// Experiment 3 : Measure time for PSolve
// //Solves generated puzzle

void cobtlp::solvetlp (int i)
{
	if(btlparray[i].puzzle_ptr == NULL) {
		cerr << "Solving an uninitialized puzzle\n";
		return;
	}
	mpz_init (btlparray[i].solution.s ) ;
	LHP_PSolve ( &param , btlparray[i].puzzle_ptr , &btlparray[i].solution ) ;
	gmp_printf("Result: %Zd\n", btlparray[i].solution.s);

	// size_t bufferSize = mpz_sizeinbase(btlparray[i].solution.s, 256)+2;
	// unsigned char* byteBuffer = new unsigned char[bufferSize]();
	// mpz_export(byteBuffer, nullptr, 1, sizeof(unsigned char), 0, 0, btlparray[i].solution.s);

	// mpz_import(btlparray[i].solution.s, bufferSize, 1, sizeof(unsigned char), 0, 0, byteBuffer);
	// cout << "bytebuffer is "<<byteBuffer<<"\narr is ";
	// REP(i,0,bufferSize-1) {
	// 	cout<<(int)byteBuffer[i]<<" ";
	// }
	// cout<<"\n";

	// string answer = convertToBase10String(byteBuffer,bufferSize);
	// cout << "answer is "<<answer<<"\n";
}

void cobtlp::batchsolvetlp ()
{
	// Preparing for Experiment 4
	// mpz_t num ;
	// mpz_init_set_ui ( num , 0 ) ;
	// for( int i = 0 ; i < len ; i++ ) {
	// 	mpz_mul_ui ( num , num , 1 << 8 ) ;
	// 	mpz_add_ui ( num , num , (uint8_t)str[i] ) ;
	// }

	LHP_puzzle_t *puzzle_array = new LHP_puzzle_t[n];

	int newn = 0;
	for ( int i = 0; i < n+1 ; i ++ ) {
		if(btlparray[i].puzzle_ptr != NULL) {
			puzzle_array[newn] = *(btlparray[i].puzzle_ptr);
			newn++;
		}
	}
	cout << "Batching " << newn << " things.\n";

	LHP_puzzle_sol_t solution ;
	mpz_init (solution.s ) ;

	LHP_puzzle_t dest_puzzle;
	LHP_init_puzzle ( &dest_puzzle ) ;
	LHP_PEval ( &param , puzzle_array , newn , &dest_puzzle ) ;
	LHP_PSolve ( &param , &dest_puzzle , &solution ) ;

	// LHP_puzzle_sol_t solution1 ;
	// mpz_init (solution1.s ) ;
	// LHP_PSolve ( &param , &puzzle_array[0] , &solution1 ) ;

	// LHP_puzzle_sol_t solution2 ;
	// mpz_init (solution2.s ) ;
	// LHP_PSolve ( &param , &puzzle_array[1] , &solution2 ) ;


	// gmp_printf("Result: %Zd\n", solution1.s);
	// gmp_printf("Result: %Zd\n", solution2.s);

	// mpz_t result;
	// mpz_init_set_ui(result,0);
	// mpz_add(result,solution1.s,solution2.s);


	// gmp_printf("Result: %Zd\n", result);
	// gmp_printf("Result: %Zd\n", solution.s);

	// printinbase10(result);
	// printinbase10(solution.s);

	// if ( mpz_cmp ( result , solution .s ) == 0 ) {
	// 	SUCCESS ( "Batch" ) ;
	// }
	// else {
	// 	FAILED ( "Batch" ) ;
	// }

	// // LHP_puzzle_sol_t solution ;

	// // mpz_init ( solution .s ) ;
	// LHP_init_puzzle ( &dest_puzzle ) ;
	// for ( int i = 0 ; i < n ; i ++ ) {
	// 	LHP_init_puzzle ( puzzle_array + i ) ;
	// 	LHP_PGen ( puzzle_array + i , &param , str, len ) ;
	// }
	// LHP_PEval ( &param , puzzle_array , n , &dest_puzzle ) ;
	// LHP_PSolve ( &param , &dest_puzzle , &solution ) ;
	// mpz_mul_ui ( num , num , n ) ;
	// if ( mpz_cmp ( num , solution .s ) == 0 ) {
	// 	SUCCESS ( "Batch" ) ;
	// }
	// else {
	// 	FAILED ( "Batch" ) ;
	// }
	// mpz_clear (num) ;
}
// Cleans the tlp
void cobtlp::cleantlp (int i)
{
	if(i > n) {
		handleErrors("Cleaning incorrect slot number.");
	}
	// else if (btlparray[i].puzzle_ptr==NULL) {
	// 	cerr << "Cleaning uninitialized puzzle\n";
	// }
	// for ( int i = 0 ; i < n ; i ++ ) {
	// 	LHP_clear_puzzle ( puzzle_array + i ) ;
	// }
	// LHP_clear_param ( &param ) ;
	LHP_clear_puzzle ( btlparray[i].puzzle_ptr);
	// free ( puzzle_array ) ;
}

void cobtlp::cleantlp ()
{
	for ( int i = 0 ; i < n+1 ; i ++ ) {
		LHP_clear_puzzle (btlparray[i].puzzle_ptr) ;
	}
	LHP_clear_param ( &param ) ;
	// LHP_clear_puzzle ( &btlparray[i].puzzle);
	// free ( puzzle_array ) ;
}