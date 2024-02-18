#ifndef _Board
#define _Board
#include "General.h"
#include "GameConfig.h"
#include "Piece.h"

static constexpr int BOMB_RADIUS = 4;

class Board 
{
	char board[GameConfig::GAME_HEIGHT][GameConfig::GAME_WIDTH]; 
	int topLevel[GameConfig::GAME_WIDTH];

	void shiftRowsDown(const int startRow);

	void performBomb(const Point& p, const int playerOffset);
	void applyGravity(const int playerOffset);
	void topLevelAfterBomb();
	int getMax(const int a, const int b) const { return (a > b) ? a : b; }
	int getMin(const int a, const int b) const { return (a < b) ? a : b; }
public:

	Board() {}
	Board(const Board& other);
	char getBoardPoint(const int row, const int col) const { return board[row][col]; }
	void setBoardPoint(Point p) { board[p.getY()][p.getX()] = GameConfig::FILLED_CELL; } // used in ComputerP
	int getTopLevelPoint(const int i) const { return topLevel[i]; }

	void initBoard();
	void clearRows(const Piece& piece);
	bool isRowFull(const int row) const;
	void printBoard(const int min) const;
	void insertPiece(const Piece& piece, const int playerOffset);

};

#endif
