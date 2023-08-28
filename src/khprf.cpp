#include "khprf.h"

// Function to perform the setup
void khprf::setup(int sz) {
    // Initialize MCL library
    n = sz;

    G1 P;
    G2 Q;
    // initpairing();
    getpairing(P, Q);

    Fr x;
    x.setByCSPRNG();
    // cout<<"x is "<<x<<"\n";

    // Compute arr1 of size n+1
    g1.resize(n + 1);
    g1[0] = P; // Generator g1

    for (int i = 1; i <= n; ++i) {
        G1::mul(g1[i], g1[i - 1], x);
    }

    // Compute arr2 of size 2n+1
    g2.resize(2 * n + 2);
    g2[0] = Q; // Generator g2

    for (int i = 1; i <= 2 * n+1; ++i) {
        G2::mul(g2[i], g2[i - 1], x);
    }

    // Set arr2[n + 1] to a null or dummy element
    g2[n + 1].clear();
}

void khprf::print() {
    cout << "n is "<<n<<"\n";
    cout << "g1: ";
    for (const auto& element : g1) {
        cout << element << " ";
    }
    cout << "\n";

    cout << "g2: ";
    for (const auto& element : g2) {
        cout << element << " ";
    }
    cout << "\n";
}

void khprf::setkey() {
    key.setByCSPRNG();
}
void khprf::setkey(Fr &key2) {
    key = key2;
}
void khprf::setpunckey(G1 &punckey2) {
    punckey = punckey2;
}

string khprf::getkey() {
    stringstream ss;
    ss << key;
    return ss.str();
}

void khprf::clearkeys() {
    key.clear();
    punckey.clear();
}

void khprf::prfeval(GT &result, int i) {
    GT val;
    pairing(val, g1[0],g2[n+1+i]);
    GT::pow(result,val,key);
}


void khprf::puncture(int i) {
    G1::mul(punckey,g1[i],key);
}

void khprf::punceval(GT &result, int punci, int i) {
    if (i==punci) {
        cout<<"cannot evaluate at punctured spot\n";
        return;
    }
    pairing(result, punckey,g2[n+1+i-punci]);
}

// ll khprf::szbytes() {
//     return 0;
// }