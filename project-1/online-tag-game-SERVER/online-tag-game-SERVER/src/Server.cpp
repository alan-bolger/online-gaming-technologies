#include "Server.h"

Server::Server(int t_port, bool t_publicBroadcast)
{
	// Start WinSock
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);

	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	// Check if the server is publically available or just locally available
	if (t_publicBroadcast)
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	}

	addr.sin_port = htons(t_port); // This is the port
	addr.sin_family = AF_INET; // This is the IPv4 socket
	sListen = socket(AF_INET, SOCK_STREAM, NULL); // This creates a socket to listen for new connections

	// This binds the address to the socket
	if (bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) 
	{
		std::string ErrorMsg = "Failed to bind the address to our listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	// This sets the sListen socket to a state where it can listen for incoming connections
	// Note: SOMAXCONN = Socket Outstanding Maximum Connections
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR) 
	{
		std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	serverptr = this;
}

bool Server::ListenForNewConnection()
{
	// This makes a new connection
	// The first 'if' statement executes if there's a connection error
	// The 'else' statement completes the connection 
	SOCKET newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);

	if (newConnection == 0)
	{
		std::cout << "Client connectio failed!" << std::endl;
		return false;
	}
	else
	{
		std::cout << "Player " << TotalConnections + 1 << " connected" << std::endl;

		// Set socket in array to be the newest connection before creating the thread to handle this client's socket
		Connections[TotalConnections] = newConnection;

		// Create a thread to handle this client. The index in the socket array for this thread is the value (i)
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(TotalConnections), NULL, NULL);
		
		std::string f_welcomeMessage = "Welcome to Online Tag!";
		SendString(TotalConnections, f_welcomeMessage);

		std::string f_playerNumber = "P" + std::to_string(TotalConnections);
		SendString(TotalConnections, f_playerNumber);

		TotalConnections += 1; // Increment connection counter
		return true;
	}
}

bool Server::ProcessPacket(int t_id, Packet _packettype)
{
	switch (_packettype)
	{
	case P_ChatMessage:
	{
		std::string f_message;
		std::string f_identifier = f_message.substr(0, 2);

		// Receives message and stores in a variable
		if (!GetString(t_id, f_message))
		{
			return false; // If we do not properly get the chat message, return false
		} 

		// Check if each player is connected
		if (f_message == "P0_true")
		{
			m_player_1_connected = true;
			allPlayersConnected();
		}

		if (f_message == "P1_true")
		{
			m_player_2_connected = true;
			allPlayersConnected();
		}

		if (f_message == "P2_true")
		{
			m_player_3_connected = true;
			allPlayersConnected();
		}

		// This is for syncing player positions across clients
		if (f_identifier == "0X" || f_identifier == "0Y" ||
			f_identifier == "1X" || f_identifier == "1Y" ||
			f_identifier == "2X" || f_identifier == "2Y")
		{
			if (f_identifier == "0X")
			{
				m_player_1_position.first = std::stoi(f_message.substr(f_message.find(":") + 1));
				m_player_1_XposUpdated = true;
			}
			else if (f_identifier == "0Y")
			{
				m_player_1_position.second = std::stoi(f_message.substr(f_message.find(":") + 1));
				m_player_1_YposUpdated = true;
			}
			else if (f_identifier == "1X")
			{
				m_player_2_position.first = std::stoi(f_message.substr(f_message.find(":") + 1));
				m_player_2_XposUpdated = true;
			}
			else if (f_identifier == "1Y")
			{
				m_player_2_position.second = std::stoi(f_message.substr(f_message.find(":") + 1));
				m_player_2_YposUpdated = true;
			}
			else if (f_identifier == "2X")
			{
				m_player_3_position.first = std::stoi(f_message.substr(f_message.find(":") + 1));
				m_player_3_XposUpdated = true;
			}
			else if (f_identifier == "2Y")
			{
				m_player_3_position.second = std::stoi(f_message.substr(f_message.find(":") + 1));
				m_player_3_YposUpdated = true;
			}	

			// If all positions have been updated then begin syncing
			if (m_player_1_XposUpdated && m_player_1_YposUpdated &&
				m_player_2_XposUpdated && m_player_2_YposUpdated &&
				m_player_3_XposUpdated && m_player_3_YposUpdated)
			{
				std::string f_player_1_Xpos = "0X:" + m_player_1_position.first;
				std::string f_player_1_Ypos = "0Y:" + m_player_1_position.second;
				std::string f_player_2_Xpos = "1X:" + m_player_1_position.first;
				std::string f_player_2_Ypos = "1Y:" + m_player_1_position.second;
				std::string f_player_3_Xpos = "2X:" + m_player_1_position.first;
				std::string f_player_3_Ypos = "2Y:" + m_player_1_position.second;

				// Sync player 1
				if (!SendString(1, f_player_1_Xpos)) { std::cout << "Send error!" << std::endl; }
				if (!SendString(1, f_player_1_Ypos)) { std::cout << "Send error!" << std::endl; }
				if (!SendString(2, f_player_1_Xpos)) { std::cout << "Send error!" << std::endl; }
				if (!SendString(2, f_player_1_Ypos)) { std::cout << "Send error!" << std::endl; }

				// Sync player 2
				if (!SendString(0, f_player_2_Xpos)) { std::cout << "Send error!" << std::endl; }
				if (!SendString(0, f_player_2_Ypos)) { std::cout << "Send error!" << std::endl; }
				if (!SendString(2, f_player_2_Xpos)) { std::cout << "Send error!" << std::endl; }
				if (!SendString(2, f_player_2_Ypos)) { std::cout << "Send error!" << std::endl; }

				// Sync player 3
				if (!SendString(0, f_player_3_Xpos)) { std::cout << "Send error!" << std::endl; }
				if (!SendString(0, f_player_3_Ypos)) { std::cout << "Send error!" << std::endl; }
				if (!SendString(1, f_player_3_Xpos)) { std::cout << "Send error!" << std::endl; }
				if (!SendString(1, f_player_3_Ypos)) { std::cout << "Send error!" << std::endl; }

				// Reset flags
				m_player_1_XposUpdated = false;
				m_player_1_YposUpdated = false;
				m_player_2_XposUpdated = false;
				m_player_2_YposUpdated = false;
				m_player_3_XposUpdated = false;
				m_player_3_YposUpdated = false;
			}
		}

		// If a collision is detected then ask the player 1 client for the game time
		if (checkCollisions())
		{
			std::string GET_TIME = "ST";
			if (!SendString(0, GET_TIME)) { std::cout << "Send error!" << std::endl; }
		}

		// Handle receiving time
		if (f_identifier == "MI")
		{
			m_minutes = std::stoi(f_message.substr(f_message.find(":") + 1));
		}
		else if (f_identifier == "SE")
		{
			m_seconds = std::stoi(f_message.substr(f_message.find(":") + 1));
		}

		// This sends a message to each connected client
		for (int i = 0; i < TotalConnections; i++)
		{
			// This makes sure that the message isn't sent back to the client that originally sent it
			if (i == t_id)
			{
				continue;
			}

			// If the client player has moved up then tell the other clients
			if (f_message == "up")
			{
				std::string UP = "P" + std::to_string(t_id) + "_up";
				if (!SendString(i, UP)) { std::cout << "Send error!" << std::endl; }
			}
			// If the client player has moved down then tell the other clients
			else if (f_message == "down")
			{
				std::string DOWN = "P" + std::to_string(t_id) + "_down";
				if (!SendString(i, DOWN)) { std::cout << "Send error!" << std::endl; }
			}

			// If the client player has moved left then tell the other clients
			if (f_message == "left")
			{
				std::string LEFT = "P" + std::to_string(t_id) + "_left";
				if (!SendString(i, LEFT)) { std::cout << "Send error!" << std::endl; }
			}
			// If the client player has moved right then tell the other clients
			else if (f_message == "right")
			{
				std::string RIGHT = "P" + std::to_string(t_id) + "_right";
				if (!SendString(i, RIGHT)) { std::cout << "Send error!" << std::endl; }
			}
		}

		// Send finish time to all clients
		if (f_message == "END")
		{
			for (int i = 0; i < TotalConnections; i++)
			{
				std::string MIN = "MI:" + std::to_string(m_minutes);
				if (!SendString(i, MIN)) { std::cout << "Send error!" << std::endl; }

				std::string SEC = "SE:" + std::to_string(m_seconds);
				if (!SendString(i, SEC)) { std::cout << "Send error!" << std::endl; }
			}
		}

		break;
	}

	// If this is executed then it means the packet is not recognised
	default:
		std::cout << "Packet " << _packettype << " is unrecognised!" << std::endl;
		break;
	}

	return true;
}

void Server::ClientHandlerThread(int t_id)
{
	Packet PacketType;

	while (true)
	{
		if (!serverptr->GetPacketType(t_id, PacketType)) //Get packet type
		{
			break;
		}
			
		if (!serverptr->ProcessPacket(t_id, PacketType)) //Process packet (packet type)
		{
			break;
		}			
	}

	std::cout << "Lost connection to player " << t_id + 1 << std::endl;
	closesocket(serverptr->Connections[t_id]);
	return;
}

void Server::allPlayersConnected()
{
	// I can only test on 2 computers at the moment so this is setup for 2 players only
	if (m_player_1_connected && m_player_2_connected) // && m_player_3_connected)
	{
		for (int i = 0; i < TotalConnections; i++)
		{
			std::string START = "start";
			if (SendString(i, START)) { std::cout << "Starting game....." << std::endl; }
		}
	}
}

/// <summary>
/// Find the distance between two points.
/// </summary>
/// <param name="x1">X position of first point.</param>
/// <param name="y1">Y position of first point.</param>
/// <param name="x2">X position of second point.</param>
/// <param name="y2">Y position of second point.</param>
/// <returns>The distance between the two specified points.</returns>
float Server::distance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

/// <summary>
/// Checks for collisions between players.
/// </summary>
/// <returns>True if there was a collision, otherwise returns false.</returns>
bool Server::checkCollisions()
{
	// Get distance between each player from each other player
	float f_distance_1_2 = distance(m_player_1_position.first, m_player_1_position.second, m_player_2_position.first, m_player_2_position.second);
	//float f_distance_2_3 = distance(m_player_2_position.first, m_player_2_position.second, m_player_3_position.first, m_player_3_position.second);
	//float f_distance_1_3 = distance(m_player_1_position.first, m_player_1_position.second, m_player_3_position.first, m_player_3_position.second);

	// Collision check
	if (f_distance_1_2 < 20) // || f_distance_1_2 < 20 || f_distance_1_2 < 20)
	{
		return true;
	}
	else
	{
		return false;
	}
}
