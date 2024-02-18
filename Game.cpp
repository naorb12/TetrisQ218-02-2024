#include "Game.h"

using namespace std;

// main game function, which contains the menu
void Game::run()
{
	srand(time(NULL));
	int sign = 0;
	bool gameFinished = true;
	while (sign != exitGame)
	{
	
		sign = printMenu(gameFinished);
		switch (sign)
		{
		case startGameHvsH:
			startNewGame(startGameHvsH);
			gameFinished = process();
			break;
		case startGameHvsPC:
			startNewGame(startGameHvsPC);
			gameFinished = process();
			break;
		case startGamePCvsPC:
			startNewGame(startGamePCvsPC);
			gameFinished = process();
			break;
		case continueGame:
			gameFinished = process();
			break;
		case instructions:
			printInstructions();
		case exitGame:
			break;
		default:
			break;
		}
	}
	clrscr();

}

// This function contains the gameplay itself
bool Game::process()
{
	char ch1, ch2;
	ch1 = ch2 = NULL_CHAR;
	while (winner == false && ch1 != ESCAPE && ch2 != ESCAPE)   // game loop
	{
		p1->isPieceHit = checkHit(*p1);
		p2->isPieceHit = checkHit(*p2);
		// check if piece hit & build new piece
		if (p1->isPieceHit == true)
		{
			for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
				drawPoint(p1->currPiece.tetrimino[i].getX() + GameConfig::PIECE_X, p1->currPiece.tetrimino[i].getY(), BACKGROUND_INTENSITY);
			performHit(p1, 0);
		}
		if (p2->isPieceHit == true)
		{
			for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
				drawPoint(p2->currPiece.tetrimino[i].getX() + GameConfig::P2_X + GameConfig::PIECE_X, p2->currPiece.tetrimino[i].getY(), BACKGROUND_INTENSITY);
			performHit(p2, GameConfig::P2_X);
		}

		movePieces(&ch1, &ch2);

		winner = checkWinner();

	}

	return winner;
}

void Game::performHit(Player* player, const int playerOffset)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	player->board.insertPiece(player->currPiece, playerOffset);
	player->board.clearRows(player->currPiece);
	player->currPiece.buildpiece();
}

// This function draws the borders of the boards
void Game::drawBorders() const
{
	int row, col;
	//Board1
	for (col = 0; col <= GameConfig::GAME_WIDTH; col++)
	{
		gotoxy(col, GameConfig::GAME_HEIGHT);
		cout << HORIZON_BORDER;
	}
	for (row = 0; row <= GameConfig::GAME_HEIGHT; row++)
	{
		gotoxy(0, row);
		cout << VERTICAL_BORDER;

		gotoxy(GameConfig::GAME_WIDTH + 1, row);
		cout << VERTICAL_BORDER;
	}

	//Board2
	for (col = GameConfig::P2_X; col <= GameConfig::GAME_WIDTH + GameConfig::P2_X; col++)
	{
		gotoxy(col, GameConfig::GAME_HEIGHT);
		cout << HORIZON_BORDER;
	}
	for (row = 0; row <= GameConfig::GAME_HEIGHT; row++)
	{
		gotoxy(GameConfig::P2_X, row);
		cout << VERTICAL_BORDER;

		gotoxy(GameConfig::GAME_WIDTH + GameConfig::P2_X + 1, row);
		cout << VERTICAL_BORDER;
	}

}

// This functions checks if a player's currPiece hit a piece in the board
bool Game::checkHit(const Player& player) const
{
	for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
	{
		if (player.currPiece.tetrimino[i].getY() == GameConfig::GAME_HEIGHT - 1)
			return true;

		if (player.board.getBoardPoint(player.currPiece.tetrimino[i].getY() + 1, player.currPiece.tetrimino[i].getX()) == GameConfig::FILLED_CELL)
			return true;
	}

	return false;
}

// This functions checks if someone won
bool Game::checkWinner() const
{
	char ch;
	bool p1Win, p2Win;
	p1Win = p2Win = false;
	for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
	{
		if (p1->board.getBoardPoint(p1->currPiece.tetrimino[i].getY() + 1, p1->currPiece.tetrimino[i].getX()) == GameConfig::FILLED_CELL && p1->currPiece.tetrimino[i].getY() <= 1)
			p2Win = true;
		if (p2->board.getBoardPoint(p2->currPiece.tetrimino[i].getY() + 1, p2->currPiece.tetrimino[i].getX()) == GameConfig::FILLED_CELL && p2->currPiece.tetrimino[i].getY() <= 1)
			p1Win = true;
	}

	if (p1Win == true)
	{
		clrscr();
		if (p2Win == true)
		{
			cout << "The result is a tie!";
		}
		else
			cout << "Player 1 won!";
		cout << "\n\nPress any key to continue...";
		ch = _getch();
		clrscr();
		return true;
	}
	else if (p2Win == true)
	{
		clrscr();
		cout << "Player 2 won!";
		cout << "\n\nPress any key to continue...";
		ch = _getch();
		clrscr();
		return true;
	}
	return false;
}

// This function initialises a new game for 2 players
void Game::startNewGame(const int gameType)
{
	clrscr();
	p1->setBoard();
	p2->setBoard();

	winner = false;
	p1->isPieceHit = p2->isPieceHit = true;

	p1->currPiece.buildpiece();
	p2->currPiece.buildpiece();

	drawBorders();
}

int Game::chooseLevel()const
{
	clrscr();
	cout << "Please choose the computer level:" << endl;
	cout << "(1) NOVICE" << endl;
	cout << "(2) GOOD" << endl;
	cout << "(3) BEST" << endl;
	int select = 0;
	while (select == 0)
	{
		select = _getch() - '0';
		switch (select)
		{
		case NOVICE+1:
			break;
		case GOOD+1:
			break;
		case BEST + 1:
			break;
		default:
			select = 0;
			break;
		}
	}
	return select - 1;

}

// This functions prints the menu and gets input from user
int Game::printMenu(bool gameFinished) 
{
	clrscr();
	if (gameFinished == true)
		cout << "Welcome to the TETRIS game!\n\n";
	cout << "(1) Start a new game - Human vs Human" << endl;
	cout << "(2) Start a new game - Human vs Computer" << endl;
	cout << "(3) Start a new game - Computer vs Computer" << endl;
	if (gameFinished == false)
		cout << "(4) Continue a paused game" << endl;
	cout << "(8) Present instructions and keys" << endl;
	cout << "(9) EXIT" << endl;

	int select = 0;
	int level;
	while (select == 0)
	{
		select = _getch() - '0';
		switch (select)
		{
		case startGameHvsH:
			deletePlayers();
			p1 = new HumanP;
			p2 = new HumanP;
			break;
		case startGameHvsPC:
			deletePlayers();
			level = chooseLevel();
			p1 = new HumanP;
			p2 = new ComputerP(level);
			break;
		case startGamePCvsPC:
			deletePlayers();
			level = chooseLevel();
			p1 = new ComputerP(level);
			p2 = new ComputerP(level);
			break;
		case continueGame:
			if (gameFinished == true)
				select = 0;
			else
				break;
		case instructions:
			break;
		case exitGame:
			break;

		default:
			select = 0;
			break;
		}
	}
	clrscr();
	drawBorders();
	if (p1 && p2)
	{
		p1->board.printBoard(0);
		p2->board.printBoard(GameConfig::P2_X);
	}
	return select;
}

// This is the main "moving" function, which also gets and performs the moves by players
void Game::movePieces(char* ch1, char* ch2)
{
	*ch1 = *ch2 = NULL_CHAR;
	drawBorders();
	p1->board.printBoard(0);
	p2->board.printBoard(GameConfig::P2_X);


	for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
	{
		drawPoint(p1->currPiece.tetrimino[i].getX() + GameConfig::PIECE_X, p1->currPiece.tetrimino[i].getY(), BACKGROUND_INTENSITY);
		drawPoint(p2->currPiece.tetrimino[i].getX() + GameConfig::P2_X + GameConfig::PIECE_X, p2->currPiece.tetrimino[i].getY(), BACKGROUND_INTENSITY);
	}

	getMove(ch1, ch2);
	for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
	{
		drawPoint(p1->currPiece.tetrimino[i].getX() + GameConfig::PIECE_X, p1->currPiece.tetrimino[i].getY(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		drawPoint(p2->currPiece.tetrimino[i].getX() + GameConfig::P2_X + GameConfig::PIECE_X, p2->currPiece.tetrimino[i].getY(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	}
	performMove(ch1, ch2);

}


// This function gets the moves by players and assigns to them if it is valid input
void Game::getMove(char* ch1, char* ch2) const
{
	Sleep(400);
	char ch = '\0';
	p1->alterMove(ch1, ch, "P1");

	while (_kbhit())
	{
	     ch = _getch();

		if (!*ch1 && (ch == left1 || ch == LEFT1 || ch == right1 || ch == RIGHT1
			|| ch == rotateclock1 || ch == ROTATECLOCK1 || ch == rotatecounterclock1 || ch == ROTATECOUNTERCLOCK1 || ch == drop1 || ch == DROP1 || ch == ESCAPE))
		{
			p1->alterMove(ch1, ch, "P1");
		}
		if (!*ch1 && (ch == left2 || ch == LEFT2 || ch == right2 || ch == RIGHT2
			|| ch == rotateclock2 || ch == ROTATECLOCK2 || ch == rotatecounterclock2 || ch == ROTATECOUNTERCLOCK2 || ch == drop2 || ch == DROP2 || ch == ESCAPE))
		{
			p2->alterMove(ch2, ch, "P2");
		}
	}
	p2->alterMove(ch2, ch, "P2");

}


// This function performs the moves chosen by players
void Game::performMove(char* ch1, char* ch2)
{

	switch (*ch1)
	{
	case left1: case LEFT1:
		p1->currPiece.moveleft(p1->board);
		break;
	case right1: case RIGHT1:
		p1->currPiece.moveRight(p1->board);
		break;
	case rotateclock1: case ROTATECLOCK1:
		p1->currPiece.rotateClockwise(p1->board);
		break;
	case rotatecounterclock1: case ROTATECOUNTERCLOCK1:
		p1->currPiece.rotateCounterClockwise(p1->board);
		break;
	case drop1: case DROP1:
		drop(*p1);
		break;
	default:
		break;
	}

	switch (*ch2)
	{
	case left2: case LEFT2:
		p2->currPiece.moveleft(p2->board);
		break;
	case right2: case RIGHT2:
		p2->currPiece.moveRight(p2->board);
		break;
	case rotateclock2: case ROTATECLOCK2:
		p2->currPiece.rotateClockwise(p2->board);
		break;
	case rotatecounterclock2: case ROTATECOUNTERCLOCK2:
		p2->currPiece.rotateCounterClockwise(p2->board);
		break;
	case drop2: case DROP2:
		drop(*p2);
		break;
	default:
		break;
	}

	// move down row
	for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
	{
		if (*ch1 != drop1 && *ch1 != DROP1)
			p1->currPiece.tetrimino[i].addToY(1);
		if (*ch2 != drop2 && *ch2 != DROP2)
			p2->currPiece.tetrimino[i].addToY(1);
	}

	if (*ch1 != ESCAPE)
		*ch1 = NULL_CHAR;
	if (*ch2 != ESCAPE)
		*ch2 = NULL_CHAR;

}

// This function checks and performs the drop move for a player, using the checkHit() function
void Game::drop(Player& p)
{
	while (checkHit(p) == false)
	{
		for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
		{
			p.currPiece.tetrimino[i].addToY(1);
		}
	}
	p.isPieceHit = true;
}

// This function prints instructions for the game
void Game::printInstructions() const
{
	clrscr();
	cout << R"(
Keys:

			Left Player            Right Player
LEFT                     a or A                 j or J
RIGHT                    d or D                 l or L
ROTATE clockwise         s or S                 k or K
ROTATE counterclockwise  w or W                 i or I (uppercase i)
DROP                     x or X                 m or M
)" << std::endl;
	cout << "\n\nPress any key to go back to menu";
	char ch = _getch();

}

void Game::deletePlayers()
{
	if (p1)
		delete p1;
	if (p2)
		delete p2;
}
