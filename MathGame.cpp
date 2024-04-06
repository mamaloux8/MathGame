#include "maths.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "constants.h"
#include "Ui.h"

#include <SFML/Graphics.hpp> 
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <stdexcept>
#include <bitset>
#include <vector>
#include <cstdlib>

class Manager
{

public:

	Manager(int numOperands=2, int maxHealth=100)
		: m_numOperands{numOperands}
		, m_maxHealth{maxHealth}
	{
	}

	void nextQuestion(sf::Sprite& healthBar)
	{
		m_numList = generateNumList(m_numOperands, m_operators);
		ui::questionText.setString(convertQuestionToString(ui::numList));
		m_input = "";
		ui::answerText.setString("");

		healthBar.setTextureRect(sf::IntRect(0, 0, m_maxHealth, healthBar.getLocalBounds().height));

	}

	void decreaseHealth(sf::Sprite& healthBar, double multiplier, double fullHealth)
	{
		if (m_health <= 0)
		{
			nextQuestion(healthBar);
			return;
		}
		else
		{
			m_health -= multiplier;
		}
	}

	const std::vector<int>& getNumList() const { return m_numList; }
	const std::string& getInput() const { return m_input; }
	int getHealth() const { return m_health; }

private:

	const int m_maxHealth{100};
	int m_health{ m_maxHealth };

	std::string m_input{};
	
	std::vector<int> m_numList{};
	int m_numOperands{ 2 };
	std::uint8_t m_operators{};
};

class UserInterface
{

public:

	UserInterface(sf::RenderWindow& window)
		: m_window{ window }
	{
		initFont();
		initHealthBar();
		initQuestionText();
		initAnswerText();
	}

	void initFont()
	{
		m_font.loadFromFile("arial.ttf");
	}

	void initHealthBar()
	{
		sf::Texture healthBarTexture{};
		healthBarTexture.loadFromFile("res/img/healthBar.png");

		m_healthBar.setTexture(healthBarTexture);
		m_healthBar.setScale(sf::Vector2f(3, 3));
		m_healthBar.setPosition(sf::Vector2f(m_window.getSize().x / 2 - (m_healthBar.getGlobalBounds().width) / 2, m_window.getSize().y / 8));

		initHealthBarContainer();
	}

	void initHealthBarContainer()
	{
		sf::Texture healthBarContainerTexture{};
		healthBarContainerTexture.loadFromFile("res/img/healthBarEmpty.png");

		m_healthBarContainer.setTexture(healthBarContainerTexture);
		m_healthBarContainer.setScale(sf::Vector2f(3, 3));
		m_healthBarContainer.setPosition(sf::Vector2f(m_window.getSize().x / 2 - (m_healthBarContainer.getGlobalBounds().width) / 2, m_window.getSize().y / 8));
	}

	void initQuestionText()
	{
		m_questionText.setFont(m_font);
		m_questionText.setCharacterSize(30);
		m_questionText.setFillColor(sf::Color::White);
		m_questionText.setPosition(sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 4));
	}

	void initAnswerText()
	{
		m_answerText.setFont(m_font);
		m_answerText.setCharacterSize(30);
		m_answerText.setPosition(sf::Vector2f(m_window.getSize().x / 2 - 100, m_window.getSize().y / 2));
	}

	void updateUI(Manager& manager)
	{
		m_questionText.setString(convertQuestionToString(manager.getNumList()));
		m_answerText.setString(manager.getInput());
		updateHealthBar(manager);
	}

	void updateHealthBar(Manager& manager)
	{
		m_healthBar.setTextureRect(sf::IntRect(0, 0, manager.getHealth(), m_healthBar.getLocalBounds().height));
	}

private:


	sf::RenderWindow& m_window;

	sf::Text m_answerText{};
	sf::Text m_questionText{};

	sf::Sprite m_healthBar{};
	sf::Sprite m_healthBarContainer{};

	sf::Font m_font{};
};


int main()
{

	srand(time(NULL));

	sf::RenderWindow window{ sf::VideoMode{2560,1440} , "MathGame" };
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);
	
	//Ticks (pour effectuer x action toutes les y frames)
	int ticks{};

	//Mise en place de la police d'écriture
	sf::Font font{};
	font.loadFromFile("arial.ttf");


	//Mise en place de l'UI pour les questions

	ui::questionText.setFont(font);
	ui::questionText.setCharacterSize(30);
	ui::questionText.setFillColor(sf::Color::White);
	ui::questionText.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 4));
	ui::questionText.setString(convertQuestionToString(ui::numList));


	
	//Mise en place de l'UI pour les Input

	ui::answerText.setFont(font);
	ui::answerText.setCharacterSize(30);
	ui::answerText.setPosition(sf::Vector2f(window.getSize().x/2-100, window.getSize().y/2));


	//Mise en place des textures
	sf::Texture healthBarTexture{};
	healthBarTexture.loadFromFile("res/img/healthBar.png");

	sf::Sprite healthBar{};
	healthBar.setTexture(healthBarTexture);
	healthBar.setScale(sf::Vector2f(3, 3));
	healthBar.setPosition(sf::Vector2f(window.getSize().x / 2 - (healthBar.getGlobalBounds().width)/2, window.getSize().y / 8));
	double healthBarFullSize{ healthBar.getLocalBounds().width };

	sf::Texture healthBarEmptyTexture{};
	healthBarEmptyTexture.loadFromFile("res/img/healthBarEmpty.png");

	sf::Sprite healthBarEmpty{};
	healthBarEmpty.setTexture(healthBarEmptyTexture);
	healthBarEmpty.setScale(sf::Vector2f(3, 3));
	healthBarEmpty.setPosition(sf::Vector2f(window.getSize().x / 2 - (healthBarEmpty.getGlobalBounds().width) / 2, window.getSize().y / 8));

	//changer d'emplacement ? 
	ui::numList = generateNumList(ui::numOperands, ui::operators);
	
	sf::Clock deltaClock;
	while (window.isOpen())
	{
	
		sf::Event event{};
		window.setFramerateLimit(60);
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();

			//Réception des Input du joueur
			constexpr int backspaceASCII{ 8 };
			constexpr int minusASCII{ 45 };

			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode == minusASCII && ui::input == "")
				{
					ui::input += event.text.unicode;
				}
				else if (event.text.unicode == backspaceASCII && ui::input != "")
				{
					ui::input.pop_back();
				}
				else if(isTextNumber(event) && ui::input.length() < 7)
				{
					ui::input += event.text.unicode;
				}

				ui::answerText.setString(ui::input);
			}
		}

		//Gesion barre de vie
		if(!(ticks%1))
			decreaseHealth(healthBar, 1, healthBarFullSize);

		//Vérification et génération des réponses

		std::string answerStr{};
		answerStr = ui::answerText.getString().toAnsiString();
		
		if (answerStr != "" && answerStr != "-")
		{
			try {
				int x{ stoi(answerStr) };
			}
			catch (std::invalid_argument& e)
			{
				return 1;
			}
			catch (std::out_of_range& e)
			{
				return 1;
			}
			catch (...)
			{
				return 1;
			}

			if (checkAnswer(ui::numList, std::stoi(answerStr)))
			{
				nextQuestion(healthBar, healthBarFullSize);
			}
		}

		//ImGui

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Options");
		ImGui::InputInt("Number of operands", &ui::numOperands);

		bool bit_addition{ (ui::operators & bitmasks::addition) != 0 };
		if(ImGui::Checkbox("Addition", &bit_addition))
		{
			ui::operators ^= bitmasks::addition;
		}

		bool bit_soustraction{ (ui::operators & bitmasks::soustraction) != 0 };
		if (ImGui::Checkbox("Soustraction", &bit_soustraction))
		{
			ui::operators ^= bitmasks::soustraction;
		}

		bool bit_multiplication{ (ui::operators & bitmasks::multiplication) != 0 };
		if (ImGui::Checkbox("Multiplication", &bit_multiplication))
		{
			ui::operators ^= bitmasks::multiplication;
		}

		bool bit_division{ (ui::operators & bitmasks::division) != 0 };
		if (ImGui::Checkbox("Division", &bit_division))
		{
			ui::operators ^= bitmasks::division;
		}


		ImGui::End();

		//Affichage

		window.clear();
		window.draw(ui::questionText);
		window.draw(ui::answerText);
		window.draw(healthBarEmpty);
		window.draw(healthBar);

		ImGui::SFML::Render(window);
		window.display();

		//Gestion des ticks
		if (ticks < 1000)
			++ticks;
		else
			ticks = 0;
	}

	ImGui::SFML::Shutdown();
	return 0;
}