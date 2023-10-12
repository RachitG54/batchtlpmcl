//SPDX-License-Identifier: MIT
/*
 * Rachit Garg
 * 2023
 * */

#ifndef graphdef
#define graphdef

#include "btlpincludes.h"

using namespace std;

class graph {
private:
	int n_left;
	int n_right;
	int deg;
	vi dist;      // Stores distance in the bipartite graph
	int NIL;          // Special node representing unmatched node
public:
	vi matchLeft; // Stores matching of left nodes to right nodes
	vi matchRight; // Stores matching of right nodes to left nodes
	vi matchedgeLeft; // Stores edge number of left node matched
	vvi graphvec;
	void initialize(int nleft, int nright, int deg, vvi &vecgraph);
	bool bfs();
	bool dfs(int u);
	int hopcroftKarp();
};

#endif // graphdef