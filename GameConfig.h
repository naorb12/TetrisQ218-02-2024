#ifndef _GameConfig
#define _GameConfig

class GameConfig
{
public:

	static constexpr int GAME_WIDTH = 12;
	static constexpr int GAME_HEIGHT = 18;
	static constexpr char EMPTY_CELL = ' ';
	static constexpr char FILLED_CELL = '*';
	static constexpr int PIECE_SIZE = 4;
	static constexpr int NUMBER_OF_PIECE_TYPE = 7;

	static constexpr int P2_X = 24;
	static constexpr int PIECE_X = 1;
};
#endif
