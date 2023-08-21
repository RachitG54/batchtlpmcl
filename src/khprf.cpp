#include "khprf.h"

// Function to perform the setup
void khprf::setup(int sz) {
    // Initialize MCL library
    n = sz;
    // initPairing(mcl::BLS12_381);
    // // if (initPairing(mcl::BLS12_381) != 0) {
    // //     cout << "Failed to initialize MCL library!" << endl;
    // //     return;
    // // }

    // // If you want to use the same BLS12-381 generator as [zkcrypto](https://www.ietf.org/archive/id/draft-irtf-cfrg-pairing-friendly-curves-11.html#section-4.2.1) then,

	// G1 P;
	// bool check;
	// const char *g1Str = "1 0x17f1d3a73197d7942695638c4fa9ac0fc3688c4f9774b905a14e3a3f171bac586c55e83ff97a1aeffb3af00adb22c6bb 0x08b3f481e3aaa0f1a09e30ed741d8ae4fcf5e095d5d00af600db18cb2c04b3edd03cc744a2888ae40caa232946c5e7e1";
	// P.setStr(&check,g1Str, 16);

	// G2 Q;
	// const char *g2Str = "1 0x24aa2b2f08f0a91260805272dc51051c6e47ad4fa403b02b4510b647ae3d1770bac0326a805bbefd48056c8c121bdb8 0x13e02b6052719f607dacd3a088274f65596bd0d09920b61ab5da61bbdc7f5049334cf11213945d57e5ac7d055d042b7e 0x0ce5d527727d6e118cc9cdc6da2e351aadfd9baa8cbdd3a76d429a695160d12c923ac9cc3baca289e193548608b82801 0x0606c4a02ea734cc32acd2b02bc28b99cb3e287e85a763af267492ab572e99ab3f370d275cec1da1aaa9075ff05f79be";
	// Q.setStr(&check,g2Str,16);

    // GT tempgt;
    // pairing(tempgt,P,Q);
    // cout<<tempgt<<" is temporary pairing operation\n";
    // Generate a random element x in the order of the asymmetric group

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
    // cout<<"key is "<<key<<"\n";

    stringstream ss;
    ss << key;
    string keystr = ss.str();
    // keylen = (keystr.size() * 4) / 3 + 1; // An approximate size for base 256 
}

string khprf::getkey() {
    stringstream ss;
    ss << key;
    return ss.str();
}

int khprf::getkeylen() {
    return keylen;
}
void khprf::clearkey() {
    key.clear();
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