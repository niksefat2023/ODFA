#include <iostream>
#include <cppsocket/tcpserver.h>
#include <cppsocket/socket.h>
#include <cppsocket/stringbuffer.h>
#include "EC_Naor_OT.h"

using namespace std;


int myrecv(void* ptr, int len);
int mysend(void* ptr, int len);

//server global stuff
CPPSocket::Socket server_init_sock;

//client and server global stuff
CPPSocket::Socket *connection;
//CPPSocket::Socket *client_sock = new CPPSocket::Socket;

//in header file
int client_network_setup(char*);
int server_network_setup();
int server_network_finish();
int client_network_finish();

int main(int argc, char *argv[])
{
	bool client, server;
	char server_ip[100];
	if(argc < 2)
	{
	 cout << "Usage:\n./test-ot -c ot_server_ip_address\n./test-ot -s" << endl;
	 return 0;
	}
	
	if (!strcmp(argv[1],"-c"))
	{
		client = true;
		server = false;
		if(argc != 3)
		{
		 cout << "Usage:\n./test-ot -[c|s] ot_server_ip_address" << endl;
		 return 0;
		}
		strncpy(server_ip,argv[2],100);
//		cout << "server ip is:" << server_ip << endl;;
	}
	else if (!strcmp(argv[1],"-s"))
	{
		client = false;
		server = true;
	}


//setting up the networking stuff
if (client)
	client_network_setup(server_ip);

if (server)
	server_network_setup();


//////////////////start - OT_n_m_d() original non-network test (Ishai-Ext)////////////////
/*
	test of ishai extension
	result: number of OTs should be multiple of 8
	

	int n_ots=129;
//	int message_size[] = {32,32,32,32,32};
	int message_size[n_ots];
	int selections[n_ots];
	for (int i=0;i<n_ots;i++)
	{
		message_size[i]=32;
		selections[i]=0;
	}

	//int selections[]={0,1,1,1,0};
	
	byte** c_messages = new byte*[n_ots];


	byte** s_messages[2];
	
	s_messages[0] = new byte*[n_ots];
	s_messages[1] = new byte*[n_ots];

	for (int i=0;i<n_ots;i++)
	{
		s_messages[0][i] = new byte[message_size[i]];
		s_messages[1][i] = new byte[message_size[i]];
		
		sprintf((char *)s_messages[0][i],"Message %3dAAAAAAAAAAAAAAAAAAA0",i);
		sprintf((char *)s_messages[1][i],"Message %3dAAAAAAAAAAAAAAAAAAA1",i);
		
		//PrintBytes("s_messages[0][i]",s_messages[0][i],32);// << endl;
		cout << "s_messages[0][i]: " <<s_messages[0][i] << endl;
	}	

	selections[78] = 1;
	selections[79] = 1;
	
	OT_n_m_d(n_ots    //number of OTs
			,message_size  //message_size: number of message bytes
			,s_messages
			,c_messages  //The array of chooser's messages (to be filled after protocol)
			,selections //The choosers selection bits
		);

	for (int i=0;i<n_ots;i++)
	{
		cout << c_messages[i] << endl;	
	}


return 0;
*/


//OT_dynamic_256alpha() original function //
/*
	int* selections = new int[n_ots];
	byte** c_messages= new byte*[n_ots];
	
	for (int i=0;i<n_ots;i++)
	{
		selections[i]=0;
		c_messages[i] = new byte[message_size[i]];
	}
	
	selections[1]=1;
	byte*** messages = new byte**[256];
	
	for (int i=0;i<256;i++)	
		messages[i] = generate_messages(12,n_ots);

cout << "herer1" << endl;	
	OT_dynamic_256alpha(n_ots,message_size,messages,c_messages,selections);
cout << "herer2" << endl;	
	for (int i=0;i<n_ots;i++)
	{
		cout << c_messages[i] << endl;
	}
	return 0;
*/	
////////////////////////////////////////////


if (client)
{
	char data[]="Hi";
	mysend(data,sizeof(data));

test_prg_256_client(28,1058);	
//////////////////start - OT_dynamic_256alpha_client() tes////////////////
/*
//comment 19 august
	int n_ots=32;
	int* message_size=new int[n_ots];
	for (int i=0;i<n_ots;i++)
		message_size[i]=8;


	int* selections = new int[n_ots];
	byte** c_messages= new byte*[n_ots];
	
	for (int i=0;i<n_ots;i++)
	{
		selections[i]=1;
		c_messages[i] = new byte[message_size[i]];
	}
	
	selections[1]=1;
	
	OT_dynamic_256alpha_client(n_ots,message_size,c_messages,selections);
	
	for (int i=0;i<n_ots;i++)
	{
		cout << c_messages[i] << endl;
	}
*/
//////////////////end - OT_dynamic_256alpha_client() tes////////////////
	
/////////////////start - test_prg_client///////////////////	
//	test_prg_client(20,250000);
/////////////////end - test_prg_client///////////////////

//////////////////start - OT_n_m_d() (Ishai-Ext)////////////////
/*
	int n_ots=512;
//	int message_size[] = {32,32,32,32,32};
	int message_size[n_ots];// = {32,32,32,32,32};
	int selections[n_ots];
	for (int i=0;i<n_ots;i++)
	{
		message_size[i]=8;
		selections[i]=0;
	}
	
	selections[100]=1;
	selections[99]=1;
	//int selections[]={0,1,1,1,0};
	
	byte** c_messages = new byte*[n_ots];
	
//	for (int i=0;i<n_ots;i++)
//	{
//		c_messages[i] = new byte[message_size[i]];
//debug:	cout << "selection[i]: " << selections[i] << endl;
//	}

	cout << "ishai client before OT run in main()" <<endl;
	OT_n_m_d_client(n_ots    //number of OTs
			,message_size  //message_size: number of message bytes
			,c_messages  //The array of chooser's messages (to be filled after protocol)
			,selections //The choosers selection bits
		);
	cout << "ishai client after OT run in main()" <<endl;	

		
	for (int i=0;i<n_ots;i++)
		cout<<"i="<<i<<endl<<c_messages[i]<<endl<<endl;
*/
//////////////////end - OT_n_m_d()////////////////	


//////////////////start - OT_80_m_d()////////////////
/*
	int n_ots=5;
	int message_size[] = {32,32,32,32,32};

	int selections[]={0,1,1,1,0};
	
	byte** c_messages = new byte*[n_ots];
	
	for (int i=0;i<n_ots;i++)
	{
		c_messages[i] = new byte[message_size[i]];
//debug:	cout << "selection[i]: " << selections[i] << endl;
	}

	
	OT_80_m_d_client(n_ots    //number of OTs
			,message_size  //message_size: number of message bytes
			,c_messages  //The array of chooser's messages (to be filled after protocol)
			,selections //The choosers selection bits
		);
		
	for (int i=0;i<n_ots;i++)
		cout<<"i="<<i<<endl<<c_messages[i]<<endl<<endl;
*/
//////////////////end - OT_80_m_d()////////////////	


//////////////////start - OT_80_256()////////////////
/*
	int n_ots=5;
	int message_size = 8;

	int selections[]={0,1,1,1,0};
	
	byte** c_messages = new byte*[n_ots];
//byte** c_messages;
	
//
//	for (int i=0;i<n_ots;i++)
//	{
//		c_messages[i] = new byte[message_size];
//debug:	cout << "selection[i]: " << selections[i] << endl;
//	}
//

	
	OT_80_256_client(n_ots    //number of OTs
			,message_size  //message_size: number of message bytes
			,c_messages  //The array of chooser's messages (to be filled after protocol)
			,selections //The choosers selection bits
		);
		
	for (int i=0;i<n_ots;i++)
		cout<<"i="<<i<<endl<<c_messages[i]<<endl<<endl;

//////////////////end - OT_80_256()////////////////	
*/
}


if(server)
{
	char data2[1024];
	myrecv(data2,3);
	cout << "we received: " << string(data2) << endl;

//////////////////start - test_prg_256_server() test////////////////
test_prg_256_server(28,1058);
//////////////////end - test_prg_256_server() /////////////////////

//////////////////start - OT_dynamic_256alpha_server() test////////////////
/*
	int n_ots=32;
	int* message_size=new int[n_ots];
	for (int i=0;i<n_ots;i++)
		message_size[i]=8;

	byte*** messages = new byte**[256];
	
	for (int i=0;i<256;i++)	
		messages[i] = generate_messages(8,n_ots);
	
	
	 sprintf((char*)messages[0][n_ots-1],"AAAAAAA");
	OT_dynamic_256alpha_server(n_ots,message_size,messages);
*/
//////////////////end - OT_dynamic_256alpha_server() test////////////////



//	test_prg_server(20,250000);


//////////////////start - OT_n_m_d()////////////////

/*
	int n_ots=512;
//	int message_size[] = {32,32,32,32,32};
	int message_size[n_ots];// = {32,32,32,32,32};
	for (int i=0;i<n_ots;i++)
		message_size[i]=8;

	byte** s_messages[2];
	
	s_messages[0] = new byte*[n_ots];
	s_messages[1] = new byte*[n_ots];

	for (int i=0;i<n_ots;i++)
	{
		s_messages[0][i] = new byte[message_size[i]];
		s_messages[1][i] = new byte[message_size[i]];
		
		sprintf((char *)s_messages[0][i],"Me%3da0",i);
		sprintf((char *)s_messages[1][i],"Me%3da1",i);
		
		//PrintBytes("s_messages[0][i]",s_messages[0][i],32);// << endl;
		cout << "s_messages[0][i]: " <<s_messages[0][i] << endl;
	}	
	
	
	cout << "ishai server before OT run in main()" <<endl;
	OT_n_m_d_server(n_ots    //number of OTs
			,message_size  //message_size: number of message bytes
			,s_messages  //The array of messages: messages[n_ots][message_size] of bytes
		);	
	cout << "ishai server after OT run in main()" <<endl;	
*/
/////

//////////////////end - OT_n_m_d()////////////////	

//////////////////start - OT_80_m_d()////////////////

/*
	int n_ots=5;
	int message_size[] = {32,32,32,32,32};
	


	byte** s_messages[2];
	
	s_messages[0] = new byte*[n_ots];
	s_messages[1] = new byte*[n_ots];

	for (int i=0;i<n_ots;i++)
	{
		s_messages[0][i] = new byte[message_size[i]];
		s_messages[1][i] = new byte[message_size[i]];
		
		sprintf((char *)s_messages[0][i],"Message %2dAAAAAAAAAAAAAAAAAAAA0",i);
		sprintf((char *)s_messages[1][i],"Message %2dAAAAAAAAAAAAAAAAAAAA1",i);
		
		//PrintBytes("s_messages[0][i]",s_messages[0][i],32);// << endl;
		cout << "s_messages[0][i]: " <<s_messages[0][i] << endl;
	}	
	
	
	
	OT_80_m_d_server(n_ots    //number of OTs
			,message_size  //message_size: number of message bytes
			,s_messages  //The array of messages: messages[n_ots][message_size] of bytes
		);	
	
*/
/////

//////////////////end - OT_80_m_d()////////////////	



//////////////////start - OT_80_256()////////////////
/*

	int n_ots=5;
	int message_size = 8;


	byte** s_messages[2];
	
	s_messages[0] = new byte*[n_ots];
	s_messages[1] = new byte*[n_ots];

	for (int i=0;i<n_ots;i++)
	{
		s_messages[0][i] = new byte[message_size];
		s_messages[1][i] = new byte[message_size];
		
		sprintf((char *)s_messages[0][i],"Mes%2da0",i);
		sprintf((char *)s_messages[1][i],"Mes%2da1",i);
		
		//PrintBytes("s_messages[0][i]",s_messages[0][i],32);// << endl;
		cout << "s_messages[0][i]: " <<s_messages[0][i] << endl;
	}	
	
	cout << "here ... " << endl;
	
	
	OT_80_256_server(n_ots    //number of OTs
			,message_size  //message_size: number of message bytes
			,s_messages  //The array of messages: messages[n_ots][message_size] of bytes
		);	
	
*/
/////

//////////////////end - OT_80_256()////////////////	



}


if (server)
	server_network_finish();

if (client)
	client_network_finish();
		
	return 0;	

}

int client_network_setup(char* server_ip)
{

	try
	{
		connection = new CPPSocket::Socket;
		CPPSocket::Address serverAddress(server_ip, 1234);
		connection->open(CPPSocket::Socket::TCP);
		connection->connect(serverAddress);
	}

	catch(CPPSocket::Exception & error)
	{
		cerr << error.getMessage() << endl;
		
		return EXIT_FAILURE;
	}
	
	cout << "Connection established with " << server_ip <<  endl;

	return 1;
}

int server_network_setup()
{
	try
	{
		CPPSocket::Address address(CPPSocket::Address::ANY_IP, 1234, false);

		server_init_sock.open(CPPSocket::Socket::TCP);
		
		server_init_sock.setsockopt(CPPSocket::SocketOption::ReuseAddr(1));
		
		server_init_sock.bind(address);
		
		server_init_sock.listen(5);
		
		CPPSocket::Address clientAddress;
		static CPPSocket::Socket tmp = server_init_sock.accept(clientAddress);
		
		connection = &tmp;
		
		clientAddress.lookup();
		
		cout << "Connection established from " << clientAddress.getName();
		cout << " on port " << clientAddress.getPort() << endl;

	}
	
	catch(CPPSocket::Exception & error)
	{
		cerr << error.getMessage() << endl;
		return EXIT_FAILURE;
	}

	return 1;
}

int client_network_finish()
{
	try
	{
		connection->close();
	}
	catch(CPPSocket::Exception & error)
	{
		cerr << error.getMessage() << endl;
		
		return EXIT_FAILURE;
	}
	
	cout << "Connection closed with server" << endl;
	return 1;
}

int server_network_finish()
{
	try
	{
		connection->close();		
		server_init_sock.close();
	}
	catch(CPPSocket::Exception & error)
	{
		cerr << error.getMessage() << endl;
		
		return EXIT_FAILURE;
	}
	
	cout << "Connection closed with client" << endl;
	return 1;
}

int mysend(void* ptr, int len)
{
	try
	{
		return(connection->send((const char *)ptr,len,0));
	}
	catch(CPPSocket::Exception & error)
	{
		cerr << error.getMessage() << endl;
		return EXIT_FAILURE;
	}
}

int myrecv(void* ptr, int len)
{
	try
	{
		return(connection->recv((char *)ptr,len,MSG_WAITALL));
	}
	catch(CPPSocket::Exception & error)
	{
		cerr << error.getMessage() << endl;
		return EXIT_FAILURE;
	}
}
