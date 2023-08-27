#include "cobtlp.h"

// Assuming for simplicity that we don't store duplicate puzzles for a single slot

void cobtlp::initialize(int number, uint64_t t) {
	n = number;
	T = t;

	timerpuzzleCRSgentime.starttime();
	LHP_init_param ( &param ) ;
	LHP_PSetup ( &param , SEC_PARAM , T) ;
	timerpuzzleCRSgentime.donetime();
	// cout <<"LHP initialized.\n" ;

	timerpairCRSgentime.starttime();
	prf.setup(n);
	timerpairCRSgentime.donetime();
	btlparray.resize(n+1);
	// cout <<"PRF initialized.\n" ;
}

void cobtlp::gentlp(int i, GT &gtelt) {
	if(i > n) {
		handleErrors("Setting incorrect slot number.");
	}


    // classbtlp bltpinst;

    prf.setkey();
    prf.puncture(i);
    string keystr = prf.getkey();

    // cout << "key is "<<keystr<<"\n";
    btlparray[i].n = n;
    btlparray[i].T = T;
    btlparray[i].slot = i;
    btlparray[i].prf.punckey = prf.punckey;
    // btlparray[i].prf.key = prf.key;

    btlparray[i].puzzle_ptr = new LHP_puzzle_t;
	LHP_init_puzzle ( btlparray[i].puzzle_ptr) ;
	/*
	Check encoding is possible or not, it is fine for our current implementation because perfect correctness and l is one and we're mapping Z_p* to
	an element in the time lock puzzle.
	*/

	LHP_PGen ( btlparray[i].puzzle_ptr , &param , keystr) ;

	GT evalGT;
	prf.prfeval(evalGT, i);

	GT::mul(btlparray[i].ctpad,evalGT,gtelt);
	// btlparray.pb(bltpinst);

    prf.clearkeys();
	// cout <<"Gen done.\n" ;
}

void cobtlp::gentlp(GT &gtelt, classbtlp &tlpinst) {
	if(tlpinst.slot > n) {
		handleErrors("Setting incorrect slot number.");
	}
	int i = tlpinst.slot;
    // classbtlp bltpinst;

	timerpairgentime.starttime();
    prf.setkey();
    prf.puncture(i);
	GT evalGT;
	prf.prfeval(evalGT, i);
	timerpairgentime.donetime();

    string keystr = prf.getkey();

    tlpinst.n = n;
    tlpinst.T = T;
    tlpinst.slot = i;
    tlpinst.prf.punckey = prf.punckey;
    // btlparray[i].prf.key = prf.key;

	timerpuzzlegentime.starttime();
    tlpinst.puzzle_ptr = new LHP_puzzle_t;
	LHP_init_puzzle ( tlpinst.puzzle_ptr) ;
	/*
	Check encoding is possible or not, it is fine for our current implementation because perfect correctness and l is one and we're mapping Z_p* to
	an element in the time lock puzzle.
	*/

	LHP_PGen ( tlpinst.puzzle_ptr , &param , keystr) ;

	timerpuzzlegentime.donetime();

	timerpairgentime.starttime();
	GT::mul(tlpinst.ctpad,evalGT,gtelt);
	timerpairgentime.donetime();

    prf.clearkeys();
}

// int cobtlp::gettlpidx(int i) {
// 	REP(j,0,btlparray.size()-1) {
// 		if(btlparray[j].slot == i) {
// 			return j;
// 		}
// 	}
// }


void cobtlp::solvetlp (int idx, GT &result)
{
	if(btlparray[idx].puzzle_ptr == NULL) {
		cerr << "Solving an uninitialized puzzle\n";
		return;
	}
	mpz_init (btlparray[idx].solution.s ) ;
	LHP_PSolve ( &param , btlparray[idx].puzzle_ptr , &btlparray[idx].solution ) ;

	Fr sumkey;
	sumkey.setStr(mpz_get_str(NULL, 10, btlparray[idx].solution.s), 10);
	prf.setkey(sumkey);

	GT sumkeyeval;
	prf.prfeval(sumkeyeval,idx);


	GT::div(result,btlparray[idx].ctpad,sumkeyeval);
	sumkey.clear();
	sumkeyeval.clear();
	mpz_clear(btlparray[idx].solution.s);

}

void cobtlp::solvetlp(GT &result, classbtlp &tlpinst)
{
	if(tlpinst.puzzle_ptr == NULL) {
		cerr << "Solving an uninitialized puzzle\n";
		return;
	}

	timerpuzzlesolvetime.starttime();
	mpz_init (tlpinst.solution.s ) ;
	LHP_PSolve ( &param , tlpinst.puzzle_ptr , &tlpinst.solution ) ;
	timerpuzzlesolvetime.donetime();

	timerpairsolvetime.starttime();
	Fr sumkey;
	sumkey.setStr(mpz_get_str(NULL, 10, tlpinst.solution.s), 10);
	prf.setkey(sumkey);
	int idx = tlpinst.slot;
	GT sumkeyeval;
	prf.prfeval(sumkeyeval,idx);
	GT::div(result,tlpinst.ctpad,sumkeyeval);
	timerpairsolvetime.donetime();

	sumkey.clear();
	sumkeyeval.clear();
	mpz_clear(tlpinst.solution.s);

}

void cobtlp::batchsolvetlp (vi &setS, vector<GT> &result)
{
	LHP_puzzle_t *puzzle_array = new LHP_puzzle_t[n];

	int newn = 0;
	vi newsetS;
	REP(i,0,setS.size()-1) {
		int j = setS[i];
		if(btlparray[j].puzzle_ptr != NULL) {
			puzzle_array[newn] = *(btlparray[j].puzzle_ptr);
			newn++;
			newsetS.pb(j);
		}
	}
	cout << "Batching " << newn << " things.\n";
	if (newn == 0) {
		return;
	}

	LHP_puzzle_t dest_puzzle;
	LHP_init_puzzle ( &dest_puzzle ) ;
	LHP_PEval ( &param , puzzle_array , newn , &dest_puzzle ) ;


	LHP_puzzle_sol_t solution ;
	mpz_init (solution.s ) ;
	LHP_PSolve ( &param , &dest_puzzle , &solution ) ;

	// printmpz(solution.s);
	// Fr sumkey(tmp);
	// Fr orderf = Fr::getModulo;
	string frstr = Fr::getModulo();//.getStr(10);
	mpz_t orderf;
	mpz_init(orderf) ;
    mpz_set_str(orderf, frstr.c_str(), 10);

    mpz_mod(solution.s, solution.s, orderf);

    // cout << "Order of the base field: " << Fr::getModulo() << std::endl;
	// printmpz(solution.s);

	// cout <<"\n\n";

	Fr sumkey;
	sumkey.setStr(mpz_get_str(NULL, 10, solution.s), 10);
	// cout << sumkey << " sumkey is executed\n";

	REP(i,0,newsetS.size()-1) {
		prf.setkey(sumkey);
		int curridx = newsetS[i];

		GT sumkeyeval;
		prf.prfeval(sumkeyeval,curridx);
		// cout <<"sumkey is "<< sumkey << "\nsumkeyeval is "<<sumkeyeval<<"\n\n\n";

		GT intval(btlparray[curridx].ctpad);
		REP(j,0,newsetS.size()-1) {
			int iteridx = newsetS[j];
			if (iteridx == curridx) continue;

			// cout <<"This shouldn't get executed.\n";
			prf.setpunckey(btlparray[iteridx].prf.punckey);
			GT compval;
			prf.punceval(compval,iteridx,curridx);

			GT::mul(intval,intval,compval);
			// cout << "Computed "<<j<<" punctured eval.\n";
			// compval.clear();
		}
		GT::div(intval,intval,sumkeyeval);
		result[curridx] = intval;
		// cout << "batch solve test " << curridx << ": " << result[i] << "\n\n\n";

		// intval.clear();
		sumkeyeval.clear();

		// GT test2val(btlparray[curridx].ctpad);
		// prf.setkey(btlparray[curridx].prf.key);
		// GT testeval;
		// prf.prfeval(testeval,curridx);
		// cout <<"prfkey is "<< btlparray[curridx].prf.key << "\nprfeval is "<<testeval<<"\n\n\n";
		// GT::div(test2val,test2val,testeval);
		// cout << "batch solve test 2 " << curridx << ": " << test2val << "\n\n\n";
	}

	sumkey.clear();

	mpz_clear(solution.s) ;
	mpz_clear(orderf);

	free(puzzle_array);
	// REP(i,0,newn-1) {
	// 	LHP_clear_puzzle (&puzzle_array[i]) ;
	// }

	LHP_clear_puzzle (&dest_puzzle) ;
}

void cobtlp::batchsolvetlp(vector<GT> &result, vector<classbtlp> &batcharray)
{
	int sz = batcharray.size();
	// cout << "Batching " << sz << " things.\n";
	if (sz == 0) {
		return;
	}

	timerpuzzlebatchsolvetime.starttime();
	LHP_puzzle_t *puzzle_array = new LHP_puzzle_t[sz];

	REP(i,0,batcharray.size()-1) {
		if(batcharray[i].puzzle_ptr == NULL) {
			cerr << "Not expecting to batch uninitialized puzzles.\n";
			abort();
		}
		else if(batcharray[i].puzzle_ptr != NULL) {
			puzzle_array[i] = *(batcharray[i].puzzle_ptr);
		}
	}

	LHP_puzzle_t dest_puzzle;
	LHP_init_puzzle ( &dest_puzzle ) ;
	LHP_PEval ( &param , puzzle_array , sz , &dest_puzzle ) ;


	LHP_puzzle_sol_t solution ;
	mpz_init (solution.s ) ;
	LHP_PSolve ( &param , &dest_puzzle , &solution ) ;

	timerpuzzlebatchsolvetime.donetime();


	timerpairbatchsolvetime.starttime();
	string frstr = Fr::getModulo();//.getStr(10);
	mpz_t orderf;
	mpz_init(orderf) ;
    mpz_set_str(orderf, frstr.c_str(), 10);

    mpz_mod(solution.s, solution.s, orderf);

	Fr sumkey;
	sumkey.setStr(mpz_get_str(NULL, 10, solution.s), 10);
	// cout << sumkey << " sumkey is executed\n";

	REP(i,0,sz-1) {
		prf.setkey(sumkey);

		GT sumkeyeval;
		int curridx = batcharray[i].slot;
		// cout << curridx << " is curridx ";
		prf.prfeval(sumkeyeval,curridx);
		// cout <<"sumkey is "<< sumkey << "\nsumkeyeval is "<<sumkeyeval<<"\n\n\n";

		GT intval(batcharray[i].ctpad);
		REP(j,0,sz-1) {
			int iteridx = batcharray[j].slot;
			// cout << iteridx << " is iteridx ";
			if (iteridx == curridx) continue;

			// cout <<"This shouldn't get executed.\n";
			prf.setpunckey(batcharray[j].prf.punckey);
			GT compval;
			prf.punceval(compval,iteridx,curridx);

			GT::mul(intval,intval,compval);
			// cout << "Computed "<<j<<" punctured eval.\n";
			// compval.clear();
		}
		// cout << "\n";
		GT::div(intval,intval,sumkeyeval);
		result[i] = intval;
		// cout << "batch solve test " << curridx << ": " << result[i] << "\n\n\n";

		// intval.clear();
		sumkeyeval.clear();
	}

	timerpairbatchsolvetime.donetime();

	sumkey.clear();

	mpz_clear(solution.s) ;
	mpz_clear(orderf);

	free(puzzle_array);

	LHP_clear_puzzle (&dest_puzzle) ;
}


// Cleans the tlp
void cobtlp::cleantlp (int i)
{
	if(i > n) {
		handleErrors("Cleaning incorrect slot number.");
	}
	else if (btlparray[i].puzzle_ptr!=NULL) {
		LHP_clear_puzzle ( btlparray[i].puzzle_ptr);
		btlparray[i].prf.clearkeys();
	}
	// for ( int i = 0 ; i < n ; i ++ ) {
	// 	LHP_clear_puzzle ( puzzle_array + i ) ;
	// }
	// LHP_clear_param ( &param ) ;
	// free ( puzzle_array ) ;
}

// Cleans the tlp
void cobtlp::cleantlp (vector<classbtlp> &batcharray)
{
	int sz = batcharray.size();
	REP(i,0,sz-1) {
		if (batcharray[i].puzzle_ptr!=NULL) {
			LHP_clear_puzzle ( batcharray[i].puzzle_ptr);
			batcharray[i].prf.clearkeys();
		}
	}
}


void cobtlp::cleantlp ()
{
	REP(i,1,n) {
		cleantlp(i);
	}
	prf.clearkeys();
	LHP_clear_param ( &param ) ;
	// LHP_clear_puzzle ( &btlparray[i].puzzle);
	// free ( puzzle_array ) ;
}