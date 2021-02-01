#include "Client.h"

bool Client::ProcessPacket(Packet t_packetType)
{
	switch (t_packetType)
	{
	case MESSAGE:
	{
		std::string f_command;
		std::string f_identifier = f_command.substr(0, 2);

		if (!GetString(f_command))
		{
			return false;
		} 
		else
		{
			// Assign players
			if (f_command == "P0") // Player 1
			{
				Globals::s_playerID = 0;
			}
			else if (f_command == "P1") // Player 2
			{
				Globals::s_playerID = 1;
			}
			else if (f_command == "P2") // Player 3
			{
				Globals::s_playerID = 2;
			}

			// Start game if all players are connected
			if (f_command == "start")
			{
				m_state = State::PLAY;
			}

			// End game is someone collides with someone else
			if (f_command == "ST")
			{
				m_state = State::SEND_TIME;
			}
			
			if (f_identifier == "MI")
			{
				std::pair<int, int> f_tempTime;
				f_tempTime.first = std::stoi(f_command.substr(f_command.find(":") + 1));
			}

			if (f_identifier == "SE")
			{ 
				std::pair<int, int> f_tempTime;
				f_tempTime.second = std::stoi(f_command.substr(f_command.find(":") + 1));
				m_state = State::FINISH;
			}

			// Player 1 movement
			else if (f_command == "P0_up")
			{
				int f_posX = m_player_1->getX();
				int f_posY = m_player_1->getY();
				f_posY -= 5;
				m_player_1->setPosition(f_posX, f_posY);
			}
			else if (f_command == "P0_down")
			{
				int f_posX = m_player_1->getX();
				int f_posY = m_player_1->getY();
				f_posY += 5;
				m_player_1->setPosition(f_posX, f_posY);
			}
			else if (f_command == "P0_left")
			{
				int f_posX = m_player_1->getX();
				int f_posY = m_player_1->getY();
				f_posX -= 5;
				m_player_1->setPosition(f_posX, f_posY);
			}
			else if (f_command == "P0_right")
			{
				int f_posX = m_player_1->getX();
				int f_posY = m_player_1->getY();
				f_posX += 5;
				m_player_1->setPosition(f_posX, f_posY);
			}

			// Player 2 movement
			else if (f_command == "P1_up")
			{
				int f_posX = m_player_2->getX();
				int f_posY = m_player_2->getY();
				f_posY -= 5;
				m_player_2->setPosition(f_posX, f_posY);
			}
			else if (f_command == "P1_down")
			{
				int f_posX = m_player_2->getX();
				int f_posY = m_player_2->getY();
				f_posY += 5;
				m_player_2->setPosition(f_posX, f_posY);
			}
			else if (f_command == "P1_left")
			{
				int f_posX = m_player_2->getX();
				int f_posY = m_player_2->getY();
				f_posX -= 5;
				m_player_2->setPosition(f_posX, f_posY);
			}
			else if (f_command == "P1_right")
			{
				int f_posX = m_player_2->getX();
				int f_posY = m_player_2->getY();
				f_posX += 5;
				m_player_2->setPosition(f_posX, f_posY);
			}

			//// Player 3 movement
			//else if (f_command == "P2_up")
			//{
			//	int f_posX = m_player_3->getX();
			//	int f_posY = m_player_3->getY();
			//	f_posY -= 5;
			//	m_player_3->setPosition(f_posX, f_posY);
			//}
			//else if (f_command == "P2_down")
			//{
			//	int f_posX = m_player_3->getX();
			//	int f_posY = m_player_3->getY();
			//	f_posY += 5;
			//	m_player_3->setPosition(f_posX, f_posY);
			//}
			//else if (f_command == "P2_left")
			//{
			//	int f_posX = m_player_3->getX();
			//	int f_posY = m_player_3->getY();
			//	f_posX -= 5;
			//	m_player_3->setPosition(f_posX, f_posY);
			//}
			//else if (f_command == "P2_right")
			//{
			//	int f_posX = m_player_3->getX();
			//	int f_posY = m_player_3->getY();
			//	f_posX += 5;
			//	m_player_3->setPosition(f_posX, f_posY);
			//}

			// This is for syncing player positions across clients
			if (f_identifier == "0X" || f_identifier == "0Y" ||
				f_identifier == "1X" || f_identifier == "1Y" ||
				f_identifier == "2X" || f_identifier == "2Y")
			{
				if (f_identifier == "0X")
				{
					m_player_1_pos.x = std::stoi(f_command.substr(f_command.find(":") + 1));
					m_player_1_XposUpdated = true;
				}
				else if (f_identifier == "0Y")
				{
					m_player_1_pos.y = std::stoi(f_command.substr(f_command.find(":") + 1));
					m_player_1_YposUpdated = true;
				}
				else if (f_identifier == "1X")
				{
					m_player_2_pos.x = std::stoi(f_command.substr(f_command.find(":") + 1));
					m_player_2_XposUpdated = true;
				}
				else if (f_identifier == "1Y")
				{
					m_player_2_pos.y = std::stoi(f_command.substr(f_command.find(":") + 1));
					m_player_2_YposUpdated = true;
				}
				else if (f_identifier == "2X")
				{
					m_player_3_pos.x = std::stoi(f_command.substr(f_command.find(":") + 1));
					m_player_3_XposUpdated = true;
				}
				else if (f_identifier == "2Y")
				{
					m_player_3_pos.y = std::stoi(f_command.substr(f_command.find(":") + 1));
					m_player_3_YposUpdated = true;
				}

				// Update players
				if (Globals::s_playerID == 0)
				{
					if (m_player_2_XposUpdated && m_player_2_YposUpdated &&
						m_player_3_XposUpdated && m_player_3_YposUpdated)
					{
						m_player_2->setPosition(m_player_2_pos.x, m_player_2_pos.x);
						m_player_3->setPosition(m_player_3_pos.x, m_player_3_pos.x);

						resetBools();
					}
				}
				else if (Globals::s_playerID == 1)
				{
					if (m_player_1_XposUpdated && m_player_1_YposUpdated &&
						m_player_3_XposUpdated && m_player_3_YposUpdated)
					{
						m_player_1->setPosition(m_player_1_pos.x, m_player_1_pos.x);
						m_player_3->setPosition(m_player_3_pos.x, m_player_3_pos.x);

						resetBools();
					}
				}
				//else if (Globals::s_playerID == 2)
				//{
				//	if (m_player_1_XposUpdated && m_player_1_YposUpdated &&
				//		m_player_2_XposUpdated && m_player_2_YposUpdated)
				//	{
				//		m_player_1->setPosition(m_player_1_pos.x, m_player_1_pos.x);
				//		m_player_2->setPosition(m_player_2_pos.x, m_player_2_pos.x);

				//		resetBools();
				//	}
				//}
			}
		}

		break;
	}
	default:
		std::cout << "Packet " << t_packetType << " is unrecognised!" << std::endl;
		break;
	}

	return true;
}

void Client::resetBools()
{
	m_player_1_XposUpdated = false;
	m_player_1_YposUpdated = false;
	m_player_2_XposUpdated = false;
	m_player_2_YposUpdated = false;
	m_player_3_XposUpdated = false;
	m_player_3_YposUpdated = false;
}

void Client::ClientThread()
{
	Packet PacketType;
	while (true)
	{
		if (!clientptr->GetPacketType(PacketType)) //Get packet type
			break; //If there is an issue getting the packet type, exit this loop
		if (!clientptr->ProcessPacket(PacketType)) //Process packet (packet type)
			break; //If there is an issue processing the packet, exit this loop
	}
	std::cout << "Lost connection to the server." << std::endl;
	if (clientptr->CloseConnection()) //Try to close socket connection..., If connection socket was closed properly
	{
		std::cout << "Socket to the server was closed successfuly." << std::endl;
	}
	else //If connection socket was not closed properly for some reason from our function
	{
		std::cout << "Socket was not able to be closed." << std::endl;
	}
}

Client::Client(std::string IP, int PORT, State &t_state) : m_state(t_state)
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	addr.sin_addr.s_addr = inet_addr(IP.c_str()); //Address (127.0.0.1) = localhost (this pc)
	addr.sin_port = htons(PORT); //Port 
	addr.sin_family = AF_INET; //IPv4 Socket
	clientptr = this; //Update ptr to the client which will be used by our client thread
}

bool Client::Connect()
{
	Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) //If we are unable to connect...
	{
		// MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	std::cout << "Connected!" << std::endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Create the client thread that will receive any data that the server sends.
	return true;
}

bool Client::CloseConnection()
{
	if (closesocket(Connection) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) //If socket error is that operation is not performed on a socket (This happens when the socket has already been closed)
			return true; //return true since connection has already been closed

		std::string ErrorMessage = "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError()) + ".";
		MessageBoxA(NULL, ErrorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

void Client::setPlayers(Player *t_player_1, Player *t_player_2, Player *t_player_3)
{ 
	m_player_1 = t_player_1;
	m_player_2 = t_player_2;
	m_player_3 = t_player_3;
}
