#ifndef _Computer
#define _Computer

#include "Player.h"

constexpr int NUM_OF_MOVES = 6;
constexpr int NUM_OF_ROTATIONS = 4;
constexpr int NUM_OF_POINTS = 4;

enum Level{ NOVICE, GOOD, BEST};
enum Moves{ LEFT, RIGHT, ROTATE_CW, ROTATE_CCW, DROP, NOTHING};
enum Score{ROW_CLEARED = 10, HOLE=3, TOPLEVEL_UP = 3};


class ComputerP : public Player {

	int level ;

	typedef struct desiredMoves {

		int score = INT_MIN;
		int move = NOTHING;

	}DESIRED_MOVES;


	void computerMove(char* ch, const std::string str) ;
	char bestMove(const std::string str);
	void simulateMove(const Piece& tempPiece,  DESIRED_MOVES* desiredArr)  ;
	bool canPlacePiece(const Piece& tempPiece, int col, int row) const;
	int getScore(int col, int row) ;
	int countHoles(const Board& tempBoard) const;
	int countTopLevelUP(const Board& tempBoard) const;
	int decideMove(const Piece& tempPiece, int desiredX)const ;
	int moveChosen(const DESIRED_MOVES* desiredArr, const std::string str) const;

	char randomMove( const std::string str)const;

	char translateMove(const int move, const std::string str) const;



public:
	ComputerP(const int _level = BEST) : level(_level) { }
	void alterMove(char* ch, char input, const std::string str) ;
	
};

#endif 


