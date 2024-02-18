#ifndef _Game
#define _Game

#include "General.h"
#include "GameConfig.h"
#include "Player.h"
#include "HumanP.h"
#include "ComputerP.h"
#include <cstring>

enum menuInput
{
	startGameHvsH = 1, startGameHvsPC = 2, startGamePCvsPC = 3, continueGame = 4, instructions = 8, exitGame = 9
};


constexpr char NULL_CHAR = '\0';
constexpr char HORIZON_BORDER = '-';
constexpr char VERTICAL_BORDER = '|';


class Game {

	Player* p1;
	Player* p2;
	bool winner = false;

	void drawBorders() const;
	bool checkHit(const Player& player) const;
	void performHit(Player* player, const int playerOffset);
	bool checkWinner() const;
	int printMenu(bool gameFinished) ;
	void printInstructions() const;
	void movePieces(char* ch1, char* ch2);
	void getMove(char* ch1, char* ch2) const;
	void performMove(char* ch1, char* ch2);
	void drop(Player& p);
	bool process();
	void startNewGame(const int gameType);
	void deletePlayers();
	int chooseLevel()const;



public:
	void run(); // menu

};


#endif