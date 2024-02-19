#include "Board.h"

// This function initialize the board to empty cells
void Board::initBoard()
{
    for (int i = 0; i < GameConfig::GAME_HEIGHT; i++) 
    {
        for (int j = 0; j < GameConfig::GAME_WIDTH; j++)
        {
            board[i][j] = GameConfig::EMPTY_CELL;
        }
    }

    for (int i = 0; i < 12; i++) 
    {
        topLevel[i] = 18;
    }
}

Board::Board(const Board& other)
{
    for (int i = 0; i < GameConfig::GAME_HEIGHT; ++i) {
        for (int j = 0; j < GameConfig::GAME_WIDTH; ++j) {
            this->board[i][j] = other.board[i][j];
        }
    }

    // Copy each element of the topLevel array
    for (int i = 0; i < GameConfig::GAME_WIDTH; ++i) {
        this->topLevel[i] = other.topLevel[i];
    }
}

// This function checks if a row is full in a board
bool Board::isRowFull(const int row) const
{
    for (int col = 0; col < GameConfig::GAME_WIDTH; col++)
    {
        if (board[row][col] == GameConfig::EMPTY_CELL)
        {
            return false;
        }
    }
    return true;
}

// This function shifts rows down 
void Board::shiftRowsDown(const int startRow)
{
    for (int row = startRow; row > 0; row--)
    {
        for (int col = 0; col < GameConfig::GAME_WIDTH; col++)
        {
            board[row][col] = board[row - 1][col];
        }
    }

    for (int col = 0; col < GameConfig::GAME_WIDTH; col++)
    {
        board[0][col] = GameConfig::EMPTY_CELL;
    }
    clrscr();
}

// This function checks if rows are full in a board, and deletes them 
void Board::clearRows(const Piece& piece)
{
    int count = 0;
    for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
    {
        int row = piece.tetrimino[i].getY();
        if (isRowFull(row))
        {
            shiftRowsDown(row);
            count++;
            i = -1;
        }
    }
    if (count > 0)
    {
        for (int j = 0; j < GameConfig::GAME_WIDTH; j++)
        {
            topLevel[j] -= count;
        }
    }

}

// This function prints the board and its pieces
void Board::printBoard(const int min) const
{
    for (int i = 0; i < GameConfig::GAME_HEIGHT; ++i)
    {
        for (int j = 0; j < GameConfig::GAME_WIDTH; ++j)
        {
            if (board[i][j] == GameConfig::FILLED_CELL)
            {
                drawPoint(j + min + GameConfig::PIECE_X, i, BACKGROUND_INTENSITY);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
        }
    }
}

// This function inserts a piece into the board
void Board::insertPiece(const Piece& piece, const int playerOffset)
{
    if (piece.getPieceType() == B && piece.tetrimino[0].getY() < GameConfig::GAME_HEIGHT - 1)
    {
        performBomb(piece.tetrimino[0], playerOffset);
    }
    else 
    {
        for (int i = 0; i < GameConfig::PIECE_SIZE; i++)
        {
            board[piece.tetrimino[i].getY()][piece.tetrimino[i].getX()] = GameConfig::FILLED_CELL;
            if (topLevel[piece.tetrimino[i].getX()] > piece.tetrimino[i].getY())
                topLevel[piece.tetrimino[i].getX()] = piece.tetrimino[i].getY();
        }
    }  
}

// This function blows 4 cells in each direction
void Board::performBomb(const Point& p, const int playerOffset)
{
    int startX = getMax(0, p.getX() - BOMB_RADIUS);
    int startY = getMax(0, p.getY() - BOMB_RADIUS);
    int endX = getMin(GameConfig::GAME_WIDTH - 1, p.getX() + BOMB_RADIUS);
    int endY = getMin(GameConfig::GAME_HEIGHT - 1, p.getY() + BOMB_RADIUS);

    for (int y = startY; y <= endY; ++y) 
    {
        for (int x = startX; x <= endX; ++x) 
        {
            board[y][x] = GameConfig::EMPTY_CELL;
            drawPoint(x + playerOffset + GameConfig::PIECE_X, y, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
    }
   applyGravity(playerOffset);
   topLevelAfterBomb();
}

// This function moves down all pieces without "ground" as result of the bomb's blast
void Board::applyGravity(const int playerOffset)
{
    for (int col = 0; col < GameConfig::GAME_WIDTH; ++col)
    {
        int emptyRow = GameConfig::GAME_HEIGHT - 1; 

        // Move blocks from bottom to top until an empty space is found
        for (int row = GameConfig::GAME_HEIGHT - 1; row >= 0; --row)
        {
            if (board[row][col] != GameConfig::EMPTY_CELL)
            {
                // Move the block to the emptyRow position
                board[emptyRow][col] = board[row][col];
                if (emptyRow != row) 
                {
                    board[row][col] = GameConfig::EMPTY_CELL;
                    drawPoint(col + playerOffset + GameConfig::PIECE_X, row, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }

                 drawPoint(col + playerOffset + GameConfig::PIECE_X, emptyRow, BACKGROUND_INTENSITY);
                 drawPoint(col + playerOffset + GameConfig::PIECE_X, emptyRow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                --emptyRow;
            }
        }
    }
}

void Board::topLevelAfterBomb()
{
    for (int i = 0; i < GameConfig::GAME_WIDTH; i++)
    {
        topLevel[i] = GameConfig::GAME_HEIGHT;   // empty column

        for (int j = 0; j < GameConfig::GAME_HEIGHT; j++)
        {
            if (board[j][i] == GameConfig::FILLED_CELL)
            {
                topLevel[i] = j; 
                break; 
            }
        }
    }
}

void Board::setBoardPoint(Point p) { 
    board[p.getY()][p.getX()] = GameConfig::FILLED_CELL; 
    if (topLevel[p.getX()] > p.getY())
        topLevel[p.getX()] = p.getY();
} // used in ComputerP

void Board::resetBoardPoint(Point p) {
    board[p.getY()][p.getX()] = GameConfig::FILLED_CELL;
    if (topLevel[p.getX()] == p.getY())
        topLevel[p.getX()]++;
} // used in ComputerP






