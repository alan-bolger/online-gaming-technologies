#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <SFML/System.hpp>

#include "Player.h"

enum Packet
{
	MESSAGE,
	P_Test
};

class Client
{
public: 
	Client(std::string IP, int PORT, State &t_state);
	bool Connect();
	bool SendString(std::string & _string);
	bool CloseConnection();
	void setPlayers(Player *t_player_1, Player *t_player_2, Player *t_player_3);
	
private:
	bool ProcessPacket(Packet _packettype);
	static void ClientThread();
	bool sendall(char * data, int totalbytes);
	bool SendInt(int _int);
	bool SendPacketType(Packet _packettype);
	bool recvall(char * data, int totalbytes);
	bool GetInt(int & _int);
	bool GetPacketType(Packet & _packettype);
	bool GetString(std::string & _string);
	void resetBools();

	SOCKET Connection; //This client's connection to the server
	SOCKADDR_IN addr; // Address to be binded to our Connection socket
	int sizeofaddr = sizeof(addr); // Need sizeofaddr for the connect function
	Player *m_player_1;
	Player *m_player_2;
	Player *m_player_3;
	sf::Vector2i m_player_1_pos;
	sf::Vector2i m_player_2_pos;
	sf::Vector2i m_player_3_pos;
	bool m_player_1_XposUpdated{ false };
	bool m_player_1_YposUpdated{ false };
	bool m_player_2_XposUpdated{ false };
	bool m_player_2_YposUpdated{ false };
	bool m_player_3_XposUpdated{ false };
	bool m_player_3_YposUpdated{ false };
	State &m_state;
};

static Client *clientptr;