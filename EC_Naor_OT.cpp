
#include "EC_Naor_OT.h"
#include <math.h>

struct f
{
	int x;
	int y;
};
		

struct HexCharStruct
{
	char c;
	HexCharStruct(char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
	return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(char _c)
{
	return HexCharStruct(_c);
}
void PrintBytes(byte* data,int length)
{
	//for (int i=0;i<sizeof(data);i++)
	//{
	//	scanf ("%s", &chr);
	//	printf ("%x\n", chr[0]);
	//	getch();
	//	cout<<data[i];
	//}
	using namespace std;
	//cout<<"Size="<<sizeof(data)<<endl;
	//for (int i=0;i<32;i++)
	//{
	//	cout<<setw(2)<<setfill('0')<<hex(data[i]);
	//}
	//cout<<endl<<endl;
	cout<<endl;
	for( unsigned int i = 0; i < length; i++ )
	{
		std::cout << "0x" << std::setbase(16) << std::setw(2) << std::setfill('0');
		std::cout << static_cast<unsigned int>( data[ i ] ) << " ";

	}
	cout<<endl;
	//CryptoPP::Integer temp;
	//for (int index=0;index<32;index++)
	//{
	//	temp.SetByte(index,data[index]);
	//}
	//cout<<temp<<endl<<endl;
}
void PrintBytes(char* x,byte* data,int length)
{
	using namespace std;
	cout<<endl;
	cout<<x<<endl;
	for( unsigned int i = 0; i < length; i++ )
	{
		std::cout << "0x" << std::setbase(16) << std::setw(2) << std::setfill('0');
		std::cout << static_cast<unsigned int>( data[ i ] ) << " ";

	}
	cout<<endl;
}

bool CompareBytes(byte* data1,byte* data2,int length)
{
	int a=0;
	a = memcmp(data1,data2,length*sizeof(byte));
	if (a==0)
	{
		return true;
	}
	else return false;
}
// This will accept 32 bytes of data and output m bytes of data
byte* PRG_32_to_m_bytes(byte* data, int m)
{
	const unsigned int SEEDSIZE = 32;
	CryptoPP::RandomPool rng;
	rng.Put(data, 32*sizeof(byte));
	byte* randomBytes = new byte[m];
	rng.GenerateBlock(randomBytes, m*sizeof(byte));
	return randomBytes;
}

byte* PRG_32_to_m_bytes_hash(byte* data, int m)
{
	// h( i | 32byte)
	CryptoPP::SHA256 hash;
	CryptoPP::Integer index=0;
	byte* x = new byte[m/32+1];
	byte* output = new byte[m];
	byte* hashed = new byte[32];
	int cur=0;
	// The number of hashes we need
	for (CryptoPP::Integer j=0;j<(m/32+1);j++)
	{
		// Convert j to byte
		for (int i=0;i<j.ByteCount();i++)
		{
			x[i]=j.GetByte(i);
		}
		hash.Update(x,j.ByteCount()*sizeof(byte));
		hash.Update(data,32*sizeof(byte));
		hash.Final(hashed);
		for (int k=0;k<32;k++)
		{
			if (cur<m)
			{
				output[cur]=hashed[k];
				cur++;
			}
			else
				break;
		}
	}
	
	delete [] x;
	delete [] hashed;
	return output;
}

byte* PRG_n_to_m_bytes_hash(byte* data, int n, int m)
{
	// h( i | 32byte)
	CryptoPP::SHA256 hash;
	CryptoPP::Integer index=0;
	byte* x = new byte[m/32+1];
	byte* output = new byte[m];
	byte* hashed = new byte[32];
	int cur=0;
	// The number of hashes we need
	for (CryptoPP::Integer j=0;j<(m/32+1);j++)
	{
		// Convert j to byte
		for (int i=0;i<j.ByteCount();i++)
		{
			x[i]=j.GetByte(i);
		}
	
		hash.Update(x,j.ByteCount()*sizeof(byte));
		hash.Update(data,n*sizeof(byte));
		hash.Final(hashed);
		
		for (int k=0;k<32;k++)
		{
			if (cur<m)
			{
				output[cur]=hashed[k];
				cur++;
			}
			else
				break;
		}
	}
	
	delete [] x;
	delete [] hashed;
	return output;
}



byte* convert_int_to_byte(int& length,CryptoPP::Integer i)
{
	length = i.ByteCount();
	byte* tempcpy=new byte[length];
	
	memset(tempcpy,'\0',length*sizeof(unsigned char));
	for (int index=0;index<length;index++)
	{
		if (index<i.ByteCount())
		{
			tempcpy[index]=i.GetByte(index);
		}
		else
			tempcpy[index]='\0';
	}
	return tempcpy;
}

byte* convert_int_to_byte(int& length,int x)
{
	CryptoPP::Integer i = x;
	length = i.ByteCount();
	byte* tempcpy=new byte[length];

	memset(tempcpy,'\0',length*sizeof(unsigned char));
	for (int index=0;index<length;index++)
	{
		if (index<i.ByteCount())
		{
			tempcpy[index]=i.GetByte(index);
		}
		else
			tempcpy[index]='\0';
	}
	return tempcpy;
}

byte* convert_int_to_kbyte(int k,int x)
{
	CryptoPP::Integer i = x;
	byte* tempcpy=new byte[k];

	memset(tempcpy,'\0',k*sizeof(unsigned char));
	for (int index=0;index<k;index++)
	{
		if (index<i.ByteCount())
		{
			tempcpy[index]=i.GetByte(index);
		}
		else
			tempcpy[index]='\0';
	}
	return tempcpy;
}
byte* convert_int_to_kbyte(int k,CryptoPP::Integer i)
{
	//CryptoPP::Integer i = x;
	byte* tempcpy=new byte[k];

	memset(tempcpy,'\0',k*sizeof(unsigned char));
	for (int index=0;index<k;index++)
	{
		if (index<i.ByteCount())
		{
			tempcpy[index]=i.GetByte(index);
		}
		else
			tempcpy[index]='\0';
	}
	return tempcpy;
}


int convert_byte_to_int(int length,byte* input)
{
	CryptoPP::Integer i;
	for (int index=0;index<length;index++)
	{
		i.SetByte(index,input[index]);
	}
	return (int)i.ConvertToLong();
}

byte* convert_int_to_byte_32(CryptoPP::Integer i)
{
	//byte *output;
	//std::st	
	//std::stringstream out;
	//out << i;
	//ss = out.str();
	//length=ss.size()+1;
	//char* in2=new char[length];
	//strcpy(in2,ss.c_str());
	//output=(unsigned char*)in2;
	//return output;

	byte* tempcpy=new byte[32];
	memset(tempcpy,0,32*sizeof(unsigned char));
	for (int index=0;index<32;index++)
	{
		if (index<i.ByteCount())
		{
			tempcpy[index]=i.GetByte(index);
		}
		else
			tempcpy[index]='\0';
	}
	return tempcpy;
}

byte* XOREncrypt(byte* dataToBeEncrypted, byte* key, int length)
{
	byte* out;
	out=new byte[length];
	for(unsigned long index = 0; index < length ; index++ )
	{
		out[index] = dataToBeEncrypted[index] ^ key[index];
	}
	return out;
}


//CryptoPP::Integer GetRandom(CryptoPP::Integer themin,CryptoPP::Integer themax)
//{
//	CryptoPP::Integer ran(random_2p[random_2p_index]);
//	random_2p_index++;
//	return ran;
//}
//
//int GetRandom(int themin,int themax)
//{
//	int ran = random_0_dfa[random_0_dfa_index];
//	random_0_dfa_index++;
//	return ran;
//}
//
//byte* GetRandomBytes(int n_bytes)
//{
//	byte* buf = new byte[n_bytes];
//	for (int i=0;i<n_bytes;i++)
//	{
//		buf[i] = random_bytes[random_bytes_index];
//		random_bytes_index++;
//	}
//	return buf;
//}

CryptoPP::Integer GetRandom(CryptoPP::Integer themin,CryptoPP::Integer themax)
{
	CStopWatch sw;
	sw.startTimer();
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::Integer Range = themax - themin;
	//calc the number of bits of randomness required 
	const unsigned int nbits = Range.BitCount(); 
	const size_t nbytes = nbits/8 + 1; 
	//generate random value within the range and assign to x 
	CryptoPP::Integer ran; 
	do 
	{ 
		//will use other source of randomness here 

		CryptoPP::SecByteBlock buf(nbytes); 
		rng.GenerateBlock( buf, buf.SizeInBytes() ); 
		if (nbytes) 
			buf[0] = (byte)CryptoPP::Crop(buf[0], nbits % 8); 
		ran = CryptoPP::Integer(buf, nbytes, 
			CryptoPP::Integer::UNSIGNED); 
	}while (ran > Range); 
	ran += themin; 
	sw.stopTimer();
	sw_r1+=sw.getElapsedTime();
	//cout<<"ran="<<ran<<endl;
	return ran;
}


int GetRandom(int themin,int themax)
{
	CStopWatch sw;
	sw.startTimer();
	//Getrandom_counter++;
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::Integer Range = themax - themin;
	//calc the number of bits of randomness required 
	const unsigned int nbits = Range.BitCount(); 
	const size_t nbytes = nbits/8 + 1; 
	//generate random value within the range and assign to x 
	CryptoPP::Integer ran; 
	do 
	{ 
		//will use other source of randomness here 

		CryptoPP::SecByteBlock buf(nbytes); 
		rng.GenerateBlock( buf, buf.SizeInBytes() ); 
		if (nbytes) 
			buf[0] = (byte)CryptoPP::Crop(buf[0], nbits % 8); 
		ran = CryptoPP::Integer(buf, nbytes, 
			CryptoPP::Integer::UNSIGNED); 
	}while (ran > Range); 
	ran += themin; 
	//cout<<"ran="<<ran<<endl;
	sw.stopTimer();
	sw_r2+=sw.getElapsedTime();
	return (int)ran.ConvertToLong();
}



// It will return an allocated n_byte array of byte
byte* GetRandomBytes(int n_bytes)
{
	CStopWatch sw;
	sw.startTimer();
	//random_bytes_counter+=n_bytes;
	CryptoPP::AutoSeededRandomPool rng;
	byte* buf = new byte[n_bytes];
	rng.GenerateBlock( buf, n_bytes );
	sw.stopTimer();
	sw_r3+=sw.getElapsedTime();
	return buf;
}

void OT(int n_ots           //number of OTs
	   ,int message_size    //message_size: number of message bytes
	   ,byte** s_messages[] //The array of messages: messages[n_inputs][message_size] of bytes
	   ,byte* c_messages[]  //The array of chooser's messages (to be filled after protocol)
	   ,int selections[]    //The choosers selection bits
	   
	   )
{
	// Alg.1 Reduce the message_size in OT
	// Alg.2 Reduce the number of OTs
	//sali
	
	if (n_ots<=80)
	{
		if ((message_size*8)<=256)
		{
			// call OT{80}{256}
			OT_80_256(n_ots,message_size,s_messages,c_messages,selections);
			
		} 
		else
		{
			//call Alg.1 to reduce message_size to 256
				//call OT{80}{256}
			OT_80_m(n_ots,message_size,s_messages,c_messages,selections);
		}
	} 
	else
	{
		//call Alg.2 to reduce the OTs to 80
		// Call Alg.1 to reduce the message_size
		// call OT{80}{256}
		OT_n_m(n_ots,message_size,s_messages,c_messages,selections);
	}
}


int OT_80_256(int n_ots    //number of OTs
			,int message_size  //message_size: number of message bytes
			,byte** s_messages[2]  //The array of messages: messages[n_ots][message_size] of bytes
			,byte* c_messages[]  //The array of chooser's messages (to be filled after protocol)
			,int selections[] //The choosers selection bits
		)
{
	try
    {
//		CStopWatch swot;
		
		//if (message_size<32)
		//{
		//	for (int ot_number=0;ot_number<n_ots;ot_number++)
		//	{
		//		for (int index=message_size;index<32;index++)
		//		{
		//			s_messages[0][ot_number][index]='\0';
		//			s_messages[1][ot_number][index]='\0';
		//		}
		//	}
		//}
		///////////////////////////////////////////////////////////////////////////////
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
		///////////////////////////////////////////////////////////////////////////////
		CryptoPP::Integer r;  // The random number selected by sender
		CryptoPP::ECPPoint CP,Cr; // The constant points C
		CryptoPP::ECPPoint gr; // The public key of sender
		CryptoPP::ECPPoint *PK[2]; //PK calculated by chooser
		CryptoPP::ECPPoint *PKs[2]; //PK calculated by chooser
		for (int al=0;al<2;al++)
		{
			PK[al]=new CryptoPP::ECPPoint[n_ots];
			PKs[al]=new CryptoPP::ECPPoint[n_ots];
		}
		CryptoPP::AutoSeededRandomPool rng;
		CryptoPP::ECP ec( p, a, b );
		if( false == ec.ValidateParameters( rng, 3 ) )
		{ throw std::string( "EC Parameter Validation Failure" ); }
		// Pre computations and Public Info (gr and C)
		CP = ec.Multiply(GetRandom(2,p),G);
		//cout<<"CP"<<endl<<CP.x<<" , "<<CP.y<<endl;
		r = GetRandom(2,p);
		//cout<<"r="<<r<<endl;
		gr = ec.Multiply(r,G);
		//cout<<"gr"<<gr.x<<" , "<<gr.y<<endl;
		Cr = ec.Multiply(r,CP);
		//cout<<"Cr"<<Cr.x<<" , "<<Cr.y<<endl;
		CryptoPP::Integer *k=new CryptoPP::Integer[n_ots]; // Random number selected by chooser for each query
		CryptoPP::ECPPoint *c_key=new CryptoPP::ECPPoint[n_ots];
		CryptoPP::ECPPoint testCP;
		////////////////////////////////////////////////////////////////////
		// Chooser Query Computation
		////////////////////////////////////////////////////////////////////
		//sw_r1=0;
		//sw_r2=0;
		//sw_r3=0;
		//swot.startTimer();
		for (int i=0;i<n_ots;i++)
		{
			k[i] = GetRandom(2,p);
			c_key[i]=ec.Multiply(k[i],gr);
			//if(i==0) {cout<<"CKey"<<c_key[i].x<<" , "<<c_key[i].y<<endl;}
			PK[selections[i]][i] = ec.Multiply(k[i],G);
			if (1==selections[i])
			{
				PK[0][i] = ec.Subtract(CP,PK[selections[i]][i]);
				//testCP=ec.Add(PK[0][i],PK[1][i]);
				//cout<<"PK0"<<endl<<PK[0][i].x<<" , "<<PK[0][i].y<<endl;
				//cout<<"PK1"<<endl<<PK[1][i].x<<" , "<<PK[1][i].y<<endl;
			}
			
		}
		//swot.stopTimer();
//		rand_time = (sw_r1+sw_r2+sw_r3);
//		sw_c_mot=swot.getElapsedTime()-rand_time;
		////////////////////////////////////////////////////////////////////
		// Chooser to Sender
		////////////////////////////////////////////////////////////////////
		for (int i=0;i<n_ots;i++)
		{
			PKs[0][i]=PK[0][i];
		}
		////////////////////////////////////////////////////////////////////
		// Server Response Computation
		////////////////////////////////////////////////////////////////////
		//sw_r1=0;
		//sw_r2=0;
		//sw_r3=0;
	//	swot.startTimer();
		CryptoPP::ECPPoint temp;
		CryptoPP::Integer *R=new CryptoPP::Integer[n_ots];
		CryptoPP::SHA256 hash0,hash1,hash_c;
		byte** s_to_c_messages[2];
		for (int al=0;al<2;al++)
		{
			s_to_c_messages[al]=new byte*[n_ots];
		}
		int m_size=0;
		int length=32;
		int p_size=p.BitCount();
		if( false == ec.ValidateParameters( rng, 3 ) )
		{ throw std::string( "EC Parameter Validation Failure" ); }
		for (int i=0;i<n_ots;i++)
		{
			PKs[0][i] = ec.Multiply(r,PKs[0][i]);
			PKs[1][i] = ec.Subtract(Cr,PKs[0][i]);
			//if(i==0) {cout<<"PKs"<<PKs[1][i].x<<" , "<<PKs[1][i].y<<endl;}
			R[i]= GetRandom(2,p);

			byte* tempbyte = convert_int_to_kbyte(length,PKs[0][i].x);
			hash0.Update(tempbyte,sizeof(byte)*length);
			byte* tempbyte_1 = convert_int_to_kbyte(length,PKs[1][i].x);
			hash1.Update(tempbyte_1,sizeof(byte)*length);

			//cout<<"PKs[0][i].y="<<PKs[0][i].y<<endl;
			//cout<<"afterconver=";
			
			//cout<<"NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN"<<endl;
			//PrintBytes(tempcpy);
			
			byte* tempbyte2 = convert_int_to_kbyte(length,PKs[0][i].y);
			//cout<<"y1"<<endl;PrintBytes(tempbyte2);
			hash0.Update(tempbyte2,sizeof(byte)*length);
			byte* tempbyte2_1 = convert_int_to_kbyte(length,PKs[1][i].y);
			//cout<<"y2"<<endl;PrintBytes(tempbyte2_1);
			hash1.Update(tempbyte2_1,sizeof(byte)*length);

			byte* tempbyte3 = convert_int_to_kbyte(length,R[i]);
			hash0.Update(tempbyte3,sizeof(byte)*length);
			hash1.Update(tempbyte3,sizeof(byte)*length);

			byte* tempbyte4 = convert_int_to_kbyte(length,0);
			byte* tempbyte5 = convert_int_to_kbyte(length,1);
			hash0.Update(tempbyte4,sizeof(byte)*length);
			hash1.Update(tempbyte5,sizeof(byte)*length);

			delete [] tempbyte;
			delete [] tempbyte2;
			delete [] tempbyte3;
			delete [] tempbyte_1;
			delete [] tempbyte2_1;
			delete [] tempbyte4;
			delete [] tempbyte5;
			byte hashed0[CryptoPP::SHA256::DIGESTSIZE];
			hash0.Final(hashed0);
			//if (i==0)
			//{
			//	PrintBytes("hashed0",hashed0,32);
			//}

			
			byte hashed1[CryptoPP::SHA256::DIGESTSIZE];
			hash1.Final(hashed1);

			

			//c_messages[i]=s_messages[i]^hashed;
			s_to_c_messages[0][i]=XOREncrypt(hashed0,s_messages[0][i],32);
			s_to_c_messages[1][i]=XOREncrypt(hashed1,s_messages[1][i],32);
			//if (i==1)
			//{
			//	PrintBytes(hashed0);
			//	PrintBytes(hashed1);
			//}

		}
		//swot.stopTimer();
//		rand_time = (sw_r1+sw_r2+sw_r3);
//		sw_s_mot=swot.getElapsedTime()-rand_time;
		////////////////////////////////////////////////////////////////////
		// Sender to Chooser
		////////////////////////////////////////////////////////////////////
		//sw_r1=0;
		//sw_r2=0;
		//sw_r3=0;
		//swot.startTimer();
		for (int i=0;i<n_ots;i++)
		{

			byte* temp0 = convert_int_to_kbyte(length,c_key[i].x);
			hash_c.Update(temp0,sizeof(byte)*length);
			byte* temp1 = convert_int_to_kbyte(length,c_key[i].y);
			hash_c.Update(temp1,sizeof(byte)*length);


			byte* tempR = convert_int_to_kbyte(length,R[i]);
			hash_c.Update(tempR,sizeof(byte)*length);
			byte* tempi = convert_int_to_kbyte(length,selections[i]);
			hash_c.Update(tempi,sizeof(byte)*length);

			delete [] temp0;
			delete [] temp1;
			delete [] tempR;
			delete [] tempi;
			
			byte hashed_c[CryptoPP::SHA256::DIGESTSIZE];
			hash_c.Final(hashed_c);
			/*if (i==0)
			{
				PrintBytes("hashed_c",hashed_c,32);
			}*/
			//if (i==1)
			//{
			//	PrintBytes(hashed_c);
			//}
			

			//c_messages[i]=s_messages[i]^hashed;
			c_messages[i]=XOREncrypt(hashed_c,s_to_c_messages[selections[i]][i],32);
		}
		//swot.stopTimer();
		//rand_time = (sw_r1+sw_r2+sw_r3);
//		sw_c_mot+=(swot.getElapsedTime()-rand_time);
		delete [] PK[0];
		delete [] PK[1];
		delete [] PKs[0];
		delete [] PKs[1];
		return 0;
    }
  
    catch (CryptoPP::Exception & e)
    {
        cerr << "Crypto++ error: " << e.what () << endl;
        return -3;
    }

    catch (runtime_error & e)
    {
        cerr << "Runtime error: " << e.what() << endl;
        return -2;
    }

    catch (exception & e)
    {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }

}

// This will reduce the message_size to 256 and call the OT_80_256
// 256 = 32 Byte, it will reduce the message size by byte to 32 bytes
int OT_80_m(int n_ots       //number of OTs
	,int message_size       //message_size: number of message bytes
	,byte** s_messages[2]   //The array of messages: messages[n_ots][message_size] of bytes
	,byte* c_messages[]     //The array of chooser's messages (to be filled after protocol)
	,int selections[]       //The choosers selection bits
)
{
	//CStopWatch swm;
	//sw_r1=0;
	//sw_r2=0;
	//sw_r3=0;
	//swm.startTimer();
	byte** S[2];
	byte** c_mes;
	c_mes=new byte*[n_ots];
	S[0] = new byte*[n_ots];
	S[1] = new byte*[n_ots];
	for (int index=0;index<n_ots;index++)
	{
		S[0][index] = GetRandomBytes(32);
		S[1][index] = GetRandomBytes(32);
	}
	byte** y[2];
	y[0]=new byte*[n_ots];
	y[1]=new byte*[n_ots];
	////////////////////////////////////////////////////////////////////
	// Sender Computation
	////////////////////////////////////////////////////////////////////
	for (int index=0;index<n_ots;index++)
	{
		y[0][index] = XOREncrypt(PRG_32_to_m_bytes_hash(S[0][index],message_size),s_messages[0][index],message_size);
		y[1][index] = XOREncrypt(PRG_32_to_m_bytes_hash(S[1][index],message_size),s_messages[1][index],message_size);
	}
//	swm.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	sw_s_temp=swm.getElapsedTime()-rand_time;

	//PrintBytes("S[1][0]",S[1][0]);
//	sw_c_mot=0;
//	sw_s_mot=0;
	OT_80_256(n_ots,32,S,c_mes,selections);
	//sw_s_temp+=sw_s_mot;
	//sw_c_temp+=sw_c_mot;
	//PrintBytes("c_mes[0]",c_mes[0]);
	//PrintBytes("PRG S[1][0],48",PRG_32_to_m_bytes_hash(S[1][0],message_size));
	//PrintBytes("PRG S[1][0],48",PRG_32_to_m_bytes_hash(S[1][0],message_size));
	//PrintBytes("PRG c_mes[0],48",PRG_32_to_m_bytes_hash(c_mes[0],message_size));
	////////////////////////////////////////////////////////////////////
	// Chooser Computation
	////////////////////////////////////////////////////////////////////
	//sw_r1=0;
	//sw_r2=0;
	//sw_r3=0;
//	swm.startTimer();
	for (int index=0;index<n_ots;index++)
	{
		c_messages[index] = XOREncrypt(PRG_32_to_m_bytes_hash(c_mes[index],message_size),y[selections[index]][index],message_size);
	}
	//swm.stopTimer();
	//rand_time = (sw_r1+sw_r2+sw_r3);
	//sw_c_temp+=(swm.getElapsedTime()-rand_time);
	for (int index=0;index<n_ots;index++)
	{
		delete y[0][index];
		delete y[1][index];
		delete S[0][index];
		delete S[1][index];
		delete c_mes[index];
	}
	delete [] y[0];
	delete [] y[1];
	delete [] S[0];
	delete [] S[1];
	delete [] c_mes;
	return 0;
}

int OT_n_m(int n_ots       //number of OTs
	,int message_size       //message_size: number of message bytes
	,byte** s_messages[2]   //The array of messages: messages[n_ots][message_size] of bytes
	,byte* c_messages[]     //The array of chooser's messages (to be filled after protocol)
	,int selections[]       //The choosers selection bits
	
)
{
	// l = message_size*sizeof(byte)
	// m = n_ots > 80
	// k = 80
	const int k = 80;
	CryptoPP::AutoSeededRandomPool rng;
	////////////////////////////////////////////////////////////////////
	// Sender Computation
	////////////////////////////////////////////////////////////////////
	int n_ots_bytes = (int)ceil((double)n_ots/8);
	int m = n_ots_bytes*8; // Is used for OT
	byte* T[k];
	byte* sel = new byte[n_ots_bytes];
	CryptoPP::Integer temp;
	bool flag;
	int cur=0;
	for (int i=0;i<n_ots_bytes;i++)
	{
		temp=0;
		for (int j=0;j<8;j++)
		{
			if (cur<n_ots)
			{
				if (selections[j]==0)
				{
					flag=false;
				}
				else
				{
					flag=true;
				}

				temp.SetBit(j,flag);
				cur++;
			}
			else
			{
				temp.SetBit(j,false);
			}
		}
		sel[i] = temp.GetByte(0);
	}
	// A random vector s\in {0,1}^80
	CryptoPP::Integer s(rng,k);
	// Prepare for 80 OTs of n_ots bits
	byte** S[2]; // 2*80 messages of size n_ots
	byte** s_mes; // 80 messages of size n_ots for output
	s_mes = new byte*[k];
	S[0] = new byte*[k];
	S[1] = new byte*[k];
	//CryptoPP::Integer T[k];
	int s_selects[k];
	cur=0;
	for (int i=0;i<k;i++)
	{
		s_selects[i]=s.GetBit(i);
		T[i] = GetRandomBytes(n_ots_bytes);
		S[0][i] = T[i];
		S[1][i] = XOREncrypt(T[i],sel,n_ots_bytes);
	}
	if (n_ots<256)
	{
		//PrintBytes("S[0][0]",S[0][0],32);
		//sali: server to be checked
		OT_80_256(k,32,S,s_mes,s_selects);
		//PrintBytes("s_mes[0]",s_mes[0],32);
	}
	else
	{
		OT_80_m(k,n_ots_bytes,S,s_mes,s_selects);
	}

	int k_bytes = (int)ceil((double)k/8);
	byte** Q = new byte*[n_ots]; // Q[row][column]
	bool* Qb[k] ; // Qb[column] [row]
	for (int i=0;i<k;i++)
	{
		Qb[i] = new bool[n_ots];
		for (int j=0;j<n_ots_bytes;j++)
		{
			temp=0;
			temp.SetByte(0,s_mes[i][j]);
			for (int index=0;index<8;index++)
			{
				Qb[i][j*8+index]=temp.GetBit(index);
			}
		}
	}
	
	for (int j=0;j<n_ots;j++)  //row
	{
		cur=0;
		Q[j] = new byte[k_bytes];
		// Fill the k_bytes*bytes
		for (int i=0;i<k_bytes;i++)
		{
			temp=0;
			for (int index=0;index<8;index++)
			{
				cur++;
				if (cur<k)
				{
					temp.SetBit(index,Qb[i*8+index][j]);
				}
				else
				{
					temp.SetBit(index,false);
				}
			}
			Q[j][i] = temp.GetByte(0);
		}
	}

	byte** y[2];
	y[0] = new byte*[n_ots];
	y[1] = new byte*[n_ots];
	
	byte *tbyte;
	int length;
	CryptoPP::SHA256 hash0,hash1;
	byte hashed [CryptoPP::SHA256::DIGESTSIZE];
	for (int i=0;i<n_ots;i++)
	{
		tbyte = convert_int_to_kbyte(32,i);
		
		hash0.Update(tbyte,32*sizeof(byte));
		hash0.Update(Q[i],k_bytes*sizeof(byte));
		//if (i==0)
		//{
		//	PrintBytes("Q",Q[i],k_bytes);
		//}
		hash0.Final(hashed);
		y[0][i] = XOREncrypt(PRG_32_to_m_bytes_hash(hashed,message_size),s_messages[0][i],message_size);

		hash0.Restart();

		hash1.Update(tbyte,32*sizeof(byte));
		hash1.Update(XOREncrypt(Q[i],sel,k_bytes),k_bytes*sizeof(byte));
		hash1.Final(hashed);
		y[1][i] = XOREncrypt(PRG_32_to_m_bytes_hash(hashed,message_size),s_messages[1][i],message_size);
		hash1.Restart();
	}

	////////////////////////////////////////////////////////////////////
	// Chooser Computation
	////////////////////////////////////////////////////////////////////
	
	// Convert byte* T[k] to byte* T2[n_ots]
	byte** T2 = new byte*[n_ots];
	bool* Tb[k] ; // Qb[column] [row]
	for (int i=0;i<k;i++)
	{
		Tb[i] = new bool[n_ots];
		for (int j=0;j<n_ots_bytes;j++)
		{
			temp=0;
			temp.SetByte(0,s_mes[i][j]);
			for (int index=0;index<8;index++)
			{
				Tb[i][j*8+index]=temp.GetBit(index);
			}
		}
	}

	for (int j=0;j<n_ots;j++)  //row
	{
		cur=0;
		T2[j] = new byte[k_bytes];
		// Fill the k_bytes*bytes
		for (int i=0;i<k_bytes;i++)
		{
			temp=0;
			for (int index=0;index<8;index++)
			{
				cur++;
				if (cur<k)
				{
					temp.SetBit(index,Tb[i*8+index][j]);
				}
				else
				{
					temp.SetBit(index,false);
				}
			}
			T2[j][i] = temp.GetByte(0);
		}
	}

	byte** ht = new byte*[n_ots];
//	byte** z = new byte*[n_ots];
	for (int i=0;i<n_ots;i++)
	{
		CryptoPP::SHA256 hash;
		tbyte = convert_int_to_kbyte(32,i);
		hash.Update(tbyte,32*sizeof(byte));
		hash.Update(T2[i],k_bytes*sizeof(byte));
		/*if (i==0)
		{
			PrintBytes("T2",T2[i],k_bytes);
		}*/
		hash.Final(hashed);
		ht[i] = PRG_32_to_m_bytes_hash(hashed,message_size);
		/*if (i==0)
		{
			PrintBytes("ht[0]",ht[i],message_size);
		}*/
		c_messages[i] = XOREncrypt(ht[i],y[selections[i]][i],message_size);
	}
	return 0;
}


int OT_80_m_d(int n_ots       //number of OTs
	,int* message_size       //message_size: number of message bytes for each OT
	,byte** s_messages[2]   //The array of messages: messages[n_ots][message_size] of bytes
	,byte* c_messages[]     //The array of chooser's messages (to be filled after protocol)
	,int selections[]       //The choosers selection bits
	
)
{
//	CStopWatch swo;
	//sw_r1=0;
	//sw_r2=0;
	//sw_r3=0;
//	swo.startTimer();
	byte** S[2];
	byte** c_mes;
	c_mes = new byte*[n_ots];
	S[0] = new byte*[n_ots];
	S[1] = new byte*[n_ots];

	for (int index=0;index<n_ots;index++)
	{
		S[0][index] = GetRandomBytes(32);
		S[1][index] = GetRandomBytes(32);
	}
	byte** y[2];
	y[0]=new byte*[n_ots];
	y[1]=new byte*[n_ots];
	////////////////////////////////////////////////////////////////////
	// Sender Computation
	////////////////////////////////////////////////////////////////////
	for (int index=0;index<n_ots;index++)
	{
		y[0][index] = XOREncrypt(PRG_32_to_m_bytes_hash(S[0][index],message_size[index]),s_messages[0][index],message_size[index]);
		y[1][index] = XOREncrypt(PRG_32_to_m_bytes_hash(S[1][index],message_size[index]),s_messages[1][index],message_size[index]);
	}
//	swo.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	sw_s_OT += (swo.getElapsedTime()-rand_time);
	//cout<<"Selection="<<selections[0]<<endl;
	//PrintBytes("S[0][0]",S[0][0],32);
//	sw_c_mot=0;
//	sw_s_mot=0;
	OT_80_256(n_ots,32,S,c_mes,selections);
	//sw_s_OT+=sw_s_mot;
//	sw_c_OT+=sw_c_mot;
	//PrintBytes("c_mes[0]",c_mes[0],32);
	//PrintBytes("PRG S[1][0],48",PRG_32_to_m_bytes_hash(S[1][0],message_size));
	//PrintBytes("PRG S[1][0],48",PRG_32_to_m_bytes_hash(S[1][0],message_size));
	//PrintBytes("PRG c_mes[0],48",PRG_32_to_m_bytes_hash(c_mes[0],message_size));
	////////////////////////////////////////////////////////////////////
	// Chooser Computation
	////////////////////////////////////////////////////////////////////
	//sw_r1=0;
	//sw_r2=0;
	//sw_r3=0;
//	swo.startTimer();
	for (int index=0;index<n_ots;index++)
	{
		c_messages[index] = XOREncrypt(PRG_32_to_m_bytes_hash(c_mes[index],message_size[index]),y[selections[index]][index],message_size[index]);
	}
//	swo.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	sw_c_OT += (swo.getElapsedTime()-rand_time);
	for (int index=0;index<n_ots;index++)
	{
		delete y[0][index];
		delete y[1][index];
		delete S[0][index];
		delete S[1][index];
		delete c_mes[index];
	}
	delete [] y[0];
	delete [] y[1];
	delete [] S[0];
	delete [] S[1];
	delete [] c_mes;

	return 0;
}


int OT_n_m_d(int n_ots       //number of OTs
	,int* message_size       //message_size: number of message bytes
	,byte** s_messages[2]   //The array of messages: messages[n_ots][message_size] of bytes
	,byte* c_messages[]     //The array of chooser's messages (to be filled after protocol)
	,int selections[]       //The choosers selection bits
)
{
//	CStopWatch swishai;
	//sw_r1=0;
	//sw_r2=0;
	//sw_r3=0;
//	swishai.startTimer();
	// l = message_size*sizeof(byte)
	// m = n_ots > 80
	// k = 80

	const int k = 80;
	CryptoPP::AutoSeededRandomPool rng;
	////////////////////////////////////////////////////////////////////
	// Sender Computation
	////////////////////////////////////////////////////////////////////
	int n_ots_bytes = (int)ceil((double)n_ots/8);
	int m = n_ots_bytes*8; // Is used for OT

	byte* T[k];
	int k_bytes = (int)ceil((double)k/8);
	int kk = k_bytes*8;
	byte* sel = new byte[n_ots_bytes];
	CryptoPP::Integer temp;
	bool flag;
	int cur=0;
	for (int i=0;i<n_ots_bytes;i++)
	{
		temp=0;
		for (int j=0;j<8;j++)
		{
			if (cur<n_ots)
			{
				if (selections[i*8+j]==0)
				{
					flag=false;
				}
				else
				{
					flag=true;
				}

				temp.SetBit(j,flag);
				cur++;
			}
			else
			{
				temp.SetBit(j,false);
			}
		}
		sel[i] = temp.GetByte(0);
	}
	
	//PrintBytes("sel",sel,n_ots_bytes);
	//for (int i=0;i<n_ots;i++)
	//{
	//	printf("selection[%d] = %d",i,selections[i]);
	//	cout<<endl;
	//}
	// A random vector s\in {0,1}^80
	//CryptoPP::Integer s(rng,k);
	// Prepare for 80 OTs of n_ots bits

	byte** S[2]; // 2*80 messages of size n_ots
	byte** s_mes; // 80 messages of size n_ots for output
	s_mes = new byte*[k];
	S[0] = new byte*[k];
	S[1] = new byte*[k];
	//CryptoPP::Integer T[k];
	int s_selects[k];
	cur=0;
	for (int i=0;i<k;i++)
	{
		T[i] = GetRandomBytes(n_ots_bytes);
		S[0][i] = T[i];
		S[1][i] = XOREncrypt(T[i],sel,n_ots_bytes);
	}

//	swishai.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	sw_c_OT+=(swishai.getElapsedTime()-rand_time);

	//sw_r1=0;
	//sw_r2=0;
	//sw_r3=0;
//	swishai.startTimer();

	byte* s_bytes = new byte[k_bytes];
	s_bytes = GetRandomBytes(k_bytes);
	cur=0;
	for (int i=0;i<k_bytes;i++)
	{
			temp=0;
			temp.SetByte(0,s_bytes[i]);
			for (int index=0;index<8;index++)
			{
				s_selects[i*8+index]=temp.GetBit(index);
			}
	}
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	swishai.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	sw_s_OT+=(swishai.getElapsedTime()-rand_time);

	if (n_ots<256)
	{
		//PrintBytes("S[0][0]",S[0][0],32);
		//PrintBytes("S[1][0]",S[1][0],32);
		//cout<<"s_select[0] = "<<s_selects[0]<<endl;
	//	sw_c_mot=0;
	//	sw_s_mot=0;
	//sali: jayeh server va client akse
		OT_80_256(k,n_ots_bytes,S,s_mes,s_selects);
	//	sw_s_OT+=sw_c_mot;
	//	sw_c_OT+=sw_s_mot;
		//PrintBytes("s_mes[0]",s_mes[0],32);
	}
	else
	{
		//sw_c_temp=0;sw_s_temp=0;
		//sali: jaye server va client akse
		OT_80_m(k,n_ots_bytes,S,s_mes,s_selects);
		//sw_c_OT+=sw_s_temp;
		//sw_s_OT+=sw_c_temp;
	}
//	sw_r1=0;
//	sw_r2=0;
//	sw_r3=0;
//	swishai.startTimer();

	byte** Q = new byte*[n_ots]; // Q[row][column]
	bool* Qb[k] ; // Qb[column] [row]
	for (int i=0;i<k;i++)
	{
		Qb[i] = new bool[n_ots];
		for (int j=0;j<n_ots_bytes;j++)
		{
			temp=0;
			temp.SetByte(0,s_mes[i][j]);
			for (int index=0;index<8;index++)
			{
				Qb[i][j*8+index]=temp.GetBit(index);
			}
		}
	}

	for (int j=0;j<n_ots;j++)  //row
	{
		cur=0;
		Q[j] = new byte[k_bytes];
		// Fill the k_bytes*bytes
		for (int i=0;i<k_bytes;i++)
		{
			temp=0;
			for (int index=0;index<8;index++)
			{
				cur++;
				if ((i*8+index)<k)
				{
					temp.SetBit(index,Qb[i*8+index][j]);
				}
				else
				{
					temp.SetBit(index,false);
				}
			}
			Q[j][i] = temp.GetByte(0);
		}
	}

	byte** y[2];
	y[0] = new byte*[n_ots];
	y[1] = new byte*[n_ots];

	byte *tbyte;
	int length;
	//byte** te=new byte*[n_ots];
	//byte** te2=new byte*[n_ots];
		
	CryptoPP::SHA256 hash0,hash1;
	byte hashed [CryptoPP::SHA256::DIGESTSIZE];
	for (int i=0;i<n_ots;i++)
	{
		tbyte = convert_int_to_kbyte(32,i);

		hash0.Update(tbyte,32*sizeof(byte));
		hash0.Update(Q[i],k_bytes*sizeof(byte));
		//if (i==0)
		//{
		//	PrintBytes("tbyte",tbyte,32);
		//	PrintBytes("Q",Q[i],k_bytes);
		//}
		hash0.Final(hashed);
		y[0][i] = XOREncrypt(PRG_32_to_m_bytes_hash(hashed,message_size[i]),s_messages[0][i],message_size[i]);
		//te[i] = PRG_32_to_m_bytes_hash(hashed,message_size[i]);
		if (i==0)
		{
			//PrintBytes("1st Hash - 0",hashed,32);
			//te=PRG_32_to_m_bytes_hash(hashed,message_size[i]);
			//PrintBytes("PRG0",te,message_size[i]);
			//PrintBytes("Message",s_messages[0][i],message_size[i]);
			//PrintBytes("y[0][i]=PRG xor message",y[0][i],message_size[i]);
		}

		hash0.Restart();

		hash1.Update(tbyte,32*sizeof(byte));
		hash1.Update(XOREncrypt(Q[i],s_bytes,k_bytes),k_bytes*sizeof(byte));
		hash1.Final(hashed);
		
		if (i==0)
		{
			/*PrintBytes("tbyte",tbyte,32);
			PrintBytes("Q xor s_bytes",XOREncrypt(Q[i],s_bytes,k_bytes),k_bytes);
			CryptoPP::Integer qq;
			qq.SetByte(0,Q[0][0]);
			cout<<"Q[0] bit0 = "<<qq.GetBit(0)<<endl;
			qq.SetByte(0,s_bytes[0]);
			cout<<"s_bytes[0] bit0 = "<<qq.GetBit(0)<<endl;*/
			//PrintBytes("1st Hash - 1",hashed,32);
			//te=PRG_32_to_m_bytes_hash(hashed,message_size[i]);
			//PrintBytes("PRG0",te,message_size[i]);
			//PrintBytes("Message",s_messages[0][i],message_size[i]);
			//PrintBytes("y[0][i]=PRG xor message",y[0][i],message_size[i]);
		}
		y[1][i] = XOREncrypt(PRG_32_to_m_bytes_hash(hashed,message_size[i]),s_messages[1][i],message_size[i]);
		//te2[i] = PRG_32_to_m_bytes_hash(hashed,message_size[i]);
		hash1.Restart();
	}
//	swishai.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	sw_s_OT+=(swishai.getElapsedTime()-rand_time);
	////////////////////////////////////////////////////////////////////
	// Chooser Computation (R)
	////////////////////////////////////////////////////////////////////
//	sw_r1=0;
//	sw_r2=0;
//	sw_r3=0;
//	swishai.startTimer();
	// Convert byte* T[k] to byte* T2[n_ots]
	byte** T2 = new byte*[n_ots];
	bool* Tb[k] ; // Qb[column] [row]
	for (int i=0;i<k;i++)
	{
		Tb[i] = new bool[n_ots];
		for (int j=0;j<n_ots_bytes;j++)
		{
			temp=0;
			temp.SetByte(0,T[i][j]);
			for (int index=0;index<8;index++)
			{
				Tb[i][j*8+index]=temp.GetBit(index);
			}
		}
	}

	for (int j=0;j<n_ots;j++)  //row
	{
		cur=0;
		T2[j] = new byte[k_bytes];
		// Fill the k_bytes*bytes
		for (int i=0;i<k_bytes;i++)
		{
			temp=0;
			for (int index=0;index<8;index++)
			{
				cur++;
				if (i*8+index<k)
				{
					temp.SetBit(index,Tb[i*8+index][j]);
				}
				else
				{
					temp.SetBit(index,false);
				}
			}
			T2[j][i] = temp.GetByte(0);
		}
	}

	byte** ht = new byte*[n_ots];
	//	byte** z = new byte*[n_ots];
	for (int i=0;i<n_ots;i++)
	{
		CryptoPP::SHA256 hash;
		tbyte = convert_int_to_kbyte(32,i);
		hash.Update(tbyte,32*sizeof(byte));
		hash.Update(T2[i],k_bytes*sizeof(byte));
		//if (i==0)
		//{
		//	PrintBytes("tbyte",tbyte,32);
		//	PrintBytes("T2",T2[i],k_bytes);
		//}
		hash.Final(hashed);
		ht[i] = PRG_32_to_m_bytes_hash(hashed,message_size[i]);
		c_messages[i] = XOREncrypt(ht[i],y[selections[i]][i],message_size[i]);
		//if (!CompareBytes(c_messages[i],s_messages[selections[i]][i],message_size[i]))
		//{
		//	PrintBytes("Q",Q[i],k_bytes);
		//	PrintBytes("Q XOR s_bytes",XOREncrypt(Q[i],s_bytes,k_bytes),k_bytes);
		//	PrintBytes("T2",T2[i],k_bytes);
		//	PrintBytes("s",s_bytes,k_bytes);
		//	//for (int index=0;index<k;index++)
		//	//{
		//	//	cout<<"s_select[i]"<<s_selects[index]<<endl;
		//	//}
		//	//PrintBytes("PRG0",te[i],message_size[i]);
		//	//PrintBytes("PRG1",te2[i],message_size[i]);
		//	//PrintBytes("PRG",ht[i],message_size[i]);
		//	//PrintBytes("y[0][i]",y[0][i],message_size[i]);
		//	//PrintBytes("y[1][i]",y[1][i],message_size[i]);
		//	//PrintBytes("c_messages[i]",c_messages[i],message_size[i]);
		//	//PrintBytes("s_messages[0][i]",s_messages[0][i],message_size[i]);
		//	//PrintBytes("s_messages[1][i]",s_messages[1][i],message_size[i]);
		//	//PrintBytes("PRG",XOREncrypt(c_messages[i],y[selections[i]][i],message_size[i]),message_size[i]);
		//	cout<<"Selection="<<selections[i]<<"  i = "<<i<<endl;
		//	cout<<"YES"<<endl;
		//	//PrintBytes("")
		//}
		//if (i==0)
		//{
		//	//PrintBytes("2nd Hash",hashed,32);
		//	PrintBytes("ht[0]",ht[i],message_size[i]);
		//	/*cout<<"Selection="<<selections[i]<<endl;
		//	cout<<"Qb[0][0] = "<<Qb[0][0]<<"    Tb[0][0] = "<<Tb[0][0]<<endl;*/
		//}
		
	}
//	swishai.stopTimer();
//	rand_time = (sw_r1+sw_r2+sw_r3);
//	sw_c_OT+=(swishai.getElapsedTime()-rand_time);

	for (int i=0;i<k;i++)
	{
		delete [] S[0][i];
		delete [] S[1][i];
		
		
		delete [] s_mes[i];
	}
	//delete [] Tb;
	//delete [] Qb;
	delete [] s_mes;
	delete [] S[0];
	delete [] S[1];
	for (int i=0;i<n_ots;i++)
	{
		delete [] Q[i];
		delete [] y[0][i];
		delete [] y[1][i];
		delete [] ht[i];
		delete [] T2[i];
	}
	delete [] Q;
	delete [] y[0];
	delete [] y[1];
	delete [] ht;
	delete [] T2;
	delete [] tbyte;
	delete [] s_bytes;

	return 0;
}


void OT_dynamic_message_size(int n_ots           //number of OTs
	,int* message_size    //message_size: number of message bytes
	,byte** s_messages[] //The array of messages: messages[n_inputs][message_size] of bytes
	,byte* c_messages[]  //The array of chooser's messages (to be filled after protocol)
	,int selections[]    //The choosers selection bits
)
{
	// Alg.1 Reduce the message_size in OT
	// Alg.2 Reduce the number of OTs
	if (n_ots<=80)
	{
		//if ((message_size*8)<=256)
		//{
		//	// call OT{80}{256}
		//	OT_80_256(n_ots,message_size,s_messages,c_messages,selections);

		//} 
		//else
		//{
			//call Alg.1 to reduce message_size to 256
			//call OT{80}{256}
			OT_80_m_d(n_ots,message_size,s_messages,c_messages,selections);
		//}
	} 
	else
	{
		//call Alg.2 to reduce the OTs to 80
		// Call Alg.1 to reduce the message_size
		// call OT{80}{256}
		OT_n_m_d(n_ots,message_size,s_messages,c_messages,selections);
	}
}


void OT_dynamic_256alpha(int n_ots
	,int *message_size
	,byte** s_messages[]
	,byte* c_messages[]
	,int selections[]
	)
{
	
	//Representation of EVV
	//size of EVV = 2^9-1=511
	//left son of index i = 2i+1
	//right son of index i = 2i+2
	//father of index i = floor (i-1)/2
	const int evv_size = 511;
	const int alpha_size = 256;
	// First prepare the keys for all OTs
	// And PADs
	// k[n_ots][n_levels=8][0 or 1][message size]
	//byte**** k = new byte***[n_ots];
	byte** key[2];
	key[0] = new byte*[8*n_ots];
	key[1] = new byte*[8*n_ots];

	// pad[n_ots][node][message bytes]
	byte*** pad = new byte**[n_ots];
	// evv[n_ots][node][left or right][message bytes]
	byte**** evv = new byte***[n_ots];

	for (int i=0;i<n_ots;i++)
	{
		for (int j=0;j<8;j++)
		{
			key[0][i*8+j] = GetRandomBytes(message_size[i]);
			key[1][i*8+j] = GetRandomBytes(message_size[i]);
		}
		pad[i] = new byte*[evv_size];
		evv[i] = new byte**[evv_size];
		for (int j=0;j<evv_size;j++)
		{
			pad[i][j] = GetRandomBytes(message_size[i]);
		}
		// Set evv for non-terminals (0 to 2^8-1)
		int row_number[9] = {1,2,4,8,16,32,64,128,256};
		int index = 0;
		for (int j=0;j<8;j++)
		{
			for (int m=0;m<row_number[j];m++)
			{
				evv[i][index] = new byte*[2];
				evv[i][index][0] = XOREncrypt(XOREncrypt(pad[i][index],key[0][i*8+7-j],message_size[i]),pad[i][index*2+1],message_size[i]);
				evv[i][index][1] = XOREncrypt(XOREncrypt(pad[i][index],key[1][i*8+7-j],message_size[i]),pad[i][index*2+2],message_size[i]);
				index++;
			}
		}
		// Set evv for terminals (2^8 to 2^9-1)
		
		for (int j=255;j<evv_size;j++)
		{
			//byte* alphabaet=convert_int_to_kbyte(message_size[i],(j-256));
			evv[i][j] = new byte*[2];
			evv[i][j][0] = XOREncrypt(pad[i][j],s_messages[j-255][i],message_size[i]);
		}
	}	

	// Do ALL OT2s using Ishai extensions
	int input_size = 8*n_ots;
	byte** c_keys = new byte*[input_size];
	int* message_size2 = new int[input_size];
	// Lets convert each OT256 selection to 8 OT2 selection
	// n_ots*OT256 selections => n_ots*8*OT2 sel
	bitset<8> temp;
	int* sel = new int[8*n_ots];
	for (int i=0;i<n_ots;i++)
	{
		temp = selections[i];
		for (int j=0;j<8;j++)
		{
			sel[i*8+j] = temp[j];
			message_size2[i*8+j] = message_size[i];
		}
	}
	//sali
	//checked 

	OT_dynamic_message_size(input_size,message_size2,key,c_keys,sel);
	//int tester=0;
	//for (int tester=0;tester<input_size;tester++)
	//{
	//	printf("i=%d",tester);
	//	PrintBytes("Should KEY",key[sel[tester]][tester],message_size2[tester]);
	//	PrintBytes("c_Key",c_keys[tester],message_size2[tester]);
	//	cout<<endl<<endl;
	//}
	
	
	// Transfers (for future isolation of code)
	byte** c_pads = new byte*[n_ots];
	for (int i=0;i<n_ots;i++)
	{
		c_pads[i] = new byte[message_size[i]];
		//for (int j=0;j<message_size[i];j++)
		//{
		//	c_pads[i][j] = pad[i][0][j];
		//}
		memcpy(c_pads[i],pad[i][0],message_size[i]);
	}
	PrintBytes("c_pads[0]",c_pads[0],message_size[0]);
	PrintBytes("pad[0][0]",pad[0][0],message_size[0]);
	
	///////////////////////////////////////////////
	// Decryption by client
	///////////////////////////////////////////////
	
	// Set initial node indexes
	for (int i=0;i<n_ots;i++)
	{
		int node = 0;
		// non-terminals
		for (int j=0;j<8;j++)
		{
			if (sel[i*8+7-j]==0)
			{
				c_pads[i] =XOREncrypt(XOREncrypt(c_keys[i*8+7-j],c_pads[i],message_size[i]),evv[i][node][0],message_size[i]);
				node = node*2+1;
			}
			else
			{
				c_pads[i] =XOREncrypt(XOREncrypt(c_keys[i*8+7-j],c_pads[i],message_size[i]),evv[i][node][1],message_size[i]);
				node = node*2+2;
			}
		}
		// below c_pads will hold the result of i-th OT256
		c_messages[i] = XOREncrypt(c_pads[i],evv[i][node][0],message_size[i]);
	}
}




//////////////////////////////////////////////////////////////////////////////
//sali - networking functions
/////////////////////////////////////////////////////////////////////////////

 int OT_80_256_client(int n_ots    //number of OTs
 			,int message_size  //message_size: number of message bytes
 			,byte* c_messages[]  //The array of chooser's messages (to be filled after protocol)
 			,int selections[] //The choosers selection bits
 		)
 {
	cout << "OT_80_256_client Computations Started" << endl ;
	try
    {
		CStopWatch swot;
//		CStopWatch swot_comm;
		
		//if (message_size<32)
		//{
		//	for (int ot_number=0;ot_number<n_ots;ot_number++)
		//	{
		//		for (int index=message_size;index<32;index++)
		//		{
		//			s_messages[0][ot_number][index]='\0';
		//			s_messages[1][ot_number][index]='\0';
		//		}
		//	}
		//}
		//PUBLIC
		///////////////////////////////////////////////////////////////////////////////
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
		///////////////////////////////////////////////////////////////////////////////
		//END-PUBLIC

//		CryptoPP::Integer r;  // The random number selected by sender
		
//		CryptoPP::ECPPoint CP,Cr; // The constant points C
		CryptoPP::ECPPoint CP; // The constant points C sent by sender
		CryptoPP::ECPPoint gr; // The public key of sender
		CryptoPP::ECPPoint *PK[2]; //PK calculated by chooser
//		CryptoPP::ECPPoint *PKs[2]; //PK calculated by chooser
		for (int al=0;al<2;al++)
			PK[al]=new CryptoPP::ECPPoint[n_ots];

		CryptoPP::AutoSeededRandomPool rng;
		CryptoPP::ECP ec( p, a, b );
		if( false == ec.ValidateParameters( rng, 3 ) )
		{ throw std::string( "EC Parameter Validation Failure" ); }

		//**receive CP and gr
		char buffer[ec.EncodedPointSize()];
		myrecv(buffer,sizeof(buffer));
		ec.DecodePoint(CP,(const byte *)buffer,sizeof(buffer));

		myrecv(buffer,sizeof(buffer));
		ec.DecodePoint(gr,(const byte *)buffer,sizeof(buffer));
		
		CryptoPP::Integer *k=new CryptoPP::Integer[n_ots]; // Random number selected by chooser for each query
		CryptoPP::ECPPoint *c_key=new CryptoPP::ECPPoint[n_ots];
		//CryptoPP::ECPPoint testCP;
		
		////////////////////////////////////////////////////////////////////
		// Chooser Query Computation
		////////////////////////////////////////////////////////////////////
		
		sw_r1=0;
		sw_r2=0;
		sw_r3=0;
		swot.startTimer();
		for (int i=0;i<n_ots;i++)
		{
			k[i] = GetRandom(2,p);
			c_key[i]=ec.Multiply(k[i],gr);
			//if(i==0) {cout<<"CKey"<<c_key[i].x<<" , "<<c_key[i].y<<endl;}
			PK[selections[i]][i] = ec.Multiply(k[i],G);
			if (1==selections[i])
			{
				PK[0][i] = ec.Subtract(CP,PK[selections[i]][i]);
				//testCP=ec.Add(PK[0][i],PK[1][i]);
				//cout<<"PK0"<<endl<<PK[0][i].x<<" , "<<PK[0][i].y<<endl;
				//cout<<"PK1"<<endl<<PK[1][i].x<<" , "<<PK[1][i].y<<endl;
			}
			
		}

		swot.stopTimer();
		rand_time = (sw_r1+sw_r2+sw_r3);
		sw_c_mot=swot.getElapsedTime()-rand_time;
		////////////////////////////////////////////////////////////////////
		// Chooser to Sender
		////////////////////////////////////////////////////////////////////

		for (int i=0;i<n_ots;i++)
		{
			//PKs[0][i]=PK[0][i];
			ec.EncodePoint((byte*)buffer,PK[0][i],false);
			mysend(buffer,sizeof(buffer));			
//			cout << PK[0][i].x << endl;
		}


		CryptoPP::ECPPoint temp;
		CryptoPP::Integer *R=new CryptoPP::Integer[n_ots];
		CryptoPP::SHA256 hash0,hash1,hash_c;
		
		byte** s_to_c_messages[2];
		for (int al=0;al<2;al++)
		{
			s_to_c_messages[al]=new byte*[n_ots];
		}
		
		int m_size=0;
		int length=32;
		int p_size=p.BitCount();
		
		CryptoPP::Integer aa[5];//={1,2,3,4,5};
		
		
		byte* buffer2 = new byte[100];
		//cout << "(R[1].MinEncodedSize()): " << (R[1].MinEncodedSize()) << endl;

		int tmplen;
		//Receiviing s_to_c array
		for (int i=0;i<n_ots;i++)
		{
			
			s_to_c_messages[0][i] = new byte[length];
			s_to_c_messages[1][i] = new byte[length];
			
			myrecv(s_to_c_messages[0][i],length);
			myrecv(s_to_c_messages[1][i],length);
			
			myrecv(&tmplen,sizeof(int));
			myrecv(buffer2,tmplen);
			
			R[i].Decode(buffer2,tmplen);
			
//			cout <<"R[i] is: " << R[i] << endl;
//			PrintBytes("s_to_c_messages[0][i]", s_to_c_messages[0][i],length);
//			PrintBytes("s_to_c_messages[1][i]", s_to_c_messages[1][i],length);

		}


		//Client: Computing Final Result
		sw_r1=0;
		sw_r2=0;
		sw_r3=0;
		swot.startTimer();
	
	
		for (int i=0;i<n_ots;i++)
		{

			byte* temp0 = convert_int_to_kbyte(length,c_key[i].x);
			hash_c.Update(temp0,sizeof(byte)*length);
			byte* temp1 = convert_int_to_kbyte(length,c_key[i].y);
			hash_c.Update(temp1,sizeof(byte)*length);


			byte* tempR = convert_int_to_kbyte(length,R[i]);
			hash_c.Update(tempR,sizeof(byte)*length);
			byte* tempi = convert_int_to_kbyte(length,selections[i]);
			hash_c.Update(tempi,sizeof(byte)*length);

			delete [] temp0;
			delete [] temp1;
			delete [] tempR;
			delete [] tempi;
			
			byte hashed_c[CryptoPP::SHA256::DIGESTSIZE];
			hash_c.Final(hashed_c);
			/*if (i==0)
			{
				PrintBytes("hashed_c",hashed_c,32);
			}*/
			//if (i==1)
			//{
			//	PrintBytes(hashed_c);
			//}
			

			//c_messages[i]=s_messages[i]^hashed;
			c_messages[i]=XOREncrypt(hashed_c,s_to_c_messages[selections[i]][i],32);
		}

		swot.stopTimer();
		rand_time = (sw_r1+sw_r2+sw_r3);
		sw_c_mot+=(swot.getElapsedTime()-rand_time);
		
		//Client: Clean-up		
		delete [] PK[0];
		delete [] PK[1];
		cout << "OT_80_256_client Computations finished" << endl ;
		return 0;		
	 }	
	 catch (CryptoPP::Exception & e)
    {
        cerr << "Crypto++ error: " << e.what () << endl;
        return -3;
    }

    catch (runtime_error & e)
    {
        cerr << "Runtime error: " << e.what() << endl;
        return -2;
    }

    catch (exception & e)
    {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }

}

int OT_80_256_server(int n_ots    //number of OTs
			,int message_size  //message_size: number of message bytes
			,byte** s_messages[2]  //The array of messages: messages[n_ots][message_size] of bytes
		)
{
	cout << "OT_80_256_server Computations Started" << endl ;
	try
    {
		CStopWatch swot;

		//if (message_size<32)
		//{
		//	for (int ot_number=0;ot_number<n_ots;ot_number++)
		//	{
		//		for (int index=message_size;index<32;index++)
		//		{
		//			s_messages[0][ot_number][index]='\0';
		//			s_messages[1][ot_number][index]='\0';
		//		}
		//	}
		//}
		//PUBLIC
		///////////////////////////////////////////////////////////////////////////////
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
		///////////////////////////////////////////////////////////////////////////////
		//END-PUBLIC
		
		CryptoPP::Integer r;  // The random number selected by sender
		CryptoPP::ECPPoint CP,Cr; // The constant points C selected by sender
		CryptoPP::ECPPoint gr; // The public key of sender
		CryptoPP::ECPPoint *PKs[2]; //PK calculated by chooser
		for (int al=0;al<2;al++)
			PKs[al]=new CryptoPP::ECPPoint[n_ots];

		CryptoPP::AutoSeededRandomPool rng;
		CryptoPP::ECP ec( p, a, b );
		if( false == ec.ValidateParameters( rng, 3 ) )
		{ throw std::string( "EC Parameter Validation Failure" ); }
		// Pre computations and Public Info (gr and C)
		CP = ec.Multiply(GetRandom(2,p),G);
		//cout<<"CP"<<endl<<CP.x<<" , "<<CP.y<<endl;
		r = GetRandom(2,p);
		//cout<<"r="<<r<<endl;
		gr = ec.Multiply(r,G);
		//cout<<"gr"<<gr.x<<" , "<<gr.y<<endl;
		Cr = ec.Multiply(r,CP);
		//cout<<"Cr"<<Cr.x<<" , "<<Cr.y<<endl;
//		CryptoPP::Integer *k=new CryptoPP::Integer[n_ots]; // Random number selected by chooser for each query
//		CryptoPP::ECPPoint *c_key=new CryptoPP::ECPPoint[n_ots];
		CryptoPP::ECPPoint testCP;
		
		//sending CP and gr
		byte buffer[ec.EncodedPointSize()];
		ec.EncodePoint((byte*)buffer,CP,false);
		mysend(buffer,sizeof(buffer));
		
		ec.EncodePoint((byte*)buffer,gr,false);
		mysend(buffer,sizeof(buffer));
		//cout << CP.x << endl << CP.y << endl;		


/* debug
		cout << "gr.x" << gr.x <<endl;	
		cout << "gr.y" << gr.y <<endl;
		
		cout << "CP.x" << gr.x <<endl;	
		cout << "CP.y" << gr.y <<endl;
*/

		//Receiving PKs0
		for (int i=0;i<n_ots;i++)
		{
			//PKs[0][i]=PK[0][i];
			myrecv(buffer,sizeof(buffer));
			ec.DecodePoint(PKs[0][i],(const byte *)buffer,sizeof(buffer));
			//cout << PKs[0][i].x << endl;
		}

		///////////////////////////////////////////////////////////
		CryptoPP::ECPPoint temp;
		CryptoPP::Integer *R=new CryptoPP::Integer[n_ots];
		CryptoPP::SHA256 hash0,hash1,hash_c;

		byte** s_to_c_messages[2];
		for (int al=0;al<2;al++)
			s_to_c_messages[al]=new byte*[n_ots];

		int m_size=0;
		int length=32;
		int p_size=p.BitCount();

		////////////////////////////////////////////////////////////////////
		// Server Response Computation
		////////////////////////////////////////////////////////////////////
		
		sw_r1=0;
		sw_r2=0;
		sw_r3=0;
		swot.startTimer();
		if( false == ec.ValidateParameters( rng, 3 ) )
		{ throw std::string( "EC Parameter Validation Failure" ); }



		byte* buffer2 = new byte[100];
		
//		cout << "(R[1].MinEncodedSize()): " << (R[1].MinEncodedSize()) << endl;
		int tmplen;
		//CryptoPP::Integer aa=23762364;
		for (int i=0;i<n_ots;i++)
		{
			PKs[0][i] = ec.Multiply(r,PKs[0][i]);
			PKs[1][i] = ec.Subtract(Cr,PKs[0][i]);
			//if(i==0) {cout<<"PKs"<<PKs[1][i].x<<" , "<<PKs[1][i].y<<endl;}
			R[i]= GetRandom(2,p);

			byte* tempbyte = convert_int_to_kbyte(length,PKs[0][i].x);
			hash0.Update(tempbyte,sizeof(byte)*length);
			byte* tempbyte_1 = convert_int_to_kbyte(length,PKs[1][i].x);
			hash1.Update(tempbyte_1,sizeof(byte)*length);

			//cout<<"PKs[0][i].y="<<PKs[0][i].y<<endl;
			//cout<<"afterconver=";
			
			//cout<<"NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN"<<endl;
			//PrintBytes(tempcpy);
			
			byte* tempbyte2 = convert_int_to_kbyte(length,PKs[0][i].y);
			//cout<<"y1"<<endl;PrintBytes(tempbyte2);
			hash0.Update(tempbyte2,sizeof(byte)*length);
			byte* tempbyte2_1 = convert_int_to_kbyte(length,PKs[1][i].y);
			//cout<<"y2"<<endl;PrintBytes(tempbyte2_1);
			hash1.Update(tempbyte2_1,sizeof(byte)*length);

			byte* tempbyte3 = convert_int_to_kbyte(length,R[i]);
			hash0.Update(tempbyte3,sizeof(byte)*length);
			hash1.Update(tempbyte3,sizeof(byte)*length);

			byte* tempbyte4 = convert_int_to_kbyte(length,0);
			byte* tempbyte5 = convert_int_to_kbyte(length,1);
			hash0.Update(tempbyte4,sizeof(byte)*length);
			hash1.Update(tempbyte5,sizeof(byte)*length);

			delete [] tempbyte;
			delete [] tempbyte2;
			delete [] tempbyte3;
			delete [] tempbyte_1;
			delete [] tempbyte2_1;
			delete [] tempbyte4;
			delete [] tempbyte5;
			byte hashed0[CryptoPP::SHA256::DIGESTSIZE];
			hash0.Final(hashed0);
			//if (i==0)
			//{
			//	PrintBytes("hashed0",hashed0,32);
			//}

			
			byte hashed1[CryptoPP::SHA256::DIGESTSIZE];
			hash1.Final(hashed1);

			

			//c_messages[i]=s_messages[i]^hashed;
			s_to_c_messages[0][i]=XOREncrypt(hashed0,s_messages[0][i],length);
			s_to_c_messages[1][i]=XOREncrypt(hashed1,s_messages[1][i],length);

	//timing
			swot.stopTimer();
			rand_time = (sw_r1+sw_r2+sw_r3);
			sw_s_mot+=swot.getElapsedTime()-rand_time;

			//socket
			//server to client: s_to_c_messages[]


			mysend(s_to_c_messages[0][i],length);
			mysend(s_to_c_messages[1][i],length);
		
			tmplen = R[i].MinEncodedSize();
			mysend(&tmplen,sizeof(int));
						
			R[i].Encode(buffer2,tmplen);
			mysend(buffer2,tmplen);
			//end-socket

	//timing
			sw_r1=0;
			sw_r2=0;
			sw_r3=0;
			swot.startTimer();


			//cout <<"R[i] is: " << R[i] << endl;
			
			//cout << "min encode: " << R[i].MinEncodedSize() << endl;
			//PrintBytes("s_to_c_messages[0][i]", s_to_c_messages[0][i],length);
			//PrintBytes("s_to_c_messages[1][i]", s_to_c_messages[1][i],length);

			//if (i==1)
			//{
			//	PrintBytes(hashed0);
			//	PrintBytes(hashed1);
			//}

		}
		swot.stopTimer();
		rand_time = (sw_r1+sw_r2+sw_r3);
		sw_s_mot+=swot.getElapsedTime()-rand_time;
	
		//Server: Clean-up
		delete [] PKs[0];
		delete [] PKs[1];
	}
    catch (CryptoPP::Exception & e)
    {
        cerr << "Crypto++ error: " << e.what () << endl;
        return -3;
    }

    catch (runtime_error & e)
    {
        cerr << "Runtime error: " << e.what() << endl;
        return -2;
    }

    catch (exception & e)
    {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }
    
    cout << "OT_80_256_server Computations Finished" << endl ;
}


int OT_80_m_d_server(int n_ots       //number of OTs
	,int* message_size       //message_size: number of message bytes for each OT
	,byte** s_messages[2]   //The array of messages: messages[n_ots][message_size] of bytes
)
{
	cout << "OT_80_m_d_server() Computations started." << endl;
	CStopWatch swo;
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	swo.startTimer();
	byte** S[2];

	S[0] = new byte*[n_ots];
	S[1] = new byte*[n_ots];

	for (int index=0;index<n_ots;index++)
	{
		S[0][index] = GetRandomBytes(32);
		S[1][index] = GetRandomBytes(32);
	}
	byte** y[2];
	y[0]=new byte*[n_ots];
	y[1]=new byte*[n_ots];
	
	////////////////////////////////////////////////////////////////////
	// Sender Computation
	////////////////////////////////////////////////////////////////////
	for (int index=0;index<n_ots;index++)
	{
		y[0][index] = XOREncrypt(PRG_32_to_m_bytes_hash(S[0][index],message_size[index]),s_messages[0][index],message_size[index]);
		y[1][index] = XOREncrypt(PRG_32_to_m_bytes_hash(S[1][index],message_size[index]),s_messages[1][index],message_size[index]);
//		PrintBytes (" y[0][index]: " ,y[0][index], message_size[index]);
//		PrintBytes (" y[1][index]: " ,y[1][index], message_size[index]);
		//sali-socket: sending y[] to client
//timing
		swo.stopTimer();
		rand_time = (sw_r1+sw_r2+sw_r3);
		sw_s_OT += (swo.getElapsedTime()-rand_time);

		mysend(y[0][index],message_size[index]);
		mysend(y[1][index],message_size[index]);

		sw_r1=0;
		sw_r2=0;
		sw_r3=0;
		swo.startTimer();		
	}
	swo.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
	sw_s_OT += (swo.getElapsedTime()-rand_time);
	//cout<<"Selection="<<selections[0]<<endl;
	//PrintBytes("S[0][0]",S[0][0],32);
	//sw_c_mot=0;
	sw_s_mot=0;

	OT_80_256_server(n_ots,32,S);

	sw_s_OT+=sw_s_mot;
	//sw_c_OT+=sw_c_mot;
	//PrintBytes("c_mes[0]",c_mes[0],32);
	//PrintBytes("PRG S[1][0],48",PRG_32_to_m_bytes_hash(S[1][0],message_size));
	//PrintBytes("PRG S[1][0],48",PRG_32_to_m_bytes_hash(S[1][0],message_size));
	//PrintBytes("PRG c_mes[0],48",PRG_32_to_m_bytes_hash(c_mes[0],message_size));

	for (int index=0;index<n_ots;index++)
	{
		delete y[0][index];
		delete y[1][index];
		delete S[0][index];
		delete S[1][index];
	}
	delete [] y[0];
	delete [] y[1];
	delete [] S[0];
	delete [] S[1];

	cout << "OT_80_m_d_server Computations finished." << endl;
	return 0;
}

int OT_80_m_d_client(int n_ots       //number of OTs
	,int* message_size       //message_size: number of message bytes for each OT
	,byte* c_messages[]     //The array of chooser's messages (to be filled after protocol)
	,int selections[]       //The choosers selection bits
)
{
	cout << "OT_80_m_d_client computations started." <<endl;
	CStopWatch swo;
	byte** c_mes;
	c_mes = new byte*[n_ots];

	//sali- socket
	byte** y[2];
	y[0]=new byte*[n_ots];
	y[1]=new byte*[n_ots];

	for (int index=0;index<n_ots;index++)
	{
		//sali-socket: receving y[] from client
		y[0][index] = new byte[message_size[index]];
		y[1][index] = new byte[message_size[index]];
	//	cout << "message_size[index] client" << message_size[index] << endl;		
		myrecv(y[0][index],message_size[index]);
		myrecv(y[1][index],message_size[index]);
	//	PrintBytes (" y[0][index]: " ,y[0][index], message_size[index]);
	//	PrintBytes (" y[1][index]: " ,y[1][index], message_size[index]);
	}
	//

	sw_c_mot=0;
	OT_80_256_client(n_ots,32,c_mes,selections);
	sw_c_OT+=sw_c_mot;
	//PrintBytes("c_mes[0]",c_mes[0],32);
	//PrintBytes("PRG S[1][0],48",PRG_32_to_m_bytes_hash(S[1][0],message_size));
	//PrintBytes("PRG S[1][0],48",PRG_32_to_m_bytes_hash(S[1][0],message_size));
	//PrintBytes("PRG c_mes[0],48",PRG_32_to_m_bytes_hash(c_mes[0],message_size));
	
	
	////////////////////////////////////////////////////////////////////
	// Chooser Computation
	////////////////////////////////////////////////////////////////////
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	swo.startTimer();
	for (int index=0;index<n_ots;index++)
	{
		c_messages[index] = XOREncrypt(PRG_32_to_m_bytes_hash(c_mes[index],message_size[index]),y[selections[index]][index],message_size[index]);
	}
	swo.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
	sw_c_OT += (swo.getElapsedTime()-rand_time);
	for (int index=0;index<n_ots;index++)
	{
		delete y[0][index];
		delete y[1][index];
		delete c_mes[index];
	}
	delete [] y[0];
	delete [] y[1];
	delete [] c_mes;

	cout << "OT_80_m_d_client computations finished." <<endl;
	return 0;
}

int OT_n_m_d_server(int n_ots       //number of OTs
	,int* message_size       //message_size: number of message bytes
	,byte** s_messages[2]   //The array of messages: messages[n_ots][message_size] of bytes
)
{
	cout << "OT_n_m_d_server Computations started." << endl;
	CStopWatch swishai;
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	swishai.startTimer();
	// l = message_size*sizeof(byte)
	// m = n_ots > 80
	// k = 80
	const int k = 80;
	CryptoPP::AutoSeededRandomPool rng;

	int n_ots_bytes = (int)ceil((double)n_ots/8);
	int m = n_ots_bytes*8; // Is used for OT

	byte* T[k];
	int k_bytes = (int)ceil((double)k/8);
	int kk = k_bytes*8;
	//byte* sel = new byte[n_ots_bytes];
	CryptoPP::Integer temp;
	bool flag;
	int cur=0;
	int s_selects[k];
	byte** s_mes; // 80 messages of size n_ots for output
	s_mes = new byte*[k];
	////////////////////////////////////////////////////////////////////
	// Sender Computation
	////////////////////////////////////////////////////////////////////

	byte* s_bytes = new byte[k_bytes];
	s_bytes = GetRandomBytes(k_bytes);
	cur=0;
	for (int i=0;i<k_bytes;i++)
	{
			temp=0;
			temp.SetByte(0,s_bytes[i]);
			for (int index=0;index<8;index++)
			{
				s_selects[i*8+index]=temp.GetBit(index);
			}
	}
	rand_time = (sw_r1+sw_r2+sw_r3);
	swishai.stopTimer();

	sw_s_OT+=(swishai.getElapsedTime()-rand_time);

	if (n_ots<256)
	{
		//PrintBytes("S[0][0]",S[0][0],32);
		//PrintBytes("S[1][0]",S[1][0],32);
		//cout<<"s_select[0] = "<<s_selects[0]<<endl;
		sw_c_mot=0;
//		cout << "n_ots_bytes is: " << n_ots_bytes << endl;
		OT_80_256_client(k,n_ots_bytes,s_mes,s_selects);
		sw_s_OT+=sw_c_mot;

		//PrintBytes("s_mes[0]",s_mes[0],32);
	}
	else
	{
		//sw_c_temp=0;
		cout << "sw_c_OT is: " << sw_c_OT  << endl;
		OT_80_m_client(k,n_ots_bytes,s_mes,s_selects);
		sw_s_OT+=sw_c_OT;
		sw_c_OT = 0;
	}


	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	swishai.startTimer();

	byte** Q = new byte*[n_ots]; // Q[row][column]
	bool* Qb[k] ; // Qb[column] [row]
	for (int i=0;i<k;i++)
	{
		Qb[i] = new bool[n_ots];
		for (int j=0;j<n_ots_bytes;j++)
		{
			temp=0;
			temp.SetByte(0,s_mes[i][j]);
			for (int index=0;index<8;index++)
			{
				Qb[i][j*8+index]=temp.GetBit(index);
			}
		}
	}

	for (int j=0;j<n_ots;j++)  //row
	{
		cur=0;
		Q[j] = new byte[k_bytes];
		// Fill the k_bytes*bytes
		for (int i=0;i<k_bytes;i++)
		{
			temp=0;
			for (int index=0;index<8;index++)
			{
				cur++;
				if ((i*8+index)<k)
				{
					temp.SetBit(index,Qb[i*8+index][j]);
				}
				else
				{
					temp.SetBit(index,false);
				}
			}
			Q[j][i] = temp.GetByte(0);
		}
	}

	byte** y[2];
	y[0] = new byte*[n_ots];
	y[1] = new byte*[n_ots];

	byte *tbyte;
	int length;
	//byte** te=new byte*[n_ots];
	//byte** te2=new byte*[n_ots];
		
	CryptoPP::SHA256 hash0,hash1;
	byte hashed [CryptoPP::SHA256::DIGESTSIZE];
	for (int i=0;i<n_ots;i++)
	{
		tbyte = convert_int_to_kbyte(32,i);

		hash0.Update(tbyte,32*sizeof(byte));
		hash0.Update(Q[i],k_bytes*sizeof(byte));
		//if (i==0)
		//{
		//	PrintBytes("tbyte",tbyte,32);
		//	PrintBytes("Q",Q[i],k_bytes);
		//}
		hash0.Final(hashed);
		y[0][i] = XOREncrypt(PRG_32_to_m_bytes_hash(hashed,message_size[i]),s_messages[0][i],message_size[i]);
		//te[i] = PRG_32_to_m_bytes_hash(hashed,message_size[i]);
		if (i==0)
		{
			//PrintBytes("1st Hash - 0",hashed,32);
			//te=PRG_32_to_m_bytes_hash(hashed,message_size[i]);
			//PrintBytes("PRG0",te,message_size[i]);
			//PrintBytes("Message",s_messages[0][i],message_size[i]);
			//PrintBytes("y[0][i]=PRG xor message",y[0][i],message_size[i]);
		}

		hash0.Restart();

		hash1.Update(tbyte,32*sizeof(byte));
		hash1.Update(XOREncrypt(Q[i],s_bytes,k_bytes),k_bytes*sizeof(byte));
		hash1.Final(hashed);
		
		if (i==0)
		{
			/*PrintBytes("tbyte",tbyte,32);
			PrintBytes("Q xor s_bytes",XOREncrypt(Q[i],s_bytes,k_bytes),k_bytes);
			CryptoPP::Integer qq;
			qq.SetByte(0,Q[0][0]);
			cout<<"Q[0] bit0 = "<<qq.GetBit(0)<<endl;
			qq.SetByte(0,s_bytes[0]);
			cout<<"s_bytes[0] bit0 = "<<qq.GetBit(0)<<endl;*/
			//PrintBytes("1st Hash - 1",hashed,32);
			//te=PRG_32_to_m_bytes_hash(hashed,message_size[i]);
			//PrintBytes("PRG0",te,message_size[i]);
			//PrintBytes("Message",s_messages[0][i],message_size[i]);
			//PrintBytes("y[0][i]=PRG xor message",y[0][i],message_size[i]);
		}
		y[1][i] = XOREncrypt(PRG_32_to_m_bytes_hash(hashed,message_size[i]),s_messages[1][i],message_size[i]);
		//te2[i] = PRG_32_to_m_bytes_hash(hashed,message_size[i]);

//timing
		swishai.stopTimer();
		rand_time = (sw_r1+sw_r2+sw_r3);
		sw_s_OT+=(swishai.getElapsedTime()-rand_time);

		//sali-socket
		mysend(y[0][i],message_size[i]);
		mysend(y[1][i],message_size[i]);
		//
		swishai.startTimer();
		hash1.Restart();
	}
	swishai.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
	sw_s_OT+=(swishai.getElapsedTime()-rand_time);

	
	for (int i=0;i<k;i++)
		delete [] s_mes[i];

	delete [] s_mes;

	for (int i=0;i<n_ots;i++)
	{
		delete [] Q[i];
		delete [] y[0][i];
		delete [] y[1][i];
	}
	delete [] Q;
	delete [] y[0];
	delete [] y[1];
	delete [] tbyte;
	delete [] s_bytes;

	cout << "OT_n_m_d_server Computations finished." << endl;
	return 0;
}

int OT_n_m_d_client(int n_ots       //number of OTs
	,int* message_size       //message_size: number of message bytes
	,byte* c_messages[]     //The array of chooser's messages (to be filled after protocol)
	,int selections[]       //The choosers selection bits
)
{
	cout << "OT_n_m_d_client Computations started." << endl;
	CStopWatch swishai;
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	swishai.startTimer();
	// l = message_size*sizeof(byte)
	// m = n_ots > 80
	// k = 80
	const int k = 80;
	CryptoPP::AutoSeededRandomPool rng;
	////////////////////////////////////////////////////////////////////
	// Choose Computation
	////////////////////////////////////////////////////////////////////
	int n_ots_bytes = (int)ceil((double)n_ots/8);
	int m = n_ots_bytes*8; // Is used for OT

	byte* T[k];
	int k_bytes = (int)ceil((double)k/8);
	int kk = k_bytes*8;
	byte* sel = new byte[n_ots_bytes];
	CryptoPP::Integer temp;
	bool flag;
	int cur=0;
	for (int i=0;i<n_ots_bytes;i++)
	{
		temp=0;
		for (int j=0;j<8;j++)
		{
			if (cur<n_ots)
			{
				if (selections[i*8+j]==0)
				{
					flag=false;
				}
				else
				{
					flag=true;
				}

				temp.SetBit(j,flag);
				cur++;
			}
			else
			{
				temp.SetBit(j,false);
			}
		}
		sel[i] = temp.GetByte(0);
	}
	
	//PrintBytes("sel",sel,n_ots_bytes);
	//for (int i=0;i<n_ots;i++)
	//{
	//	printf("selection[%d] = %d",i,selections[i]);
	//	cout<<endl;
	//}
	// A random vector s\in {0,1}^80
	//CryptoPP::Integer s(rng,k);
	// Prepare for 80 OTs of n_ots bits

	byte** S[2]; // 2*80 messages of size n_ots
	byte** s_mes; // 80 messages of size n_ots for output
	s_mes = new byte*[k];
	S[0] = new byte*[k];
	S[1] = new byte*[k];
	//CryptoPP::Integer T[k];
	int s_selects[k];
	cur=0;
	for (int i=0;i<k;i++)
	{
		T[i] = GetRandomBytes(n_ots_bytes);
		S[0][i] = T[i];
		S[1][i] = XOREncrypt(T[i],sel,n_ots_bytes);
	}

	swishai.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
	sw_c_OT+=(swishai.getElapsedTime()-rand_time);

	if (n_ots<256)
	{
		//PrintBytes("S[0][0]",S[0][0],32);
		//PrintBytes("S[1][0]",S[1][0],32);
		//cout<<"s_select[0] = "<<s_selects[0]<<endl;
		sw_s_mot=0;
	//sali: jayeh server va client akse
		OT_80_256_server(k,n_ots_bytes,S);
		sw_c_OT+=sw_s_mot;
		sw_s_mot = 0;
		//PrintBytes("s_mes[0]",s_mes[0],32);
	}
	else
	{
		//sw_s_mot=0;
		//sali: jaye server va client akse
		cout << "sw_s_OT is: " << sw_s_OT <<endl;
		OT_80_m_server(k,n_ots_bytes,S);
		sw_c_OT+=sw_s_OT;
		sw_s_OT = 0;
	}

	byte** y[2];
	y[0] = new byte*[n_ots];
	y[1] = new byte*[n_ots];

	byte *tbyte;
	int length;
	//byte** te=new byte*[n_ots];
	//byte** te2=new byte*[n_ots];
		
	CryptoPP::SHA256 hash0,hash1;
	byte hashed [CryptoPP::SHA256::DIGESTSIZE];
	
	//sali-socket
	for (int i=0;i<n_ots;i++)
	{
		y[0][i] = new byte[message_size[i]];
		y[1][i] = new byte[message_size[i]];
		myrecv(y[0][i],message_size[i]);
		myrecv(y[1][i],message_size[i]);
	}

	////////////////////////////////////////////////////////////////////
	// Chooser Computation (R)
	////////////////////////////////////////////////////////////////////
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	swishai.startTimer();
	// Convert byte* T[k] to byte* T2[n_ots]
	byte** T2 = new byte*[n_ots];
	bool* Tb[k] ; // Qb[column] [row]
	for (int i=0;i<k;i++)
	{
		Tb[i] = new bool[n_ots];
		for (int j=0;j<n_ots_bytes;j++)
		{
			temp=0;
			temp.SetByte(0,T[i][j]);
			for (int index=0;index<8;index++)
			{
				Tb[i][j*8+index]=temp.GetBit(index);
			}
		}
	}

	for (int j=0;j<n_ots;j++)  //row
	{
		cur=0;
		T2[j] = new byte[k_bytes];
		// Fill the k_bytes*bytes
		for (int i=0;i<k_bytes;i++)
		{
			temp=0;
			for (int index=0;index<8;index++)
			{
				cur++;
				if (i*8+index<k)
				{
					temp.SetBit(index,Tb[i*8+index][j]);
				}
				else
				{
					temp.SetBit(index,false);
				}
			}
			T2[j][i] = temp.GetByte(0);
		}
	}

	byte** ht = new byte*[n_ots];
	//	byte** z = new byte*[n_ots];
	for (int i=0;i<n_ots;i++)
	{
		CryptoPP::SHA256 hash;
		tbyte = convert_int_to_kbyte(32,i);
		hash.Update(tbyte,32*sizeof(byte));
		hash.Update(T2[i],k_bytes*sizeof(byte));
		//if (i==0)
		//{
		//	PrintBytes("tbyte",tbyte,32);
		//	PrintBytes("T2",T2[i],k_bytes);
		//}
		hash.Final(hashed);
		ht[i] = PRG_32_to_m_bytes_hash(hashed,message_size[i]);
		c_messages[i] = XOREncrypt(ht[i],y[selections[i]][i],message_size[i]);
		//if (!CompareBytes(c_messages[i],s_messages[selections[i]][i],message_size[i]))
		//{
		//	PrintBytes("Q",Q[i],k_bytes);
		//	PrintBytes("Q XOR s_bytes",XOREncrypt(Q[i],s_bytes,k_bytes),k_bytes);
		//	PrintBytes("T2",T2[i],k_bytes);
		//	PrintBytes("s",s_bytes,k_bytes);
		//	//for (int index=0;index<k;index++)
		//	//{
		//	//	cout<<"s_select[i]"<<s_selects[index]<<endl;
		//	//}
		//	//PrintBytes("PRG0",te[i],message_size[i]);
		//	//PrintBytes("PRG1",te2[i],message_size[i]);
		//	//PrintBytes("PRG",ht[i],message_size[i]);
		//	//PrintBytes("y[0][i]",y[0][i],message_size[i]);
		//	//PrintBytes("y[1][i]",y[1][i],message_size[i]);
		//	//PrintBytes("c_messages[i]",c_messages[i],message_size[i]);
		//	//PrintBytes("s_messages[0][i]",s_messages[0][i],message_size[i]);
		//	//PrintBytes("s_messages[1][i]",s_messages[1][i],message_size[i]);
		//	//PrintBytes("PRG",XOREncrypt(c_messages[i],y[selections[i]][i],message_size[i]),message_size[i]);
		//	cout<<"Selection="<<selections[i]<<"  i = "<<i<<endl;
		//	cout<<"YES"<<endl;
		//	//PrintBytes("")
		//}
		//if (i==0)
		//{
		//	//PrintBytes("2nd Hash",hashed,32);
		//	PrintBytes("ht[0]",ht[i],message_size[i]);
		//	/*cout<<"Selection="<<selections[i]<<endl;
		//	cout<<"Qb[0][0] = "<<Qb[0][0]<<"    Tb[0][0] = "<<Tb[0][0]<<endl;*/
		//}
		
	}
	swishai.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
	sw_c_OT+=(swishai.getElapsedTime()-rand_time);

	for (int i=0;i<k;i++)
	{
		delete [] S[0][i];
		delete [] S[1][i];
	}
	delete [] S[0];
	delete [] S[1];
	for (int i=0;i<n_ots;i++)
	{
		delete [] y[0][i];
		delete [] y[1][i];
		delete [] ht[i];
		delete [] T2[i];
	}

	delete [] y[0];
	delete [] y[1];
	delete [] ht;
	delete [] T2;
	delete [] tbyte;

	cout << "OT_n_m_d_client Computations finished." << endl;
	return 0;
}



int OT_80_m_client(int n_ots       //number of OTs
	,int message_size       //message_size: number of message bytes for each OT
	,byte* c_messages[]     //The array of chooser's messages (to be filled after protocol)
	,int selections[]       //The choosers selection bits
)
{
	cout << "OT_80_m_client computations started." <<endl;
	CStopWatch swo;
	byte** c_mes;
	c_mes = new byte*[n_ots];

	//sali- socket
	byte** y[2];
	y[0]=new byte*[n_ots];
	y[1]=new byte*[n_ots];
	for (int index=0;index<n_ots;index++)
	{
		//sali-socket: receving y[] from client
		y[0][index] = new byte[message_size];
		y[1][index] = new byte[message_size];
		
		myrecv(y[0][index],message_size);
		myrecv(y[1][index],message_size);
	}
	//

	sw_c_mot=0;
	OT_80_256_client(n_ots,32,c_mes,selections);
	sw_c_OT+=sw_c_mot;
	//PrintBytes("c_mes[0]",c_mes[0],32);
	//PrintBytes("PRG S[1][0],48",PRG_32_to_m_bytes_hash(S[1][0],message_size));
	//PrintBytes("PRG S[1][0],48",PRG_32_to_m_bytes_hash(S[1][0],message_size));
	//PrintBytes("PRG c_mes[0],48",PRG_32_to_m_bytes_hash(c_mes[0],message_size));
	
	////////////////////////////////////////////////////////////////////
	// Chooser Computation
	////////////////////////////////////////////////////////////////////
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	swo.startTimer();
	for (int index=0;index<n_ots;index++)
	{
		c_messages[index] = XOREncrypt(PRG_32_to_m_bytes_hash(c_mes[index],message_size),y[selections[index]][index],message_size);
	}
	swo.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
	sw_c_OT += (swo.getElapsedTime()-rand_time);
	for (int index=0;index<n_ots;index++)
	{
		delete y[0][index];
		delete y[1][index];
		delete c_mes[index];
	}
	delete [] y[0];
	delete [] y[1];
	delete [] c_mes;

	cout << "OT_80_m_client computations finished." <<endl;
	return 0;
}

int OT_80_m_server(int n_ots       //number of OTs
	,int message_size       //message_size: number of message bytes for each OT
	,byte** s_messages[2]   //The array of messages: messages[n_ots][message_size] of bytes
)
{
	cout << "OT_80_m_server computations started." << endl;
	CStopWatch swo;
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;
	swo.startTimer();
	byte** S[2];
	S[0] = new byte*[n_ots];
	S[1] = new byte*[n_ots];

	for (int index=0;index<n_ots;index++)
	{
		S[0][index] = GetRandomBytes(32);
		S[1][index] = GetRandomBytes(32);
	}
	byte** y[2];
	y[0]=new byte*[n_ots];
	y[1]=new byte*[n_ots];
	////////////////////////////////////////////////////////////////////
	// Sender Computation
	////////////////////////////////////////////////////////////////////
	for (int index=0;index<n_ots;index++)
	{
		y[0][index] = XOREncrypt(PRG_32_to_m_bytes_hash(S[0][index],message_size),s_messages[0][index],message_size);
		y[1][index] = XOREncrypt(PRG_32_to_m_bytes_hash(S[1][index],message_size),s_messages[1][index],message_size);
	//timing
		swo.stopTimer();
		rand_time = (sw_r1+sw_r2+sw_r3);
		sw_s_OT += (swo.getElapsedTime()-rand_time);

		//sali-socket: sending y[] to client
		mysend(y[0][index],message_size);
		mysend(y[1][index],message_size);
		sw_r1=0;
		sw_r2=0;
		sw_r3=0;
		swo.startTimer();
	}
	swo.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
	sw_s_OT += (swo.getElapsedTime()-rand_time);
	//cout<<"Selection="<<selections[0]<<endl;
	//PrintBytes("S[0][0]",S[0][0],32);

	sw_s_mot=0;

	OT_80_256_server(n_ots,32,S);

	sw_s_OT+=sw_s_mot;

	//PrintBytes("c_mes[0]",c_mes[0],32);
	//PrintBytes("PRG S[1][0],48",PRG_32_to_m_bytes_hash(S[1][0],message_size));
	//PrintBytes("PRG S[1][0],48",PRG_32_to_m_bytes_hash(S[1][0],message_size));
	//PrintBytes("PRG c_mes[0],48",PRG_32_to_m_bytes_hash(c_mes[0],message_size));

	for (int index=0;index<n_ots;index++)
	{
		delete y[0][index];
		delete y[1][index];
		delete S[0][index];
		delete S[1][index];

	}
	delete [] y[0];
	delete [] y[1];
	delete [] S[0];
	delete [] S[1];


	cout << "OT_80_m_server computations finished." << endl;
	return 0;
}



void OT_dynamic_message_size_client(int n_ots           //number of OTs
	,int* message_size    //message_size: number of message bytes
	,byte* c_messages[]  //The array of chooser's messages (to be filled after protocol)
	,int selections[]    //The choosers selection bits
)
{
	cout << "OT_dynamic_message_size_client started!" << endl;
	// Alg.1 Reduce the message_size in OT
	// Alg.2 Reduce the number of OTs
	if (n_ots<=80)
	{
		//if ((message_size*8)<=256)
		//{
		//	// call OT{80}{256}
		//	OT_80_256(n_ots,message_size,s_messages,c_messages,selections);

		//} 
		//else
		//{
			//call Alg.1 to reduce message_size to 256
			//call OT{80}{256}
			//cout << "n_ots: " <<n_ots << "  message_size: " << message_size[0] << endl;
			OT_80_m_d_client(n_ots,message_size,c_messages,selections);
		//}
	} 
	else
	{
		//call Alg.2 to reduce the OTs to 80
		// Call Alg.1 to reduce the message_size
		// call OT{80}{256}
		OT_n_m_d_client(n_ots,message_size,c_messages,selections);
	}
	cout << "OT_dynamic_message_size_client finished!" << endl;
}

void OT_dynamic_message_size_server(int n_ots           //number of OTs
	,int* message_size    //message_size: number of message bytes
	,byte** s_messages[] //The array of messages: messages[n_inputs][message_size] of bytes
)
{
	// Alg.1 Reduce the message_size in OT
	// Alg.2 Reduce the number of OTs
	cout << "OT_dynamic_message_size_server started!" << endl;
	if (n_ots<=80)
	{
		//if ((message_size*8)<=256)
		//{
		//	// call OT{80}{256}
		//	OT_80_256(n_ots,message_size,s_messages,c_messages,selections);

		//} 
		//else
		//{
			//call Alg.1 to reduce message_size to 256
			//call OT{80}{256}
			//cout << "n_ots: " <<n_ots << "  message_size: " << message_size[0] << endl;			
			OT_80_m_d_server(n_ots,message_size,s_messages);
		//}
	} 
	else
	{
		//call Alg.2 to reduce the OTs to 80
		// Call Alg.1 to reduce the message_size
		// call OT{80}{256}
		OT_n_m_d_server(n_ots,message_size,s_messages);
	}
	cout << "OT_dynamic_message_size_server finished!" << endl;
}





void OT_dynamic_256alpha_client(int n_ots
	,int *message_size
	,byte* c_messages[]
	,int selections[]
	)
{
	CStopWatch swo;
			
	cout << "OT_dynamic_256alpha_client() started" << endl;	

	const int evv_size = 511;
	const int alpha_size = 256;

	byte**** evv = new byte***[n_ots];

	for (int i=0;i<n_ots;i++)
	{
		evv[i] = new byte**[evv_size];

		// Set evv for non-terminals (0 to 2^8-1)
		int row_number[9] = {1,2,4,8,16,32,64,128,256};
		int index = 0;
		for (int j=0;j<8;j++)
		{
			for (int m=0;m<row_number[j];m++)
			{
				evv[i][index] = new byte*[2];
				evv[i][index][0] = new byte[message_size[i]];
				evv[i][index][1] = new byte[message_size[i]];
				myrecv(evv[i][index][0],message_size[i]);
				myrecv(evv[i][index][1],message_size[i]);
				index++;
			}
		}
		// Set evv for terminals (2^8 to 2^9-1)
		
		for (int j=255;j<evv_size;j++)
		{
			evv[i][j] = new byte*[2];
			evv[i][j][0] = new byte[message_size[i]];
			myrecv(evv[i][j][0],message_size[i]);
		}
	}	

	// Do ALL OT2s using Ishai extensions
	int input_size = 8*n_ots;
	byte** c_keys = new byte*[input_size];
	int* message_size2 = new int[input_size];
	// Lets convert each OT256 selection to 8 OT2 selection
	// n_ots*OT256 selections => n_ots*8*OT2 sel
	bitset<8> temp;
	int* sel = new int[8*n_ots];
	for (int i=0;i<n_ots;i++)
	{
		temp = selections[i];
		for (int j=0;j<8;j++)
		{
			sel[i*8+j] = temp[j];
			message_size2[i*8+j] = message_size[i];
		}
	}
	//sali
	//checked 

	OT_dynamic_message_size_client(input_size,message_size2,c_keys,sel);
	//int tester=0;
	//for (int tester=0;tester<input_size;tester++)
	//{
	//	printf("i=%d",tester);
	//	PrintBytes("Should KEY",key[sel[tester]][tester],message_size2[tester]);
	//	PrintBytes("c_Key",c_keys[tester],message_size2[tester]);
	//	cout<<endl<<endl;
	//}
	
	
	// Transfers (for future isolation of code)
	byte** c_pads = new byte*[n_ots];
	for (int i=0;i<n_ots;i++)
	{
		c_pads[i] = new byte[message_size[i]];
		myrecv(c_pads[i],message_size[i]);
	}
//	PrintBytes("c_pads[0]",c_pads[0],message_size[0]);
//	PrintBytes("pad[0][0]",pad[0][0],message_size[0]);
	
	///////////////////////////////////////////////
	// Decryption by client
	///////////////////////////////////////////////
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;

	swo.startTimer();
	// Set initial node indexes
	for (int i=0;i<n_ots;i++)
	{
		int node = 0;
		// non-terminals
		for (int j=0;j<8;j++)
		{
			if (sel[i*8+7-j]==0)
			{
				c_pads[i] =XOREncrypt(XOREncrypt(c_keys[i*8+7-j],c_pads[i],message_size[i]),evv[i][node][0],message_size[i]);
				node = node*2+1;
			}
			else
			{
				c_pads[i] =XOREncrypt(XOREncrypt(c_keys[i*8+7-j],c_pads[i],message_size[i]),evv[i][node][1],message_size[i]);
				node = node*2+2;
			}
		}
		// below c_pads will hold the result of i-th OT256
		c_messages[i] = XOREncrypt(c_pads[i],evv[i][node][0],message_size[i]);
	}

	swo.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
	sw_c_OT += (swo.getElapsedTime()-rand_time);
	
	cout << "OT_dynamic_256alpha_client() finished" << endl;	
}



void OT_dynamic_256alpha_server(int n_ots
	,int *message_size
	,byte** s_messages[]
	)
{
	cout << "OT_dynamic_256alpha_server() started" << endl;	

	CStopWatch swo;
	sw_r1=0;
	sw_r2=0;
	sw_r3=0;

	swo.startTimer();
	//Representation of EVV
	//size of EVV = 2^9-1=511
	//left son of index i = 2i+1
	//right son of index i = 2i+2
	//father of index i = floor (i-1)/2
	const int evv_size = 511;
	const int alpha_size = 256;
	// First prepare the keys for all OTs
	// And PADs
	// k[n_ots][n_levels=8][0 or 1][message size]
	//byte**** k = new byte***[n_ots];
	byte** key[2];
	key[0] = new byte*[8*n_ots];
	key[1] = new byte*[8*n_ots];

	byte*** pad = new byte**[n_ots];
	byte**** evv = new byte***[n_ots];



	for (int i=0;i<n_ots;i++)
	{
		for (int j=0;j<8;j++)
		{
			key[0][i*8+j] = GetRandomBytes(message_size[i]);
			key[1][i*8+j] = GetRandomBytes(message_size[i]);
		}
		pad[i] = new byte*[evv_size];
		evv[i] = new byte**[evv_size];
		for (int j=0;j<evv_size;j++)
		{
			pad[i][j] = GetRandomBytes(message_size[i]);
		}
		// Set evv for non-terminals (0 to 2^8-1)
		int row_number[9] = {1,2,4,8,16,32,64,128,256};
		int index = 0;

		for (int j=0;j<8;j++)
		{

			for (int m=0;m<row_number[j];m++)
			{
				evv[i][index] = new byte*[2];
				evv[i][index][0] = XOREncrypt(XOREncrypt(pad[i][index],key[0][i*8+7-j],message_size[i]),pad[i][index*2+1],message_size[i]);
				evv[i][index][1] = XOREncrypt(XOREncrypt(pad[i][index],key[1][i*8+7-j],message_size[i]),pad[i][index*2+2],message_size[i]);
				swo.stopTimer();
				sw_s_OT += swo.getElapsedTime();

				mysend(evv[i][index][0],message_size[i]);
				mysend(evv[i][index][1],message_size[i]);
				sw_r1=0;
				sw_r2=0;
				sw_r3=0;
			
				swo.startTimer();
				index++;
			}
		}
		// Set evv for terminals (2^8 to 2^9-1)

		for (int j=255;j<evv_size;j++)
		{
			//byte* alphabaet=convert_int_to_kbyte(message_size[i],(j-256));
			evv[i][j] = new byte*[2];
			evv[i][j][0] = XOREncrypt(pad[i][j],s_messages[j-255][i],message_size[i]);
			swo.stopTimer();
			sw_s_OT += swo.getElapsedTime();
			mysend(evv[i][j][0],message_size[i]);
			sw_r1=0;
			sw_r2=0;
			sw_r3=0;
			
			swo.startTimer();
		}

	}	

	// Do ALL OT2s using Ishai extensions
	int input_size = 8*n_ots;
//	byte** c_keys = new byte*[input_size];
	int* message_size2 = new int[input_size];
	// Lets convert each OT256 selection to 8 OT2 selection
	// n_ots*OT256 selections => n_ots*8*OT2 sel

	for (int i=0;i<n_ots;i++)
		for (int j=0;j<8;j++)
			message_size2[i*8+j] = message_size[i];

	swo.stopTimer();
	rand_time = (sw_r1+sw_r2+sw_r3);
	sw_s_OT += (swo.getElapsedTime()-rand_time);

	OT_dynamic_message_size_server(input_size,message_size2,key);
	//int tester=0;
	//for (int tester=0;tester<input_size;tester++)
	//{
	//	printf("i=%d",tester);
	//	PrintBytes("Should KEY",key[sel[tester]][tester],message_size2[tester]);
	//	PrintBytes("c_Key",c_keys[tester],message_size2[tester]);
	//	cout<<endl<<endl;
	//}
	for (int i=0;i<n_ots;i++)
		mysend(pad[i][0],message_size[i]);

//	PrintBytes("pad[0][0]",pad[0][0],message_size[0]);
	
	cout << "OT_dynamic_256alpha_server() finished" << endl;
}
