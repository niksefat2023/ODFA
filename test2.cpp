#include <stdio.h>
#include <sstream>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <math.h>
#include <iostream>
#include "cryptopp-lib/cryptlib.h"
#include "cryptopp-lib/oids.h"
#include "cryptopp-lib/osrng.h"
#include "cryptopp-lib/eccrypto.h"
#include "cryptopp-lib/asn.h"
#include "cryptopp-lib/ecp.h"
#include "cryptopp-lib/ec2n.h"
#include "cryptopp-lib/simple.h"
#include "cryptopp-lib/sha.h"

#include <stdlib.h>

using namespace std;
void PrintBytes(byte* data,int length);

int main()
{
	byte* a;
	a = new byte[2];
	//sprintf(a,"%d",257);
	int b=0;
	int c=0;
	memcpy(a,&b,2);
	printf("%d %d\n",a,*a);
	PrintBytes((byte*)a,2);
	
	memcpy(&c,a,2);
	printf("c: %d\n",c);
	return 1;
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
