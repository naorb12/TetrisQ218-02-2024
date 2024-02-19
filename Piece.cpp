#include "Piece.h"
#include "Board.h"

// This function randomises a piece type for the piece
void Piece::buildpiece()
{
	pieceType = setPieceType();
	int midX = GameConfig::GAME_WIDTH / 2;
	int startX = midX - 1;
	int startY = 0;

	switch (pieceType)
	{
	case I:
		for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
		{
			tetrimino[i].setXAndY(startX + i, startY);
		}
		break;

	case J:
		tetrimino[0].setXAndY(startX, startY);
		for (int i = 1; i < GameConfig::PIECE_SIZE; i++)
		{
			tetrimino[i].setXAndY(startX + i - 1, startY + 1);
		}
		break;

	case L:
		tetrimino[0].setXAndY(startX + 2, startY);
		for (int i = 1; i < GameConfig::PIECE_SIZE; i++)
		{
			tetrimino[i].setXAndY(startX + i - 1, startY + 1);
		}
		break;

	case O:
		for (int i = 0; i < GameConfig::PIECE_SIZE / 2; i++)
		{
			tetrimino[i].setXAndY(startX + i, startY);
			tetrimino[i + 2].setXAndY(startX + i, startY + 1);
		}
		break;

	case S:
		for (int i = 0; i < GameConfig::PIECE_SIZE / 2; i++)
		{
			tetrimino[i].setXAndY(startX + i, startY + 1);
			tetrimino[i + 2].setXAndY(startX + i + 1, startY);
		}
		break;

	case T:
		for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
		{
			if (i < 3)
				tetrimino[i].setXAndY(startX + i, startY + 1);
			else
				tetrimino[i].setXAndY(startX + 1, startY);
		}
		break;

	case Z:
		for (int i = 0; i < GameConfig::PIECE_SIZE / 2; i++)
		{
			tetrimino[i].setXAndY(startX + i, startY);
			tetrimino[i + 2].setXAndY(startX + i + 1, startY + 1);
		}
		break;
	case B:
		for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
		{
			tetrimino[i].setXAndY(startX, startY);
		}
	}
}

Piece::Piece(const Piece& other) : pieceType(other.pieceType)
{
	for (int i = 0; i < GameConfig::PIECE_SIZE; ++i) 
	{
		this->tetrimino[i].setX(other.tetrimino[i].getX());
		this->tetrimino[i].setY(other.tetrimino[i].getY());
	}
}

int Piece::getLeft() const
{
	int left = tetrimino[0].getX();
	for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
	{
		if(tetrimino[i].getX()<=left)
			left = tetrimino[i].getX();
	}
	return left;
}

int Piece::getMin() const // highest Y = lowest point on board
{
	int min = tetrimino[0].getY();
	for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
	{
		if (tetrimino[i].getY() >= min)
			min = tetrimino[i].getY();
	}
	return min;
}

int Piece::getMax() const // lowest Y = highest point on board
{
	int max = tetrimino[0].getY();
	for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
	{
		if (tetrimino[i].getY() <= max)
			max = tetrimino[i].getY();
	}
	return max;
}

int Piece::setPieceType() {
	int number = rand() % 100;
	if (number < 5)
		return B;
	else
		return (number) % GameConfig::NUMBER_OF_PIECE_TYPE;
}

// This function cheks and moves a piece right on the board
void Piece::moveRight(const Board& board)
{
	bool checkRight = true;
	for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
	{
		if (tetrimino[i].getX() == GameConfig::GAME_WIDTH - 1 || board.getBoardPoint(tetrimino[i].getY() + 1, tetrimino[i].getX() + 1) == GameConfig::FILLED_CELL)
			checkRight = false;
	}
	if (checkRight == true)
	{
		for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
		{
			tetrimino[i].addToX(1);
		}
	}
}

// This function cheks and moves a piece left on the board
void Piece::moveleft(const Board& board)
{
	bool checkLeft = true;
	for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
	{
		if (tetrimino[i].getX() == 0 || board.getBoardPoint(tetrimino[i].getY() + 1, tetrimino[i].getX() - 1) == GameConfig::FILLED_CELL)
			checkLeft = false;
	}
	if (checkLeft == true)
	{
		for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
		{
			tetrimino[i].addToX(-1);
		}
	}
}

// This function checks and rotates a piece clockwise
void Piece::rotateClockwise(const Board& board)
{
	if (pieceType != O && pieceType != B)
	{
		Point center = tetrimino[1];

		for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
		{
			int newX = center.getY() - tetrimino[i].getY() + center.getX();
			int newY = tetrimino[i].getX() - center.getX() + center.getY();

			// Check boundary conditions
			if (newX < 0 || newX >= GameConfig::GAME_WIDTH || newY < 0 || newY >= GameConfig::GAME_HEIGHT)
			{
				return;
			}

			// Check collision with other pieces in board
			if (board.getBoardPoint(newY + 1,newX + 1 )== GameConfig::FILLED_CELL)
			{
				return;
			}
		}

		for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
		{
			int newX = center.getY() - tetrimino[i].getY() + center.getX();
			int newY = tetrimino[i].getX() - center.getX() + center.getY();
			tetrimino[i].setXAndY(newX, newY);
		}
	}
}

// This function checks and rotates a piece counter clockwise
void Piece::rotateCounterClockwise(const Board& board)
{
	if (pieceType != O && pieceType != B)
	{
		Point center = tetrimino[1];

		for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
		{
			int newX = -1 * (tetrimino[i].getY() - center.getY()) + center.getX();
			int newY = (tetrimino[i].getX() - center.getX()) + center.getY();

			// Check boundary conditions
			if (newX < 0 || newX >= GameConfig::GAME_WIDTH || newY < 0 || newY >= GameConfig::GAME_HEIGHT)
			{
				return;
			}

			// Check collision with other pieces in board
			if (board.getBoardPoint(newY + 1, newX + 1) == GameConfig::FILLED_CELL)
			{
				return;
			}
		}

		for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
		{
			int newX = -1 * (tetrimino[i].getY() - center.getY()) + center.getX();
			int newY = (tetrimino[i].getX() - center.getX()) + center.getY();
			tetrimino[i].setXAndY(newX, newY);
		}
	}
}