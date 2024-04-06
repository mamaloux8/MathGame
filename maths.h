#ifndef MATHS_H
#define MATHS_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp> 
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "Random.h"

bool checkDivisionByZero(std::vector<int> numList);
int countBits(std::uint8_t n);
std::vector<int> generateNumList(int nbOperands, std::uint8_t operators);
std::string convertQuestionToString(std::vector<int> numList);
bool checkAnswer(std::vector<int> numList, int answer);
bool isTextNumber(sf::Event event);

void decreaseHealth(sf::Sprite& healthBar, double multiplier, double fullHealth);
void nextQuestion(sf::Sprite& healthBar, double fullHealth);

#endif