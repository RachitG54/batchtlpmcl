#ifndef btlpincludes
#define btlpincludes

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/time.h>
#include <gmp.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <mcl/bls12_381.hpp>
#include <openssl/sha.h>

using namespace std;
using namespace std::chrono;
using namespace mcl::bn;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> ii;
typedef vector<ii> vii;
typedef long long ll;
typedef vector<ll> vlli;
typedef unsigned char uchar;

//#define DEBUG

// #ifdef DEBUG
// #define NETWORKTEST
// #endif

#define sz(a) int((a).size())
#define pb push_back
#define all(c) (c).begin(),(c).end()
#define tr(c,i) for(typeof((c).begin()) i = (c).begin(); i!=(c).end(); i++)
#define present(c,e) ((c).find(e) != (c).end())
#define cpresent(c,e) (find(all(c),e) != (c).end())
#define REP(i,a,b) for(int i=int(a); i<=int(b); i++)
#define constlen 20

#define PRIME_ITERS 25
#define SEC_PARAM 1024

void handleErrors(string s);
void convertToByteString(const std::string& numberStr, unsigned char* byteBuffer, size_t bufferSize);
string convertToBase10String(const unsigned char* byteBuffer, size_t bufferSize);
void initpairing();
void getpairing(G1 &P, G2 &Q);
void getrandGT(GT &randgt);

// uchar* readkeys(uint32_t &keylen, string keyname);
// set<string> readset(string name);
// void storeset(set<BIGNUM*> s, string name);
// set<string> readset2(string name);
// void storeset2(set<BIGNUM*> s, string name, uint32_t size);
// msgpack::object_handle readobject(string name);
// uchar** readlabels(int &n,int &labelsize);
// uint64_t toBigInt(uchar* bytes, uint32_t len);
// uint64_t toBigInt(vector<uchar> bytes, uint32_t len);
// uint32_t toInt(uchar* bytes);
// uchar* tochar(uint32_t value, uint32_t len);
// void getAllLinesFromFile(string fileName, vector<string>& output);
// void get_file_contents(const char *filename, string &contents);

// // Unpacking two message pack items one after the other, using templates for variable datatyoe
// template <class T, class U> void readobjstore (T &obj, U &obj2, string name) {
// 	ifstream fin;
// 	msgpack::unpacker pac;
//     fin.open("./src/EDB/"+name,ios::in);
//     stringstream ss;
//     ss << fin.rdbuf();
//     string buffer = ss.str();
//     // feeds the buffer.
//     pac.reserve_buffer(buffer.size());
//     memcpy(pac.buffer(), buffer.data(), buffer.size());
//     pac.buffer_consumed(buffer.size());

//     // now starts streaming deserialization.
//     msgpack::object_handle oh;
//     pac.next(oh);
//     msgpack::object msgobj = oh.get();
//     msgobj.convert(obj);
//     pac.next(oh);
//     msgpack::object msgobj2 = oh.get();
//     msgobj2.convert(obj2);
//     fin.close();
// }
// // Reading file from any datatype and returning file size
// template <class T> int readfrommsgpackfile(T &obj, string filename) {
//     ifstream inp(filename,ios::binary);

//     std::stringstream buffer;
//     buffer << inp.rdbuf();
//     string str = buffer.str();
//     inp.close();
//     {
//         msgpack::object_handle oh = msgpack::unpack(str.data(),str.size());
//         oh.get().convert(obj);
//     }
//     int filelen = str.size();
//     return filelen;
// }
// // Writing any datatype into file using message pack
// template <class T> int writetomsgpackfile(T &obj, string filename) {
//     msgpack::sbuffer sbuf;
//     msgpack::pack(sbuf, obj);
//     ofstream out(filename,ios::binary);
//     out.write(sbuf.data(),sbuf.size());
//     out.close();
//     return sbuf.size();
// }
// int filelength(string filename);
// int compress_vector(string &message);
// int decompress_vector(string &message);

#endif // Includes