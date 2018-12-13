#include <iostream>
#include <cstdlib>
#include <vector>
#include "time.h"
#include "Simulator.h"
#include "Solver.h"
using namespace std;

int main()
{
	srand(time(NULL));
	Board b;
	b.display();
	vector<int> moves;
	int action;
	int win;
	do {
		Tree t;
		t.launchSearch(b,220);
		win = b.doMove(t.bestAction());
		b.display();
		moves.clear();
		b.getMove(moves);
		if (win != 0 || moves.size() == 0) {
			break;
		}
		// user
		bool test(true);
		while (test){
		try{
			std::cin>>action;
        		win = b.doMove(action);
			test = false;
		} catch (invalid_argument e) {
			test = true;
			cout << e.what() << endl;
		}}
			
			
		b.display();
		moves.clear();
		b.getMove(moves);
	} while (moves.size() > 0 && win == 0);
	return 0;
}
