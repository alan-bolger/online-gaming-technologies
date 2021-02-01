#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif

#include <cstdlib>

#include "Game.h"

int Globals::s_playerID = 0;
std::string Globals::sc_server_IP = "192.168.1.15";

/// <summary>
/// Main.
/// </summary>
/// <returns>1 for successful exit.</returns>
int main()
{
	Game &f_game = Game();
	f_game.run();

	return 1;
}