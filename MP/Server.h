// Server.h
#ifndef _SERVER_H_
#define _SERVER_H_

#include "Utils.h"

// The IP address for the server set to default local.
#define SERVERIP "127.0.0.1"

// The UDP port number for the server is also set to an arbitrary local.
#define SERVERPORT 6001


struct NetMSG
{
	float posX;
	float posZ;
};

class Server
{
public:
	Server();
	~Server();

	// Runs the listening routine.
	bool Run();
	void SendMSG(float _posX, float _posZ);
	bool DataRecvd();
	// Process an incoming message.
	void ProcessMSG(const NetMSG *message);
	NetMSG GetMSG();

private:
	// Opens a socket and set it up for listening.
	void openSocket();

	// The socket.
	SOCKET m_sock;

	// The message holder handle.
	NetMSG m_Message;

	// Variable that sets whether network data has been received since the last frame.
	bool data_recvd;

	// Socket address structs.
	sockaddr_in clientAddr;
	sockaddr_in serverAddr;

	// Define sets.
	fd_set readfds;
	fd_set writefds;

	// Variables.
	struct timeval wait;
	int numfd;
	int bytes_read;
	int address_length;
	char receive_data[sizeof(NetMSG)];
	char send_data[sizeof(NetMSG)];
};

#endif