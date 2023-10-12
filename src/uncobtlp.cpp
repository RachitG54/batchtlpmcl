//SPDX-License-Identifier: MIT OR GPL-3.0-only
/*
 * Rachit Garg <rachg96@cs.utexas.edu>
 * 2023
 * */

#include "uncobtlp.h"

void getRandomSubset(vi &result, int n, int d) {
    // vi subset;
    vi allRightNodes(n);
    for (int i = 1; i <= n; i++) {
        allRightNodes[i-1] = i;
    }

    // random_device rd;
	// mt19937 rng(rd);
	mt19937 rng(high_resolution_clock::now().time_since_epoch().count());
    shuffle(allRightNodes.begin(), allRightNodes.end(),rng);
    // random_shuffle(allRightNodes.begin(), allRightNodes.end());
    REP(i,0,d-1) {
    	result[i] = allRightNodes[i];
    }
    // subset.assign(allRightNodes.begin(), allRightNodes.begin() + d);
    // return subset;
}

void uncobtlp::initialize(int noleft, int noright, int d, uint64_t t, int genornot){
	n_right = noright;
	n_left = noleft;
	deg = d;
	T = t;
	cobtlpinst.initialize(n_right,T,genornot);
}

// void uncobtlp::gentlp(GT &gtelt) {
// 	classuncobtlp newtlp;
// 	newtlp.initialize(deg);
// 	vi newset(deg);
// 	getRandomSubset(newset,n_right, deg);
// 	REP(i,0,deg-1) {
// 		newtlp.btlpvec[i].slot = newset[i];
// 		cobtlpinst.gentlp(gtelt, newtlp.btlpvec[i]);
// 	}
// 	uncobtlparray.pb(newtlp);
// }


void uncobtlp::gentlp(GT &gtelt,classuncobtlp &newtlp) {
	newtlp.initialize(deg);
	vi newset(deg);
	getRandomSubset(newset,n_right, deg);
	REP(i,0,deg-1) {
		newtlp.btlpvec[i].slot = newset[i];
		cobtlpinst.gentlp(gtelt, newtlp.btlpvec[i]);
	}
}

void uncobtlp::solvetlp(GT &result,classuncobtlp &newtlp) {
	//You can solve any of the tlp's
	cobtlpinst.solvetlp(result,newtlp.btlpvec[0]);
}

void uncobtlp::batchsolvetlp(vector<GT> &result, vector<classuncobtlp> &batcharray) {
	graph graphinst;
	int szbarr = batcharray.size();
	// cout << "here1\n";

	timergraphbatchsolvetime.starttime();
	vvi fullgraph(szbarr+1, vi(deg, -1));
	REP(i,1,szbarr) {
		REP(j,0,deg-1) {
			fullgraph[i][j] = batcharray[i-1].btlpvec[j].slot;
		}
	}
	graphinst.initialize(n_left,n_right,deg,fullgraph);
	int match = graphinst.hopcroftKarp();
	timergraphbatchsolvetime.donetime();

	if( match != szbarr) {
		cerr << "Can only match "<< match << " nodes, matching is incomplete.\n";
		return;
	} 

	vector<classbtlp> batchsolvedarray;
	REP(i,0,szbarr - 1) {
		int node = i+1;
		int nodematched = graphinst.matchLeft[i+1];
		int edgematched = graphinst.matchedgeLeft[i+1];
		// cout << "node "<<node<< " nodematched "<<nodematched<<" edge number matched "<<edgematched<<"\n";
		batchsolvedarray.pb(batcharray[i].btlpvec[edgematched]);
		// cout << batchsolvedarray[i].slot<<" is slot\n";
		// int node = batcharray[i].btlpvec[i].
		// batchsolvedarray.pb(batcharray[i].btlpvec[])
	}
	// cout <<"\n";
	cobtlpinst.batchsolvetlp(result,batchsolvedarray);
	//You can solve any of the tlp's
}


ll uncobtlp::crsszbytes() {
	ll sz = 0;
	sz += 4; // for n_left
	sz += 4; // for n_right
	sz += 4; // for degree

	sz += cobtlpinst.crsszbytes();
	return sz;
}

void uncobtlp::storetlp(string &str) {
	stringstream ss;
}
// // Cleans the tlp
// void cobtlp::cleantlp (int i)
// {
// 	if(i > n) {
// 		handleErrors("Cleaning incorrect slot number.");
// 	}
// 	else if (btlparray[i].puzzle_ptr!=NULL) {
// 		LHP_clear_puzzle ( btlparray[i].puzzle_ptr);
// 		btlparray[i].prf.clearkeys();
// 	}
// 	// for ( int i = 0 ; i < n ; i ++ ) {
// 	// 	LHP_clear_puzzle ( puzzle_array + i ) ;
// 	// }
// 	// LHP_clear_param ( &param ) ;
// 	// free ( puzzle_array ) ;
// }

// void cobtlp::cleantlp ()
// {
// 	REP(i,1,n) {
// 		cleantlp(i);
// 	}
// 	prf.clearkeys();
// 	LHP_clear_param ( &param ) ;
// 	// LHP_clear_puzzle ( &btlparray[i].puzzle);
// 	// free ( puzzle_array ) ;
// }