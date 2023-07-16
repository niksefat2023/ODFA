#include "OAE.h"


byte* expand_bytes(byte* source,int source_size,int expanded_size)
{
	byte* expanded = new byte[expanded_size];
	
	if (expanded_size>source_size)
	{
		memcpy(expanded,source,source_size*sizeof(byte));
		memset(expanded[source_size],0,(expanded_size-source_size)*sizeof(byte));
		
	}
	else
	{
		// We should shrink it then !!!
		memcpy(expanded,source,expanded_size*sizeof(byte));
	}
	
	return expanded;
}

byte* combine_bytes(byte* one,int one_size,byte* two,int two_size)
{
	byte* combination = new byte[one_size+two_size];
	for (int i=0;i<one_size;i++)
	{
		combination[i] = one[i];
	}
	for (int i=0;i<two_size;i++)
	{
		combination[i+one_size] = two[i];
	}
	return combination;
}

void divide_bytes(byte* combination,byte* one,int one_size,byte* two,int two_size)
{
	//one = new byte[one_size];
	//two = new byte[two_size];
	for (int i=0;i<one_size;i++)
	{
		one[i] = combination[i];
	}
	for (int i=0;i<two_size;i++)
	{
		two[i] = combination[i+one_size];
	}
}

void random_permute(int a[],int size)
{
	int temp_index,temp_value;
	for (int i=0;i<size;i++)
	{
		temp_index = GetRandom(0,size-1);
		temp_value = a[temp_index];
		a[temp_index] = a[i];
		a[i] = temp_value;
	}
}

byte* OAE_XOR(int input_size, // The number of clients inputs
	int* inputs,			// The client inputs
	int dfa_size,			// The number of DFA states
	int* dfa[2],			// The DFA with 2 inputs {0,1}
	int message_size,		// The size of message in the last state (in Bits)
	byte** messages			// The message in each state Byte*
	)
{
//	CStopWatch sw;
//	sw.startTimer();
	// Finding the minimum number of bytes for each state
	int state_size; // Maximum size of states
	CryptoPP::Integer q = dfa_size;
	int logq =  q.BitCount();
	int logq_bytes = (int)ceil((double)logq/8);
	int message_bytes = (int)ceil((double)message_size/8);
	state_size = message_bytes + (input_size-1)*logq_bytes;
	int* state_sizes =  new int[input_size];
	// Build PAD matrix
	// Build Keys K[2][input_size]
	byte** keys[2];
	keys[0] = new byte*[input_size];
	keys[1] = new byte*[input_size];
	byte*** pad = new byte**[input_size];
	for (int i=0;i<input_size;i++)
	{
		pad[i] = new byte*[dfa_size];
		state_sizes[i] = state_size-i*logq_bytes;
		keys[0][i] = GetRandomBytes(state_sizes[i]);
		keys[1][i] = GetRandomBytes(state_sizes[i]);
		for (int j=0;j<dfa_size;j++)
		{
			pad[i][j] = GetRandomBytes(state_sizes[i]);
		}
	}

	// Build PER matrix
	int** per = new int*[input_size+1];
	int** rper = new int*[input_size+1];
	for (int i=0;i<(input_size+1);i++)
	{
		per[i] = new int[dfa_size];
		rper[i] = new int[dfa_size];
		for (int j=0;j<dfa_size;j++)
		{
			per[i][j] = j;
		}
		random_permute(per[i],dfa_size);
		for (int j=0;j<dfa_size;j++)
		{
			rper[i][per[i][j]]=j;
		}
	}



	// Build DFA Matrix M
	//byte*** M[2];
	byte*** PM[2];
	byte*** GPM[2]; 
	int length;
	int xx;
	//M[0] = new byte**[input_size];
	PM[0] = new byte**[input_size];
	GPM[0] = new byte**[input_size];
	//M[1] = new byte**[input_size];
	PM[1] = new byte**[input_size];
	GPM[1] = new byte**[input_size];
	for (int i=0;i<input_size;i++)
	{
		//M[0][i] = new byte*[dfa_size];
		PM[0][i] = new byte*[dfa_size];
		GPM[0][i] = new byte*[dfa_size];
		//M[1][i] = new byte*[dfa_size];
		PM[1][i] = new byte*[dfa_size];
		GPM[1][i] = new byte*[dfa_size];
		for (int j=0;j<dfa_size;j++)
		{
			if (i==0)
			{
				PM[0][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[0][0]]);
				//xx = convert_byte_to_int(logq_bytes,PM[0][i][j]);
				GPM[0][i][j] = combine_bytes(pad[i+1][per[i+1][dfa[0][0]]],state_sizes[i+1],PM[0][i][j],logq_bytes);
				GPM[0][i][j] = XOREncrypt(GPM[0][i][j],keys[0][i],state_sizes[i]);
				//if (j==0) PrintBytes("Key[0]",keys[0][i],state_sizes[i]);

				PM[1][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[1][0]]);
				GPM[1][i][j] = combine_bytes(pad[i+1][per[i+1][dfa[1][0]]],state_sizes[i+1],PM[1][i][j],logq_bytes);
				GPM[1][i][j] = XOREncrypt(GPM[1][i][j],keys[1][i],state_sizes[i]);
				//if (j==0) PrintBytes("Key[1]",keys[0][i],state_sizes[i]);
			}
			else
			{
				if (i<(input_size-1))
				{
					PM[0][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[0][rper[i][j]]]);
					GPM[0][i][j] = combine_bytes(pad[i+1][per[i+1][dfa[0][rper[i][j]]]],state_sizes[i+1],PM[0][i][j],logq_bytes);
					GPM[0][i][j] = XOREncrypt(XOREncrypt(GPM[0][i][j],pad[i][j],state_sizes[i]),keys[0][i],state_sizes[i]);

					PM[1][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[1][rper[i][j]]]);
					GPM[1][i][j] = combine_bytes(pad[i+1][per[i+1][dfa[1][rper[i][j]]]],state_sizes[i+1],PM[1][i][j],logq_bytes);
					GPM[1][i][j] = XOREncrypt(XOREncrypt(GPM[1][i][j],pad[i][j],state_sizes[i]),keys[1][i],state_sizes[i]);
				}
				else
				{
					GPM[0][i][j] = messages[dfa[0][rper[i][j]]];
					GPM[0][i][j] = XOREncrypt(XOREncrypt(GPM[0][i][j],pad[i][j],state_sizes[i]),keys[0][i],state_sizes[i]);
					GPM[1][i][j] = messages[dfa[1][rper[i][j]]];
					GPM[1][i][j] = XOREncrypt(XOREncrypt(GPM[1][i][j],pad[i][j],state_sizes[i]),keys[1][i],state_sizes[i]);
					//cout<<"last state for "<<j<<" is "<<dfa[inputs[i]][rper[i][j]]<<endl;
					//cout<<"M = "<<messages[dfa[inputs[i]][rper[i][j]]]<<endl;
				}
			}
		}
		delete [] PM[0][i];
		delete [] PM[1][i];
	}
	delete [] PM[0];
	delete [] PM[1];
//	sw.stopTimer();
//	sw_garbled_dfa = sw.getElapsedTime();
	// OT to transfer keys
	byte** c_keys = new byte*[input_size];
	sw_c_OT = 0 ;
	sw_s_OT = 0 ;
	//sali chack
	OT_dynamic_message_size(input_size,state_sizes,keys,c_keys,inputs);
	//for (int i=0;i<10;i++)
	//{
	//	cout<<"i="<<i<<endl;
	//	PrintBytes("Should KEY",keys[inputs[i]][i],state_sizes[i]);
	//	PrintBytes("c_Key",c_keys[i],state_sizes[i]);
	//	cout<<endl<<endl;
	//}
//	sw.startTimer();
	byte* result;			// The result of DFA evaluation
	int cur_state_index=0;
	byte* cur_state;
	int NS = 0;
	byte* NS_bytes = new byte[logq_bytes];
	byte* pad_bytes=0;
	byte* temp=0;
	for (int i=0;i<input_size;i++)
	{
		if (i==0)
		{
			cur_state = XOREncrypt(GPM[inputs[i]][i][NS],c_keys[i],state_sizes[i]);

			pad_bytes = new byte[state_sizes[i+1]];
			divide_bytes(cur_state,pad_bytes,state_sizes[i+1],NS_bytes,logq_bytes);
			//PrintBytes("Current State",cur_state,state_sizes[i+1]+logq_bytes);
			//PrintBytes("pad",pad_bytes,state_sizes[i+1]);
			//PrintBytes("NS",NS_bytes,logq_bytes);
			NS = convert_byte_to_int(logq_bytes,NS_bytes);
			//cout<<"rper[ns] = "<<rper[i+1][NS]<<endl;
		}
		else
		{
			if (i<(input_size-1))
			{
				temp = XOREncrypt(GPM[inputs[i]][i][NS],c_keys[i],state_sizes[i]);
				cur_state = XOREncrypt(temp,pad_bytes,state_sizes[i]);
				divide_bytes(cur_state,pad_bytes,state_sizes[i+1],NS_bytes,logq_bytes);
				NS = convert_byte_to_int(logq_bytes,NS_bytes);
				/*if (NS>dfa_size)
				{
				cout<<"WTF, NS in the baghalies"<<endl;
				}*/
				//PrintBytes("Current State",cur_state,state_sizes[i+1]+logq_bytes);
				//PrintBytes("pad",pad_bytes,state_sizes[i+1]);
				//PrintBytes("NS",NS_bytes,logq_bytes);
				//PrintBytes("per[NS]",,logq_bytes);
				//cout<<"rper[ns] = "<<rper[i+1][NS]<<endl;
			}
			else
			{

				temp = XOREncrypt(GPM[inputs[i]][i][NS],c_keys[i],state_sizes[i]);
				cur_state = XOREncrypt(temp,pad_bytes,state_sizes[i]);
				result = cur_state;
			}
		}
	}
//	sw.stopTimer();
//	sw_c = sw.getElapsedTime();
	delete [] state_sizes;
	for (int i=0;i<input_size;i++)
	{

		delete [] keys[0][i];
		delete [] keys[1][i];
		for (int j=0;j<dfa_size;j++)
		{
			delete [] pad[i][j];
			delete [] GPM[0][i][j];
			delete [] GPM[1][i][j];
		}
		delete [] pad[i];
		delete [] GPM[0][i];
		delete [] GPM[1][i];
	}
	delete [] GPM[0];
	delete [] GPM[1];
	for (int i=0;i<(input_size+1);i++)
	{
		delete [] per[i];
		delete [] rper[i];
	}

	return result;
}




byte* OAE_PRG(int input_size, // The number of clients inputs
	int* inputs,			// The client inputs
	int dfa_size,			// The number of DFA states
	int* dfa[2],			// The DFA with 2 inputs {0,1}
	int message_size,		// The size of message in the last state (in Bits)
	byte** messages			// The message in each state Byte*
	)
{
//	CStopWatch sw;
//	sw.startTimer();
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	// Finding the minimum number of bytes for each state
	int state_size; // Maximum size of states
	CryptoPP::Integer q = dfa_size;
	int logq =  q.BitCount();
	int logq_bytes = (int)ceil((double)logq/8);
	int message_bytes = (int)ceil((double)message_size/8);
	state_size = message_bytes + logq_bytes;
	int* state_sizes =  new int[input_size];
	// Build PAD matrix
	// Build Keys K[2][input_size]
	byte** keys[2] ;
	keys[0] = new byte*[input_size];
	keys[1] = new byte*[input_size];
	byte*** pad = new byte**[input_size];
	for (int i=0;i<input_size;i++)
	{
		pad[i] = new byte*[dfa_size];
		state_sizes[i] = state_size;
		keys[0][i] = GetRandomBytes(state_size);
		keys[1][i] = GetRandomBytes(state_size);
		for (int j=0;j<dfa_size;j++)
		{
			pad[i][j] = GetRandomBytes(message_bytes);
		}
	}

	// Build PER matrix
	int** per = new int*[input_size+1];
	int** rper = new int*[input_size+1];
	for (int i=0;i<(input_size+1);i++)
	{
		per[i] = new int[dfa_size];
		rper[i] = new int[dfa_size];
		for (int j=0;j<dfa_size;j++)
		{
			per[i][j] = j;
		}
		random_permute(per[i],dfa_size);
		for (int j=0;j<dfa_size;j++)
		{
			rper[i][per[i][j]]=j;
		}
	}



	// Build DFA Matrix M
	//byte*** M[2];
	byte*** PM[2];
	byte*** GPM[2]; 
	int length;
	int xx;
	//M[0] = new byte**[input_size];
	PM[0] = new byte**[input_size];
	GPM[0] = new byte**[input_size];
	//M[1] = new byte**[input_size];
	PM[1] = new byte**[input_size];
	GPM[1] = new byte**[input_size];
	for (int i=0;i<input_size;i++)
	{
		//M[0][i] = new byte*[dfa_size];
		PM[0][i] = new byte*[dfa_size];
		GPM[0][i] = new byte*[dfa_size];
		//M[1][i] = new byte*[dfa_size];
		PM[1][i] = new byte*[dfa_size];
		GPM[1][i] = new byte*[dfa_size];
		for (int j=0;j<dfa_size;j++)
		{
			if (i==0)
			{
				PM[0][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[0][0]]);
				//xx = convert_byte_to_int(logq_bytes,PM[0][i][j]);
				GPM[0][i][j] = combine_bytes(pad[i+1][per[i+1][dfa[0][0]]],message_bytes,PM[0][i][j],logq_bytes);
				GPM[0][i][j] = XOREncrypt(GPM[0][i][j],keys[0][i],state_size);
				//if (j==0) PrintBytes("Key[0]",keys[0][i],state_sizes[i]);

				PM[1][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[1][0]]);
				GPM[1][i][j] = combine_bytes(pad[i+1][per[i+1][dfa[1][0]]],message_bytes,PM[1][i][j],logq_bytes);
				GPM[1][i][j] = XOREncrypt(GPM[1][i][j],keys[1][i],state_size);
				//if (j==0) PrintBytes("Key[1]",keys[0][i],state_sizes[i]);
			}
			else
			{
				if (i<(input_size-1))
				{
					PM[0][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[0][rper[i][j]]]);
					GPM[0][i][j] = combine_bytes(pad[i+1][per[i+1][dfa[0][rper[i][j]]]],message_bytes,PM[0][i][j],logq_bytes);
					GPM[0][i][j] = XOREncrypt(XOREncrypt(GPM[0][i][j],PRG_n_to_m_bytes_hash(pad[i][j],message_bytes,state_size),state_size),keys[0][i],state_size);

					PM[1][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[1][rper[i][j]]]);
					GPM[1][i][j] = combine_bytes(pad[i+1][per[i+1][dfa[1][rper[i][j]]]],message_bytes,PM[1][i][j],logq_bytes);
					GPM[1][i][j] = XOREncrypt(XOREncrypt(GPM[1][i][j],PRG_n_to_m_bytes_hash(pad[i][j],message_bytes,state_size),state_size),keys[1][i],state_size);
				}
				else
				{
					PM[0][i][j] = convert_int_to_kbyte(logq_bytes,1);
					GPM[0][i][j] = combine_bytes(messages[dfa[0][rper[i][j]]],message_bytes,PM[0][i][j],logq_bytes);
					//GPM[0][i][j] = messages[dfa[0][rper[i][j]]];
					GPM[0][i][j] = XOREncrypt(XOREncrypt(GPM[0][i][j],PRG_n_to_m_bytes_hash(pad[i][j],message_bytes,state_size),state_size),keys[0][i],state_size);
					PM[1][i][j] = convert_int_to_kbyte(logq_bytes,1);
					//GPM[1][i][j] = messages[dfa[1][rper[i][j]]];
					GPM[1][i][j] = combine_bytes(messages[dfa[1][rper[i][j]]],message_bytes,PM[1][i][j],logq_bytes);
					GPM[1][i][j] = XOREncrypt(XOREncrypt(GPM[1][i][j],PRG_n_to_m_bytes_hash(pad[i][j],message_bytes,state_size),state_size),keys[1][i],state_size);
					//cout<<"last state for "<<j<<" is "<<dfa[inputs[i]][rper[i][j]]<<endl;
					//cout<<"M = "<<messages[dfa[inputs[i]][rper[i][j]]]<<endl;
				}
			}
		}
		delete [] PM[0][i];
		delete [] PM[1][i];
	}
	delete [] PM[0];
	delete [] PM[1];

//	sw.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	sw_garbled_dfa = sw.getElapsedTime()-rand_time;
	// OT to transfer keys
	byte** c_keys = new byte*[input_size];
	OT_dynamic_message_size(input_size,state_sizes,keys,c_keys,inputs);
	//for (int tester=0;tester<input_size;tester++)
	//{
	//	printf("i=%d",tester);
	//	//cout<<"i="<<tester<<endl;
	//	PrintBytes("Should KEY",keys[inputs[tester]][tester],state_sizes[tester]);
	//	PrintBytes("c_Key",c_keys[tester],state_sizes[tester]);
	//	cout<<endl<<endl;
	//}
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
//	sw.startTimer();
	byte* result;			// The result of DFA evaluation
	int cur_state_index=0;
	byte* cur_state;
	int NS = 0;
	byte* NS_bytes = new byte[logq_bytes];
	byte* pad_bytes=0;
	byte* temp=0;
	for (int i=0;i<input_size;i++)
	{
		if (i==0)
		{
			cur_state = XOREncrypt(GPM[inputs[i]][i][NS],c_keys[i],state_size);

			pad_bytes = new byte[message_bytes];
			divide_bytes(cur_state,pad_bytes,message_bytes,NS_bytes,logq_bytes);
			/*PrintBytes("Current State",cur_state,state_size);
			PrintBytes("pad",pad_bytes,message_bytes);
			PrintBytes("NS",NS_bytes,logq_bytes);*/
			NS = convert_byte_to_int(logq_bytes,NS_bytes);
			//cout<<"rper[ns] = "<<rper[i+1][NS]<<endl;
		}
		else
		{
			if (i<(input_size-1))
			{
				temp = XOREncrypt(GPM[inputs[i]][i][NS],c_keys[i],state_size);
				cur_state = XOREncrypt(temp,PRG_n_to_m_bytes_hash(pad_bytes,message_bytes,state_size),state_size);
				divide_bytes(cur_state,pad_bytes,message_bytes,NS_bytes,logq_bytes);
				NS = convert_byte_to_int(logq_bytes,NS_bytes);
				/*if (NS>dfa_size)
				{
				cout<<"WTF, NS in the baghalies"<<endl;
				}*/
				//PrintBytes("PRG1",PRG_32_to_m_bytes_hash(pad_bytes,state_size),state_size);

				//PrintBytes("Current State",cur_state,state_size);
				//PrintBytes("pad",pad_bytes,message_bytes);
				//PrintBytes("NS",NS_bytes,logq_bytes);
				//PrintBytes("per[NS]",,logq_bytes);
				//cout<<"rper[ns] = "<<rper[i+1][NS]<<endl;
			}
			else
			{

				temp = XOREncrypt(GPM[inputs[i]][i][NS],c_keys[i],state_size);
				cur_state = XOREncrypt(temp,PRG_n_to_m_bytes_hash(pad_bytes,message_bytes,state_size),state_size);
				divide_bytes(cur_state,pad_bytes,message_bytes,NS_bytes,logq_bytes);
				result = pad_bytes;
			}
		}
	}
//	sw.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	sw_c = sw.getElapsedTime()-rand_time;

	delete [] state_sizes;
	for (int i=0;i<input_size;i++)
	{

		delete [] keys[0][i];
		delete [] keys[1][i];
		for (int j=0;j<dfa_size;j++)
		{
			delete [] pad[i][j];
			delete [] GPM[0][i][j];
			delete [] GPM[1][i][j];
		}
		delete [] pad[i];
		delete [] GPM[0][i];
		delete [] GPM[1][i];
	}
	delete [] GPM[0];
	delete [] GPM[1];
	for (int i=0;i<(input_size+1);i++)
	{
		delete [] per[i];
		delete [] rper[i];
	}
	return result;
}



byte* OAE_PRG_256(int input_size, // The number of clients inputs
	int* inputs,			// The client inputs (converted from asci to int already)
	int dfa_size,			// The number of DFA states
	int* dfa[256],			// The DFA with 256 inputs
	int message_size,		// The size of message in the last state (in Bits)
	byte** messages			// The message in each state Byte*
	)
{
//	CStopWatch sw;
//	sw.startTimer();
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	// Finding the minimum number of bytes for each state
	int state_size; // Maximum size of states
	CryptoPP::Integer q = dfa_size;
	int logq =  q.BitCount();
	int logq_bytes = (int)ceil((double)logq/8);
	//cout << "logq_bytes: " << logq_bytes << endl;
	int message_bytes = (int)ceil((double)message_size/8);
	state_size = message_bytes + logq_bytes;
	int* state_sizes =  new int[input_size];
	// Build PAD matrix
	// Build Keys K[256][input_size] (256 keys for each row)
	byte** keys[256] ;

	byte*** pad = new byte**[input_size];
	for (int i=0;i<input_size;i++)
	{
		pad[i] = new byte*[dfa_size];
		state_sizes[i] = state_size;

		for (int j=0;j<dfa_size;j++)
		{
			//sali
			pad[i][j] = GetRandomBytes(message_bytes);
			//pad[i][j] = convert_int_to_kbyte(message_bytes,0); 
			//cout << " x ";
	//		PrintBytes(pad[i][j],message_bytes);
			
		}
	}
	for (int j=0;j<256;j++)
	{
		keys[j] = new byte*[input_size];
		for (int i=0;i<input_size;i++)
		{
			//sali
			keys[j][i] = GetRandomBytes(state_size);
			//keys[j][i] = convert_int_to_kbyte(state_size,0);
		}
	}

	// Build PER matrix
	int** per = new int*[input_size+1];
	int** rper = new int*[input_size+1];
	for (int i=0;i<(input_size+1);i++)
	{
		per[i] = new int[dfa_size];
		rper[i] = new int[dfa_size];
		for (int j=0;j<dfa_size;j++)
		{
			per[i][j] = j;
		}
		random_permute(per[i],dfa_size);
		for (int j=0;j<dfa_size;j++)
		{
			rper[i][per[i][j]]=j;
		}
	}

	//cout<< "xxx1\n";

	// Build DFA Matrix M
	//byte*** M[2];
	byte*** PM[256];
	byte*** GPM[256]; 
	int length;
	int xx;
	for (int z=0;z<256;z++)
	{
		PM[z] = new byte**[input_size];
		GPM[z] = new byte**[input_size];
	}
	for (int i=0;i<input_size;i++)
	{
		for (int z=0;z<256;z++)
		{
			PM[z][i] = new byte*[dfa_size];
			GPM[z][i] = new byte*[dfa_size];
		}
		for (int j=0;j<dfa_size;j++)
		{
			if (i==0)
			{
				for (int z=0;z<256;z++)
				{
					PM[z][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[z][0]]);
					GPM[z][i][j] = combine_bytes(pad[i+1][per[i+1][dfa[z][0]]],message_bytes,PM[z][i][j],logq_bytes);
					GPM[z][i][j] = XOREncrypt(GPM[z][i][j],keys[z][i],state_size);
				}
				//if (j==0) PrintBytes("Key[1]",keys[0][i],state_sizes[i]);
			}
			else
			{
				if (i<(input_size-1))
				{
					for (int z=0;z<256;z++)
					{
						PM[z][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[z][rper[i][j]]]);
						GPM[z][i][j] = combine_bytes(pad[i+1][per[i+1][dfa[z][rper[i][j]]]],message_bytes,PM[z][i][j],logq_bytes);
//sali						
GPM[z][i][j] = XOREncrypt(XOREncrypt(GPM[z][i][j],PRG_n_to_m_bytes_hash(pad[i][j],message_bytes,state_size),state_size),keys[z][i],state_size);
//						GPM[z][i][j] = XOREncrypt(XOREncrypt(GPM[z][i][j],convert_int_to_kbyte(state_size,0),state_size),keys[z][i],state_size);
					}
				}
				else
				{
					for (int z=0;z<256;z++)
					{
						PM[z][i][j] = convert_int_to_kbyte(logq_bytes,1);
						GPM[z][i][j] = combine_bytes(messages[dfa[z][rper[i][j]]],message_bytes,PM[z][i][j],logq_bytes);
//salt						
GPM[z][i][j] = XOREncrypt(XOREncrypt(GPM[z][i][j],PRG_n_to_m_bytes_hash(pad[i][j],message_bytes,state_size),state_size),keys[z][i],state_size);
//						GPM[z][i][j] = XOREncrypt(XOREncrypt(GPM[z][i][j],convert_int_to_kbyte(state_size,0),state_size),keys[z][i],state_size);
					}
				}
			}
		}
		for (int z=0;z<256;z++)
		{
			delete [] PM[z][i];
		}
	}
	//cout << "xxx2\n";
	for (int z=0;z<256;z++)
	{
		delete [] PM[z];
	}

//sali - check GPM
/*
        for (int i=1;i<=1;i++)
                	for (int j=0;j<dfa_size;j++)
                for (int z=0;z<256;z++)
				PrintBytes("GPM",GPM[z][i][j],state_size);	
*/
//end - sali

//	sw.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	sw_garbled_dfa = sw.getElapsedTime()-rand_time;
	// OT to transfer keys
	byte** c_keys = new byte*[input_size];
	OT_dynamic_256alpha(input_size,state_sizes,keys,c_keys,inputs);
	//cout << "xxx3\n";
/*
	for (int tester=0;tester<input_size;tester++)
	{
	//	printf("i=%d",tester);
		//cout<<"i="<<tester<<endl;
	//	PrintBytes("Should KEY",keys[inputs[tester]][tester],state_sizes[tester]);
		//PrintBytes("Should KEY0",keys[0][tester],state_sizes[tester]);
		//PrintBytes("Should KEY1",keys[1][tester],state_sizes[tester]);
	//	PrintBytes("c_Key",c_keys[tester],state_sizes[tester]);
	//	cout<<endl<<endl;
	}
*/
	//cout << "xxx4\n";
 	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
//	sw.startTimer();
	byte* result;			// The result of DFA evaluation
	int cur_state_index=0;
	byte* cur_state;
	cur_state=new byte[state_size];
	int NS = 0;
	byte* NS_bytes = new byte[logq_bytes];
	byte* pad_bytes=0;
	byte* temp=0;
	for (int i=0;i<input_size;i++)
	{
		if (i==0)
		{
			//XOREncrypt2(cur_state,GPM[inputs[i]][i][NS],c_keys[i],state_size);
			cur_state=XOREncrypt(GPM[inputs[i]][i][NS],c_keys[i],state_size);

			pad_bytes = new byte[message_bytes];
//sali				
//PrintBytes("XOREncrypt two items: ", XOREncrypt(pad_bytes,pad[i][NS],message_bytes) ,message_bytes);
			divide_bytes(cur_state,pad_bytes,message_bytes,NS_bytes,logq_bytes);
			//PrintBytes("Current State",cur_state,state_size);
			//PrintBytes("next pad row 0",pad_bytes,message_bytes);
			//PrintBytes("NS",NS_bytes,logq_bytes);
			NS = convert_byte_to_int(logq_bytes,NS_bytes);
			//cout<<"rper[ns] = "<<rper[i+1][NS]<<endl;
		}
		else
		{
			if (i<(input_size-1))
			{
				temp = XOREncrypt(GPM[inputs[i]][i][NS],c_keys[i],state_size);
				//XOREncrypt2(cur_state,temp,PRG_n_to_m_bytes_hash(pad_bytes,message_bytes,state_size),state_size);
				//sali
				cur_state=XOREncrypt(temp,PRG_n_to_m_bytes_hash(pad_bytes,message_bytes,state_size),state_size);
				//cur_state=XOREncrypt(temp,convert_int_to_kbyte(state_size,0),state_size);

				//PrintBytes("XOREncrypt two items: ", XOREncrypt(pad_bytes,pad[i][NS],message_bytes) ,message_bytes);
			
				//PrintBytes("cur_state: ",cur_state,state_size);
				divide_bytes(cur_state,pad_bytes,message_bytes,NS_bytes,logq_bytes);
				//PrintBytes("NS_bytes: ",NS_bytes,1);
				NS = convert_byte_to_int(logq_bytes,NS_bytes);
				cout << "NS is:" << NS << endl;
				if (NS>dfa_size)
				{
				cout<<"WTF, NS in the baghalies"<<endl;
				}
				//PrintBytes("PRG1",PRG_32_to_m_bytes_hash(pad_bytes,state_size),state_size);

				//PrintBytes("Current State",cur_state,state_size);
				//PrintBytes("next pad",pad_bytes,message_bytes);
				//PrintBytes("NS",NS_bytes,logq_bytes);
				//PrintBytes("per[NS]",,logq_bytes);
				//cout<<"rper[ns] = "<<rper[i+1][NS]<<endl;
			}
			else
			{

				//temp = XOREncrypt(GPM[inputs[i]][i][NS],c_keys[i],state_size);
				temp = XOREncrypt(GPM[inputs[i]][i][NS],c_keys[i],state_size);
				//sali
				cur_state = XOREncrypt(temp,PRG_n_to_m_bytes_hash(pad_bytes,message_bytes,state_size),state_size);
				//cur_state = XOREncrypt(temp,convert_int_to_kbyte(state_size,0),state_size);
				divide_bytes(cur_state,pad_bytes,message_bytes,NS_bytes,logq_bytes);
				result = pad_bytes;
			}
		}
	}
	//cout << "xxx5\n";
//	sw.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	sw_c = sw.getElapsedTime()-rand_time;

	delete [] state_sizes;
	for (int i=0;i<input_size;i++)
	{
		for (int z=0;z<256;z++)
		{
			delete [] keys[z][i];
		}
		for (int j=0;j<dfa_size;j++)
		{
			delete [] pad[i][j];
			for (int z=0;z<256;z++)
			{
				delete [] GPM[z][i][j];
			}
		}
		delete [] pad[i];
		for (int z=0;z<256;z++)
		{
			delete [] GPM[z][i];
		}
	}
	for (int z=0;z<256;z++)
	{
		delete [] GPM[z];
	}
	for (int i=0;i<(input_size+1);i++)
	{
		delete [] per[i];
		delete [] rper[i];
	}
	//cout << "before return result...\n" ;
	return result;
}


////////////////////////////////////////////////////
//sali - Network implementation
////////////////////////////////////////////////////

int OAE_PRG_server(int input_size, // The number of clients inputs
	int dfa_size,			// The number of DFA states
	int* dfa[2],			// The DFA with 2 inputs {0,1}
	int message_size,		// The size of message in the last state (in Bits)
	byte** messages			// The message in each state Byte*
	)
{
	cout << "OAE_PRG_server() started." << endl;
	CStopWatch sw;
	sw.startTimer();
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	// Finding the minimum number of bytes for each state
	int state_size; // Maximum size of states
	CryptoPP::Integer q = dfa_size;
	int logq =  q.BitCount();
	int logq_bytes = (int)ceil((double)logq/8);
	int message_bytes = (int)ceil((double)message_size/8);
	state_size = message_bytes + logq_bytes;
	int* state_sizes =  new int[input_size];


	// Build PAD matrix
	// Build Keys K[2][input_size]
	byte** keys[2] ;
	keys[0] = new byte*[input_size];
	keys[1] = new byte*[input_size];
	byte*** pad = new byte**[input_size];


	for (int i=0;i<input_size;i++)
	{
		pad[i] = new byte*[dfa_size];
		state_sizes[i] = state_size;
		keys[0][i] = GetRandomBytes(state_size);
		keys[1][i] = GetRandomBytes(state_size);
		for (int j=0;j<dfa_size;j++)
		{
			pad[i][j] = GetRandomBytes(message_bytes);
		}
	}


	//sali	byte** c_keys = new byte*[input_size];
	OT_dynamic_message_size_server(input_size,state_sizes,keys);


	// Build PER matrix
	int** per = new int*[input_size+1];
	int** rper = new int*[input_size+1];
	for (int i=0;i<(input_size+1);i++)
	{
		per[i] = new int[dfa_size];
		rper[i] = new int[dfa_size];
		for (int j=0;j<dfa_size;j++)
		{
			per[i][j] = j;
		}
		random_permute(per[i],dfa_size);
		for (int j=0;j<dfa_size;j++)
		{
			rper[i][per[i][j]]=j;
		}
	}



	// Build DFA Matrix M
	//byte*** M[2];
	byte*** PM[2];
	byte*** GPM[2]; 
	int length;
	int xx;
	//M[0] = new byte**[input_size];
	PM[0] = new byte**[input_size];
	GPM[0] = new byte**[input_size];
	//M[1] = new byte**[input_size];
	PM[1] = new byte**[input_size];
	GPM[1] = new byte**[input_size];
	for (int i=0;i<input_size;i++)
	{
		//M[0][i] = new byte*[dfa_size];
		PM[0][i] = new byte*[dfa_size];
		GPM[0][i] = new byte*[dfa_size];
		//M[1][i] = new byte*[dfa_size];
		PM[1][i] = new byte*[dfa_size];
		GPM[1][i] = new byte*[dfa_size];
		for (int j=0;j<dfa_size;j++)
		{
			if (i==0)
			{
				PM[0][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[0][0]]);
				//xx = convert_byte_to_int(logq_bytes,PM[0][i][j]);
				GPM[0][i][j] = combine_bytes(pad[i+1][per[i+1][dfa[0][0]]],message_bytes,PM[0][i][j],logq_bytes);
				GPM[0][i][j] = XOREncrypt(GPM[0][i][j],keys[0][i],state_size);
				//if (j==0) PrintBytes("Key[0]",keys[0][i],state_sizes[i]);

				PM[1][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[1][0]]);
				GPM[1][i][j] = combine_bytes(pad[i+1][per[i+1][dfa[1][0]]],message_bytes,PM[1][i][j],logq_bytes);
				GPM[1][i][j] = XOREncrypt(GPM[1][i][j],keys[1][i],state_size);
				//if (j==0) PrintBytes("Key[1]",keys[0][i],state_sizes[i]);
			}
			else
			{
				if (i<(input_size-1))
				{
					PM[0][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[0][rper[i][j]]]);
					GPM[0][i][j] = combine_bytes(pad[i+1][per[i+1][dfa[0][rper[i][j]]]],message_bytes,PM[0][i][j],logq_bytes);
					GPM[0][i][j] = XOREncrypt(XOREncrypt(GPM[0][i][j],PRG_n_to_m_bytes_hash(pad[i][j],message_bytes,state_size),state_size),keys[0][i],state_size);

					PM[1][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[1][rper[i][j]]]);
					GPM[1][i][j] = combine_bytes(pad[i+1][per[i+1][dfa[1][rper[i][j]]]],message_bytes,PM[1][i][j],logq_bytes);
					GPM[1][i][j] = XOREncrypt(XOREncrypt(GPM[1][i][j],PRG_n_to_m_bytes_hash(pad[i][j],message_bytes,state_size),state_size),keys[1][i],state_size);
				}
				else
				{
					PM[0][i][j] = convert_int_to_kbyte(logq_bytes,1);
					GPM[0][i][j] = combine_bytes(messages[dfa[0][rper[i][j]]],message_bytes,PM[0][i][j],logq_bytes);
					//GPM[0][i][j] = messages[dfa[0][rper[i][j]]];
					GPM[0][i][j] = XOREncrypt(XOREncrypt(GPM[0][i][j],PRG_n_to_m_bytes_hash(pad[i][j],message_bytes,state_size),state_size),keys[0][i],state_size);
					PM[1][i][j] = convert_int_to_kbyte(logq_bytes,1);
					//GPM[1][i][j] = messages[dfa[1][rper[i][j]]];
					GPM[1][i][j] = combine_bytes(messages[dfa[1][rper[i][j]]],message_bytes,PM[1][i][j],logq_bytes);
					GPM[1][i][j] = XOREncrypt(XOREncrypt(GPM[1][i][j],PRG_n_to_m_bytes_hash(pad[i][j],message_bytes,state_size),state_size),keys[1][i],state_size);
					//cout<<"last state for "<<j<<" is "<<dfa[inputs[i]][rper[i][j]]<<endl;
					//cout<<"M = "<<messages[dfa[inputs[i]][rper[i][j]]]<<endl;
				}
			}
			//sali-socket
			mysend(GPM[0][i][j],state_size);
			mysend(GPM[1][i][j],state_size);
			delete [] GPM[0][i][j];
			delete [] GPM[1][i][j];
			delete [] PM[0][i][j];
			delete [] PM[1][i][j];
			delete [] pad[i][j];
		}
		delete [] GPM[0][i];
		delete [] GPM[1][i];
		
		delete [] PM[0][i];
		delete [] PM[1][i];
		delete [] pad[i];
		delete [] per[i];
		delete [] rper[i];
		
	}
	delete [] per[input_size];
	delete [] rper[input_size];


	delete [] PM[0];
	delete [] PM[1];

	delete [] GPM[0];
	delete [] GPM[1];

//	sw.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	sw_garbled_dfa = sw.getElapsedTime()-rand_time;
	// OT to transfer keys


	
	delete [] state_sizes;
	for (int i=0;i<input_size;i++)
	{

		delete [] keys[0][i];
		delete [] keys[1][i];
//		for (int j=0;j<dfa_size;j++)
//		{
//			delete [] pad[i][j];
//			delete [] GPM[0][i][j];
//			delete [] GPM[1][i][j];
//		}
//		delete [] pad[i];
//		delete [] GPM[0][i];
//		delete [] GPM[1][i];
	}
//	delete [] GPM[0];
//	delete [] GPM[1];
//	for (int i=0;i<(input_size+1);i++)
//	{
//		delete [] per[i];
//		delete [] rper[i];
//	}
	
	cout << "OAE_PRG_server() finished" << endl;
	return 1;
}




byte* OAE_PRG_client(int input_size, // The number of clients inputs
	int* inputs,			// The client inputs
	int dfa_size,			// The number of DFA states
	int message_size		// The size of message in the last state (in Bits)
	)
{
	cout << "OAE_PRG_client() started." << endl;
	CStopWatch sw;
	sw.startTimer();
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	// Finding the minimum number of bytes for each state
	int state_size; // Maximum size of states
	CryptoPP::Integer q = dfa_size;
	int logq =  q.BitCount();
	int logq_bytes = (int)ceil((double)logq/8);
	int message_bytes = (int)ceil((double)message_size/8);
	state_size = message_bytes + logq_bytes;
	int* state_sizes =  new int[input_size];



	//byte*** M[2];
	byte*** GPM[2]; 
	int length;
	int xx;


	// OT to transfer keys
	byte** c_keys = new byte*[input_size];
		
	for (int i=0;i<input_size;i++)
		state_sizes[i]=state_size;
		
	OT_dynamic_message_size_client(input_size,state_sizes,c_keys,inputs);


	//sali-socket receiving GPM
	GPM[0] = new byte**[input_size];
	GPM[1] = new byte**[input_size];
	for (int i=0;i<input_size;i++)
	{
		GPM[0][i] = new byte*[dfa_size];
		GPM[1][i] = new byte*[dfa_size];
		
		for (int j=0;j<dfa_size;j++)
		{
			GPM[0][i][j] = new byte[state_size];
			GPM[1][i][j] = new byte[state_size];
			myrecv(GPM[0][i][j],state_size);
			myrecv(GPM[1][i][j],state_size);
		}
		
		
		
		
	}
	
	//for (int tester=0;tester<input_size;tester++)
	//{
	//	printf("i=%d",tester);
	//	//cout<<"i="<<tester<<endl;
	//	PrintBytes("Should KEY",keys[inputs[tester]][tester],state_sizes[tester]);
	//	PrintBytes("c_Key",c_keys[tester],state_sizes[tester]);
	//	cout<<endl<<endl;
	//}
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	sw.startTimer();
	byte* result;			// The result of DFA evaluation
	int cur_state_index=0;
	byte* cur_state;
	int NS = 0;
	byte* NS_bytes = new byte[logq_bytes];
	byte* pad_bytes=0;
	byte* temp=0;
	for (int i=0;i<input_size;i++)
	{
		if (i==0)
		{
			cur_state = XOREncrypt(GPM[inputs[i]][i][NS],c_keys[i],state_size);

			pad_bytes = new byte[message_bytes];
			divide_bytes(cur_state,pad_bytes,message_bytes,NS_bytes,logq_bytes);
			/*PrintBytes("Current State",cur_state,state_size);
			PrintBytes("pad",pad_bytes,message_bytes);
			PrintBytes("NS",NS_bytes,logq_bytes);*/
			NS = convert_byte_to_int(logq_bytes,NS_bytes);
			//cout<<"rper[ns] = "<<rper[i+1][NS]<<endl;
		}
		else
		{
			if (i<(input_size-1))
			{
				temp = XOREncrypt(GPM[inputs[i]][i][NS],c_keys[i],state_size);
				cur_state = XOREncrypt(temp,PRG_n_to_m_bytes_hash(pad_bytes,message_bytes,state_size),state_size);
				divide_bytes(cur_state,pad_bytes,message_bytes,NS_bytes,logq_bytes);
				NS = convert_byte_to_int(logq_bytes,NS_bytes);
				/*if (NS>dfa_size)
				{
				cout<<"WTF, NS in the baghalies"<<endl;
				}*/
				//PrintBytes("PRG1",PRG_32_to_m_bytes_hash(pad_bytes,state_size),state_size);

				//PrintBytes("Current State",cur_state,state_size);
				//PrintBytes("pad",pad_bytes,message_bytes);
				//PrintBytes("NS",NS_bytes,logq_bytes);
				//PrintBytes("per[NS]",,logq_bytes);
				//cout<<"rper[ns] = "<<rper[i+1][NS]<<endl;
			}
			else
			{

				temp = XOREncrypt(GPM[inputs[i]][i][NS],c_keys[i],state_size);
				cur_state = XOREncrypt(temp,PRG_n_to_m_bytes_hash(pad_bytes,message_bytes,state_size),state_size);
				divide_bytes(cur_state,pad_bytes,message_bytes,NS_bytes,logq_bytes);
				result = pad_bytes;
			}
		}
	}
//	sw.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	sw_c = sw.getElapsedTime()-rand_time;

	delete [] state_sizes;
	for (int i=0;i<input_size;i++)
	{

	//	delete [] keys[0][i];
	//	delete [] keys[1][i];
		for (int j=0;j<dfa_size;j++)
		{
		//	delete [] pad[i][j];
			delete [] GPM[0][i][j];
			delete [] GPM[1][i][j];
		}
		//delete [] pad[i];
		delete [] GPM[0][i];
		delete [] GPM[1][i];
	}
	delete [] GPM[0];
	delete [] GPM[1];
//	for (int i=0;i<(input_size+1);i++)
//	{
		//delete [] per[i];
		//delete [] rper[i];
//	}
	cout << "OAE_PRG_client() finished." << endl;
	return result;
}



byte* OAE_PRG_256_client(int input_size, // The number of clients inputs
	int* inputs,			// The client inputs (converted from asci to int already)
	int dfa_size,			// The number of DFA states
	int message_size		// The size of message in the last state (in Bits)
	)
{
	cout << "OAE_PRG_256_client() started." << endl;
	CStopWatch sw;
//	sw.startTimer();
//	sw_r1=0;
//	sw_r2=0;
//	sw_r3=0;
	// Finding the minimum number of bytes for each state
	int state_size; // Maximum size of states
	CryptoPP::Integer q = dfa_size;
	int logq =  q.BitCount();
	int logq_bytes = (int)ceil((double)logq/8);
	//cout << "logq_bytes: " << logq_bytes << endl;
	int real_message_bytes = (int)ceil((double)message_size/8);
	int message_bytes = 10;
	if (message_bytes<real_message_bytes)
	{
		message_bytes = real_message_bytes;
	}
	
	state_size = message_bytes + logq_bytes;
	int* state_sizes =  new int[input_size];

	int length;
	int xx;


//	sw.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	sw_garbled_dfa = sw.getElapsedTime()-rand_time;
	// OT to transfer keys
	byte** c_keys = new byte*[input_size];

	for (int i=0;i<input_size;i++)
		state_sizes[i]=state_size;
	
	OT_dynamic_256alpha_client(input_size,state_sizes,c_keys,inputs);


 	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	sw.startTimer();
	byte* result;			// The result of DFA evaluation
	int cur_state_index=0;
	byte* cur_state;
	cur_state=new byte[state_size];
	int NS = 0;
	byte* NS_bytes = new byte[logq_bytes];
	byte* pad_bytes=0;
	byte* temp=0;
	byte* target = new byte[state_size];
	byte* tmpbuffer = new byte[state_size];

	for (int i=0;i<input_size;i++)
	{
		sw.stopTimer();
		rand_time = (sw_r1+sw_r2+sw_r3);
		sw_c += sw.getElapsedTime()-rand_time;

		for (int j=0;j<dfa_size;j++)
		{
			for (int z=0;z<256;z++)
			{
				if (z==inputs[i] && j==NS)
					myrecv(target,state_size);
				else
					myrecv(tmpbuffer,state_size);
			}
		}
		
		sw.startTimer();
		if (i==0)
		{
			cur_state=XOREncrypt(target,c_keys[i],state_size);
			pad_bytes = new byte[message_bytes];
			divide_bytes(cur_state,pad_bytes,message_bytes,NS_bytes,logq_bytes);
			NS = convert_byte_to_int(logq_bytes,NS_bytes);
		}
		else
		{
			if (i<(input_size-1))
			{
				temp = XOREncrypt(target,c_keys[i],state_size);
				cur_state=XOREncrypt(temp,PRG_n_to_m_bytes_hash(pad_bytes,message_bytes,state_size),state_size);
				divide_bytes(cur_state,pad_bytes,message_bytes,NS_bytes,logq_bytes);
				NS = convert_byte_to_int(logq_bytes,NS_bytes);
				cout << "NS is:" << NS << endl;
				if (NS>dfa_size)
				{
					cout<<"WTF, NS in the baghalies"<<endl;
				}
			}
			else
			{
				temp = XOREncrypt(target,c_keys[i],state_size);
				cur_state = XOREncrypt(temp,PRG_n_to_m_bytes_hash(pad_bytes,message_bytes,state_size),state_size);
				divide_bytes(cur_state,pad_bytes,message_bytes,NS_bytes,logq_bytes);
				result = pad_bytes;
			}
		}
	}

	// Check if it is the expanded message so we can shrink it
	if (message_bytes>real_message_bytes)
	{
		byte* shrinked_result = expand_bytes(result,message_bytes,real_message_bytes);
		delete [] result;
	}

	sw.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
	sw_c += sw.getElapsedTime()-rand_time;

	delete [] state_sizes;
	delete [] tmpbuffer;
	delete [] target;
	
	//cout << "before return result...\n" ;
	cout << "OAE_PRG_256_client() finished." << endl;	
	if (message_bytes>real_message_bytes)
	{
		return shrinked_result;
	}
	else
	{
		return result;
	}
	
}


byte* OAE_PRG_256_server(int input_size, // The number of clients inputs
	int dfa_size,			// The number of DFA states
	int* dfa[256],			// The DFA with 256 inputs
	int message_size,		// The size of message in the last state (in Bits)
	byte** messages			// The message in each state Byte*
	)
{
	cout << "OAE_PRG_256_server() started." << endl;	
	CStopWatch sw;
	sw.startTimer();
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	// Finding the minimum number of bytes for each state
	int state_size; // Maximum size of states
	CryptoPP::Integer q = dfa_size;
	int logq =  q.BitCount();
	int logq_bytes = (int)ceil((double)logq/8);
	//cout << "logq_bytes: " << logq_bytes << endl;
	//int message_bytes = (int)ceil((double)message_size/8);
	int real_message_bytes = (int)ceil((double)message_size/8);
	int message_bytes = 10;
	if (message_bytes<real_message_bytes)
	{
		message_bytes = real_message_bytes;
	}
	else
	{
		//Expand messages to 10 bytes by padding
		//expand_bytes(source,source_size,expanded_size)
		byte** expanded_messages = new byte*[256]
		for (int z=0;z<256;z++)
		{
			expanded_messages[z] = expand_bytes(messages[z],real_message_bytes,message_bytes);
		}	
	}
	

	state_size = message_bytes + logq_bytes;
	cout << "state_size is: " << state_size << endl;
	int* state_sizes =  new int[input_size];
	// Build PAD matrix
	// Build Keys K[256][input_size] (256 keys for each row)
	byte** keys[256] ;

	byte*** pad = new byte**[input_size];
	for (int i=0;i<input_size;i++)
	{
		pad[i] = new byte*[dfa_size];
		state_sizes[i] = state_size;

		for (int j=0;j<dfa_size;j++)
		{
			//sali
			pad[i][j] = GetRandomBytes(message_bytes);
			//pad[i][j] = convert_int_to_kbyte(message_bytes,0); 
			//cout << " x ";
	//		PrintBytes(pad[i][j],message_bytes);
			
		}
	}
	for (int j=0;j<256;j++)
	{
		keys[j] = new byte*[input_size];
		for (int i=0;i<input_size;i++)
		{
			//sali
			keys[j][i] = GetRandomBytes(state_size);
			//keys[j][i] = convert_int_to_kbyte(state_size,0);
		}
	}
	
	// OT to transfer keys	
	OT_dynamic_256alpha_server(input_size,state_sizes,keys);

	// Build PER matrix
	int** per = new int*[input_size+1];
	int** rper = new int*[input_size+1];
	for (int i=0;i<(input_size+1);i++)
	{
		per[i] = new int[dfa_size];
		rper[i] = new int[dfa_size];
		for (int j=0;j<dfa_size;j++)
		{
			per[i][j] = j;
		}
		random_permute(per[i],dfa_size);
		for (int j=0;j<dfa_size;j++)
		{
			rper[i][per[i][j]]=j;
		}
	}

/*
	cout << "pad and per allocation finished....going for garbeling...." <<endl;
	   for (int i=0;i<=1000000000;i++)
            for (int j=0;j<=4;j++);
            
    cout << "start garbeling***..." <<endl;
*/
                
	//cout<< "xxx1\n";

	// Build DFA Matrix M
	//byte*** M[2];
	byte*** PM[256];
	byte*** GPM[256]; 
	int length;
	int xx;
	byte* xor_output;
	byte* prg_output;
	byte* comb_output;
	
	for (int z=0;z<256;z++)
	{
		PM[z] = new byte**[input_size];
		GPM[z] = new byte**[input_size];
	}
	for (int i=0;i<input_size;i++)
	{
		for (int z=0;z<256;z++)
		{
			PM[z][i] = new byte*[dfa_size];
			GPM[z][i] = new byte*[dfa_size];
		}
		for (int j=0;j<dfa_size;j++)
		{
			if (i==0)
			{
				for (int z=0;z<256;z++)
				{
					PM[z][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[z][0]]);
					comb_output = combine_bytes(pad[i+1][per[i+1][dfa[z][0]]],message_bytes,PM[z][i][j],logq_bytes);
					GPM[z][i][j] = XOREncrypt(comb_output,keys[z][i],state_size);
					delete [] comb_output;
				}
				//if (j==0) PrintBytes("Key[1]",keys[0][i],state_sizes[i]);
			}
			else
			{
				if (i<(input_size-1))
				{
					for (int z=0;z<256;z++)
					{
						PM[z][i][j] = convert_int_to_kbyte(logq_bytes,per[i+1][dfa[z][rper[i][j]]]);
						comb_output = combine_bytes(pad[i+1][per[i+1][dfa[z][rper[i][j]]]],message_bytes,PM[z][i][j],logq_bytes);

						prg_output = PRG_n_to_m_bytes_hash(pad[i][j],message_bytes,state_size);
							
						xor_output = XOREncrypt(comb_output,prg_output,state_size);
						delete [] prg_output;
						delete [] comb_output;
						
						GPM[z][i][j] = XOREncrypt(xor_output,keys[z][i],state_size);
						delete [] xor_output;
					}
				}
				else
				{
					for (int z=0;z<256;z++)
					{
						PM[z][i][j] = convert_int_to_kbyte(logq_bytes,1);
						comb_output = combine_bytes(expanded_messages[dfa[z][rper[i][j]]],message_bytes,PM[z][i][j],logq_bytes);

						prg_output = PRG_n_to_m_bytes_hash(pad[i][j],message_bytes,state_size);

						xor_output = XOREncrypt(comb_output,prg_output,state_size);
						delete [] prg_output;
						delete [] comb_output;
						
						GPM[z][i][j] = XOREncrypt(xor_output,keys[z][i],state_size);
						delete [] xor_output;
					}
				}
			}
			for (int z=0;z<256;z++)
			{
				mysend(GPM[z][i][j],state_size);
				delete [] GPM[z][i][j];
				delete [] PM[z][i][j];
			}
			delete [] pad[i][j];	
		}
		
		for (int z=0;z<256;z++)
		{
			delete [] PM[z][i];
			delete [] GPM[z][i];
			delete [] keys[z][i];
		}
		delete [] pad[i];
		delete [] per[i];
		delete [] rper[i];
	}
	//cout << "xxx2\n";
	delete [] state_sizes;
	for (int z=0;z<256;z++)
	{
		delete [] PM[z];
		delete [] GPM[z];
	}

	sw.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
	sw_garbled_dfa = sw.getElapsedTime()-rand_time;
	cout << "OAE_PRG_256_server() finished." << endl;	
}
