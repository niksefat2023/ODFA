#ifdef _DEBUG
#  pragma comment ( lib, "cryptlibd" )
#else
#  pragma comment ( lib, "cryptlib" )
#endif

#ifndef CRYPTOPP_H
#define CRYPTOPP_H
// Crypto++ Library

#include <stdio.h>
//#include <conio.h>

#include <iomanip>
#include "cryptopp-lib/osrng.h"

// Runtime Includes
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <stdexcept>
using std::exception;
using std::runtime_error;
#include "EC_Naor_OT.h"
#include "Experiments.h"
#include <time.h>
#include "hr_time.h"

//extern CryptoPP::Integer *random_2p;
//extern byte* random_bytes;
//extern int *random_0_dfa;
//extern int Getrandom_counter;
//extern int random_bytes_counter;
//
//extern int random_bytes_index;
//extern int random_0_dfa_index;
//extern int random_2p_index;

#endif
