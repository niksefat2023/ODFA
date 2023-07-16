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

void usage(char**);


void usage(char** argv)
{
	cout << "Usage:\n" << argv[0] << " -c \"server_IP_address\" \"input_file\" \n" << argv[0] << " -s" << " \"dfa_file\"" << endl;
}

int main(int argc, char *argv[])
{
	bool client, server;
	char server_ip[100];
	char input_file[100];
	char dfa_file[100];
	
	if(argc < 2)
	{
		usage(argv);
		return 0;
	}
	
	if (!strcmp(argv[1],"-c"))
	{
		client = true;
		server = false;
		if(argc != 4)
		{
			usage(argv);
			return 0;
		}
		strncpy(server_ip,argv[2],100);
		strncpy(input_file,argv[3],100);
	//		cout << "server ip is:" << server_ip << endl;;
	}
	else if (!strcmp(argv[1],"-s"))
	{
		client = false;
		server = true;
		if(argc != 3)
		{
			usage(argv);
			return 0;
		}
		strncpy(dfa_file,argv[2],100);
	}
	
	
	//setting up the networking stuff
	if (client)
	{
		client_network_setup(server_ip);
	
		//Some sort of Initialization message
		char data[]="Hi";
		cout << "Sending Initialization Message... (Hi)" <<endl;
		mysend(data,sizeof(data));

		CStopWatch alltime;
		alltime.startTimer();
		test_prg_256_client_file(input_file);
		alltime.stopTimer();

		client_network_finish();
		
		s_stopwatch timer1;
		
		timer1.client_ot = sw_c_OT;
		timer1.client = sw_c;
		timer1.garbled_dfa = sw_garbled_dfa;
		timer1.server_ot = sw_s_OT;
		
		PrintTimer(timer1);	
		
		cout << "Total Client received Bytes: " << comm_recv <<endl;
		cout << "Total Client sent Bytes: " << comm_send <<endl;
		cout << "Total Client communication: " << comm_recv+comm_send << " (" << ceil((comm_recv+comm_send)/(1024*1024)) << "MB)" << endl;
		cout << "Total Client Time (ms): "<< alltime.getElapsedTime() *1000 << endl;
	}
	
	if (server)
	{
		server_network_setup();
	
		char data2[1024];
		myrecv(data2,3);
		cout << "Initialization Message Received. (" << string(data2) << ")" <<endl;
		
		CStopWatch alltime;
		alltime.startTimer();
		test_prg_256_server_file(dfa_file);
		alltime.stopTimer();
	
		server_network_finish();
		s_stopwatch timer2;
		
		timer2.client_ot = sw_c_OT;
		timer2.client = sw_c;
		timer2.garbled_dfa = sw_garbled_dfa;
		timer2.server_ot = sw_s_OT;
		
		PrintTimer(timer2);

		cout << "Total Server received Bytes: " << comm_recv <<endl;
		cout << "Total Server sent Bytes: " << comm_send <<endl;
		cout << "Total Server communication: " << comm_recv+comm_send << " (" << ceil((comm_recv+comm_send)/(1024*1024)) << "MB)" << endl;
		cout << "Total Server Time (ms): "<< alltime.getElapsedTime() *1000 << endl;
/*        for (int i=0;i<=1000000000;i++)
                for (int j=0;j<=4;j++);
        */
	}
	
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
	comm_send+=len;
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
	comm_recv+=len;
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
