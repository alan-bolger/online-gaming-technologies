#ifndef START_H
#define START_H

#include <SFML/Graphics.hpp>

#include "Globals.h"
#include "Client.h"

class Start
{
public:
	Start(sf::Font &t_font, Client *t_client);
	~Start();
	void connect();
	void update();
	void draw(sf::RenderWindow &t_window);

private:
	sf::Font &m_font;
	Client *m_client;
	sf::Text m_title;
	sf::Text m_text;
	sf::Text m_connectText;
	bool m_connect{ false };
};

#endif // !START_H

