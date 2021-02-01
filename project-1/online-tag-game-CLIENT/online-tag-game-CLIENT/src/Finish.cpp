#include "Finish.h"

/// <summary>
/// Finish constructor.
/// </summary>
/// <param name="t_font">A reference to a sf::Font</param>
Finish::Finish(sf::Font &t_font) : m_font(t_font)
{
	m_message.setFont(m_font);
	m_message.setCharacterSize(72);
	m_message.setString("Game over!");
	m_message.setFillColor(sf::Color::Blue);
	m_message.setOrigin(std::round(m_message.getGlobalBounds().width / 2), std::round(m_message.getGlobalBounds().height / 2));
	m_message.setPosition(500, 100);

	m_time.setFont(m_font);
	m_time.setCharacterSize(64);
	m_time.setFillColor(sf::Color::Green);
	m_time.setOrigin(std::round(m_message.getGlobalBounds().width / 2), std::round(m_message.getGlobalBounds().height / 2));
	m_time.setPosition(500, 350);
}

/// <summary>
/// Finish destructor.
/// </summary>
Finish::~Finish()
{

}

/// <summary>
/// This function sets the amount of time it took for the game to finish.
/// </summary>
/// <param name="t_time">The elapsed time from game start to end.</param>
void Finish::setTime(std::pair<int, int> t_time)
{
	m_time.setString("Time taken: " + std::to_string(t_time.first) + " : " + std::to_string(t_time.second));
	m_time.setOrigin(std::round(m_time.getGlobalBounds().width / 2), std::round(m_time.getGlobalBounds().height / 2));
}

/// <summary>
/// Update finish screen.
/// </summary>
void Finish::update()
{

}

/// <summary>
/// Draw finish screen.
/// </summary>
void Finish::draw(sf::RenderWindow &t_window)
{
	t_window.draw(m_message);
	t_window.draw(m_time);
}
