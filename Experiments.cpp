#include "Experiments.h"
#include <math.h>

double sw_r1=0,sw_r2=0,sw_r3=0;
double sw_garbled_dfa=0,sw_s_OT=0,sw_c_OT=0,sw_c=0,sw_c_mot=0,sw_s_mot=0,sw_c_temp=0,sw_s_temp=0;
double rand_time=0;

unsigned int comm_send=0,comm_recv=0;

using namespace std;


void PrintTimer(s_stopwatch timer)
{
	cout<<"Client Time      <ms>:"<<timer.client*1000<<endl;
	cout<<"Client OT Time   <ms>:"<<timer.client_ot*1000<<endl;
	cout<<"garbled dfa Time <ms>:"<<timer.garbled_dfa*1000<<endl;
	cout<<"Server OT Time   <ms>:"<<timer.server_ot*1000<<endl<<endl;
	cout<<"Client Sum       <ms>:"<<(timer.client+timer.client_ot)*1000<<endl;
	cout<<"Server Sum       <ms>:"<<(timer.garbled_dfa+timer.server_ot)*1000<<endl;
}

void Print_Experiment(char* exp,s_exp exp_res[],int n_exp,bool append)
{
	ofstream file;
	char filename[100];
	sprintf(filename, "%s.txt",exp);
	
	if(!append){
		file.open(filename);
		file<<setw(10)<<"Exp Name"<<setw(5)<<"Q"<<setw(5)<<"N"<<setw(5)<<"Runs"<<setw(10)<<"C"<<setw(12)<<"COT"<<setw(10)<<"S"<<setw(10)<<"SOT"<<setw(10)<<"CSum"<<setw(10)<<"SSum"<<setw(10)<<"All"<<endl;
	}
	else
	{
		file.open(filename,ios::out | ios::app);
	}

	for (int i=0;i<n_exp;i++)
	{
		file<<setw(10)<<exp_res[i].exp_name<<setw(5)<<exp_res[i].q<<setw(5)<<exp_res[i].n<<setw(5)<<exp_res[i].runs<<setw(12)<<exp_res[i].stopwatch.client*1000<<setw(10)<<exp_res[i].stopwatch.client_ot*1000<<setw(10)<<exp_res[i].stopwatch.garbled_dfa*1000<<setw(10)<<exp_res[i].stopwatch.server_ot*1000<<setw(10)<<(exp_res[i].stopwatch.client+exp_res[i].stopwatch.client_ot)*1000<<setw(10)<<(exp_res[i].stopwatch.garbled_dfa+exp_res[i].stopwatch.server_ot)*1000<<setw(10)<<(exp_res[i].stopwatch.client+exp_res[i].stopwatch.client_ot+exp_res[i].stopwatch.garbled_dfa+exp_res[i].stopwatch.server_ot)*1000<<endl;
		
	}
	file.close();
}

void Cout_Experiment(char* exp,s_exp exp_res[],int n_exp)
{
	for (int i=0;i<n_exp;i++)
	{
		cout<<setw(10)<<exp_res[i].exp_name<<setw(5)<<exp_res[i].q<<setw(5)<<exp_res[i].n<<setw(5)<<exp_res[i].runs<<setw(12)<<exp_res[i].stopwatch.client*1000<<setw(10)<<exp_res[i].stopwatch.client_ot*1000<<setw(10)<<exp_res[i].stopwatch.garbled_dfa*1000<<setw(10)<<exp_res[i].stopwatch.server_ot*1000<<setw(10)<<(exp_res[i].stopwatch.client+exp_res[i].stopwatch.client_ot)*1000<<setw(10)<<(exp_res[i].stopwatch.garbled_dfa+exp_res[i].stopwatch.server_ot)*1000<<setw(10)<<(exp_res[i].stopwatch.client+exp_res[i].stopwatch.client_ot+exp_res[i].stopwatch.garbled_dfa+exp_res[i].stopwatch.server_ot)*1000<<endl;
		
	}
}

//OAE_XOR(int input_size, // The number of clients inputs
//	int* inputs,			// The client inputs
//	int dfa_size,			// The number of DFA states
//	int* dfa[2],			// The DFA with 2 inputs {0,1}
//	byte* result,			// The result of DFA evaluation
//	int message_size,		// The size of message in the last state (in Bits)
//	byte** messages			// The message in each state Byte*
//	)
//void CreateRandomPool(int input_size,int dfa_size)
//{
//	CryptoPP::Integer *random_2p;
//	byte* random_bytes;
//	int *random_0_dfa;
//	random_2p= new CryptoPP::Integer[10000*input_size*input_size];
//	CryptoPP::Integer p("6277101735386680763835789423207666416083908700390324961279");
//	for (int i=0;i<10000*input_size*input_size;i++)
//	{
//		random_2p[i] = GetRandom2(2,p);
//	}
//	random_bytes = new byte[input_size*dfa_size*input_size*1000];
//	random_bytes = GetRandomBytes2(input_size*dfa_size*input_size*1000);
//	random_0_dfa = new int[dfa_size*dfa_size*1000];
//	for (int i=0;i<dfa_size*dfa_size*1000;i++)
//	{
//		random_0_dfa[i] = GetRandom2(0,dfa_size-1);
//	}
//}

int** generate_dfa(int dfa_size)
{
	int** dfa = new int*[2];
	dfa[0] = new int[dfa_size];
	dfa[1] = new int[dfa_size];
	for (int i=0;i<dfa_size;i++)
	{
		dfa[0][i] = GetRandom(0,dfa_size-1);
		dfa[1][i] = GetRandom(0,dfa_size-1);
	}
	return dfa;
}

int** generate_dfa(int dfa_size,int n_alpha)
{
	int** dfa = new int*[n_alpha];
	for (int i=0;i<n_alpha;i++)
	{
		dfa[i] = new int[dfa_size];
	}
	for (int i=0;i<dfa_size;i++)
	{
		for (int j=0;j<n_alpha;j++)
		{
			dfa[j][i] = GetRandom(0,dfa_size-1);
		}
	}
	return dfa;
}

int* generate_inputs(int input_size)
{
	int* inputs = new int[input_size];
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::Integer temp(rng,input_size);
	for (int i=0;i<input_size;i++)
	{
		inputs[i] = temp.GetBit(i);
	}
	return inputs;
}

int read_dfa_file(char *nfile, int* s_start, int* states_num, int* accepting_num,int*** accepting_states_ids,int*** transitions,int n_alpha)
{  
  ifstream nfa_file;
  int i, tmps, j;

  unsigned int num_states, num_accepting_states, num_accept_ids, num_transitions;
  int tmp_st, tmp_acc_id, tmp_loc_s;

  nfa_file.open(nfile);
  if (!nfa_file) {
    cerr << "File could not be openned" << endl;
    exit(1);
  }

  /* read the number of states and num of (acc_state_no, accept_id) pairs */
  /* sali: now we assume that num of (acc_state_no, accept_id) is equal to number of accepting states. (One id per each accepting state) */
  nfa_file >> num_states >> num_accept_ids;
  
  /* allocate space and read accepting states and their signature-IDs */
  *accepting_states_ids = (int **)malloc(num_accept_ids * sizeof(int*));  
  for (i = 0; i < num_accept_ids; i++) {
    (*accepting_states_ids)[i]=(int*) malloc(2*sizeof(int));
    nfa_file >> tmp_st >> tmp_acc_id;

    (*accepting_states_ids)[i][0]= tmp_st;
    (*accepting_states_ids)[i][1]= tmp_acc_id;
  }
  
  /* read number of accepting states and the accepting state array */
  /* sali: not important for dfas */
  nfa_file >> num_accepting_states;
  for (i = 0; i < num_accepting_states; i++) {
    nfa_file >> tmps;
  } 
    
  /* read start state and the number of transitions */

    nfa_file >> *s_start >> num_transitions;
  
  //debug
//  cout << "Number of transitions " << num_transitions << endl;

  /* allocate space for transition table */
  *transitions = (int **)malloc(n_alpha * sizeof(int *));
  if (*transitions == NULL)
    perror("out of memory"), exit(1);
cout <<  "hi... here" <<endl; 
 //sali: read transition
  unsigned int tmp1,tmp2,tmp3;

        for (int z=0;z<n_alpha;z++)
        {
    		(*transitions)[z] =  (int *)malloc(num_states * sizeof(int));
    		if ((*transitions)[z] == NULL)
      			perror("out of memory"), exit(1);
        }

  for (i = 0; i < num_states; i++) {
    for (j=0;j<n_alpha;j++)
    {
        nfa_file >> tmp1 >> tmp2 >> tmp3;
        (*transitions)[tmp2][i] = tmp3;
    }
  } 
 
cout <<  "hi... here2" <<endl; 
  *states_num = num_states;
  *accepting_num = num_accepting_states;
  nfa_file.close();  
}


byte** generate_messages(int message_size,int dfa_size)
{
	int message_bytes = (int)ceil((double)message_size/8);
	char** messages = new char*[dfa_size];
//	char** new_messages = new char*[dfa_size];
//	CryptoPP::Integer temp;

	for (int i=0;i<dfa_size;i++)
	{
		messages[i] = new char[message_bytes];
		sprintf(messages[i], "M%06d", i);
	}

	
/*	for (int i=0;i<dfa_size;i++)
	{
		new_messages[i] = new char[12];
		sprintf(new_messages[i], "Message%04d", i);
		messages[i] = new byte[12];
		for (int j=0;j<12;j++)
		{
			messages[i] = reinterpret_cast<unsigned char*>(new_messages[i]);
		}
		/ *temp = (i+23);
		messages[i] = new byte[message_bytes];
		for (int j=0;j<message_bytes;j++)
		{
			if (j<temp.ByteCount())
			{
				messages[i][j] = temp.GetByte(j);
			}
			else
			{
				messages[i][j] = '\0';
			}
			
		}* /
		
	}
*/
	return (byte**)messages;
}
//sali
byte** set_messages(int message_size,int dfa_size,int** accepting_states_id,int accepting_num)
{
		//rewritten by salman on August18,2011
        int message_bytes = (int)ceil((double)message_size/8);
        byte** messages = new byte*[dfa_size];
        CryptoPP::Integer temp;

		if (message_bytes > 4)
		{		
			for (int i=0;i<dfa_size;i++)
			{
				messages[i] = new byte[message_bytes];
				memset(messages[i],0,message_bytes);
			}

	        for (int i=0;i<accepting_num;i++)
	        {
				memset(messages[accepting_states_id[i][0]],1,message_bytes);
	        }
//			cerr << "Error. Message bytes should not be more than 4 bytes." << endl;
//			exit(1);
		}
		else
		{
			int tmp=0;
	        for (int i=0;i<dfa_size;i++)
	        {
					messages[i] = new byte[message_bytes];
					//message_byte should be max 4 bytes which is enough for # of signature 
					//all dfa nodes have 0(clean) label  unless otherwise set to another value by the following loop
					memcpy(messages[i],&tmp,message_bytes);
	       }
	        
	        for (int i=0;i<accepting_num;i++)
	        {
	                //accepting_states_id[i][0] is the state number
	                //accepting_states_id[i][1] is the signature ID
					tmp = accepting_states_id[i][1];
					memcpy(messages[accepting_states_id[i][0]],&tmp,message_bytes);
	        }
		} 
        return messages;
}

s_stopwatch avg_test_xor(int input_size,int dfa_size,int n_runs)
{
	s_stopwatch stopwatch,t_timer;
	stopwatch.client_ot = 0;
	stopwatch.client = 0;
	stopwatch.garbled_dfa = 0;
	stopwatch.server_ot = 0;
	for (int i=0;i<n_runs;i++)
	{
		t_timer = test_xor(input_size,dfa_size);
		stopwatch.client_ot += t_timer.client_ot;
		stopwatch.client += t_timer.client;
		stopwatch.garbled_dfa += t_timer.garbled_dfa;
		stopwatch.server_ot += t_timer.server_ot;
	}
	stopwatch.client = stopwatch.client/n_runs;
	stopwatch.client_ot = stopwatch.client_ot/n_runs;
	stopwatch.garbled_dfa = stopwatch.garbled_dfa/n_runs;
	stopwatch.server_ot = stopwatch.server_ot/n_runs;
	s_exp temp[1];
	temp[0].stopwatch = stopwatch;
	temp[0].n=input_size;
	temp[0].q=dfa_size;
	temp[0].runs=n_runs;
	Cout_Experiment("Xor",temp,1);
	return stopwatch;
}
s_stopwatch avg_test_prg(int input_size,int dfa_size,int n_runs)
{
	s_stopwatch stopwatch,t_timer;
	stopwatch.client_ot = 0;
	stopwatch.client = 0;
	stopwatch.garbled_dfa = 0;
	stopwatch.server_ot = 0;
	for (int i=0;i<n_runs;i++)
	{
		t_timer = test_prg(input_size,dfa_size);
		stopwatch.client_ot += t_timer.client_ot;
		stopwatch.client += t_timer.client;
		stopwatch.garbled_dfa += t_timer.garbled_dfa;
		stopwatch.server_ot += t_timer.server_ot;
	}
	stopwatch.client = stopwatch.client/n_runs;
	stopwatch.client_ot = stopwatch.client_ot/n_runs;
	stopwatch.garbled_dfa = stopwatch.garbled_dfa/n_runs;
	stopwatch.server_ot = stopwatch.server_ot/n_runs;
	return stopwatch;
}
double avg_test_norm(int input_size,int dfa_size,int n_runs)
{
	double avg_time = 0;
	for (int i=0;i<n_runs;i++)
	{
		avg_time += test_norm(input_size,dfa_size);
	}
	avg_time=avg_time/n_runs;
	return avg_time;
}

s_stopwatch test_xor(int input_size,int dfa_size)
{
	s_stopwatch timer;
	int message_size = 12*8;
	//CreateRandomPool(input_size,dfa_size);
	int* inputs = generate_inputs(input_size);
	int** dfa = generate_dfa(dfa_size);
	byte** messages = generate_messages(message_size,dfa_size);
	byte* result;

	//for (int i=0;i<dfa_size;i++)
	//{
	//	cout<<i<<" = "<<dfa[0][i]<<"  "<<dfa[1][i]<<endl;	
	//}
	//cout<<"messages"<<endl;
	//for (int i=0;i<dfa_size;i++)
	//{
	//	cout<<i<<" = "<<messages[i]<<endl;	
	//}
	////inputs[0]=1;
	//cout<<"inputs"<<endl;
	//for (int i=0;i<input_size;i++)
	//{
	//	cout<<i<<" = "<<inputs[i]<<endl;	
	//}
	double rand_time;
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	sw_garbled_dfa=0;sw_s_OT=0;sw_c_OT=0;sw_c=0;sw_c_mot=0;sw_s_mot=0;

	//CStopWatch s_xor;
	//s_xor.startTimer();
	result = OAE_XOR(input_size,inputs,dfa_size,dfa,message_size,messages);
	//s_xor.stopTimer();
	//rand_time = (sw_r1+sw_r2+sw_r3);
	//timer = s_xor.getElapsedTime()-rand_time;
	timer.client_ot = sw_c_OT;
	timer.client = sw_c;
	timer.garbled_dfa = sw_garbled_dfa;
	timer.server_ot = sw_s_OT;
	return timer;
	//cout<<"dfa oblivious XOR"<<endl<<result<<endl;
	//cout<< "Overall Time (ms): " << (s_xor.getElapsedTime())*1000<<endl;
	//cout<< "Random  Time (ms): " << rand_time*1000<<endl;
	//cout<< "Net     Time (ms): " << timer*1000<<endl;
}
s_stopwatch test_prg(int input_size,int dfa_size)
{
	s_stopwatch timer;
	int message_size = 12*8;
	//CreateRandomPool(input_size,dfa_size);
	int* inputs = generate_inputs(input_size);
	int** dfa = generate_dfa(dfa_size);
	byte** messages = generate_messages(message_size,dfa_size);
	byte* result;

	//for (int i=0;i<dfa_size;i++)
	//{
	//	cout<<i<<" = "<<dfa[0][i]<<"  "<<dfa[1][i]<<endl;	
	//}
	//cout<<"messages"<<endl;
	//for (int i=0;i<dfa_size;i++)
	//{
	//	cout<<i<<" = "<<messages[i]<<endl;	
	//}
	////inputs[0]=1;
	//cout<<"inputs"<<endl;
	//for (int i=0;i<input_size;i++)
	//{
	//	cout<<i<<" = "<<inputs[i]<<endl;	
	//}
	double rand_time;
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;

	CStopWatch s_prg;
	s_prg.startTimer();
	//sali: check
	result = OAE_PRG(input_size,inputs,dfa_size,dfa,message_size,messages);
	timer.client_ot = sw_c_OT;
	timer.client = sw_c;
	timer.garbled_dfa = sw_garbled_dfa;
	timer.server_ot = sw_s_OT;
	s_prg.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
	timer.server_ot = s_prg.getElapsedTime()-rand_time;
	return timer;
 
	//cout<<"dfa oblivious PRG"<<endl;
	//cout<<result<<endl;
	//cout<< "Overall Time (ms): " << (s_prg.getElapsedTime())*1000<<endl;
	//cout<< "Random  Time (ms): " << rand_time*1000<<endl;
	//cout<< "Net     Time (ms): " << timer*1000<<endl;
}

s_stopwatch test_prg_256(int input_size,int dfa_size)
{
	s_stopwatch timer;
	int message_size = 12*8;
	//CreateRandomPool(input_size,dfa_size);
//sali
//	int* inputs = generate_inputs(input_size);

	//char input[]="test/webdriver";
	char input[]="testesti/wwwboard/passwd.txt";
	input_size = sizeof(input)-1;
	int* inputs = new int[input_size];
	for (int i=0;i<input_size;i++)
	{
		inputs[i]=int(input[i]);
		printf ("asc code: %d\n",inputs[i]);
	}

	cout << input << endl;
	cout << inputs[1] << endl;
//sali
//	int** dfa = generate_dfa(dfa_size,256);

    int states_num,accepting_num,s_start;
    int** accepting_states_ids;
    int** transitions;
    int** dfa;
    
    read_dfa_file("sig10.dfa",&s_start, &states_num, &accepting_num, &accepting_states_ids, &dfa,256);

    printf("# states: %d\n",states_num);
    printf("# accepting states: %d\n",accepting_num);    
    printf("start is: %d\n",s_start);
    printf("transition test.. Next State for:\nState 3,Alphabet 92: %d\nState 3,Alphabet 67:%d\nState 0,Alphabet 2:%d \n", dfa[92][3],dfa[67][3], dfa[2][0]);

dfa_size = states_num;

	//sali
	/*cout << "the dfa is:\n" ;
	for (int i=0;i<dfa_size;i++)
	{
		for (int j=0;j<256;j++)
			cout << dfa[j][i] << " " ;
		cout << endl;
	}*/
	//end - sali

	//sali
//	byte** messages = generate_messages(message_size,dfa_size);
	byte** messages = set_messages(message_size,dfa_size,accepting_states_ids,accepting_num);

/*
	for (int i=0;i<dfa_size;i++)
		cout << messages[i] << endl;	
*/		


	byte* result;
	byte* result_orig;

	result_orig = Eval_DFA(input_size,inputs,dfa_size,dfa,message_size,messages);
	
	//for (int i=0;i<dfa_size;i++)
	//{
	//	cout<<i<<" = "<<dfa[0][i]<<"  "<<dfa[1][i]<<endl;	
	//}
	//cout<<"messages"<<endl;
	//for (int i=0;i<dfa_size;i++)
	//{
	//	cout<<i<<" = "<<messages[i]<<endl;	
	//}
	////inputs[0]=1;
	//cout<<"inputs"<<endl;
	//for (int i=0;i<input_size;i++)
	//{
	//	cout<<i<<" = "<<inputs[i]<<endl;	
	//}
	double rand_time;
	/*sw_r1=0;
	sw_r2=0;
	sw_r3=0;*/

	/*CStopWatch s_prg;
	s_prg.startTimer();*/
//	printf ("here 2 ... \n");
	cout << input_size << " " << dfa_size << " " << message_size << endl;
	result = OAE_PRG_256(input_size,inputs,dfa_size,dfa,message_size,messages);
//	printf ("here 3 ... \n");
/*	timer.client_ot = sw_c_OT;
	timer.client = sw_c;
	timer.garbled_dfa = sw_garbled_dfa;
	timer.server_ot = sw_s_OT;*/
	/*s_prg.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	timer = s_prg.getElapsedTime()-rand_time;*/
	//cout<<"dfa oblivious PRG"<<endl;
	cout << "Original result is: "<< result_orig << endl;
	cout<<"result is: "<< result<<endl;
//	PrintBytes(result,message_size/8);
	//cout<< "Overall Time (ms): " << (s_prg.getElapsedTime())*1000<<endl;
	//cout<< "Random  Time (ms): " << rand_time*1000<<endl;
	//cout<< "Net     Time (ms): " << timer*1000<<endl;
	return timer;
}


double test_norm(int input_size,int dfa_size)
{

	int message_size = 12*8;
	//CreateRandomPool(input_size,dfa_size);
	int* inputs = generate_inputs(input_size);
	int** dfa = generate_dfa(dfa_size);
	byte** messages = generate_messages(message_size,dfa_size);
	byte* result;

	//for (int i=0;i<dfa_size;i++)
	//{
	//	cout<<i<<" = "<<dfa[0][i]<<"  "<<dfa[1][i]<<endl;	
	//}
	//cout<<"messages"<<endl;
	//for (int i=0;i<dfa_size;i++)
	//{
	//	cout<<i<<" = "<<messages[i]<<endl;	
	//}
	////inputs[0]=1;
	//cout<<"inputs"<<endl;
	//for (int i=0;i<input_size;i++)
	//{
	//	cout<<i<<" = "<<inputs[i]<<endl;	
	//}
	double timer=0;
	double rand_time;
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;

	CStopWatch s_norm;
	s_norm.startTimer();
	result = Eval_DFA(input_size,inputs,dfa_size,dfa,message_size,messages);
	s_norm.stopTimer();
	//cout<<"dfa normal"<<endl<<result<<endl<< "Time (ms): " << (s_norm.getElapsedTime())*1000<<endl;
	return (s_norm.getElapsedTime());
	//cout<<"getrandom = "<<Getrandom_counter<<endl;
	//cout<<"get_random_bytes = "<<random_bytes_counter<<endl;

}

double test(int input_size,int dfa_size)
{
	
	int message_size = 12*8;
	//CreateRandomPool(input_size,dfa_size);
	int* inputs = generate_inputs(input_size);
	int** dfa = generate_dfa(dfa_size);
	byte** messages = generate_messages(message_size,dfa_size);
	byte* result;

	for (int i=0;i<dfa_size;i++)
	{
		cout<<i<<" = "<<dfa[0][i]<<"  "<<dfa[1][i]<<endl;	
	}
	cout<<"messages"<<endl;
	for (int i=0;i<dfa_size;i++)
	{
		cout<<i<<" = "<<messages[i]<<endl;	
	}
	//inputs[0]=1;
	cout<<"inputs"<<endl;
	for (int i=0;i<input_size;i++)
	{
		cout<<i<<" = "<<inputs[i]<<endl;	
	}
	double timer=0;
	double rand_time;
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;

//	CStopWatch s_xor,s_prg,s_ran,s_norm;
	//s_xor.startTimer();
	result = OAE_XOR(input_size,inputs,dfa_size,dfa,message_size,messages);
//	s_xor.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
//	timer = s_xor.getElapsedTime()-rand_time;
	cout<<"dfa oblivious XOR"<<endl<<result<<endl;
//	cout<< "Overall Time (ms): " << (s_xor.getElapsedTime())*1000<<endl;
	cout<< "Random  Time (ms): " << rand_time*1000<<endl;
	cout<< "Net     Time (ms): " << timer*1000<<endl;

	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
//	s_prg.startTimer();
	//check
	result = OAE_PRG(input_size,inputs,dfa_size,dfa,message_size,messages);
	//s_prg.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
//	timer = s_xor.getElapsedTime()-rand_time;
	cout<<"dfa oblivious PRG"<<endl;
	cout<<result<<endl;
//	cout<< "Overall Time (ms): " << (s_prg.getElapsedTime())*1000<<endl;
	cout<< "Random  Time (ms): " << rand_time*1000<<endl;
	cout<< "Net     Time (ms): " << timer*1000<<endl;

	//s_norm.startTimer();
	result = Eval_DFA(input_size,inputs,dfa_size,dfa,message_size,messages);
	//s_norm.stopTimer();
//	cout<<"dfa normal"<<endl<<result<<endl<< "Time (ms): " << (s_norm.getElapsedTime())*1000<<endl;
	//cout<<"getrandom = "<<Getrandom_counter<<endl;
	//cout<<"get_random_bytes = "<<random_bytes_counter<<endl;
	return 0;
}

double benchmark_sub(int n_sub)
{
	// NIST curve P-192
	CryptoPP::Integer p("6277101735386680763835789423207666416083908700390324961279");
	// a: fixed to speed underlying operations
	CryptoPP::Integer a("-3");
	// b: Satifies b^2 * c === -27 (mod p)
	CryptoPP::Integer b("0x64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1");
	// Cofactor (f) and Order (r)
	CryptoPP::Integer f = 1;
	CryptoPP::Integer rr("6277101735386680763835789423176059013767194773182842284081");
	// n: n = f * r
	CryptoPP::Integer n = f * rr;
	// s: Input seed to SHA-1 algorithm
	//    Not used (included for completeness)
	CryptoPP::Integer s("0x3045ae6fc8422f64ed579528d38120eae12196d5");
	// c: Output of SHA-1 Algorithm
	//    Not used (included for completeness)
	CryptoPP::Integer c("0x3099d2bbbfcb2538542dcd5fb078b6ef5f3d6fe2c745de65");
	// x, y: Base Point G
	CryptoPP::Integer x("0x188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012");
	CryptoPP::Integer y("0x07192b95ffc8da78631011ed6b24cdd573f977a11e794811");
	CryptoPP::ECPPoint G(x,y);
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::ECP ec( p, a, b );
	CryptoPP::Integer r;
	//cout<<"r="<<r<<endl;
	CryptoPP::ECPPoint *points1= new CryptoPP::ECPPoint[n_sub];
	CryptoPP::ECPPoint *points2= new CryptoPP::ECPPoint[n_sub];
	for (int i=0;i<n_sub;i++)
	{
		r = GetRandom(2,p);
		points1[i] =  ec.Multiply(r,G);
		r = GetRandom(2,p);
		points2[i] =  ec.Multiply(r,G);
	}
//	CStopWatch sw;
	double totaltime=0;
	for (int i=0;i<n_sub;i++)
	{
//		sw.startTimer();
		points1[i] = ec.Subtract(points1[i],points2[i]);
//		sw.stopTimer();
//		totaltime += sw.getElapsedTime();
	}
	totaltime*=1000;
	return (totaltime/n_sub);
}

double benchmark_mult(int n_mult)
{
	// NIST curve P-192
	CryptoPP::Integer p("6277101735386680763835789423207666416083908700390324961279");
	// a: fixed to speed underlying operations
	CryptoPP::Integer a("-3");
	// b: Satifies b^2 * c === -27 (mod p)
	CryptoPP::Integer b("0x64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1");
	// Cofactor (f) and Order (r)
	CryptoPP::Integer f = 1;
	CryptoPP::Integer rr("6277101735386680763835789423176059013767194773182842284081");
	// n: n = f * r
	CryptoPP::Integer n = f * rr;
	// s: Input seed to SHA-1 algorithm
	//    Not used (included for completeness)
	CryptoPP::Integer s("0x3045ae6fc8422f64ed579528d38120eae12196d5");
	// c: Output of SHA-1 Algorithm
	//    Not used (included for completeness)
	CryptoPP::Integer c("0x3099d2bbbfcb2538542dcd5fb078b6ef5f3d6fe2c745de65");
	// x, y: Base Point G
	CryptoPP::Integer x("0x188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012");
	CryptoPP::Integer y("0x07192b95ffc8da78631011ed6b24cdd573f977a11e794811");
	CryptoPP::ECPPoint G(x,y);
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::ECP ec( p, a, b );
	CryptoPP::Integer r;
	//cout<<"r="<<r<<endl;
	CryptoPP::ECPPoint *points1= new CryptoPP::ECPPoint[n_mult];
	CryptoPP::Integer *rs= new CryptoPP::Integer[n_mult];
	for (int i=0;i<n_mult;i++)
	{
		r = GetRandom(2,p);
		points1[i] =  ec.Multiply(r,G);
		rs[i] = GetRandom(2,p);
	}
//	CStopWatch sw;
	double totaltime=0;
	for (int i=0;i<n_mult;i++)
	{
//		sw.startTimer();
		points1[i] = ec.Multiply(rs[i],points1[i]);
//		sw.stopTimer();
//		totaltime += sw.getElapsedTime();
	}
	totaltime*=1000;
	return (totaltime/n_mult);
}


//sali - networking experiments
s_stopwatch test_prg_client(int input_size,int dfa_size)
{
	s_stopwatch timer;
	int message_size = 12*8;
	//CreateRandomPool(input_size,dfa_size);
	int* inputs = generate_inputs(input_size);
	byte* result;

	////inputs[0]=1;
	cout<<"inputs"<<endl;
/*	for (int i=0;i<input_size;i++)
	{
		cout<<i<<" = "<<inputs[i]<<endl;	
	}
*/
	double rand_time;
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;

	CStopWatch s_prg;
	s_prg.startTimer();
	//sali: check
	result = OAE_PRG_client(input_size,inputs,dfa_size,message_size);
	timer.client_ot = sw_c_OT;
	timer.client = sw_c;
	timer.garbled_dfa = sw_garbled_dfa;
	timer.server_ot = sw_s_OT;
	s_prg.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
	timer.server_ot = s_prg.getElapsedTime()-rand_time;

	cout<<"dfa oblivious PRG result is:"<<endl;
	cout<<result<<endl;

	for (int i=0;i<input_size;i++)
		mysend(&(inputs[i]),sizeof(int));


	return timer;
	//cout<< "Overall Time (ms): " << (s_prg.getElapsedTime())*1000<<endl;
	//cout<< "Random  Time (ms): " << rand_time*1000<<endl;
	//cout<< "Net     Time (ms): " << timer*1000<<endl;
}

s_stopwatch test_prg_server(int input_size,int dfa_size)
{
	s_stopwatch timer;
	int message_size = 12*8;
	//CreateRandomPool(input_size,dfa_size);
	//int* inputs = generate_inputs(input_size);
cout << "here 0" <<endl;
	int** dfa = generate_dfa(dfa_size);
cout << "here 1" <<endl;
	byte** messages = generate_messages(message_size,dfa_size);
cout << "here 2" <<endl;
	byte* result;

/*
	for (int i=0;i<dfa_size;i++)
	{
		cout<<i<<" = "<<dfa[0][i]<<"  "<<dfa[1][i]<<endl;	
	}
	cout<<"messages"<<endl;
	for (int i=0;i<dfa_size;i++)
	{
		cout<<i<<" = "<<messages[i]<<endl;	
	}
*/
	double rand_time;
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;

	CStopWatch s_prg;
	s_prg.startTimer();
	//sali: check
	OAE_PRG_server(input_size,dfa_size,dfa,message_size,messages);
	timer.client_ot = sw_c_OT;
	timer.client = sw_c;
	timer.garbled_dfa = sw_garbled_dfa;
	timer.server_ot = sw_s_OT;
	s_prg.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
	timer.server_ot = s_prg.getElapsedTime()-rand_time;

	int* inputs = new int[input_size];
	
	for (int i=0;i<input_size;i++)
		myrecv(&(inputs[i]),sizeof(int));

	byte* result2=new byte[message_size];

	result2=Eval_DFA(input_size,inputs,dfa_size,dfa,message_size,messages);
	
		
	cout << "non-oblivious evaluation result is:" <<  result2<<endl;




	return timer;
 
	//cout<<"dfa oblivious PRG"<<endl;
	//cout<<result<<endl;
	//cout<< "Overall Time (ms): " << (s_prg.getElapsedTime())*1000<<endl;
	//cout<< "Random  Time (ms): " << rand_time*1000<<endl;
	//cout<< "Net     Time (ms): " << timer*1000<<endl;
}


s_stopwatch test_prg_256_client(int input_size,int dfa_size)
{
	s_stopwatch timer;
	int message_size = 2*8;
	int message_bytes = message_size / 8;
	//CreateRandomPool(input_size,dfa_size);
//sali
//	int* inputs = generate_inputs(input_size);

	//char input[]="test/webdriver";
	char input[]="testesti/wwwboard/passwd.txt";
	input_size = sizeof(input)-1;
	int* inputs = new int[input_size];

	for (int i=0;i<input_size;i++)
	{
		inputs[i]=int(input[i]);
		printf ("asc code: %d\n",inputs[i]);
	}

	cout << input << endl;
	cout << inputs[1] << endl;
//sali
//	int** dfa = generate_dfa(dfa_size,256);


	byte* result;
	int result_int=0;
//	byte* result_orig;

//	result_orig = Eval_DFA(input_size,inputs,dfa_size,dfa,message_size,messages);
	
	//for (int i=0;i<dfa_size;i++)
	//{
	//	cout<<i<<" = "<<dfa[0][i]<<"  "<<dfa[1][i]<<endl;	
	//}
	//cout<<"messages"<<endl;
	//for (int i=0;i<dfa_size;i++)
	//{
	//	cout<<i<<" = "<<messages[i]<<endl;	
	//}
	////inputs[0]=1;
	//cout<<"inputs"<<endl;
	//for (int i=0;i<input_size;i++)
	//{
	//	cout<<i<<" = "<<inputs[i]<<endl;	
	//}
	double rand_time;
	/*sw_r1=0;
	sw_r2=0;
	sw_r3=0;*/

	/*CStopWatch s_prg;
	s_prg.startTimer();*/
//	printf ("here 2 ... \n");
	cout << input_size << " " << dfa_size << " " << message_size << endl;
	result = OAE_PRG_256_client(input_size,inputs,dfa_size,message_size);
//	printf ("here 3 ... \n");
/*	timer.client_ot = sw_c_OT;
	timer.client = sw_c;
	timer.garbled_dfa = sw_garbled_dfa;
	timer.server_ot = sw_s_OT;*/
	/*s_prg.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	timer = s_prg.getElapsedTime()-rand_time;*/
	//cout<<"dfa oblivious PRG"<<endl;
//	cout << "Original result is: "<< result_orig << endl;
//	cout<<"result is: "<< (int)result<<endl;
	memcpy(&result_int,result,message_bytes);
	printf("result is: %d\n",result_int);	
//	PrintBytes(result,message_size/8);
	//cout<< "Overall Time (ms): " << (s_prg.getElapsedTime())*1000<<endl;
	//cout<< "Random  Time (ms): " << rand_time*1000<<endl;
	//cout<< "Net     Time (ms): " << timer*1000<<endl;
	return timer;
}


s_stopwatch test_prg_256_server(int input_size,int dfa_size)
{
	s_stopwatch timer;


    int states_num,accepting_num,s_start;
    int** accepting_states_ids;
    int** transitions;
    int** dfa;
    
    read_dfa_file("dfa/sig10.dfa",&s_start, &states_num, &accepting_num, &accepting_states_ids, &dfa,256);
	int message_size = 2*8;
	
    printf("# states: %d\n",states_num);
    printf("# accepting states: %d\n",accepting_num);    
    printf("start is: %d\n",s_start);
    printf("transition test.. Next State for:\nState 3,Alphabet 92: %d\nState 3,Alphabet 67:%d\nState 0,Alphabet 2:%d \n", dfa[92][3],dfa[67][3], dfa[2][0]);

	dfa_size = states_num;

	//sali
	/*cout << "the dfa is:\n" ;
	for (int i=0;i<dfa_size;i++)
	{
		for (int j=0;j<256;j++)
			cout << dfa[j][i] << " " ;
		cout << endl;
	}*/
	//end - sali

	//sali
//	byte** messages = generate_messages(message_size,dfa_size);
	byte** messages = set_messages(message_size,dfa_size,accepting_states_ids,accepting_num);

	int tmp=0;

	for (int i=0;i<dfa_size;i++)
	{
		memcpy(&tmp,messages[i],2);
		//cout << messages[i] << endl;	
		cout << "messages[" << i << "]= " << tmp <<endl;
	}
			


//	byte* result;
//	byte* result_orig;

//	result_orig = Eval_DFA(input_size,inputs,dfa_size,dfa,message_size,messages);
	
	//for (int i=0;i<dfa_size;i++)
	//{
	//	cout<<i<<" = "<<dfa[0][i]<<"  "<<dfa[1][i]<<endl;	
	//}
	//cout<<"messages"<<endl;
	//for (int i=0;i<dfa_size;i++)
	//{
	//	cout<<i<<" = "<<messages[i]<<endl;	
	//}
	////inputs[0]=1;
	//cout<<"inputs"<<endl;
	//for (int i=0;i<input_size;i++)
	//{
	//	cout<<i<<" = "<<inputs[i]<<endl;	
	//}
	double rand_time;
	/*sw_r1=0;
	sw_r2=0;
	sw_r3=0;*/

	/*CStopWatch s_prg;
	s_prg.startTimer();*/
//	printf ("here 2 ... \n");
	cout << input_size << " " << dfa_size << " " << message_size << endl;
	OAE_PRG_256_server(input_size,dfa_size,dfa,message_size,messages);
	printf ("here 3 ... \n");
/*	timer.client_ot = sw_c_OT;
	timer.client = sw_c;
	timer.garbled_dfa = sw_garbled_dfa;
	timer.server_ot = sw_s_OT;*/
	/*s_prg.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	timer = s_prg.getElapsedTime()-rand_time;*/
	//cout<<"dfa oblivious PRG"<<endl;
//	cout << "Original result is: "<< result_orig << endl;
//	cout<<"result is: "<< result<<endl;
//	PrintBytes(result,message_size/8);
	//cout<< "Overall Time (ms): " << (s_prg.getElapsedTime())*1000<<endl;
	//cout<< "Random  Time (ms): " << rand_time*1000<<endl;
	//cout<< "Net     Time (ms): " << timer*1000<<endl;
	return timer;
}




s_stopwatch test_prg_256_client_file(char* input_file)
{
	s_stopwatch timer;
	int message_size = 1*8;
	int message_bytes = message_size / 8;
	//CreateRandomPool(input_size,dfa_size);
//sali
//	int* inputs = generate_inputs(input_size);

	//char input[]="test/webdriver";
	FILE *fp;
	char buffer[2048];
	char c;
	int dfa_size,input_size;
	
	if((fp = fopen(input_file, "r"))==NULL) {
		printf("Cannot open input file.\n");
	exit(1);
	}
	
	int size=0;
	while (!feof(fp))
	{
		c=(char)fgetc(fp);
		buffer[size]=c;
		size++;
	}
	buffer[size-2]='\0';

//	cout << "the size of read is: " << size << endl;
	
	fclose(fp);
	
  	input_size = size-2;//sizeof(input)-1;
	int* inputs = new int[input_size];
	
	for(int i=0; i<input_size;i++)
		inputs[i]= int(buffer[i]);


	//reading input_size
	mysend(&input_size,sizeof(int));
	
	//sending dfa_size
	myrecv(&dfa_size,sizeof(int));


	byte* result;
	int result_int=0;
	double rand_time;
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;

	CStopWatch s_prg;
	s_prg.startTimer();

//	cout << input_size << " " << dfa_size << " " << message_size << endl;
	result = OAE_PRG_256_client(input_size,inputs,dfa_size,message_size);
//	printf ("here 3 ... \n");
	timer.client_ot = sw_c_OT;
	timer.client = sw_c;
	timer.garbled_dfa = sw_garbled_dfa;
	timer.server_ot = sw_s_OT;
	s_prg.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
//	timer = s_prg.getElapsedTime()-rand_time;
	//cout<<"dfa oblivious PRG"<<endl;
//	cout << "Original result is: "<< result_orig << endl;
//	cout<<"result is: "<< result<<endl;
//	PrintBytes("reslut is: ",result,2);
//	memcpy(&result_int,result,message_bytes);
	printf("result is: %d\n",result_int);		
//	PrintBytes(result,message_size/8);
	//cout<< "Overall Time (ms): " << (s_prg.getElapsedTime())*1000<<endl;
	//cout<< "Random  Time (ms): " << rand_time*1000<<endl;
	//cout<< "Net     Time (ms): " << timer*1000<<endl;
	return timer;
}


s_stopwatch test_prg_256_server_file(char* dfa_file)
{
	s_stopwatch timer;
//	int message_size = 12*8;
	int message_size = 1*8;	
	int dfa_size,input_size;

    int states_num,accepting_num,s_start;
    int** accepting_states_ids;
    int** transitions;
    int** dfa;
    
    read_dfa_file(dfa_file,&s_start, &states_num, &accepting_num, &accepting_states_ids, &dfa,256);

    printf("# states: %d\n",states_num);
    printf("# accepting states: %d\n",accepting_num);    
    printf("start is: %d\n",s_start);
    printf("transition test.. Next State for:\nState 3,Alphabet 92: %d\nState 3,Alphabet 67:%d\nState 0,Alphabet 2:%d \n", dfa[92][3],dfa[67][3], dfa[2][0]);

	dfa_size = states_num;
	
	//reading input_size
	myrecv(&input_size,sizeof(int));
	
	//sending dfa_size
	mysend(&dfa_size,sizeof(int));

	byte** messages = set_messages(message_size,dfa_size,accepting_states_ids,accepting_num);
	
	double rand_time;
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;

	CStopWatch s_prg;
	s_prg.startTimer();

	cout << input_size << " " << dfa_size << " " << message_size << endl;
	OAE_PRG_256_server(input_size,dfa_size,dfa,message_size,messages);
	return timer;
}
