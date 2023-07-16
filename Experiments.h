#ifdef _DEBUG
#  pragma comment ( lib, "cryptlibd" )
#else
#  pragma comment ( lib, "cryptlib" )
#endif
#ifndef EC_EXPERIMENTS_H
#define EC_EXPERIMENTS_H
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
#include <iostream>
#include <fstream>

#include "EC_Naor_OT.h"
#include "OAE.h"
#include "DFA.h"
#include "cryptopp.h"


struct s_stopwatch
{
	double garbled_dfa; //main protocol server time
	double client;      //main protocol client time
	double server_ot;   //server OT time
	double client_ot;   //Client OT time
};

struct s_exp
{
	s_stopwatch stopwatch;
	int q;
	int n;
	int runs;
	char* exp_name;
};

int** generate_dfa(int dfa_size);
int* generate_inputs(int input_size);
byte** generate_messages(int message_size,int dfa_size);
void PrintTimer(struct s_stopwatch timer);
void Print_Experiment(char* exp,s_exp exp_res[],int n_exp,bool append);
void Cout_Experiment(char* exp,s_exp exp_res[],int n_exp);
struct s_stopwatch avg_test_xor(int input_size,int dfa_size,int n_runs);
struct s_stopwatch avg_test_prg(int input_size,int dfa_size,int n_runs);
double avg_test_norm(int input_size,int dfa_size,int n_runs);
struct s_stopwatch test_xor(int input_size,int dfa_size);
struct s_stopwatch test_prg(int input_size,int dfa_size);
double test_norm(int input_size,int dfa_size);
s_stopwatch test_prg_256(int input_size,int dfa_size);

double benchmark_mult(int n_mult);
double benchmark_sub(int n_sub);
void test();
//sali
int read_dfa_file(char *nfile, int* s_start, int* states_num, int* accepting_num,int*** accepting_states_ids,int*** transitions,int n_alpha);
byte** set_messages(int message_size,int dfa_size,int** accepting_states_id,int accepting_num);


extern double sw_r1,sw_r2,sw_r3;
extern double sw_garbled_dfa,sw_s_OT,sw_c_OT,sw_c,sw_c_mot,sw_s_mot,sw_c_temp,sw_s_temp;
extern double rand_time;
extern unsigned int comm_send,comm_recv;

//sali - network
s_stopwatch test_prg_client(int input_size,int dfa_size);
s_stopwatch test_prg_server(int input_size,int dfa_size);
s_stopwatch test_prg_256_client(int input_size,int dfa_size);
s_stopwatch test_prg_256_server(int input_size,int dfa_size);

s_stopwatch test_prg_256_client_file(char* input_file);
s_stopwatch test_prg_256_server_file(char* dfa_file);

#endif

