#include "Solver.h"

#include <math.h>
#include <vector>
#include <cstdlib> //random
#include <iostream>
#include <stdexcept>

extern const int N_ACTIONS;
const double UCT_COEFF(0.7); // sqrt(1/2)
const double WIN_REWARD(1);
const double TIE_REWARD(0.5);
const double LOST_REWARD(0);

Node::Node(State &state, int player) : id(0), m_state(state), m_nEvaluation(0), m_sumRewards(0),  m_parentNode(-1), m_player(player), m_action(-1){
	state.getMove(m_actions);
}

Node::Node(State &state, int parentNode, int player, std::vector<Node> &nodes, int action) : m_state(state), m_nEvaluation(0), m_sumRewards(0), m_parentNode(parentNode), m_player(player), m_action(action) {
	id = nodes.size();
	nodes[parentNode].addChild(id);
	state.getMove(m_actions);
}

bool Node::isFullyExpanded() const {
	return (m_actions.size() == 0);
}

void Node::addChild(int child) {
	m_children.push_back(child);
}

bool Node::isRoot() const {
	return (m_parentNode == -1);
}

bool Node::isTerminal() const {
	std::vector<int> moves;
	m_state.getMove(moves);
	return (moves.empty() || m_state.getWinner()!=0);
}

int Node::getN() const {
	return m_nEvaluation;
}

int Node::getRandomAction() {
	int index(rand() % m_actions.size());
	int action = m_actions[index];
	m_actions.erase(m_actions.begin() + index);
	return action;
}

State Node::getState() const {
	return m_state;
}

int Node::getPlayer() const {
	return m_player;
}	

void Node::representation() const {
	//m_state.display();
	std::cout << "Node: " << id << std::endl << "Children: ";
	for (unsigned long int i=0; i<m_children.size(); i++) {
		std::cout << m_children[i] << ", ";
	}
	std::cout << std::endl << "Parent: " << m_parentNode << std::endl;
	std::cout << "Action : " << m_action << std::endl;
	std::cout << "Neval: " <<  m_nEvaluation << ", sumR " << m_sumRewards << ", player:" << m_player << std::endl << std::endl;
}

void Node::addReward(double reward) {
	m_sumRewards += reward;
	m_nEvaluation++;
}

int Node::getParent() const {
	return m_parentNode;
}

double Node::getUCT(int nEvaluationParent) const {
	if (this->isRoot()){
		throw std::invalid_argument("Impossible to call getUCT on the root node");
	}
	double exploration = UCT_COEFF*sqrt(2*log(nEvaluationParent)/m_nEvaluation);
	//std::cout << UCT_COEFF << nEvaluationParent << m_nEvaluation << std::endl;
	double exploitation = m_sumRewards/m_nEvaluation;
	//std::cout << "Exploration : " << exploration << ", Exploitation : " << exploitation << std::endl;
	return exploitation + exploration;
}

unsigned long Node::sizeChildren() const {
	return m_children.size();
}

int Node::getChild(int index) const {
	return m_children[index];
}

double Node::getValue() const {
	return m_sumRewards / m_nEvaluation;
}

int Node::getAction() const {
	return m_action;
}

Tree::Tree(): m_iteration(0){
}

void Tree::launchSearch(Board boardInit, int budget) {
	m_playerIA = boardInit.getCurrentPlayer();
	State rootState(boardInit.getState());
	int currentPlayer(boardInit.getCurrentPlayer());
	m_nodes.push_back(Node(rootState, currentPlayer));

	int nodeToExpand(0);
	int step(0);
	while(m_iteration<budget) {
		m_iteration++;
		nodeToExpand = this->treePolicy();
		if (!m_nodes[nodeToExpand].isFullyExpanded()) {
			// Expand
			int action (m_nodes[nodeToExpand].getRandomAction());
			State temp(m_nodes[nodeToExpand].getState());
			currentPlayer = m_nodes[nodeToExpand].getPlayer();
		
			temp.doMove(action, currentPlayer);
			currentPlayer = currentPlayer % 2 + 1;
			m_nodes.push_back(Node(temp, nodeToExpand, currentPlayer, m_nodes, action));

			//Reward
			Board b(m_nodes.back().getState(), m_nodes.back().getPlayer());
			int winner(b.randomPlays());
			this->backUp(winner, m_nodes.size()-1);
			step++;
		}
		else {
			//Reward
			Board b(m_nodes[nodeToExpand].getState(), m_nodes[nodeToExpand].getPlayer());
			int winner(b.randomPlays());
			this->backUp(winner, nodeToExpand);
			step++;
		}
	}

	for(unsigned long i=0; i<m_nodes.size(); i++){
		//m_nodes[i].representation();
	}
}

int Tree::bestAction() const {
	
	int best(-1);
	double bestValue(-999);
	double value;
	for(unsigned long i=0; i<m_nodes.front().sizeChildren(); i++){
		value = m_nodes[m_nodes.front().getChild(i)].getValue();
		std::cout << "Node: " << m_nodes[m_nodes.front().getChild(i)].getAction() << " value : " << value <<std::endl;
		if (value > bestValue){
			best = m_nodes.front().getChild(i);
			bestValue = value;
		}
	}
	std::cout << " best : " << best << " action " << m_nodes[best].getAction() << std::endl;
	return m_nodes[best].getAction();
}

int Tree::treePolicy() {
	int node(-1);
	int parent(0);
	do {
		if (!m_nodes[parent].isFullyExpanded() && rand()%10<8){
			return parent;
		}
		node = this->getBestChild(parent);
		if (node == parent) {
			if (m_nodes[parent].isFullyExpanded()) {
				return node;
			} else {
				return parent;
			}
		}
		if (m_nodes[node].isTerminal()) {
			if (m_nodes[parent].isFullyExpanded()) {
				return node;
			} else {
				return parent;
			}
		}
		parent = node;
	}while(true);
}

void Tree::backUp(double winner, int node) {
	if (winner == m_nodes[node].getPlayer()) {
		m_nodes[node].addReward(LOST_REWARD);
	}
	else if (winner == 0){
		m_nodes[node].addReward(TIE_REWARD);
	}
	else {
		m_nodes[node].addReward(WIN_REWARD);
	}
	if (!m_nodes[node].isRoot()) {
		this->backUp(winner, m_nodes[node].getParent());
	}	

}


int Tree::getBestChild(int node) {
	if (m_nodes[node].sizeChildren() == 0) {
		return node;

	}
	double maxUCT(-999999);
	double UCT;
	int index(-1);
	int nEvaluation = m_nodes[node].getN();
	for (unsigned long i=0; i<m_nodes[node].sizeChildren(); i++) {
		UCT = m_nodes[m_nodes[node].getChild(i)].getUCT(nEvaluation);
		if (UCT > maxUCT) {
			maxUCT = UCT;
			index = i;
		}
	}
	return m_nodes[node].getChild(index);
}
