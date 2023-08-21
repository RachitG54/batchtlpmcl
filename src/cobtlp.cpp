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
	LHP_init_puzzle ( &btlparray[i].puzzle) ;
	/*
	Check encoding is possible or not, it is for our current implementation because l is one and we're mapping Z_p* to
	an element in the time lock puzzle.
	*/
	// keystr = "300";
	cout<<"keystr is "<<keystr<<"\n";

    size_t keybyteslen = keystr.size()/2+2;
    unsigned char* keybytes = new unsigned char[keybyteslen]();

	convertToByteString(keystr, keybytes, keybyteslen);
	// cout<<"encoded string is "<<keybytes<<"\n";
	// cout<<"here\n";
	// cout<<"keybytes is "<<keybytes<<"\narr is ";
	// REP(i,0,keybyteslen-1) {
	// 	cout <<(int)keybytes[i]<<" ";
	// }
	// cout<<"\n";
	LHP_PGen ( &btlparray[i].puzzle , &param , keybytes , keybyteslen) ;

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
	mpz_init (btlparray[i].solution.s ) ;
	LHP_PSolve ( &param , &btlparray[i].puzzle , &btlparray[i].solution ) ;

	size_t bufferSize = mpz_sizeinbase(btlparray[i].solution.s, 256)+2;
	unsigned char* byteBuffer = new unsigned char[bufferSize]();
	mpz_export(byteBuffer, nullptr, 1, sizeof(unsigned char), 0, 0, btlparray[i].solution.s);

	// mpz_import(btlparray[i].solution.s, bufferSize, 1, sizeof(unsigned char), 0, 0, byteBuffer);
	// cout << "bytebuffer is "<<byteBuffer<<"\narr is ";
	// REP(i,0,bufferSize-1) {
	// 	cout<<(int)byteBuffer[i]<<" ";
	// }
	// cout<<"\n";

	string answer = convertToBase10String(byteBuffer,bufferSize);
	cout << "answer is "<<answer<<"\n";
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

	LHP_puzzle_t* dest_puzzle;
	// LHP_puzzle_sol_t solution ;

	// mpz_init ( solution .s ) ;
	LHP_init_puzzle ( &dest_puzzle ) ;
	for ( int i = 0 ; i < n ; i ++ ) {
		LHP_init_puzzle ( puzzle_array + i ) ;
		LHP_PGen ( puzzle_array + i , &param , str, len ) ;
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
void cobtlp::cleantlp (int i)
{
	// for ( int i = 0 ; i < n ; i ++ ) {
	// 	LHP_clear_puzzle ( puzzle_array + i ) ;
	// }
	// LHP_clear_param ( &param ) ;
	LHP_clear_puzzle ( &btlparray[i].puzzle);
	// free ( puzzle_array ) ;
}

void cobtlp::cleantlp ()
{
	for ( int i = 0 ; i < n ; i ++ ) {
		LHP_clear_puzzle (&btlparray[i].puzzle) ;
	}
	LHP_clear_param ( &param ) ;
	// LHP_clear_puzzle ( &btlparray[i].puzzle);
	// free ( puzzle_array ) ;
}