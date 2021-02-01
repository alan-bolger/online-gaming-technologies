#include "Server.h"

bool Server::recvall(int t_id, char * data, int totalbytes)
{
	int f_totalBytesReceived = 0;

	// Receive all bytes
	while (f_totalBytesReceived < totalbytes)
	{
		int f_returnCheck = recv(Connections[t_id], data, totalbytes - f_totalBytesReceived, NULL);

		// Error check
		if (f_returnCheck == SOCKET_ERROR)
		{
			return false;
		}
			
		// Add to total bytes received
		f_totalBytesReceived += f_returnCheck;
	}

	return true;
}

bool Server::sendall(int t_id, char * data, int totalbytes)
{
	int f_totalBytesSent = 0;

	// Send all bytes
	while (f_totalBytesSent < totalbytes)
	{
		int RetnCheck = send(Connections[t_id], data + f_totalBytesSent, totalbytes - f_totalBytesSent, NULL);

		// Error check
		if (RetnCheck == SOCKET_ERROR)
		{
			return false;
		}
			
		// Add to total bytes sent
		f_totalBytesSent += RetnCheck; 
	}

	return true;
}

bool Server::SendInt(int t_id, int _int)
{
	if (!sendall(t_id, (char*)&_int, sizeof(int))) //Try to send int... If int fails to send
		return false; //Return false: int not successfully sent
	return true; //Return true: int successfully sent
}

bool Server::GetInt(int t_id, int & _int)
{
	if (!recvall(t_id, (char*)&_int, sizeof(int))) //Try to receive int... If int fails to be recv'd
		return false; //Return false: Int not successfully received
	return true;//Return true if we were successful in retrieving the int
}

bool Server::SendPacketType(int t_id, Packet _packettype)
{
	if (!sendall(t_id, (char*)&_packettype, sizeof(Packet))) //Try to send packet type... If packet type fails to send
		return false; //Return false: packet type not successfully sent
	return true; //Return true: packet type successfully sent
}

bool Server::GetPacketType(int t_id, Packet & _packettype)
{
	if (!recvall(t_id, (char*)&_packettype, sizeof(Packet))) //Try to receive packet type... If packet type fails to be recv'd
		return false; //Return false: packet type not successfully received
	return true;//Return true if we were successful in retrieving the packet type
}

bool Server::SendString(int t_id, std::string & _string)
{
	if (!SendPacketType(t_id, P_ChatMessage)) //Send packet type: Chat Message, If sending packet type fails...
		return false; //Return false: Failed to send string
	int bufferlength = _string.size(); //Find string buffer length
	if (!SendInt(t_id, bufferlength)) //Send length of string buffer, If sending buffer length fails...
		return false; //Return false: Failed to send string buffer length
	if (!sendall(t_id, (char*)_string.c_str(), bufferlength)) //Try to send string buffer... If buffer fails to send,
		return false; //Return false: Failed to send string buffer
	return true; //Return true: string successfully sent
}

bool Server::GetString(int t_id, std::string & _string)
{
	int bufferlength; //Holds length of the message
	if (!GetInt(t_id, bufferlength)) //Get length of buffer and store it in variable: bufferlength
		return false; //If get int fails, return false
	char * buffer = new char[bufferlength + 1]; //Allocate buffer
	buffer[bufferlength] = '\0'; //Set last character of buffer to be a null terminator so we aren't printing memory that we shouldn't be looking at
	if (!recvall(t_id, buffer, bufferlength)) //receive message and store the message in buffer array. If buffer fails to be received...
	{
		delete[] buffer; //delete buffer to prevent memory leak
		return false; //return false: Fails to receive string buffer
	}
	_string = buffer; //set string to received buffer message
	delete[] buffer; //Deallocate buffer memory (cleanup to prevent memory leak)
	return true;//Return true if we were successful in retrieving the string
}
