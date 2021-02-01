#include "Start.h"

/// <summary>
/// Start constructor.
/// </summary>
/// <param name="t_font">A reference to a TTF font.</param>
Start::Start(sf::Font &t_font, Client *t_client) : m_font(t_font), m_client(t_client)
{
	m_title.setFont(m_font);
	m_title.setCharacterSize(72);
	m_title.setString("3 Player Online Tag");
	m_title.setFillColor(sf::Color::Blue);
	m_title.setOrigin(std::round(m_title.getGlobalBounds().width / 2), std::round(m_title.getGlobalBounds().height / 2));
	m_title.setPosition(500, 100);	

	m_text.setFont(m_font);
	m_text.setCharacterSize(48);
	m_text.setString("Control your player with the arrow keys. Press SPACE to connect to the server.");
	m_text.setOrigin(std::round(m_text.getGlobalBounds().width / 2), std::round(m_text.getGlobalBounds().height / 2));
	m_text.setPosition(500, 300);
	
	m_connectText.setFont(m_font);
	m_connectText.setCharacterSize(52);
	m_connectText.setPosition(500, 500);
}

/// <summary>
/// Start destructor.
/// </summary>
Start::~Start()
{
	delete m_client;
}

/// <summary>
/// Connect to the server.
/// </summary>
void Start::connect()
{
	if (!m_client->Connect())
	{
		m_connectText.setFillColor(sf::Color::Red);
		m_connectText.setString("Error! Unable to connect to server!");
		m_connectText.setOrigin(std::round(m_connectText.getGlobalBounds().width / 2), std::round(m_connectText.getGlobalBounds().height / 2));
	}
	else
	{
		m_connectText.setFillColor(sf::Color::Green);
		m_connectText.setString("Connected! Waiting on other players.....");
		m_connectText.setOrigin(std::round(m_connectText.getGlobalBounds().width / 2), std::round(m_connectText.getGlobalBounds().height / 2));
		m_connect = true;
	}
}

/// <summary>
/// Update start screen.
/// </summary>
void Start::update()
{
	if (!m_connect)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			connect();
			m_client->SendString("P" + std::to_string(Globals::s_playerID) + "_true");
		}
	}
}

/// <summary>
/// Draw start screen.
/// </summary>
void Start::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_title);
	t_window.draw(m_text);
	t_window.draw(m_connectText);
}
