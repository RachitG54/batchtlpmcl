// #ifndef khprf
// #define khprf

// #include <mcl/bls12_381.hpp>

// class pp {
// private:
//     pairing_ptr pairing;
//     element_t *g1arr;  // size n+1. Jesko: I put g1 in this because g1^{x^0}=g1 and then the indeces for the arrary matches the paper
//     element_t *g2arr;  // size 2n+2 with n+1 null. Jesko: same here
//     int n;

// };

// class EDBSetup {
// private:
// 	SSEECDH group;
// 	uint32_t indlen = 4;
// 	uint32_t KEYLENGTH;

// public:
// 	uint32_t enclen = 32;
// 	TSet TSetobj;
//     EDBSetup();
//     void initialize();
//     uint32_t getindlen();
//     ~EDBSetup();
// };

// #endif // khprf