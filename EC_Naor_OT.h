#ifdef _DEBUG
#  pragma comment ( lib, "cryptlibd" )
#else
#  pragma comment ( lib, "cryptlib" )
#endif
#ifndef EC_NAOR_OT_H
#define EC_NAOR_OT_H
#include "cryptopp.h"
// Crypto++ Includes
#include "cryptopp-lib/cryptlib.h"
#include "cryptopp-lib/oids.h"
#include "cryptopp-lib/osrng.h"
#include "cryptopp-lib/eccrypto.h"
#include "cryptopp-lib/asn.h"
#include "cryptopp-lib/ecp.h"
#include "cryptopp-lib/ec2n.h"
#include "cryptopp-lib/simple.h"
#include "cryptopp-lib/sha.h"
#include <sstream>
#include <iomanip>
#include <bitset>

//sali
#include <math.h>
#include <cppsocket/tcpserver.h>
#include <cppsocket/socket.h>
#include <cppsocket/stringbuffer.h>
//CPPSocket::Socket *connection;
int myrecv(void* ptr, int len);
int mysend(void* ptr, int len);



using namespace std;


// This function will implement the elliptic curve version of 
// amortized naor-paskin oblivious transfer protocol.
void PrintBytes(byte* data,int length);

void PrintBytes(char* x,byte* data,int length);
byte* PRG_32_to_m_bytes(byte* data, int m);
byte* PRG_32_to_m_bytes_hash(byte* data, int m);
byte* PRG_n_to_m_bytes_hash(byte* data, int n, int m);
byte* convert_int_to_byte(int& length,CryptoPP::Integer i);
byte* convert_int_to_byte(int& length,int x);
byte* convert_int_to_kbyte(int k,int x);
int convert_byte_to_int(int length,byte* input);
byte* XOREncrypt(byte* dataToBeEncrypted, byte* key, int length);

// Use precomputed random numbers
byte* GetRandomBytes(int n_bytes);
CryptoPP::Integer GetRandom(CryptoPP::Integer themin,CryptoPP::Integer themax);
int GetRandom(int themin,int themax);


//byte* GetRandomBytes2(int n_bytes);
//CryptoPP::Integer GetRandom2(CryptoPP::Integer themin,CryptoPP::Integer themax);
//int GetRandom2(int themin,int themax);


void OT(int n_ots           //number of OTs
	,int message_size    //message_size: number of message bytes
	,byte** s_messages[] //The array of messages: messages[n_inputs][message_size] of bytes
	,byte* c_messages[]  //The array of chooser's messages (to be filled after protocol)
	,int selections[]    //The choosers selection bits
);

int OT_80_256(
	 int n_ots             //number of OTs
	,int message_size      //message_size: number of message bytes
	,byte** s_messages[2]  //The array of messages: messages[n_ots][message_size] of bytes
	,byte* c_messages[]    //The array of chooser's messages (to be filled after protocol)
	,int selections[]      //The choosers selection bits
);

int OT_80_m(int n_ots       //number of OTs
	,int message_size       //message_size: number of message bytes
	,byte** s_messages[2]   //The array of messages: messages[n_ots][message_size] of bytes
	,byte* c_messages[]     //The array of chooser's messages (to be filled after protocol)
	,int selections[]       //The choosers selection bits
);

int OT_n_m(int n_ots        //number of OTs
	,int message_size       //message_size: number of message bytes
	,byte** s_messages[2]   //The array of messages: messages[n_ots][message_size] of bytes
	,byte* c_messages[]     //The array of chooser's messages (to be filled after protocol)
	,int selections[]       //The choosers selection bits
);

int OT_80_m_d(int n_ots       //number of OTs
	,int* message_size        //message_size: number of message bytes
	,byte** s_messages[2]     //The array of messages: messages[n_ots][message_size] of bytes
	,byte* c_messages[]       //The array of chooser's messages (to be filled after protocol)
	,int selections[]         //The choosers selection bits
);

int OT_n_m_d(int n_ots        //number of OTs
	,int* message_size        //message_size: number of message bytes
	,byte** s_messages[2]     //The array of messages: messages[n_ots][message_size] of bytes
	,byte* c_messages[]       //The array of chooser's messages (to be filled after protocol)
	,int selections[]         //The choosers selection bits
);


void OT_dynamic_message_size(int n_ots           //number of OTs
	,int* message_size    //message_size: number of message bytes
	,byte** s_messages[]  //The array of messages: messages[n_inputs][message_size] of bytes
	,byte* c_messages[]   //The array of chooser's messages (to be filled after protocol)
	,int selections[]     //The choosers selection bits
);

void OT_dynamic_256alpha(int n_ots
	,int* message_size
	,byte** s_messages[]
	,byte* c_messages[]
	,int selections[]
);

//sali
int OT_80_256_server(int n_ots    //number of OTs
			,int message_size  //message_size: number of message bytes
			,byte** s_messages[2]  //The array of messages: messages[n_ots][message_size] of bytes
		);
		
 int OT_80_256_client(int n_ots    //number of OTs
 			,int message_size  //message_size: number of message bytes
 			,byte* c_messages[]  //The array of chooser's messages (to be filled after protocol)
 			,int selections[] //The choosers selection bits
 		);
		

int OT_80_m_d_server(int n_ots       //number of OTs
	,int* message_size       //message_size: number of message bytes for each OT
	,byte** s_messages[2]   //The array of messages: messages[n_ots][message_size] of bytes
);

int OT_80_m_d_client(int n_ots       //number of OTs
	,int* message_size       //message_size: number of message bytes for each OT
	,byte* c_messages[]     //The array of chooser's messages (to be filled after protocol)
	,int selections[]       //The choosers selection bits
);

int OT_n_m_d_server(int n_ots       //number of OTs
	,int* message_size       //message_size: number of message bytes
	,byte** s_messages[2]   //The array of messages: messages[n_ots][message_size] of bytes
);

int OT_n_m_d_client(int n_ots       //number of OTs
	,int* message_size       //message_size: number of message bytes
	,byte* c_messages[]     //The array of chooser's messages (to be filled after protocol)
	,int selections[]       //The choosers selection bits
);

int OT_80_m_server(int n_ots       //number of OTs
	,int message_size       //message_size: number of message bytes for each OT
	,byte** s_messages[2]   //The array of messages: messages[n_ots][message_size] of bytes
);

int OT_80_m_client(int n_ots       //number of OTs
	,int message_size       //message_size: number of message bytes for each OT
	,byte* c_messages[]     //The array of chooser's messages (to be filled after protocol)
	,int selections[]       //The choosers selection bits
);

void OT_dynamic_message_size_client(int n_ots           //number of OTs
	,int* message_size    //message_size: number of message bytes
	,byte* c_messages[]   //The array of chooser's messages (to be filled after protocol)
	,int selections[]     //The choosers selection bits
);

void OT_dynamic_message_size_server(int n_ots           //number of OTs
	,int* message_size    //message_size: number of message bytes
	,byte** s_messages[]  //The array of messages: messages[n_inputs][message_size] of bytes
);

void OT_dynamic_256alpha_client(int n_ots
	,int *message_size
	,byte* c_messages[]
	,int selections[]
	);

void OT_dynamic_256alpha_server(int n_ots
	,int *message_size
	,byte** s_messages[]
	);

#endif

