#ifndef EC_DFA_H
#define EC_DFA_H
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


byte* Eval_DFA(int input_size, // The number of clients inputs
	int* inputs,			// The client inputs
	int dfa_size,			// The number of DFA states
	int* dfa[2],			// The DFA with 2 inputs {0,1}
	int message_size,		// The size of message in the last state (in Bits)
	byte** messages
	);


#endif

