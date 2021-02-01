#ifndef FINISH_H
#define FINISH_H

#include <SFML/Graphics.hpp>

class Finish
{
public:
	Finish(sf::Font &t_font);
	~Finish();
	void setTime(std::pair<int, int> t_time);
	void update();
	void draw(sf::RenderWindow &t_window);

private:
	sf::Font &m_font;
	sf::Text m_message;
	sf::Text m_time;
};

#endif // !FINISH_H

