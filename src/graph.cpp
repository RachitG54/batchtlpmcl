//SPDX-License-Identifier: MIT OR GPL-3.0-only
/*
 * Rachit Garg <rachg96@cs.utexas.edu>
 * 2023
 * */

#include "graph.h"

void graph::initialize(int noleft, int noright, int d, vvi &vecgraph) {
    n_left = noleft;
    n_right = noright;
    deg = d;
    graphvec = vecgraph;
    dist.resize(n_left+1);
    matchLeft.resize(n_left+1);
    matchedgeLeft.resize(n_left+1);
    matchRight.resize(n_right+1);
    NIL = 0;
}
bool graph::bfs() {
    int N = n_left;
    int M = n_right;
    queue<int> q;

    for (int i = 1; i <= N; i++) {
        if (matchLeft[i] == NIL) {
            dist[i] = 0;
            q.push(i);
        } else {
            dist[i] = INT_MAX;
        }
    }

    dist[NIL] = INT_MAX;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u != NIL) {
            for (int v : graphvec[u]) {
                if (dist[matchRight[v]] == INT_MAX) {
                    dist[matchRight[v]] = dist[u] + 1;
                    q.push(matchRight[v]);
                }
            }
        }
    }

    return dist[NIL] != INT_MAX;
}

bool graph::dfs(int u) {
    if (u != NIL) {
        REP(i,0,sz(graphvec[u])-1) {
            int v = graphvec[u][i];
        // for (int v : graphvec[u]) {
            if (dist[matchRight[v]] == dist[u] + 1 && dfs(matchRight[v])) {
                matchLeft[u] = v;
                matchedgeLeft[u] = i;
                matchRight[v] = u;
                return true;
            }
        }

        dist[u] = INT_MAX;
        return false;
    }

    return true;
}

int graph::hopcroftKarp() {
    int N = n_left;
    int M = n_right;

    fill(all(matchLeft),NIL);
    fill(all(matchRight),NIL);
    fill(all(dist),NIL);
    
    // cout <<"here in hopcroft\n";

    int matching = 0;

    while (bfs()) {

        // cout <<"here in hopcroft2\n";
        for (int i = 1; i <= N; i++) {
            if (matchLeft[i] == NIL && dfs(i)) {
                matching++;
            }
        }
    }

    return matching;
}