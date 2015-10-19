#include <iostream>
#include <map>
#include <string>
#include <vector>

class Node {
public:
	int length;
	int link;
	std::map<char,int> nextNodes;
};

class SuffixAutomaton {
public:
	std::vector<Node> automatonStates;

	SuffixAutomaton(const int length) {
		automatonStates.resize(length * 2);
		root = -1;
		currentSize = 0;
		lastAdded = 0;
		automatonStates[0].length = 0;
		automatonStates[0].link = root;
		++currentSize;
	}

	void addSymbol (char currentSymbol) {
		int currentPosition = currentSize++;
		automatonStates[currentPosition].length = automatonStates[lastAdded].length + 1;
		int position = lastAdded;
		while ((position != root) && (!(automatonStates[position].nextNodes.count(currentSymbol)))) {
			automatonStates[position].nextNodes[currentSymbol] = currentPosition;
			position = automatonStates[position].link;
		}
		if (position == root) {
			automatonStates[currentPosition].link = 0;
		} else {
			int nextPosition = automatonStates[position].nextNodes[currentSymbol];
			if (automatonStates[position].length + 1 == automatonStates[nextPosition].length) {
				automatonStates[currentPosition].link = nextPosition;
			} else {
				currentSize++; //position of created clone
				automatonStates[currentSize].length = automatonStates[position].length + 1;
				automatonStates[currentSize].nextNodes = automatonStates[nextPosition].nextNodes;
				automatonStates[currentSize].link = automatonStates[nextPosition].link;
				while ((position != root) && (automatonStates[position].nextNodes[currentSymbol] == nextPosition)) {
					automatonStates[position].nextNodes[currentSymbol] = currentSize;
					position = automatonStates[position].link;
				}
				automatonStates[nextPosition].link = automatonStates[currentPosition].link = currentSize;
			}
		}
		lastAdded = currentPosition;
	}

private:
	int currentSize;
	int lastAdded;
	int root;
};

int CountSubstrings(const SuffixAutomaton& currentAutomaton, const Node& currentNode) {
	int result = 1;
	for (std::map<char,int>::const_iterator currentIterator = currentNode.nextNodes.cbegin(); currentIterator != currentNode.nextNodes.cend(); ++currentIterator) {
		result += CountSubstrings(currentAutomaton, currentAutomaton.automatonStates[currentIterator->second]);
	}	
	return result;
}


int main() {
	std::string inputString;
	std::cin >> inputString;
	SuffixAutomaton currentAutomaton(inputString.length());
	for (int position = 0; position < inputString.length(); ++position) {
		currentAutomaton.addSymbol(inputString[position]);
	}

	std::cout << CountSubstrings(currentAutomaton, currentAutomaton.automatonStates[0]) - 1;

	return 0;
}