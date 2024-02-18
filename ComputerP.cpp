#include "ComputerP.h"

void ComputerP::alterMove(char* ch, char input , const std::string str) 
{
	if (_kbhit())
	{
		input = _getch();
	}

	if (input == ESCAPE)
		*ch = input;
	else
	{
		computerMove(ch, str);
	}
}

void ComputerP::computerMove(char* ch, const std::string str) 
{
	int number = rand() % 100;
	switch (level)
	{
	case BEST:
		*ch = bestMove(str);
		break;
	case GOOD:
		if(number<95)
			*ch = bestMove(str);
		else
			*ch = randomMove(str);
		break;
	case NOVICE:
		if (number < 90)
			*ch = bestMove(str);
		else
			*ch = randomMove(str);
		break;
	default:
		break;
	}
}

char ComputerP::bestMove(const std::string str) 
{
	DESIRED_MOVES rotationsArr[NUM_OF_ROTATIONS];
	int move;

	for (int i = 0; i < NUM_OF_ROTATIONS; i++)
	{
		Piece tempPiece( this->currPiece); 
		if (i != 0)
		{
			tempPiece.rotateClockwise(this->board);
			bool rotated = false;
			for (int j = 0; j < NUM_OF_POINTS; j++)
			{
				if (tempPiece.tetrimino[j].getX() != this->currPiece.tetrimino[j].getX() || tempPiece.tetrimino[j].getY() != this->currPiece.tetrimino[j].getY())
					rotated = true;
			}
			if (rotated == true)
			{
				simulateMove(tempPiece, rotationsArr + i);
			}
		}
		else
		{
			simulateMove(tempPiece, rotationsArr + i);
		}

	}
	move = moveChosen(rotationsArr, str);
	
	return translateMove(move, str);
}

void ComputerP::simulateMove(const Piece& tempPiece,  DESIRED_MOVES* desiredArr) 
{
	for (int i = 0; i < GameConfig::GAME_WIDTH; i++)
	{
		if (canPlacePiece(tempPiece, i, board.getTopLevelPoint(i)))
		{
			Board tempBoard(board); // copy ctor
			int score = getScore(tempPiece, board, i, board.getTopLevelPoint(i));
			if (score > desiredArr->score)
			{
				desiredArr->score = score;
				desiredArr->move = decideMove( tempPiece, i);
			}
		}
	}
}

int ComputerP::getScore(const Piece& tempPiece,  Board& tempBoard, int col, int row) 
{
	Piece p(tempPiece); //copy ctor
	int score = 0;
	for (int i = 0; i < GameConfig::PIECE_SIZE; ++i)
	{
		p.tetrimino[i].setX(col + tempPiece.tetrimino[i].getX()); 
		p.tetrimino[i].setY(row + tempPiece.tetrimino[i].getY());
		tempBoard.setBoardPoint(p.tetrimino[i]);
	}

	for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
	{
		int row = p.tetrimino[i].getY();
		if (tempBoard.isRowFull(row))
		{
			score += ROW_CLEARED;
		}
	}
	
	score = score - (countHoles(tempBoard) * HOLE);

	return score;
}

int ComputerP::countHoles(const Board& tempBoard) const
{
	int holeCount = 0;
	for (int x = 0; x < GameConfig::GAME_WIDTH; ++x) 
	{
		bool foundBlock = false;
		for (int y = 0; y < GameConfig::GAME_HEIGHT; ++y)
		{
			if (tempBoard.getBoardPoint(y,x) == GameConfig::FILLED_CELL) 
			{
				foundBlock = true;
			}
			else if (foundBlock && tempBoard.getBoardPoint(y, x) == GameConfig::EMPTY_CELL)
			{
				// Found a hole
				holeCount++;
			}
		}
	}
	return holeCount;
}

int ComputerP::decideMove(const Piece& tempPiece, int desiredX) const
{
	int left = tempPiece.getLeft();

	int difference = desiredX - left;

	if (difference > 0)
	{
		return RIGHT; // Move right
	}
	else if (difference < 0) 
	{
		return LEFT; 
	}
	else 
	{
		return NOTHING; 
	}
}

bool ComputerP::canPlacePiece(const Piece& tempPiece, int col, int row) const
{
	int left = tempPiece.getLeft();
	int xDifference = abs(col - left);
	int min = tempPiece.getMin();
	int yDifference = row - min - 1;

	for (int i = 0; i < GameConfig::PIECE_SIZE; ++i) {
		int x = xDifference + tempPiece.tetrimino[i].getX(); // Horizontal position remains the same
		int y = yDifference + tempPiece.tetrimino[i].getY(); // Vertical position needs careful boundary checking

		// Corrected boundary check for the bottom of the board
		if (x < 0 || x >= GameConfig::GAME_WIDTH || y < 0 || y >= GameConfig::GAME_HEIGHT) {
			return false; // The block is outside the board's boundaries
		}

		// Check if the block's position is already occupied on the board
		if (board.getBoardPoint(y,x) != 0) {
			return false; // The block overlaps with an existing block
		}
	}
	return true;
}

int ComputerP::moveChosen(const DESIRED_MOVES* desiredArr, const std::string str) const
{
	int numRotate = 0;
	desiredMoves max = desiredArr[0];

	for (int i = 0; i < NUM_OF_ROTATIONS; i++)
	{
		if (desiredArr[i].score > max.score)
		{
			max = desiredArr[i];
			numRotate = i;
		}
	}

	if (numRotate > 0)
	{
		if (numRotate == 3)
			return ROTATE_CCW;
		else
			return ROTATE_CW;
	}
	else
		return max.move;


}


char ComputerP::randomMove(const std::string str)const
{
	char ch = '\0';
	int number = rand() % NUM_OF_MOVES;
	ch = translateMove(number, str);

	return ch;
}

char ComputerP::translateMove(const int move, const std::string str) const
{
	char ch = '\0';
	if (str.compare("P1") == 0)
	{
		switch (move)
		{
		case LEFT:
			ch = LEFT1;
			break;
		case RIGHT:
			ch = RIGHT1;
			break;
		case ROTATE_CW:
			ch = ROTATECLOCK1;
			break;
		case ROTATE_CCW:
			ch = ROTATECOUNTERCLOCK1;
			break;
		case DROP:
			ch = DROP1;
			break;
		case NOTHING:
			break;
		default:
			break;
		}
	}
	else {
		switch (move)
		{
		case LEFT:
			ch = LEFT2;
			break;
		case RIGHT:
			ch = RIGHT2;
			break;
		case ROTATE_CW:
			ch = ROTATECLOCK2;
			break;
		case ROTATE_CCW:
			ch = ROTATECOUNTERCLOCK2;
			break;
		case DROP:
			ch = DROP2;
			break;
		case NOTHING:
			break;
		default:
			break;
		}
	}
	return ch;
}
