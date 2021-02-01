#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

#include "Globals.h"

class Client;

class Player
{
public:
	Player();
	~Player();
	void update(Client *t_client = nullptr);
	int getX();
	int getY();
	void syncPosition(Client *t_client);
	void sendTime(Client *t_client);
	std::pair<int, int> getTime();
	std::pair<int, int> getFinishTime();
	void setFinishTime(std::pair<int, int> t_time);
	void setPosition(int t_x, int t_y);
	void draw(sf::RenderWindow &t_window);
	void setColour(sf::Color t_colour);

private:
	sf::CircleShape m_playerCircle;
	sf::Vector2i m_position;
	sf::Clock m_timerClock;
	sf::Time m_timer;
	std::string UP = "up";
	std::string DOWN = "down";
	std::string LEFT = "left";
	std::string RIGHT = "right";
	std::pair<int, int> m_finishTime;
};

#endif // !PLAYER_H
