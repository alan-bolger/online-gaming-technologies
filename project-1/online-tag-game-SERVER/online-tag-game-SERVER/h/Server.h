#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>

enum Packet
{
	P_ChatMessage,
	P_Test
};

class Server
{
public:
	Server(int t_port, bool t_publicBroadcast = true);
	bool ListenForNewConnection();

private:
	bool sendall(int t_id, char * data, int totalbytes);
	bool recvall(int t_id, char * data, int totalbytes);
	bool SendInt(int t_id, int _int);
	bool GetInt(int t_id, int & _int);
	bool SendPacketType(int t_id, Packet _packettype);
	bool GetPacketType(int t_id, Packet & _packettype);
	bool SendString(int t_id, std::string & _string);
	bool GetString(int t_id, std::string & _string);
	bool ProcessPacket(int t_id, Packet _packettype);
	static void ClientHandlerThread(int t_id);
	void allPlayersConnected();
	float distance(int x1, int y1, int x2, int y2);
	bool checkCollisions();

private:
	SOCKET Connections[100];
	SOCKADDR_IN addr;
	int TotalConnections = 0;		
	int addrlen = sizeof(addr);
	SOCKET sListen;
	bool m_player_1_connected{ false };
	bool m_player_2_connected{ false };
	bool m_player_3_connected{ false };
	bool m_player_1_XposUpdated{ false };
	bool m_player_1_YposUpdated{ false };
	bool m_player_2_XposUpdated{ false };
	bool m_player_2_YposUpdated{ false };
	bool m_player_3_XposUpdated{ false };
	bool m_player_3_YposUpdated{ false };
	std::pair<int, int> m_player_1_position;
	std::pair<int, int> m_player_2_position;
	std::pair<int, int> m_player_3_position;
	int m_minutes;
	int m_seconds;
};

static Server *serverptr; // Serverptr is necessary so the static ClientHandler method can access the server instance/functions.
