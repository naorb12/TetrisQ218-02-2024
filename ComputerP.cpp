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
			int score = getScore(i, board.getTopLevelPoint(i));
			if (score > desiredArr->score)
			{
				desiredArr->score = score;
				desiredArr->move = decideMove( tempPiece, i);
			}
		}
	}
}

int ComputerP::getScore( int col, int row) 
{
	Piece p(this->currPiece);
	Board b(board);

	int left = p.getLeft();
	int xDifference = (col - left);
	int min = p.getMin();
	int yDifference = row - min - 1;

	int score = 0;
	for (int i = 0; i < GameConfig::PIECE_SIZE; ++i)
	{
		p.tetrimino[i].addToX(xDifference);
		p.tetrimino[i].addToY(yDifference);
		b.setBoardPoint(p.tetrimino[i]);
	}

	if (p.getPieceType() == B)
	{
		return bombComputer(p, b);
	}

	for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
	{
		int rowClear = p.tetrimino[i].getY();
		if (b.isRowFull(rowClear))
		{
			for (int j = rowClear; j > 0; j--)
			{
				for (int col = 0; col < GameConfig::GAME_WIDTH; col++)
				{
					if (b.getBoardPoint(j - 1, col) == GameConfig::FILLED_CELL)
						b.setBoardPoint(Point(col, j));
					else
						b.resetBoardPoint(Point(col, j));
				}
			}

			for (int col = 0; col < GameConfig::GAME_WIDTH; col++)
			{
				b.resetBoardPoint(Point(col, 0));
			}
			score += ROW_CLEARED;
		}
	}
	
	score = score - (countHoles(b, p) * HOLE) - (countTopLevelUP(b) * TOPLEVEL_UP) + p.getMax();

	return score;
}

int ComputerP::countHoles( const Board& tempBoard, const Piece& p) const
{
	int newHoleCount = 0;
	for (int x = 0; x < GameConfig::GAME_WIDTH; ++x) {
		bool foundBlockBefore = false;
		bool foundBlockAfter = false;
		for (int y = 0; y < GameConfig::GAME_HEIGHT; ++y) {
			if (board.getBoardPoint(y, x) == GameConfig::FILLED_CELL) {
				foundBlockBefore = true;
			}
			if (tempBoard.getBoardPoint(y, x) == GameConfig::FILLED_CELL) {
				foundBlockAfter = true;
			}

			// Check for a new hole: a transition from no block above to block above
			if (!foundBlockBefore && foundBlockAfter && tempBoard.getBoardPoint(y, x) == GameConfig::EMPTY_CELL) {
				newHoleCount++;
				foundBlockBefore = true; // Update to prevent counting this space as a new hole again
			}
		}
	}
	return newHoleCount;
}

int ComputerP::countTopLevelUP(const Board& tempBoard) const
{
	int max = 0;

	for (int i = 0; i < GameConfig::GAME_WIDTH; i++)
	{
		if (board.getTopLevelPoint(i) - tempBoard.getTopLevelPoint(i) > max)
			max = board.getTopLevelPoint(i) - tempBoard.getTopLevelPoint(i);
	}
	return max;
}

int ComputerP::decideMove(const Piece& tempPiece, int desiredX) const
{
	int left = tempPiece.getLeft();

	int difference = desiredX - left;

	if (difference > 0)
	{
		return RIGHT; 
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
	Piece p(tempPiece);
	Board b(board);

	int left = p.getLeft();
	int xDifference = (col - left);
	int sign = (xDifference > 0) - (xDifference < 0);
	int min = p.getMin();
	int yDifference = row - min - 1;

	if (yDifference < abs(xDifference))
		return false;

	bool check = true;
	for (int j = 0; j < abs(xDifference); j++)
	{
		for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
		{
			if (p.tetrimino[i].getX() == GameConfig::GAME_WIDTH - 1 || p.tetrimino[i].getX() == 0 || board.getBoardPoint(p.tetrimino[i].getY() + 1, p.tetrimino[i].getX() + 1) == GameConfig::FILLED_CELL)
				return false;
			else
				p.tetrimino[i].addToX(sign);
		}
	}

	for (int i = 0; i < yDifference; i++)
	{
		for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
		{
			if (p.tetrimino[i].getY() == GameConfig::GAME_HEIGHT - 1)
				return false;

			if (b.getBoardPoint(p.tetrimino[i].getY() + 1, p.tetrimino[i].getX()) == GameConfig::FILLED_CELL)
				return false;
			else
				p.tetrimino[i].addToY(1);
		}
	}

	return true;
}


int ComputerP::moveChosen(const DESIRED_MOVES* desiredArr, const std::string str) const
{
	int numRotate = 0;
	desiredMoves max;


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

int ComputerP::bombComputer(const Piece & p,  Board & tempBoard)
{
	int countBefore = 0;
	int countAfter = 0;
	for (int i = 0; i < GameConfig::GAME_WIDTH; i++)
	{
		for (int j = 0; j < GameConfig::GAME_HEIGHT; j++)
		{
			if (tempBoard.getBoardPoint(j, i) == GameConfig::FILLED_CELL)
				countBefore++;
		}
	}

	int startX = getMax(0, p.tetrimino[0].getX() - BOMB_RADIUS);
	int startY = getMax(0, p.tetrimino[0].getY() - BOMB_RADIUS);
	int endX = getMin(GameConfig::GAME_WIDTH - 1, p.tetrimino[0].getX() + BOMB_RADIUS);
	int endY = getMin(GameConfig::GAME_HEIGHT - 1, p.tetrimino[0].getY() + BOMB_RADIUS);

	for (int y = startY; y <= endY; ++y)
	{
		for (int x = startX; x <= endX; ++x)
		{
			tempBoard.resetBoardPoint(Point(x,y));
		}
	}

	for (int i = 0; i < GameConfig::GAME_WIDTH; i++)
	{
		for (int j = 0; j < GameConfig::GAME_HEIGHT; j++)
		{
			if (tempBoard.getBoardPoint(j, i) == GameConfig::FILLED_CELL)
				countAfter++;
		}
	}

	return (countAfter - countBefore) * BOMB_PC;
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
