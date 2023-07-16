#include "cryptopp.h"
// Crypto++ Include
 // PRNG



//CryptoPP::Integer *random_2p;
//byte* random_bytes;
//int *random_0_dfa;
//int Getrandom_counter;
//int random_bytes_counter;
//int random_bytes_index;
//int random_0_dfa_index;
//int random_2p_index;
using namespace std;

int main()
{
	//random_2p_index=0;
	//random_0_dfa_index=0;
	//random_bytes_index=0;
	//Getrandom_counter=0;
	//random_bytes_counter=0;
	    // Scratch Area
    //const unsigned int BLOCKSIZE = 16 * 8;
    //byte pcbScratch[ BLOCKSIZE ];

    // Random Block



    CryptoPP::AutoSeededRandomPool rng;

	cout << "Hi 1" << endl;
	s_stopwatch prg_,xor_,norm_;

	CStopWatch s_prg;
	s_prg.startTimer();

    for (int i=0;i<1000000000;i++);

	s_prg.stopTimer();
	//rand_time = (sw_r1+sw_r2+sw_r3);
	cout << "elapsed time:" << s_prg.getElapsedTime() << endl;


	//xor = avg_test_xor(1000,20,20);
	//cout<<"XOR time <ms> = "<<xor*1000<<endl;
//	prg_ = avg_test_prg(1000,20,5);
//	cout<<"PRG time <ms> = "<<prg_*1000<<endl;
	//norm_ = avg_test_norm(1000,20,10);
	//cout<<"Nor time <ms> = "<<norm_*1000<<endl;


	s_stopwatch timer2;
	timer2 = avg_test_prg(20,1000,5);
	PrintTimer(timer2);
	//
	cout<<endl<<endl;

	//const int ne=1;
	//int x[ne]={40};
	//s_exp exps[ne];
	//for(int i=0;i<ne;i++)
	//{
	//	exps[i].exp_name = "XOR";
	//	exps[i].n = x[i];
	//	exps[i].q = 20;
	//	exps[i].runs =5;
	//	exps[i].stopwatch = avg_test_xor(exps[i].n,exps[i].q,exps[i].runs);
	//}
	////Cout_Experiment("XOR2",exps,ne);
	//Print_Experiment("XOR2",exps,ne,false);
	printf ("here0...\n");
	cout << "hey you..." << endl;
//test_prg_256(5,5);
	printf ("here1...\n");
	/*int message_size = 12*8;
	byte* message1 = new byte[32];
	byte* message2 = new byte[32];
	byte* res;
	for (int i=0;i<32;i++)
	{
		message1[i] = 48;
		message2[i] = 49;
	}
//	CStopWatch sw;
	double t1=0;
	for (int j=0;j<1000;j++)
	{
//		sw.startTimer();
		res = XOREncrypt(message1,message1,32);
//		sw.stopTimer();
//		t1+=sw.getElapsedTime();
	}
//	cout<<"XOR Time"<<t1<<endl;
	double t2=0;

	for (int j=0;j<1000;j++)
	{
//		sw.startTimer();
		res = PRG_32_to_m_bytes(message1,32);
//		sw.stopTimer();
//		t2+=sw.getElapsedTime();
	}

//	cout<<"PRG Time"<<t2<<endl;

//	cout<<"Ratio = "<<t2/t1<<endl;

	double st,mt;
	st = benchmark_sub(1000);
	mt = benchmark_mult(1000);
	cout<<"Sub time = "<<st<<endl;
	cout<<"Mul time = "<<mt<<endl;
	cout<<"mul/sub = "<<mt/st<<endl;
	cout<<"256 * Sub time = "<<st*256<<endl;
	cout<<"8 * mul time = "<<mt*8<<endl;*/
	//cout<<"First"<<swt.getElapsedTime()*1000<<endl;

	//swt.startTimer();
	//prg = avg_test_xor(100,20,2);
	//swt.stopTimer();
	//cout<<"Second"<<swt.getElapsedTime()*1000<<endl;
 //   rng.GenerateBlock( pcbScratch, BLOCKSIZE );
	//

 //   // Output
 //   std::cout << "The generated random block is:" << std::endl;
	//for( unsigned int i = 0; i < BLOCKSIZE; i++ )
	//{
	//	std::cout << "0x" << std::setbase(16) << std::setw(2) << std::setfill('0');
	//	std::cout << static_cast<unsigned int>( pcbScratch[ i ] ) << " ";

	//}
	 // EC_80_256 test /////////////////////
	//byte** messages[2];
	//messages[0]=new byte*[2];
	//messages[1]=new byte*[2];
	//byte* c_message[2];
	//int selections[2];
	//selections[0]=1;
	//selections[1]=0;
	//messages[0][0]=(unsigned char*)"1";
	//messages[1][0]=(unsigned char*)"2";
	//messages[0][1]=(unsigned char*)"3";
	//messages[1][1]=(unsigned char*)"4";
	//OT_n_m(2,48,messages,c_message,selections);
	//PrintBytes(c_message[0],48);
	//cout<<c_message[0]<<endl;
	//cout<<c_message[1]<<endl;

	//const int n_ots=30;
	//byte** s_messages[2];
	//int selections[n_ots];
	//byte* c_message[n_ots];
	//int* message_size =  new int[n_ots];
	//s_messages[0] = new byte*[n_ots];
	//s_messages[1] = new byte*[n_ots];
	//CryptoPP::Integer temp;
	//int length=32;
	//for (int i=0;i<n_ots;i++)
	//{
	//	//temp=i;
	//	s_messages[0][i]=convert_int_to_kbyte(length,i*2);
	//	s_messages[1][i]=convert_int_to_kbyte(length,i*2+1);
	//	message_size[i]=20;
	//	selections[i]=0;
	//}
	//s_messages[0][0]=(unsigned char*)"abc";
	//s_messages[0][1]=(unsigned char*)"efg";
	//OT_80_m_d(n_ots,message_size,s_messages,c_message,selections);
	//for (int i=0;i<2;i++)
	//{
	//	cout<<"i="<<i<<endl<<c_message[i]<<endl<<endl;
	//}
	//CryptoPP::Integer q = 9;
	//int logq =  q.BitCount();
	//cout<<logq<<endl;
//	test();
	/*int x[20];
	for (int j=0;j<20;j++)
	{
		x[j] = j;
	}
	random_permute(x,20);
	for (int j=0;j<20;j++)
	{
		cout<<"x"<<j<<"="<<x[j]<<endl;
	}*/

	 ///////////////////////////////////////
	/*CryptoPP::Integer ii("6277101735386680763835789423207666416083908700390324961279"),temp;
	byte* tempcpy=new byte[32];
	memset(tempcpy,'0',32*sizeof(unsigned char));
	for (int index=0;index<32;index++)
	{
		if (index<=ii.ByteCount())
		{
			tempcpy[index]=ii.GetByte(index);
		}
		else
			tempcpy[index]='\0';

	}

	for (int index=0;index<32;index++)
	{
		temp.SetByte(index,tempcpy[index]);
	}
	cout<<ii<<endl;
	cout<<temp<<endl;*/
	//byte* S[2];
	//S[0]=new byte[32];
	//S[1]=new byte[32];
	//memset(S[0],'\0',32*sizeof(byte));
	//cout<<S[0]<<endl;
	/*CryptoPP::Integer test("123");
	cout<<test;*/
	//cout<<messages[0][1]<<endl;
	//PrintBytes(c_message[0]);
	//PrintBytes(c_message[1]);
	//cout<<c_message[0]<<endl;
	//cout<<c_message[1]<<endl;
    std::cout << std::endl;
	return 0;
}

