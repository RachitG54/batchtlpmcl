#include "btlpincludes.h"

void handleErrors(string s) {
  // ERR_print_errors_fp(stderr);
  cerr << "Error: " << s << endl;
  abort();
}

void initpairing() {
  initPairing(mcl::BLS12_381);
  // if (initPairing(mcl::BLS12_381) != 0) {
  //     cout << "Failed to initialize MCL library!" << endl;
  //     return;
  // }
}
void getpairing(G1 &P, G2 &Q) {
  // If you want to use the same BLS12-381 generator as [zkcrypto](https://www.ietf.org/archive/id/draft-irtf-cfrg-pairing-friendly-curves-11.html#section-4.2.1) then,
  // G1 P;
  bool check;
  const char *g1Str = "1 0x17f1d3a73197d7942695638c4fa9ac0fc3688c4f9774b905a14e3a3f171bac586c55e83ff97a1aeffb3af00adb22c6bb 0x08b3f481e3aaa0f1a09e30ed741d8ae4fcf5e095d5d00af600db18cb2c04b3edd03cc744a2888ae40caa232946c5e7e1";
  P.setStr(&check,g1Str, 16);

  // G2 Q;
  const char *g2Str = "1 0x24aa2b2f08f0a91260805272dc51051c6e47ad4fa403b02b4510b647ae3d1770bac0326a805bbefd48056c8c121bdb8 0x13e02b6052719f607dacd3a088274f65596bd0d09920b61ab5da61bbdc7f5049334cf11213945d57e5ac7d055d042b7e 0x0ce5d527727d6e118cc9cdc6da2e351aadfd9baa8cbdd3a76d429a695160d12c923ac9cc3baca289e193548608b82801 0x0606c4a02ea734cc32acd2b02bc28b99cb3e287e85a763af267492ab572e99ab3f370d275cec1da1aaa9075ff05f79be";
  Q.setStr(&check,g2Str,16);
}


void getrandGT(GT &randgt) {
  G1 P;
  G2 Q;
  getpairing(P,Q);

  GT gt;
  pairing(gt, P,Q);
  Fr x;
  x.setByCSPRNG();

  GT::pow(randgt,gt,x);
}

void printmpz(mpz_t &num) {
  char *tmp = mpz_get_str(NULL,10,num);
  cout << "Printed mpz integer "<<tmp<<"\n";
}

// template<typename T>
// ll getmclbytes(const T& x) {
//   char ser[1000];
//   ll sz = x.serialize(ser,1000); // returns size of punctured key
//   return sz;
// }

ll getmclbytes(const G1& x) {
  char ser[1000];
  ll sz = x.serialize(ser,1000); // returns size of punctured key
  return sz;
}
ll getmclbytes(const G2& x) {
  char ser[1000];
  ll sz = x.serialize(ser,1000); // returns size of punctured key
  return sz;
}
ll getmclbytes(const GT& x) {
  char ser[1000];
  ll sz = x.serialize(ser,1000); // returns size of punctured key
  return sz;
}


ll getmpzbytes(const mpz_t& x) {
  size_t sizeInBits = mpz_sizeinbase(x, 2);
  ll sz = (sizeInBits + 7) / 8;
  return sz;
}

double CRSgentime = 0;
double pairCRSgentime = 0;
double puzzleCRSgentime = 0;
double gentime = 0;
double pairgentime = 0;
double puzzlegentime = 0;
double solvetime = 0;
double pairsolvetime = 0;
double puzzlesolvetime = 0;
double batchsolvetime = 0;
double pairbatchsolvetime = 0;
double puzzlebatchsolvetime = 0;
double graphbatchsolvetime = 0;
ll CRSbytes = 0;
ll puzzlebytes = 0;

// void convertToByteString(const std::string& numberStr, unsigned char* byteBuffer, size_t bufferSize) {


//   mpz_t number;
//   mpz_init(number);
//   mpz_init_set_ui ( number , 0 ) ;
//   for(int i = 0 ; i < numberStr ; i++) {
//     mpz_mul_ui ( s , s , 1 << 8 ) ;
//     mpz_add_ui ( s , s , (uint8_t)str[i] ) ;
//   }
//   mpz_clear(number);
// }

// Convert a very long base 10 number to binary bytes
// void convertToByteString(const std::string& numberStr, unsigned char* byteBuffer, size_t bufferSize) {
//     mpz_t number;
//     mpz_init(number);
//     mpz_set_str(number, numberStr.c_str(), 10);

//     // gmp_printf("Number: %Zd\n", number);
//     // size_t byteCount = mpz_sizeinbase(number, 10)+2;

//     mpz_t remainder;
//     mpz_init(remainder);
//     REP(i,0,bufferSize-1) {
//       mpz_mod_ui(remainder, number, 256);
//       byteBuffer[i] = mpz_get_ui(remainder);
//       mpz_div_ui(number,number,256);
//       // cout << (int)byteBuffer[i]<<" ";
//     }
//     mpz_clear(remainder);
//     // if (byteCount > bufferSize) {
//     //    cerr << byteCount << " " << bufferSize << "\n";
//     //    cerr << "Buffer size is insufficient for the conversion." << endl;
//     //     mpz_clear(number);
//     //     return;
//     // }

//     // mpz_export(byteBuffer, nullptr, 1, sizeof(unsigned char), 0, 0, number);
//     mpz_clear(number);
// }

// string convertToBase10String(const unsigned char* byteBuffer, size_t bufferSize) {
//     mpz_t number;
//     mpz_init(number);

//     mpz_init_set_ui ( number , 0 ) ;
//     mpz_t powr;
//     mpz_init(powr);

//     mpz_t intval;
//     mpz_init(intval);

//     mpz_set_ui(powr, 1);
//     for(int i = 0 ; i < bufferSize-1 ; i++) {
//       mpz_mul_ui ( intval , powr, (uint8_t)byteBuffer[i]) ;
//       mpz_add ( number , number , intval ) ;
//       mpz_mul_ui ( powr , powr , 1 << 8 ) ;
//       gmp_printf("Result: %Zd\n", number);
//     }
//     mpz_clear(intval);
//     mpz_clear(powr);
//     // mpz_import(number, bufferSize, 1, sizeof(unsigned char), 0, 0, byteBuffer);

//     // gmp_printf("New Number: %Zd\n", number);

//     char* base10Str = mpz_get_str(nullptr, 10, number);
//     string result = base10Str;

//     mpz_clear(number);
//     free(base10Str);

//     return result;
// }

// void printinbase10(mpz_t &num) {
//   size_t bufferSize = mpz_sizeinbase(num, 256)+2;
//   unsigned char* byteBuffer = new unsigned char[bufferSize]();
//   mpz_export(byteBuffer, nullptr, 1, sizeof(unsigned char), 0, 0, num);
//   string answer = convertToBase10String(byteBuffer,bufferSize);
//   cout << "answer is "<<answer<<"\n";
// }