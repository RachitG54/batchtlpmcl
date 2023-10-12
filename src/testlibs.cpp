//SPDX-License-Identifier: MIT
/*
 * Rachit Garg
 * 2023
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
#include "uncobtlp.h"
#include "btlptime.h"


using namespace mcl::bn;

// Experimental Parameters
int n = 200 ;
string str = "111";
int repeat1 = 1;
int repeat2 = 1;
uint64_t timeT = 1000000;

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

    // cout << mclBn_getG1ByteSize() << "\n";
    cout << Fr::getByteSize() << "\n";
    cout << G1::getSerializedByteSize() << "\n";
    cout << G2::getSerializedByteSize() << "\n";

    char serG1[1000];
    P.serialize(serG1,900);
    string x = P.getStr();
    // P.serialize(x);
    cout << strlen(serG1) <<"\n";
    cout << x << "\n";

    
    // cout << GT::getSerializedByteSize() << "\n";
    // cout << G2::getByteSize() << "\n";
    // cout << GT::getByteSize() << "\n";

    Fr r;
    r.setByCSPRNG();

    GT testpow;
    GT::pow(testpow,gt,r);

	int testsz = testpow.serialize(serG1,900);
    cout << testsz<<"\n";

    testsz = P.serialize(serG1,900);
    cout << testsz<<"\n";


    testsz = Q.serialize(serG1,900);
    cout << testsz<<"\n";

    // cout << x << "\n";
    // cout << x << " is value\n";
    // stringstream ss;
    // ss << x;
    // string xstr = ss.str();
    // cout << xstr << " is serialized\n";

    // Fr x2(xstr);
    // cout << x2<<"\n";
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

void testbtlpfn() {
	// btlp testbtlp;
	// testbtlp.initialize(10);
	// testbtlp.setuptlp(1000000);
	// testbtlp.gentlp();
	// testbtlp.solvetlp();
	// testbtlp.batchtlp();
	// testbtlp.cleantlp();
}

void testcobtlpfn() {
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
}

void testcobtlpfnarg() {
	cobtlp testcobtlp;
	testcobtlp.initialize(10,1000000);

	int test = 10;
	vector<GT> randgt(test+1);
	vector<GT> solvegt(test+1);
	vector<GT> batchsolvegt(test+1);

	vector<classbtlp> batcharray(test);

	vector<classbtlp> batcharraytest;
	vector<GT> randgttest;

	// vi testarr;
	REP(i,1,test) {
		getrandGT(randgt[i]);
		// cout << "test " << i << ": " << randgt[i] << "\n";
		batcharray[i-1].slot = i;
		testcobtlp.gentlp(randgt[i],batcharray[i-1]);
		if(i%3 == 0) {
			batcharraytest.pb(batcharray[i-1]);
			randgttest.pb(randgt[i]);
			cout << "including test "<<i<<" in my batching.\n";
		}
		// cout << batcharray[i-1].slot << " is the slot\n";
		// testarr.pb(i);
	}

	cout <<"Gen done.\n" ;

	// cout<<"\n\n\n";

	REP(i,1,test) {
		testcobtlp.solvetlp(solvegt[i],batcharray[i-1]);
	}

	REP(i,1,test) {
		if(solvegt[i] != randgt[i]) {
			cerr << "Solve test "<<i<<" failed.\n";
		}
	}

	cout <<"Solve done.\n" ;
	// cout<<"\n\n\n";

	testcobtlp.batchsolvetlp(batchsolvegt,batcharraytest);
	// testcobtlp.batchsolvetlp(testarr,batchsolvegt);

	REP(i,0,sz(randgttest)-1) {
		if(batchsolvegt[i] != randgttest[i]) {
			cerr << "Batch Solve test "<<i<<" failed.\n";
			cout << batchsolvegt[i] << "\n\n\n" << randgttest[i] << "\nTest i done.\n\n";
		}
	}

	cout <<"Batch solve done.\n" ;
	testcobtlp.cleantlp(batcharray);
}

// void testuncobtlpfn(int n_left = 10, int n_right = 20, int deg = 5) {
// 	uncobtlp testuncobtlp;
// 	// int n_left = 10;
// 	// int n_right = 20;
// 	// int deg = 5;
// 	int timeT = 1000000;
// 	int repeat = 1;

// 	timerCRSgentime.starttime();
// 	REP(j,0,repeat-1)
// 		testuncobtlp.initialize(n_left,n_right,deg,timeT);
// 	timerCRSgentime.donetime();

// 	CRSgentime = timergentime.getTime();
// 	CRSbytes = testuncobtlp.crsszbytes();



// 	int test = n_left;
// 	vector<GT> randgt(test+1);
// 	vector<GT> solvegt(test+1);
// 	vector<GT> batchsolvegt(test+1);

// 	vector<classuncobtlp> batcharray(test);
// 	puzzlebytes = 0;
// 	REP(i,1,test) {
// 		getrandGT(randgt[i]);
// 		// cout << "test " << i << ": " << randgt[i] << "\n";
// 		// batcharray[i-1].slot = i;
// 		timergentime.starttime();
// 		REP(j,0,repeat-1)
// 			testuncobtlp.gentlp(randgt[i],batcharray[i-1]);
// 		timergentime.donetime();


// 		puzzlebytes += batcharray[i-1].szbytes();

// 		// cout << timergentime.getTime() <<" ";
// 		// pairgentime += timerpairgentime.getTime();
// 		// puzzlegentime += timerpuzzlegentime.getTime();
// 		// cout << batcharray[i-1].slot << " is the slot\n";
// 		// testarr.pb(i);
// 	}
// 	// cout<<"\n";
// 	cout <<"Gen done.\n" ;
	

// 	// // cout<<"\n\n\n";

// 	REP(i,1,test) {
// 		timersolvetime.starttime();
// 		testuncobtlp.solvetlp(solvegt[i],batcharray[i-1]);
// 		timersolvetime.donetime();
// 	}

// 	REP(i,1,test) {
// 		if(solvegt[i] != randgt[i]) {
// 			cerr << "Solve test "<<i<<" failed.\n";
// 		}
// 	}

// 	cout <<"Solve done.\n" ;

// 	timerbatchsolvetime.starttime();
// 	REP(j,0,repeat-1)
// 		testuncobtlp.batchsolvetlp(batchsolvegt,batcharray);
// 	timerbatchsolvetime.donetime();
// 	// // testcobtlp.batchsolvetlp(testarr,batchsolvegt);

// 	REP(i,1,test) {
// 		if(batchsolvegt[i-1] != randgt[i]) {
// 			cerr << "Batch Solve test "<<i<<" failed.\n";
// 			cout << batchsolvegt[i-1] << "\n\n\n" << randgt[i] << "\nTest i done.\n\n";
// 		}
// 	}

// 	cout <<"Batch solve done.\n" ;

// 	CRSgentime = timerCRSgentime.getTime()/(repeat);
// 	pairCRSgentime = timerpairCRSgentime.getTime()/(repeat);
// 	puzzleCRSgentime = timerpuzzleCRSgentime.getTime()/(repeat);

// 	gentime = timergentime.getTime()/(repeat*test);
// 	pairgentime = timerpairgentime.getTime()/(repeat*test);
// 	puzzlegentime = timerpuzzlegentime.getTime()/(repeat*test);

// 	solvetime = timersolvetime.getTime()/(test);
// 	pairsolvetime = timerpairsolvetime.getTime()/(test);
// 	puzzlesolvetime = timerpuzzlesolvetime.getTime()/(test);

// 	batchsolvetime = timerbatchsolvetime.getTime()/(repeat);
// 	pairbatchsolvetime = timerpairbatchsolvetime.getTime()/(repeat);
// 	puzzlebatchsolvetime = timerpuzzlebatchsolvetime.getTime()/(repeat);
// 	graphbatchsolvetime = timergraphbatchsolvetime.getTime()/(repeat);
	
// 	puzzlebytes = puzzlebytes/(test);

// 	int N = n_left;

// 	cout << "n_left	n_right	deg	timeT	CRSgentime	pairCRSgentime	puzzleCRSgentime	gentime	pairgentime	puzzlegentime	solvetime	pairsolvetime	puzzlesolvetime	batchsolvetime	pairbatchsolvetime	puzzlebatchsolvetime	graphbatchsolvetime	CRSbytes	puzzlebytes" << "\n";
// 	cout << n_left << "\t" << n_right << "\t" << deg << "\t" << timeT << "\t" << CRSgentime << "\t" << pairCRSgentime << "\t" << puzzleCRSgentime << "\t" << gentime << "\t" << pairgentime << "\t" << puzzlegentime << "\t" << solvetime << "\t" << pairsolvetime << "\t" << puzzlesolvetime << "\t" << batchsolvetime << "\t" << pairbatchsolvetime << "\t" << puzzlebatchsolvetime << "\t" << graphbatchsolvetime << "\t" << CRSbytes << "\t" << puzzlebytes;
// 	cout << "\n";
// 	// cout << "Time CRS taken is "<<CRSgentime<<" "<<pairCRSgentime<<" "<<puzzleCRSgentime<<" "<<"\n";
// 	// cout << "Time gen taken is "<<gentime<<" "<<pairgentime<<" "<<puzzlegentime<<" "<<"\n";
// 	// cout << "Time solve taken is "<<solvetime<<" "<<pairsolvetime<<" "<<puzzlesolvetime<<" "<<"\n";
// 	// cout << "Time batchsolve taken is "<<batchsolvetime<<" "<<pairbatchsolvetime<<" "<<puzzlebatchsolvetime<<" "<<graphbatchsolvetime<<" "<<"\n";
// 	// testcobtlp.cleantlp(batcharray);
// }


void printexpuncobtlpfn(int n_left = 10, int n_right = 20, int deg = 5) {
	uncobtlp testuncobtlp;

	timerCRSgentime.starttime();
	REP(j,0,repeat1-1)
		testuncobtlp.initialize(n_left,n_right,deg,timeT,2);
	timerCRSgentime.donetime();

	cout <<"CRS setup.\n" ;

	CRSgentime = timerCRSgentime.getTime()/(repeat1);
	double rsagentime = timerrsagentime.getTime()/(repeat1);

	CRSgentime = CRSgentime - rsagentime + adjusttime;

	pairCRSgentime = timerpairCRSgentime.getTime()/(repeat1);

	puzzleCRSgentime = timerpuzzleCRSgentime.getTime()/(repeat1);
	puzzleCRSgentime = puzzleCRSgentime - rsagentime + adjusttime;

	CRSbytes = testuncobtlp.crsszbytes();

	int test = n_left;
	vector<GT> randgt(test+1);
	vector<GT> solvegt(test+1);
	vector<GT> batchsolvegt(test+1);

	vector<classuncobtlp> batcharray(test);
	puzzlebytes = 0;
	REP(i,1,test) {
		getrandGT(randgt[i]);
		timergentime.starttime();
		REP(j,0,repeat1-1)
			testuncobtlp.gentlp(randgt[i],batcharray[i-1]);
		timergentime.donetime();


		puzzlebytes += batcharray[i-1].szbytes();
		// cout << puzzlebytes << " is one puzzle size"<<"\n";
	}
	cout <<"Gen done.\n" ;

	int puzzlessolved = 1;
	REP(i,1,puzzlessolved) {
		timersolvetime.starttime();
		testuncobtlp.solvetlp(solvegt[i],batcharray[i-1]);
		timersolvetime.donetime();
	}

	cout <<"Solve done.\n" ;

	timerbatchsolvetime.starttime();
	REP(j,0,repeat2-1) {
		testuncobtlp.batchsolvetlp(batchsolvegt,batcharray);
	}
	timerbatchsolvetime.donetime();

	// REP(i,1,test) {
	// 	if(batchsolvegt[i-1] != randgt[i]) {
	// 		cerr << "Batch Solve test "<<i<<" failed.\n";
	// 		cout << batchsolvegt[i-1] << "\n\n\n" << randgt[i] << "\nTest i done.\n\n";
	// 	}
	// }

	cout <<"Batch solve done.\n" ;

	// CRSgentime = timerCRSgentime.getTime()/(repeat1);
	// pairCRSgentime = timerpairCRSgentime.getTime()/(repeat1);
	// puzzleCRSgentime = timerpuzzleCRSgentime.getTime()/(repeat1);

	gentime = timergentime.getTime()/(repeat1*test);
	pairgentime = timerpairgentime.getTime()/(repeat1*test);
	puzzlegentime = timerpuzzlegentime.getTime()/(repeat1*test);

	solvetime = timersolvetime.getTime()/(puzzlessolved);
	pairsolvetime = timerpairsolvetime.getTime()/(puzzlessolved);
	puzzlesolvetime = timerpuzzlesolvetime.getTime()/(puzzlessolved);

	batchsolvetime = timerbatchsolvetime.getTime()/(repeat2);
	pairbatchsolvetime = timerpairbatchsolvetime.getTime()/(repeat2);
	puzzlebatchsolvetime = timerpuzzlebatchsolvetime.getTime()/(repeat2);
	graphbatchsolvetime = timergraphbatchsolvetime.getTime()/(repeat2);
	
	puzzlebytes = puzzlebytes/(test);

	int N = n_left;

	cout << "n_left	n_right	deg	timeT	CRSgentime	pairCRSgentime	puzzleCRSgentime	gentime	pairgentime	puzzlegentime	solvetime	pairsolvetime	puzzlesolvetime	batchsolvetime	pairbatchsolvetime	puzzlebatchsolvetime	graphbatchsolvetime	CRSbytes	puzzlebytes\n";
	cout << n_left << "\t" << n_right << "\t" << deg << "\t" << timeT << "\t" << CRSgentime << "\t" << pairCRSgentime << "\t" << puzzleCRSgentime << "\t" << gentime << "\t" << pairgentime << "\t" << puzzlegentime << "\t" << solvetime << "\t" << pairsolvetime << "\t" << puzzlesolvetime << "\t" << batchsolvetime << "\t" << pairbatchsolvetime << "\t" << puzzlebatchsolvetime << "\t" << graphbatchsolvetime << "\t" << CRSbytes << "\t" << puzzlebytes;
	cout << "\n";


	// cout << "Time CRS taken is "<<CRSgentime<<" "<<pairCRSgentime<<" "<<puzzleCRSgentime<<" "<<"\n";
	// cout << "Time gen taken is "<<gentime<<" "<<pairgentime<<" "<<puzzlegentime<<" "<<"\n";
	// cout << "Time solve taken is "<<solvetime<<" "<<pairsolvetime<<" "<<puzzlesolvetime<<" "<<"\n";
	// cout << "Time batchsolve taken is "<<batchsolvetime<<" "<<pairbatchsolvetime<<" "<<puzzlebatchsolvetime<<" "<<graphbatchsolvetime<<" "<<"\n";
	// testcobtlp.cleantlp(batcharray);
}

void printexptrivialfn(int n_left = 10) {

	// Code for trivial scheme benchmarks

	int repeattrivial = 1;
	btlptime trivialtimerCRSgentime;
	btlptime trivialtimergentime;
	btlptime trivialtimersolvetime;

	double trivialpuzzlebytes = 0;
	double trivialCRSbytes = 0;

	// LHP_init_puzzle ( &dest_puzzle ) ;
	puzzle_array = (LHP_puzzle_t*) malloc ( sizeof ( LHP_puzzle_t ) * repeattrivial ) ;

	trivialtimerCRSgentime.starttime();
	REP(j,0,repeat1-1) {
		LHP_init_param ( &param ) ;
		LHP_PSetup ( &param , SEC_PARAM , timeT, 2) ;
	}
	trivialtimerCRSgentime.donetime();

	double trivialCRSgentime = trivialtimerCRSgentime.getTime()/(repeat1);
	double trivialrsagentime = timerrsagentime.getTime()/(repeat1);

	trivialCRSgentime = trivialCRSgentime - trivialrsagentime + adjusttime;

	cout << "CRS generated.\n";

	trivialCRSbytes += getmpzbytes(param.T);
	trivialCRSbytes += getmpzbytes(param.N);
	trivialCRSbytes += getmpzbytes(param.g);
	trivialCRSbytes += getmpzbytes(param.h);

	for ( int i = 0 ; i < repeattrivial ; i ++ ) {
		trivialtimergentime.starttime();
		LHP_init_puzzle ( puzzle_array + i ) ;
		LHP_PGen ( puzzle_array + i , &param , str) ;
		trivialtimergentime.donetime();

		trivialpuzzlebytes += getmpzbytes((puzzle_array+i)->u);
		trivialpuzzlebytes += getmpzbytes((puzzle_array+i)->v);
		// cout << getmpzbytes((puzzle_array+i)->u) << getmpzbytes((puzzle_array+i)->v) << "\n";
	}

	cout << "puzzle generated.\n";

	mpz_init ( solution .s ) ;
	trivialtimersolvetime.starttime();
	for ( int i = 0 ; i < repeattrivial ; i ++ ) {
		LHP_PSolve ( &param , puzzle_array + i , &solution ) ;
	}
	trivialtimersolvetime.donetime();

	cout << "puzzle solved.\n";
	
	for ( int i = 0 ; i < repeattrivial ; i ++ ) {
		LHP_clear_puzzle ( puzzle_array + i ) ;
	}
	LHP_clear_param ( &param ) ;
	free ( puzzle_array ) ;

	double trivialgentime = trivialtimergentime.getTime()/(repeattrivial);
	double trivialsolvetime = trivialtimersolvetime.getTime()/(repeattrivial);
	double trivialbatchsolvetime = trivialsolvetime*n_left;


	trivialpuzzlebytes = trivialpuzzlebytes/(repeattrivial);

	cout << "n_left	timeT	trivialCRSgentime	trivialgentime	trivialsolvetime	trivialbatchsolvetime	trivialCRSbytes	trivialpuzzlebytes" << "\n";
	cout << n_left << "\t" << timeT<<"\t"<<trivialCRSgentime<<"\t"<<trivialgentime<<"\t"<<trivialsolvetime<<"\t"<<trivialbatchsolvetime<<"\t"<<trivialCRSbytes<<"\t"<<trivialpuzzlebytes;
	cout << "\n";
}

double logBinomialCoefficient(ll n, ll k) {
    double result = 0.0;
    if (k == 0) return 1.0;

    // Since C(n, k) = C(n, n-k)
    if (k > n - k)
        k = n - k;

    // Calculate logarithmic value of [n*(n-1)*...*(n-k+1)] / [k*(k-1)*...*1]
    for (ll i = 0; i < k; ++i) {
        result += log2(n - i);
    }

    for (ll i = 0; i < k; ++i) {
        result -= log2(i + 1);
    }

    return result;
}

double evaluateExpression(ll n, ll q, int m, int d, int print = 0) {
    double result = 0.0;

    for (ll i = d+1; i <= n; ++i) {
    	double eval = ((i * d *1.0) * log2((i - 1.0) / m));
        double term = logBinomialCoefficient(q, i) +
                      logBinomialCoefficient(m, i - 1) + eval;
        // if(print == 1) cout << term <<" "<<eval<<"\n";

        result += pow(2, term);
        // result += term;
    }

    return result;
}

int paramcomputer (ll n, ll q, ll m, int print = 0) {

	// can do binary search here, but already so efficient
	REP(i,1,128) {
	    double logResult = log2(evaluateExpression(n, q, m, i,print));
		if (print == 1) std::cout << "At index " << i <<" log x = " << logResult << std::endl;
	    if (logResult < -40.0) {
		    // std::cout << "At index " << i <<" log x = " << logResult << std::endl;
	    	return i;
	    }
	}
	return 128;

	// binary searched option

	int left = 1;
	int right;
	if (n < 128) right = n; else right = 128;
	int mid;
	while (left <= right) {
		mid = (left+right)/2;
		// cout << left << " "<<mid<< " "<<right<<"\n";
		double logResult = log2(evaluateExpression(n, q, m, mid,print));
	    // std::cout << "At index " << mid <<" log x = " << logResult << std::endl;
		if (logResult < -40.0) {
			right = mid-1;
	    }
	    else {
	    	left = mid+1;
	    }
	}
	return left;

}

void matchingparams(int argc, char* argv[]) {
	if(argc == 3) {
		int n_left = atoi(argv[1]);
		int n_right = atoi(argv[2]);
		ll q = n_left;
		// q = 1099511627776;
		int degree = paramcomputer(n_left,q,n_right,1);
		cout << "degree is "<<degree<<"\n";
	}
	if (argc == 2) {
		int n_left = atoi(argv[1]);
		ll q = n_left;
		ll left = n_left;
		ll right = 100000;
		ll mid;
		while(left <= right) {
			mid = (left+right)/2;
			// cout << left << " "<<mid<< " "<<right<<"\n";
			int degree = paramcomputer(n_left,q,mid);
			if (degree > 3) {
				left = mid+1;
			}
			else {
				right = mid -1;
			}
		}
		// cout << "binary search answer is "<<left<<"\n";
		ll n_right = left;
		int degree = paramcomputer(n_left,q,n_right,0);
		// double result = evaluateExpression(n_left,q,n_right,degree,1);
		// cout <<"result is "<<result<<"\n";
		cout << n_left << " " <<n_right<<" "<<degree<<"\n";
	}
}

int main ( int argc , char* argv[] )
{
	if(SEC_PARAM == 512) {
		adjusttime = 156352;
	}
	else if (SEC_PARAM == 1024) {
		adjusttime = 841260;
	}
	else if  (SEC_PARAM == 2048) {
		adjusttime = 8410887;
	}
	else if  (SEC_PARAM == 3072) {
		adjusttime = 127897769;
	}
	initpairing();

	// testlhtlp();
	// testmcl();
	// testgmp();
	// testkhprf();
	btlptime timer1;
	int n = 100000;
	khprf prf1;
	timer1.starttime();
	prf1.setup(n);
	timer1.donetime();
	cout << timer1.getTime();
	return 0;

	// printf("GMP version: %s\n", gmp_version);
	// return 0;
	// matchingparams(argc,argv);
	// return 0;
	// array computed by running matchingparams optimized at degree 3
	int arr_right[101] = {140, 199, 242, 276, 306, 333, 357, 380, 400, 420, 438, 456, 473, 489, 504, 519, 533, 547, 561, 574, 586, 599, 611, 622, 634, 645, 656, 667, 678, 688, 698, 708, 718, 727, 737, 746, 756, 765, 774, 782, 791, 800, 808, 816, 825, 833, 841, 849, 857, 864, 872, 880, 887, 895, 902, 910, 924, 938, 953, 969, 984, 999, 1015, 1030, 1046, 1061, 1077, 1092, 1108, 1123, 1139, 1154, 1170, 1186, 1201, 1217, 1232, 1248, 1263, 1279, 1295, 1310, 1326, 1341, 1357, 1373, 1388, 1404, 1419, 1435, 1451, 1466, 1482, 1497, 1513, 1529, 1544, 1560, 1575, 1591};

	if (argc < 2) {
		int repeathere = 6;
		REP(j,0,repeathere-1) {
			LHP_init_param ( &param ) ;
			LHP_PSetup ( &param , SEC_PARAM , 1000, 0) ;
		}
		double timetaken = timerrsagentime.getTime()/repeathere;
		printf("%lf\n",timetaken);
		// cout << timetaken << "\n";
		// cerr << "Not the right parameters.\n";
		return 0;
	}
	string expname = (string)argv[1];

	if (expname == "trivial") {
		if (argc == 4) {
			int n_left = atoi(argv[2]);
			timeT = atoi(argv[3]);
			printexptrivialfn(n_left);
		}
	}
	else if (expname == "optimized") {
		if (argc == 4) {
			int n_left = atoi(argv[2]);
			timeT = atoi(argv[3]);
			int n_right = 10000;
			if (n_left % 10 == 0 && n_left < 1001 && n_left > 9) {
				int ind = n_left/10 - 1;
				n_right = arr_right[ind];
			}
			int q = n_left;
			int degree = paramcomputer(n_left,q,n_right,0);
			if (n_left % 10 == 0 && n_left < 1001 && n_left > 9) {
				if (degree != 3) {
					cerr << "Error in experimental setup.\n";
					return 0;
				}
			}
			cout << n_left << " " <<n_right<<" "<<degree<<"\n";

			printexpuncobtlpfn(n_left,n_right,degree);
		}
	}
	else {
		cout <<"No experiment running.\n";
	}
	// if(argc == 5) {
	// 	int n_left = atoi(argv[1]);
	// 	int n_right = atoi(argv[2]);
	// 	int degree = atoi(argv[3]);
	// 	int q = n_left;

	// 	// testuncobtlpfn(n_left,n_right,degree);
	// 	printexpuncobtlpfn(n_left,n_right,degree);
	// }
	// else if (argc==3) {
	// 	int n_left = atoi(argv[1]);
	// 	int n_right = 10000;
	// 	if (n_left % 10 == 0 && n_left < 1001 && n_left > 9) {
	// 		int ind = n_left/10 - 1;
	// 		n_right = arr_right[ind];
	// 	}
	// 	int q = n_left;
	// 	int degree = paramcomputer(n_left,q,n_right,0);
	// 	if (n_left % 10 == 0 && n_left < 1001 && n_left > 9) {
	// 		if (degree != 3) {
	// 			cerr << "Error in experimental setup.\n";
	// 			return 0;
	// 		}
	// 	}
	// 	cout << n_left << " " <<n_right<<" "<<degree<<"\n";

	// 	// testuncobtlpfn(n_left,n_right,degree);
	// 	printexpuncobtlpfn(n_left,n_right,degree);
	// 	printexptrivialfn(n_left);
	// }


	// // testcobtlpfnarg();
	
	// else {
	// 	testuncobtlpfn();
	// }

	
	return 0;
}
