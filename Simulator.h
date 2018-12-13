#ifndef DEF_SIMULATOR
#define DEF_SIMULATOR

#include <vector>
#include <array>
const int SIZE = 9;

class State{
	public:
	State();
	void getMove(std::vector<int>& moves) const;
	void doMove(int action, int player);
	int getWinner() const;
	void display() const;

	private:
	int board[SIZE]; // 0 for no pion, 1 for player 1, 2 for player 2


};

class Board{
	public:
	Board();
	Board(State stateInit, int player);
	int doMove(int action); //return the winner 
	void getMove(std::vector<int>& moves) const;
	void display() const;
	int getCurrentPlayer() const;
	State getState() const;
	int randomPlays();

	private:
	int currentPlayer;
	State state;
	

};


#endif
