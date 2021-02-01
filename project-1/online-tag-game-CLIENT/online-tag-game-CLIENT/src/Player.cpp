#include "Player.h"
#include "Client.h"

/// <summary>
/// Player constructor.
/// </summary>
Player::Player()
{
	m_playerCircle.setRadius(10);
	m_playerCircle.setOrigin(5, 5);
	m_timer = sf::Time::Zero;
}

/// <summary>
/// Player destructor.
/// </summary>
Player::~Player()
{

}

/// <summary>
/// Update player logic.
/// </summary>
/// <param name="t_client">A pointer to the client object.</param>
void Player::update(Client *t_client)
{
	// Only player 1 keeps the game time
	if (Globals::s_playerID == 0)
	{
		m_timer += m_timerClock.restart();
	}

	if (t_client != nullptr)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (t_client->SendString(UP))
			{
				m_position.y -= 5;
				if (m_position.y < 0) { m_position.y = 0; }
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			if (t_client->SendString(DOWN))
			{
				m_position.y += 5;
				if (m_position.y > 800) { m_position.y = 800; }
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			if (t_client->SendString(LEFT))
			{
				m_position.x -= 5;
				if (m_position.x < 0) { m_position.x = 0; }
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			if (t_client->SendString(RIGHT))
			{
				m_position.x += 5;
				if (m_position.y > 1000) { m_position.x = 1000; }
			}
		}
	}

	m_playerCircle.setPosition(m_position.x, m_position.y);
}

/// <summary>
/// Get the current X position of the player.
/// </summary>
/// <returns>The X position of the player.</returns>
int Player::getX()
{
	return m_position.x;
}

/// <summary>
/// Get the current Y position of the player.
/// </summary>
/// <returns>The Y position of the player.</returns>
int Player::getY()
{
	return m_position.y;
}

/// <summary>
/// Sends the player's current position to the server.
/// </summary>
/// /// <param name="t_client">A pointer to the client object.</param>
void Player::syncPosition(Client *t_client)
{
	std::string POS_X = std::to_string(Globals::s_playerID) + "X:" + std::to_string(getX());
	t_client->SendString(POS_X);

	std::string POS_Y = std::to_string(Globals::s_playerID) + "Y:" + std::to_string(getY());
	t_client->SendString(POS_Y);
}

/// <summary>
/// This sends the current time to the server.
/// It is used to grab the time at the moment the game finishes.
/// The time is then displayed on the finish screen.
/// </summary>
/// <param name="t_client">A pointer to the client object.</param>
void Player::sendTime(Client *t_client)
{
	// Retrieve game time minutes
	std::string MIN = "MI:" + std::to_string(getTime().first);
	t_client->SendString(MIN);

	// Retrieve game time seconds
	std::string SEC = "SE:" + std::to_string(getTime().second);
	t_client->SendString(SEC);

	// This tells the server to send the finish time to all clients
	std::string ST = "END";
	t_client->SendString(ST);
}

/// <summary>
/// This function returns the current time that has elapsed since the game started.
/// </summary>
/// <returns>std::pair: first = minutes / second = seconds</returns>
std::pair<int, int> Player::getTime()
{
	std::pair<int, int> f_time;
	int f_seconds = m_timer.asSeconds();
	
	if (f_seconds > 60)
	{
		f_time.first = f_seconds / 60;
		f_time.second = f_seconds % 60;
	}
	else
	{
		f_time.first = 0;
		f_time.second = f_seconds;
	}	

	return f_time;
}

/// <summary>
/// This returns the game end time.
/// </summary>
/// <returns>std::pair: first = minutes / second = seconds</returns>
std::pair<int, int> Player::getFinishTime()
{
	return m_finishTime;
}

/// <summary>
/// This is used for setting the game finish time from the client object.
/// </summary>
/// <param name="t_time">std::pair: first = minutes / second = seconds</param>
void Player::setFinishTime(std::pair<int, int> t_time)
{
	m_finishTime = t_time;
}

/// <summary>
/// Update player's position.
/// </summary>
void Player::setPosition(int t_x, int t_y)
{
	m_position.x = t_x;
	m_position.y = t_y;
	m_playerCircle.setPosition(m_position.x, m_position.x);
}

/// <summary>
/// Draw player sprite.
/// </summary>
/// <param name="t_window">A reference to the render window.</param>
void Player::draw(sf::RenderWindow &t_window)
{
	t_window.draw(m_playerCircle);
}

/// <summary>
/// Set the colour of the player.
/// </summary>
/// <param name="t_colour">The new colour of the player.</param>
void Player::setColour(sf::Color t_colour)
{
	m_playerCircle.setFillColor(t_colour);
}
