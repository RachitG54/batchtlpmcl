#include <iostream>
#include <vector>
#include <mcl/bls12_381.hpp>

using namespace mcl::bn;
using namespace std;

// Function to perform the setup
void setup(int n, vector<G1>& arr1, vector<G2>& arr2) {
    // Initialize MCL library
    initPairing(mcl::BLS12_381);
    // if (initPairing(mcl::BLS12_381) != 0) {
    //     cout << "Failed to initialize MCL library!" << endl;
    //     return;
    // }

    // If you want to use the same BLS12-381 generator as [zkcrypto](https://www.ietf.org/archive/id/draft-irtf-cfrg-pairing-friendly-curves-11.html#section-4.2.1) then,

	G1 P;
	bool check;
	const char *g1Str = "1 0x17f1d3a73197d7942695638c4fa9ac0fc3688c4f9774b905a14e3a3f171bac586c55e83ff97a1aeffb3af00adb22c6bb 0x08b3f481e3aaa0f1a09e30ed741d8ae4fcf5e095d5d00af600db18cb2c04b3edd03cc744a2888ae40caa232946c5e7e1";
	P.setStr(&check,g1Str, 16);

	G2 Q;
	const char *g2Str = "1 0x24aa2b2f08f0a91260805272dc51051c6e47ad4fa403b02b4510b647ae3d1770bac0326a805bbefd48056c8c121bdb8 0x13e02b6052719f607dacd3a088274f65596bd0d09920b61ab5da61bbdc7f5049334cf11213945d57e5ac7d055d042b7e 0x0ce5d527727d6e118cc9cdc6da2e351aadfd9baa8cbdd3a76d429a695160d12c923ac9cc3baca289e193548608b82801 0x0606c4a02ea734cc32acd2b02bc28b99cb3e287e85a763af267492ab572e99ab3f370d275cec1da1aaa9075ff05f79be";
	Q.setStr(&check,g2Str,16);

    // Generate a random element x in the order of the asymmetric group
    Fr x;
    x.setByCSPRNG();

    // Compute arr1 of size n+1
    arr1.resize(n + 1);
    arr1[0] = P; // Generator g1

    for (int i = 1; i <= n; ++i) {
        G1::mul(arr1[i], arr1[i - 1], x);
    }

    // Compute arr2 of size 2n+1
    arr2.resize(2 * n + 1);
    arr2[0] = Q; // Generator g2

    for (int i = 1; i <= 2 * n; ++i) {
        G2::mul(arr2[i], arr2[i - 1], x);
    }

    // Set arr2[n + 1] to a null or dummy element
    arr2[n + 1].clear();
}

int main() {
    // Define the variables
    int n = 10;
    vector<G1> arr1;
    vector<G2> arr2;

    // Call the setup function
    setup(n, arr1, arr2);

    // Print the computed arrays
    cout << "arr1: ";
    for (const auto& element : arr1) {
        cout << element << " ";
    }
    cout << endl;

    cout << "arr2: ";
    for (const auto& element : arr2) {
        cout << element << " ";
    }
    cout << endl;

    return 0;
}
