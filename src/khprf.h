#ifndef khprfdef
#define khprfdef

#include <iostream>
#include <vector>
#include <mcl/bls12_381.hpp>


using namespace mcl::bn;
using namespace std;

class khprf {
private:
    vector<G1> g1;
    vector<G2> g2;
    int n;
    Fr key;
public:
	void setup(int sz);
	void setkey();
	void prfeval(GT &result, int i);
	void puncture(G1 &result, int i);
	void punceval(GT &result, G1 &punckey, int punci, int i);
	void print();
};

#endif // khprfdef