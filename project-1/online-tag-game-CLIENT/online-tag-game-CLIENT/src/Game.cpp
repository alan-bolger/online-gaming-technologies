#include "Game.h"

/// <summary>
/// Game constructor.
/// </summary>
Game::Game() : m_window{ sf::VideoMode{ 1000, 800, 32 }, "Online Tag (Client)", sf::Style::Default }
{
	// Load app font
	m_font.loadFromFile("assets/fonts/default.ttf");

	// Set starting state
	m_state = State::START;

	// Create client with connection to server using port 8888
	m_client = new Client(Globals::sc_server_IP, 8888, m_state);

	// Create start and finish screens
	m_start = new Start(m_font, m_client);
	m_finish = new Finish(m_font);

	m_player_1 = new Player();
	m_player_2 = new Player();
	m_player_3 = new Player();

	// Set player colours
	m_player_1->setColour(sf::Color::Red);
	m_player_2->setColour(sf::Color::Green);
	m_player_3->setColour(sf::Color::Blue);

	// Set player starting positions
	m_player_1->setPosition(200, 400);
	m_player_2->setPosition(500, 400);
	m_player_3->setPosition(800, 400);

	// Set pointers to each player so the client can issue commands to them
	m_client->setPlayers(m_player_1, m_player_2, m_player_3);
}

/// <summary>
/// Game destructor.
/// </summary>
Game::~Game()
{
	delete m_player_1;
	delete m_player_2;
	delete m_player_3;
}

/// <summary>
/// Run.
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Clock sync;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f);
	sf::Time syncTimer = sf::Time::Zero;
	sf::Time positionSyncTime = sf::seconds(0.5f);

	while (m_window.isOpen() && !m_exitGame)
	{
		processEvents();
		timeSinceLastUpdate += clock.restart();
		syncTimer += sync.restart();

		if (syncTimer > positionSyncTime)
		{
			syncPositions();
			sf::Time syncTimer = sf::Time::Zero;
		}

		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
			update();
			draw();
		}
	}
}

/// <summary>
/// Process events.
/// </summary>
void Game::processEvents()
{
	sf::Event f_event;

	while (m_window.pollEvent(f_event))
	{
		if (sf::Event::Closed == f_event.type)
		{
			m_window.close();
		}

		if (sf::Event::KeyPressed == f_event.type)
		{
			if (sf::Keyboard::Escape == f_event.key.code)
			{
				m_exitGame = true;
			}
		}
	}
}

/// <summary>
/// Update everything.
/// </summary>
void Game::update()
{
	switch (m_state)
	{
	case State::START:
		m_start->update();
		break;

	case State::PLAY:
		switch (Globals::s_playerID)
		{
		case 0:
			m_player_1->update(m_client);
			m_player_2->update();
			m_player_3->update();
			break;

		case 1:
			m_player_1->update();
			m_player_2->update(m_client);
			m_player_3->update();
			break;

		case 2:
			m_player_1->update();
			m_player_2->update();
			m_player_3->update(m_client);
			break;
		}

		break;

	case State::SEND_TIME:
		if (Globals::s_playerID == 0)
		{
			m_player_1->sendTime(m_client);
		}

		break;

	case State::TIME_DISPLAY:
		m_finish->setTime(m_player_1->getFinishTime());
		break;

	case State::FINISH:	
		break;
	}
}

/// <summary>
/// Draw everything.
/// </summary>
void Game::draw()
{
	m_window.clear();

	switch (m_state)
	{
	case State::START:
		m_start->draw(m_window);
		break;

	case State::PLAY:
		m_player_1->draw(m_window);
		m_player_2->draw(m_window);
		m_player_3->draw(m_window);
		break;

	case State::SEND_TIME:
		m_player_1->draw(m_window);
		m_player_2->draw(m_window);
		m_player_3->draw(m_window);
		break;

	case State::FINISH:
		m_finish->draw(m_window);
		break;
	}

	m_window.display();	
}

/// <summary>
/// Syncs all of the players positions.
/// This is run every 0.5 seconds.
/// </summary>
void Game::syncPositions()
{
	switch (Globals::s_playerID)
	{
	case 0:
		m_player_1->syncPosition(m_client);
		break;

	case 1:
		m_player_2->syncPosition(m_client);
		break;

	case 2:
		m_player_3->syncPosition(m_client);
		break;
	}
}

