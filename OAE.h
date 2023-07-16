#ifdef _DEBUG
#  pragma comment ( lib, "cryptlibd" )
#else
#  pragma comment ( lib, "cryptlib" )
#endif
#ifndef EC_OAE_H
#define EC_OAE_H
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
#include "EC_Naor_OT.h"

void random_permute(int a[],int size);
byte* combine_bytes(byte* one,int one_size,byte* two,int two_size);
void divide_bytes(byte* combination,byte* one,int one_size,byte* two,int two_size);
byte* OAE_XOR(int input_size, // The number of clients inputs
	int* inputs,			// The client inputs
	int dfa_size,			// The number of DFA states
	int* dfa[2],			// The DFA with 2 inputs {0,1}
	int message_size,		// The size of message in the last state (in Bits)
	byte** messages			// The message in each state Byte*
	);

byte* OAE_PRG(int input_size, // The number of clients inputs
	int* inputs,			// The client inputs
	int dfa_size,			// The number of DFA states
	int* dfa[2],			// The DFA with 2 inputs {0,1}
	int message_size,		// The size of message in the last state (in Bits)
	byte** messages			// The message in each state Byte*
	);

byte* OAE_PRG_256(int input_size, // The number of clients inputs
	int* inputs,			// The client inputs (converted from asci to int already)
	int dfa_size,			// The number of DFA states
	int* dfa[256],			// The DFA with 256 inputs
	int message_size,		// The size of message in the last state (in Bits)
	byte** messages			// The message in each state Byte*
	);


//sali
byte* OAE_PRG_client(int input_size, // The number of clients inputs
	int* inputs,			// The client inputs
	int dfa_size,			// The number of DFA states
	int message_size		// The size of message in the last state (in Bits)
	);

int OAE_PRG_server(int input_size, // The number of clients inputs
	int dfa_size,			// The number of DFA states
	int* dfa[2],			// The DFA with 2 inputs {0,1}
	int message_size,		// The size of message in the last state (in Bits)
	byte** messages			// The message in each state Byte*
	);

byte* OAE_PRG_256_client(int input_size, // The number of clients inputs
	int* inputs,			// The client inputs (converted from asci to int already)
	int dfa_size,			// The number of DFA states
	int message_size		// The size of message in the last state (in Bits)
	);

byte* OAE_PRG_256_server(int input_size, // The number of clients inputs
	int dfa_size,			// The number of DFA states
	int* dfa[256],			// The DFA with 256 inputs
	int message_size,		// The size of message in the last state (in Bits)
	byte** messages			// The message in each state Byte*
	);


#endif

