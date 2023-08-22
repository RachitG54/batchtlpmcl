/*
 * Adithya Bhat <bhat24@purdue.edu>
 * 2019
 * */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <gmp.h>
#include <mcl/bls12_381.hpp>

#include "lhp.h"
#include "util.h"
#include "khprf.h"
#include "btlp.h"
#include "cobtlp.h"

// #define SEC_PARAM 1024

using namespace mcl::bn;

// Experimental Parameters
int n = 200 ;
string str = "111";

/* Some variables to declare */
LHP_param_t param ;
LHP_puzzle_t puzzle ;
LHP_puzzle_t *puzzle_array ;
LHP_puzzle_t dest_puzzle ;
LHP_puzzle_sol_t solution ;


void minimum_sample(const G1& P, const G2& Q)
{
	Fr a;
	const Fr b = 456;
	Fp12 e1, e2;
	pairing(e1, P, Q);
	G2 aQ;
	G1 bP;
	a.setHashOf("abc", 3);
	printf("a = %s\n", a.getStr(16).c_str());
	printf("a - b = %s\n", (a - b).getStr(16).c_str());
	G2::mul(aQ, Q, a);
	G1::mul(bP, P, b);
	pairing(e2, bP, aQ);
	Fp12::pow(e1, e1, a * b);
	printf("pairing = %s\n", e1.getStr(16).c_str());
	printf("%s\n", e1 == e2 ? "ok" : "ng");
}

void miller_and_finel_exp(const G1& P, const G2& Q)
{
	Fp12 e1, e2;
	pairing(e1, P, Q);

	millerLoop(e2, P, Q);
	finalExp(e2, e2);
	printf("%s\n", e1 == e2 ? "ok" : "ng");
}

void precomputed(const G1& P, const G2& Q)
{
	Fp12 e1, e2;
	pairing(e1, P, Q);
	std::vector<Fp6> Qcoeff;
	precomputeG2(Qcoeff, Q);
	precomputedMillerLoop(e2, P, Qcoeff);
	finalExp(e2, e2);
	printf("%s\n", e1 == e2 ? "ok" : "ng");
}

/* Initailization code */
static void INIT ()
{
	LHP_init_puzzle ( &dest_puzzle ) ;
	puzzle_array = (LHP_puzzle_t*) malloc ( sizeof ( LHP_puzzle_t ) * n ) ;
}

/* Measure time for PSetup */
static void SETUP_TEST ()
{
	LHP_init_param ( &param ) ;
	LHP_PSetup ( &param , SEC_PARAM , 1000000 ) ;
	SUCCESS ( "Setup" ) ;
}

/* Measure time for PGen */
static void GEN_TEST ()
{
	LHP_init_puzzle ( &puzzle ) ;
	LHP_PGen ( &puzzle , &param , str) ;
	SUCCESS ( "Gen" ) ;
}

// Experiment 3 : Measure time for PSolve
static void SOLVE_TEST ()
{
	mpz_init ( solution .s ) ;
	LHP_PSolve ( &param , &puzzle , &solution ) ;
	mpz_t num ;
	mpz_init(num);
    mpz_set_str(num, str.c_str(), 10);
	// mpz_init_set_ui ( num , 0 ) ;
	// int len = str.size();
	// for( int i = 0 ; i < len ; i++ ) {
	// 	mpz_mul_ui ( num , num , 1 << 8 ) ;
	// 	mpz_add_ui ( num , num , (uint8_t)str[i] ) ;
	// }
	if ( mpz_cmp ( num , solution .s ) == 0 ) {
		SUCCESS ( "Solver" ) ;
	}
	else {
		FAILED ( "Solver" ) ;
	}
	mpz_clear (num) ;
}

// Experiment 4 : Measure time for PEval
static void BATCH_TEST ()
{
	// Preparing for Experiment 4
	mpz_t num ;
	mpz_init(num);
    mpz_set_str(num, str.c_str(), 10);

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

static void CLEAN ()
{
	for ( int i = 0 ; i < n ; i ++ ) {
		LHP_clear_puzzle ( puzzle_array + i ) ;
	}
	LHP_clear_param ( &param ) ;
	LHP_clear_puzzle ( &puzzle ) ;
	LHP_clear_puzzle ( &dest_puzzle ) ;
	free ( puzzle_array ) ;
}

void testkhprf() {
	int n = 10;
	khprf prf1;
	prf1.setup(10);

	// Fp& order = G1::getOrder();
	// cout<<"order of G1 is "<<order<<"\n";

	// const Fp& order = Fp::getModulo();

    // Print the order
    // std::cout << "Order of the base field: " << Fr::getModulo() << std::endl;

    prf1.print();

    // cout<<"\n\n";
    prf1.setkey();

    vector<GT> evals;
    evals.resize(n+1);
    for(int i=1; i<=n; i++) {
	    prf1.prfeval(evals[i],i);
	    cout<<"Evaluation at "<<i<<" is "<<evals[i]<<"\n";
    }

    cout<<"Beginning punctured test.\n\n";
    for(int i = 1;i<=n;i++) {
    	prf1.puncture(i);
    	for(int j = 1; j <=n; j++) {
    		GT tempval;
    		tempval.clear();
    		prf1.punceval(tempval,i,j);  
    		cout<<"Punctured evaluation at "<<j<<" is "<<tempval<<"\n";		
    	}
    }
}
void testlhtlp() {
	INIT ();
	SETUP_TEST ();
	GEN_TEST ();
	SOLVE_TEST ();
	BATCH_TEST ();
	CLEAN (); // Clean up
}

void testmcl() {
	initPairing(mcl::BLS12_381);
	puts("BLS12`_381");


	// G1 P;
	// G2 Q;
	// hashAndMapToG1(P, "abc", 3);
	// hashAndMapToG2(Q, "abc", 3);
	// printf("P = %s\n", P.serializeToHexStr().c_str());
	// printf("Q = %s\n", Q.serializeToHexStr().c_str());

	// minimum_sample(P, Q);
	// miller_and_finel_exp(P, Q);
	// precomputed(P, Q);

	G1 P;
	bool check;
	const char *g1Str = "1 0x17f1d3a73197d7942695638c4fa9ac0fc3688c4f9774b905a14e3a3f171bac586c55e83ff97a1aeffb3af00adb22c6bb 0x08b3f481e3aaa0f1a09e30ed741d8ae4fcf5e095d5d00af600db18cb2c04b3edd03cc744a2888ae40caa232946c5e7e1";
	P.setStr(&check,g1Str, 16);

	G2 Q;
	const char *g2Str = "1 0x24aa2b2f08f0a91260805272dc51051c6e47ad4fa403b02b4510b647ae3d1770bac0326a805bbefd48056c8c121bdb8 0x13e02b6052719f607dacd3a088274f65596bd0d09920b61ab5da61bbdc7f5049334cf11213945d57e5ac7d055d042b7e 0x0ce5d527727d6e118cc9cdc6da2e351aadfd9baa8cbdd3a76d429a695160d12c923ac9cc3baca289e193548608b82801 0x0606c4a02ea734cc32acd2b02bc28b99cb3e287e85a763af267492ab572e99ab3f370d275cec1da1aaa9075ff05f79be";
	Q.setStr(&check,g2Str,16);

	// Fr customMessage;
    // customMessage.setStr("12345678901234567890123456789012", 10); // Example custom message

	GT gt;
    pairing(gt, P,Q);

    Fr x;
    x.setByCSPRNG();

    // GT testpow;
    // GT::pow(testpow,gt,x);

    cout << x << " is value\n";
    stringstream ss;
    ss << x;
    string xstr = ss.str();
    cout << xstr << " is serialized\n";

    Fr x2(xstr);
    cout << x2<<"\n";
    // GT y;

    // G1 a;
    // a.setRand();
    // pairing(y, a,Q);
    // y.setRand();
    // cout << y << "\n";
    // cout << getGeneratorOfG2() << "\n";

    // cout << testpow<<"\n";
    // GT customMessage;
    // customMessage.setStr("12345678901234567890123456789012", 10); // Example custom message

	// string plaintext = "Secret message";
	// GT encodedPlaintext;
	// mapToGT(encodedPlaintext, plaintext.c_str(), plaintext.size());

	// char decodedPlaintext[1024]; // Assuming maximum plaintext size is 1024 bytes
    // mapToOriginal(decodedPlaintext, encodedPlaintext);

    // cout << "Plaintext: " << plaintext << endl;
}
void testgmp() {
    const char* num1Str = "23897102878690413305191302092546677624011637124451414656172074653561492208238";
    const char* num2Str = "27841012422131426519476849700634573104435441904749167517298484148522579403538";

    mpz_t num1, num2, result;
    mpz_init(num1);
    mpz_init(num2);
    mpz_init(result);

    mpz_set_str(num1, num1Str, 10);
    mpz_set_str(num2, num2Str, 10);

    // Perform addition
    mpz_add(result, num1, num2);

    // Print the result
    gmp_printf("Result: %Zd\n", result);

    char * tmp = mpz_get_str(NULL,10,num1);

    Fr x(tmp);
    cout << x << "\n";
    // Clean up
    mpz_clear(num1);
    mpz_clear(num2);
    mpz_clear(result);

    return;
}

int main ( int argc , char* argv[] )
{
	// testlhtlp();
	
	// testkhprf();

	// btlp testbtlp;
	// testbtlp.initialize(10);
	// testbtlp.setuptlp(1000000);
	// testbtlp.gentlp();
	// testbtlp.solvetlp();
	// testbtlp.batchtlp();
	// testbtlp.cleantlp();

	// testmcl();
	// testgmp();

	initpairing();
	cobtlp testcobtlp;
	testcobtlp.initialize(10,1000000);

	int test = 10;
	vector<GT> randgt(test+1);
	vector<GT> solvegt(test+1);
	vector<GT> batchsolvegt(test+1);

	vi testarr;
	REP(i,1,test) {
		getrandGT(randgt[i]);
		// cout << "test " << i << ": " << randgt[i] << "\n";
		testcobtlp.gentlp(i,randgt[i]);
		testarr.pb(i);
	}

	cout <<"Gen done.\n" ;

	// cout<<"\n\n\n";

	REP(i,1,test) {
		testcobtlp.solvetlp(i,solvegt[i]);
	}

	REP(i,1,test) {
		if(solvegt[i] != randgt[i]) {
			cerr << "Solve test "<<i<<" failed.\n";
		}
	}

	cout <<"Solve done.\n" ;
	// cout<<"\n\n\n";

	testcobtlp.batchsolvetlp(testarr,batchsolvegt);
	// testcobtlp.batchsolvetlp(testarr,batchsolvegt);

	REP(i,1,test) {
		if(batchsolvegt[i] != randgt[i]) {
			cerr << "Batch Solve test "<<i<<" failed.\n";
		}
	}

	cout <<"Batch solve done.\n" ;
	testcobtlp.cleantlp();
	return 0;
}
