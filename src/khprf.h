#ifndef khprfdef
#define khprfdef

#include "btlpincludes.h"

class khprf {
private:
    vector<G1> g1;
    vector<G2> g2;
    int n;
    Fr key;
    G1 punckey;
    int keylen = 0;
public:
	void setup(int sz);
	void setkey();
	string getkey();
	int getkeylen();
	void clearkey();
	void prfeval(GT &result, int i);
	void puncture(int i);
	void punceval(GT &result, int punci, int i);
	void print();
};

#endif // khprfdef