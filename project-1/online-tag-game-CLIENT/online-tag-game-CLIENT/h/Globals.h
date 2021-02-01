#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>

class Globals
{
public:
	static int s_playerID;
	static std::string sc_server_IP;
	int port{ 8888 };
};

enum class State
{
	START,
	PLAY,
	SEND_TIME,
	TIME_DISPLAY,
	FINISH
};

#endif // !GLOBALS_H
