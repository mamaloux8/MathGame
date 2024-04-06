#include "maths.h"
#include "constants.h"
#include "Ui.h"

int countBits(std::uint8_t n)
{
	int count{};

	while (n)
	{
		count += n & 1;
		n >>= 1;
	}

	return count;

}

bool checkDivisionByZero(std::vector<int> numList)
{
	for (int i{}; i < numList.size(); ++i)
	{
		if ((i % 2) && (numList[i] == 3) && ((numList[i + 1]) == 0))
		{
			return true;
		}
	}

	return false;
}

std::vector<int> generateNumList(int nbOperands, std::uint8_t operators)
{

	int nbOperators{ countBits(operators) };

	if (nbOperators <= 0)
	{
		std::vector<int> num(1);
		return num;
	}

	std::vector<int> num((nbOperands * 2) - 1);

	//Generating random numbers for getting random operands and operators
	for (int i{}; i < num.size(); ++i)
	{
		if (!(i % 2))
		{
			num[i] = Random::get(0, 12);
		}
		else
		{

		randomOperatorLoop:

			//Add random operator to the numList (they are represented in the numList as numbers)
			num[i] = Random::get(0, 3);

			std::uint8_t randomNbBitflag{ 0b0000'0001 };
			randomNbBitflag <<= num[i];

			//Check if the random number we took is an operator the user selected (otherwise we use goto to generate another number)
			if(!static_cast<bool>(randomNbBitflag & operators))
				goto randomOperatorLoop;
			
		}
	}

	//Check if there's a division by zero in the question
	if (checkDivisionByZero(num))
	{
		generateNumList(nbOperands, operators);
	}

	return num;
}


std::string convertQuestionToString(std::vector<int> numList)
{
	std::string questionString{};

	for (int i = 0; i < numList.size(); i++)
	{

		if (!(i % 2))
		{
			questionString.append(std::to_string(numList[i]));
		}
		else
		{
			switch (numList[i])
			{
			case 0:
				questionString.append(" + ");
				break;
			case 1:
				questionString.append(" - ");
				break;
			case 2:
				questionString.append(" * ");
				break;
			case 3:
				questionString.append(" / ");
				break;
			default:
				questionString.append(" + ");
				break;

			}

		}

	}

	return questionString;
}

bool checkAnswer(std::vector<int> numList, int answer)
{
	int correctAnswer{ numList[0] };

	//Check for multiplication or division, then do the operation
	for (int i = 0; i < numList.size(); i++)
	{
		if ((i % 2) && numList[i]==3)
		{
			numList[i - 1] /= numList[i + 1];
			numList.erase(numList.begin() + i + 1);
			numList.erase(numList.begin() + i);
			--i;
		}
		else if ((i % 2) && numList[i] == 2)
		{
			numList[i - 1] *= numList[i + 1];
			numList.erase(numList.begin() + i + 1);
			numList.erase(numList.begin() + i);
			--i;
		}
	}

	//Check for addition or substraction, then do the operation
	for (int i = 0; i < numList.size(); i++)
	{
		if ((i % 2) && numList[i] == 1)
		{
			numList[i - 1] -= numList[i + 1];
			numList.erase(numList.begin() + i + 1);
			numList.erase(numList.begin() + i);
			--i;
		}
		else if ((i % 2) && numList[i] == 0)
		{
			numList[i - 1] += numList[i + 1];
			numList.erase(numList.begin() + i + 1);
			numList.erase(numList.begin() + i);
			--i;
		}
	}

	return answer == numList[0];

}

bool isTextNumber(sf::Event event)
{
	if (event.text.unicode >= 48 && event.text.unicode <= 57)
		return true;

	return false;
}

void decreaseHealth(sf::Sprite& healthBar, double multiplier, double fullHealth)
{

	double health{ healthBar.getLocalBounds().width - multiplier };
	if (health <= 0)
	{
		nextQuestion(healthBar, fullHealth);
		return;
	}
	else
	{
		healthBar.setTextureRect(sf::IntRect(0, 0, health, healthBar.getLocalBounds().height));
	}
}

void nextQuestion(sf::Sprite& healthBar, double fullHealth)
{
	ui::numList = generateNumList(ui::numOperands, ui::operators);
	ui::questionText.setString(convertQuestionToString(ui::numList));
	ui::input = "";
	ui::answerText.setString("");

	healthBar.setTextureRect(sf::IntRect(0, 0, fullHealth, healthBar.getLocalBounds().height));

}
