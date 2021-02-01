#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "Start.h"
#include "Finish.h"
#include "Player.h"
#include "Client.h"

class Game
{
public:
	Game();
	~Game();
	void run();

private:
	sf::RenderWindow m_window;
	State m_state;
	Start *m_start;
	Finish *m_finish;
	Client *m_client;
	Player *m_player_1;
	Player *m_player_2;
	Player *m_player_3;
	sf::Font m_font;
	bool m_exitGame{ false };

	void processEvents();
	void update();
	void draw();
	void syncPositions();
};

#endif // !GAME_H
