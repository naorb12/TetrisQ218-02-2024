#ifndef _Player
#define _Player

#include <string>
#include "Board.h"
#include "Piece.h"

constexpr char ESCAPE = 27;

enum keyPlayer1
{
	left1 = 97, LEFT1 = 65, right1 = 100, RIGHT1 = 68, rotateclock1 = 115, ROTATECLOCK1 = 83, rotatecounterclock1 = 119, ROTATECOUNTERCLOCK1 = 87,
	drop1 = 120, DROP1 = 88
};

enum keyPlayer2
{
	left2 = 106, LEFT2 = 74, right2 = 108, RIGHT2 = 76, rotateclock2 = 107, ROTATECLOCK2 = 75, rotatecounterclock2 = 105, ROTATECOUNTERCLOCK2 = 73,
	drop2 = 109, DROP2 = 77
};

class Player {

public:
	Player() {}
	Board board;
	Piece currPiece;
	bool isPieceHit = true;

	// This function initialise a PLAYER'S board
	void setBoard() { this->board.initBoard();  }
	virtual void alterMove(char* ch, char input, const std::string str)  = 0;
};

#endif
