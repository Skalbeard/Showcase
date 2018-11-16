// Server.cpp
#include "pch.h"
#include "Server.h"


Server::Server()
{
	// First open the console.
	openConsoleWindow();
	// Initialize.
	startWinSock();
	// Open socket.
	openSocket();
}


Server::~Server()
{
	printf("Quitting\n");
	closesocket(m_sock);
	WSACleanup();
}


void Server::openSocket()
{
	// Create a UDP socket.
	m_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		die("socket failed");
	}

	// Fill out a sockaddr_in structure to describe the address we'll listen on.
	serverAddr.sin_family = AF_INET;
	// htons converts the address to network language.
	serverAddr.sin_port = htons(SERVERPORT);
	serverAddr.sin_addr.S_un.S_addr = inet_addr(SERVERIP);

	// Bind the socket to that address.
	if (bind(m_sock, (const sockaddr *)&serverAddr, sizeof(serverAddr)) != 0)
	{
		die("bind failed");
	}

	// FD counter for the select() parameter.
	numfd = m_sock + 1;

	address_length = sizeof(sockaddr);

	// We want no timeout or it will block.
	wait.tv_sec = 0;
	wait.tv_usec = 0;

	// Initialize the message struct variables to the starting position values of the other player.
	m_Message.posX = -4.0f;
	m_Message.posZ = -8.0f;

	data_recvd = false;

	// ntohs does the opposite of htons.
	printf("Server socket bound to address %s, port %d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
}

bool Server::Run()
{
	// Clear the set.
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);

	// Add the descriptors to the set.
	FD_SET(m_sock, &readfds);
	FD_SET(m_sock, &writefds);
	
	// Select call.
	int receive = select(numfd, &readfds, &writefds, NULL, &wait);

	// If select has failed.
	if (receive == -1)
	{
		perror("select() error!");
	}

	// If there is something to receive.
	else if (receive != 0)
	{
		// Descriptor has data.
		if (FD_ISSET(m_sock, &readfds))
		{
			data_recvd = true;

			// Clear descriptor.
			FD_CLR(m_sock, &readfds);

			// Fill the buffer with data.
			bytes_read = recvfrom(m_sock, receive_data, sizeof(receive_data), 0, (struct sockaddr *)&clientAddr, &address_length);
			//* Error checking for debugging.
			/*if (bytes_read == SOCKET_ERROR)
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					// Would block catch.
					die("would block");
				}
				else
				{
					// Something went wrong.
					die("some other error");
				}
			}*/
			// Process the message.
			ProcessMSG((const NetMSG *)receive_data);
		}
	}
	return true;
}

void Server::SendMSG(float _posX, float _posZ)
{
	// Check the FD set for writable.
	if (FD_ISSET(m_sock, &writefds))
	{
		data_recvd = false;

		// Clear descriptor.
		FD_CLR(m_sock, &writefds);

		// Prepare the message.
		NetMSG message;
		// Fill in the data.
		message.posX = _posX;
		message.posZ = _posZ;

		// Prepare the buffer.
		memcpy(send_data, &message, sizeof(NetMSG));

		// Send the data.
		sendto(m_sock, send_data, strlen(send_data), 0, (struct sockaddr *)&clientAddr, sizeof(struct sockaddr));
		printf("Sent network message: pos_x: %f, pos_z: %f\n", message.posX, message.posZ);
	}
}

bool Server::DataRecvd()
{
	return data_recvd;
}

// Process an incoming message.
void Server::ProcessMSG(const NetMSG *message)
{
	m_Message.posX = message->posX;
	m_Message.posZ = message->posZ;
	printf("Got network message: pos_x: %f, pos_z: %f\n", m_Message.posX, m_Message.posZ);
}

NetMSG Server::GetMSG()
{
	return m_Message;
}