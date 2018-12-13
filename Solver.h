#ifndef DEF_WORKER
#define DEF_WORKER

#include <vector>
#include "Simulator.h"

class Node{
	public:
	Node(State &state, int player);
	Node(State &state, int parentNode, int player, std::vector<Node> &nodes, int action);
	bool isFullyExpanded() const;
	void addChild(int child);
	bool isRoot() const;
	bool isTerminal() const;
	int getN() const;
	int getRandomAction();
	State getState() const;
	int getPlayer() const;
	void representation() const;
	void addReward(double reward);
	void incrementEvaluation();
	int getParent() const;
	double getUCT(int nEvaluationParent) const;
	unsigned long sizeChildren() const;
	int getChild(int index) const;
	double getValue() const;
	int getAction() const;

	private:
	int id;
	State m_state;
	int m_nEvaluation;
	double m_sumRewards;
	int m_parentNode;
	std::vector<int> m_children;
	std::vector<int> m_actions;
	int m_player;
	int m_action;

};


class Tree{
	public:
	Tree();
	void launchSearch(Board boardInit, int budget);
	int bestAction() const;
	int treePolicy();
	int expand(Node* node);
	void backUp(double winner, int node);
	int getBestChild(int node);

	
	private:
	int m_playerIA;
	int m_iteration;
	int m_budget;
	std::vector<Node> m_nodes;
};


#endif
