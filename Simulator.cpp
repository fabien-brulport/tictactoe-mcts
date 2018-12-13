#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include "Simulator.h"

using namespace std;

extern const int SIZE;

char representation(int player) {
	switch (player) {
		case 1:
			return 'X';
		case 2:
			return 'O';
		default:
			return ' ';
	}
}

State::State() {
	for (int i=0; i<SIZE; i++){
		board[i] = 0;
	}
}

void State::getMove(std::vector<int>& moves) const {
	for (int i=0; i<SIZE; i++) {
		if (board[i] == 0) {
			moves.push_back(i);
		}
	}
}

void State::doMove(int action, int player) {
	if (action < 0 || action>8){
		throw std::invalid_argument("action is not in [0:8]");
	}
	if (board[action]!=0) {
		throw std::invalid_argument("Action is not possible");
	}
	board[action] = player; 
}

int State::getWinner() const {
	for (int i=1; i<3; i++) {
		if (board[0] == board[1] && board[1] == board[2] && board[2] == i)
			return i;
		else if (board[3] == board[4] && board[4] == board[5] && board[5] == i)
			return i;
		else if (board[6] == board[7] && board[7] == board[8] && board[8] == i)
			return i;
		else if (board[0] == board[3] && board[3] == board[6] && board[6] == i)
			return i;
		else if (board[1] == board[4] && board[4] == board[7] && board[7] == i)
			return i;
		else if (board[2] == board[5] && board[5] == board[8] && board[8] == i)
			return i;
		else if (board[0] == board[4] && board[4] == board[8] && board[8] == i)
			return i;
		else if (board[2] == board[4] && board[4] == board[6] && board[6] == i)
			return i;
	}
	return 0;
}

void State::display() const {
	cout << "Player 1 (X)  -  Player 2 (O)" << endl << endl;
	cout << endl;
	cout << "     |     |     " << endl;
	cout << "  " << representation(board[0]) << "  |  " << representation(board[1]) << "  |  " << representation(board[2]) << endl;
	cout << "_____|_____|_____" << endl;
	cout << "     |     |     " << endl;
	cout << "  " << representation(board[3]) << "  |  " << representation(board[4]) << "  |  " << representation(board[5]) << endl;
	cout << "_____|_____|_____" << endl;
	cout << "     |     |     " << endl;
	cout << "  " << representation(board[6]) << "  |  " << representation(board[7]) << "  |  " << representation(board[8]) << endl;
	cout << "     |     |     " << endl << endl;
}



Board::Board(): currentPlayer(1){
}

Board::Board(State stateInit, int player) :currentPlayer(player), state(stateInit){
}

void Board::getMove(std::vector<int>& moves) const {
	state.getMove(moves);	
}

int Board::doMove(int action) {
	state.doMove(action, currentPlayer);
	currentPlayer = currentPlayer % 2 + 1;
	return state.getWinner();
}

void Board::display() const{
	state.display();
}

int Board::getCurrentPlayer() const {
	return currentPlayer;
}

State Board::getState() const {
	return state;
}

int Board::randomPlays(){
	std::vector<int> moves;
	int action;
	int winner(state.getWinner());
	state.getMove(moves);
	while(moves.size()>0 && winner==0){
		action = moves[rand() % moves.size()];
		winner = this->doMove(action);
		moves.clear();
		state.getMove(moves);
	}
	return winner;
}
