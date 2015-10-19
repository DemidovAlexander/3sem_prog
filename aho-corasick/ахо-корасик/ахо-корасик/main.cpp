#include <iostream>
#include <vector>
#include <string>

const int NUM_OF_SYMBOLS = 26;

class Node {
public:
	Node* childrenNodes[NUM_OF_SYMBOLS];    
	Node* transition[NUM_OF_SYMBOLS];   
	std::vector<int> terminalPatternPositions;
	Node* parent;        
	Node* suffixLink;       
	Node* compressedSuffixLink;             
	int charToParent;   
	bool isTerminal;            
};

class Automaton {
private:
	Node* root;

public:
	Automaton() {
		root = new Node;
		for (int nextSymbol = 0; nextSymbol < 26; ++nextSymbol) {
			root->childrenNodes[nextSymbol] = 0;
			root->transition[nextSymbol] = 0;
		}
		root->parent = root;
		root->suffixLink = root;
		root->compressedSuffixLink = root;
		root->isTerminal = false;
	}

	void addPatternToTree(const std::string& pattern, int patternPosition) {
		Node* currentNode = root;
		for (int currentPosition = 0; currentPosition < (int)pattern.length(); ++currentPosition) {
			char symbol = pattern[currentPosition] - 'a';
			if (currentNode->childrenNodes[symbol] == 0) {
				currentNode->childrenNodes[symbol] = new Node;
				for (int nextSymbol = 0; nextSymbol < 26; ++nextSymbol) {
					currentNode->childrenNodes[symbol]->childrenNodes[nextSymbol] = 0;
					currentNode->childrenNodes[symbol]->transition[nextSymbol] = 0;
				}
				currentNode->childrenNodes[symbol]->suffixLink = 0;
				currentNode->childrenNodes[symbol]->compressedSuffixLink = 0;
				currentNode->childrenNodes[symbol]->parent = currentNode;
				currentNode->childrenNodes[symbol]->charToParent = symbol;
				currentNode->childrenNodes[symbol]->isTerminal = false;
			}
			currentNode = currentNode->childrenNodes[symbol];
		}
		currentNode->isTerminal = true;
		currentNode->terminalPatternPositions.push_back(patternPosition);
	}

	void lookThroughText(const std::string& text, std::vector<int>* patternParts) {  
		Node* currentNode = root;
		for (int position = 0; position < (int)text.length(); ++position) {
			char symbol = text[position] - 'a';
			currentNode = getTransition(currentNode, symbol);
			Node* checkNode = currentNode;
			while (checkNode != root) {
				for (int currentPatternPosition = 0; currentPatternPosition < (int)checkNode->terminalPatternPositions.size(); ++currentPatternPosition) {
					if (position - checkNode->terminalPatternPositions[currentPatternPosition] >= 0) {
						++(*patternParts)[position - checkNode->terminalPatternPositions[currentPatternPosition]];
					}
				}
				checkNode = getCompressedSuffixLink(checkNode);
			}
		}
	}

private:
	Node* getSuffixLink(Node* currentNode) {
		if (!currentNode->suffixLink) {  
			if ((currentNode == root) || (currentNode->parent == root)) {
				currentNode->suffixLink = root;
			} else {
				currentNode->suffixLink = getTransition(getSuffixLink(currentNode->parent), currentNode->charToParent);
			}
		}
		return currentNode->suffixLink;
	}

	Node* getTransition(Node* currentNode, char symbol) {
		if (!currentNode->transition[symbol]) {     
			if (currentNode->childrenNodes[symbol]) {
				currentNode->transition[symbol] = currentNode->childrenNodes[symbol];
			} else if (currentNode == root) {
				currentNode->transition[symbol] = root;
			} else {
				currentNode->transition[symbol] = getTransition(getSuffixLink(currentNode), symbol);
			}
		}
		return currentNode->transition[symbol];
	}

	Node* getCompressedSuffixLink(Node* currentNode) {
		if (!currentNode->compressedSuffixLink) {         
			if (getSuffixLink(currentNode)->isTerminal) {
				currentNode->compressedSuffixLink = getSuffixLink(currentNode);
			} else if (getSuffixLink(currentNode) == root) {
				currentNode->compressedSuffixLink = root;
			} else {
				currentNode->compressedSuffixLink = getCompressedSuffixLink(getSuffixLink(currentNode));
			}
		}
		return currentNode->compressedSuffixLink;
	}
};

void addPattern(Automaton* currentAutomaton, int* numberOfParts, int* patternLength, int* startMask) {
	std::string pattern;
	std::cin >> pattern;

	int position = 0;
	while (position < (int)pattern.length() && pattern[position] == '?') {
		++(*startMask);
		++position;
	}

	(*patternLength) = pattern.length();
	int leftPosition = 0;
	for (position = 0; position < (int)pattern.length(); ++position) {
		if (pattern[position] == '?') {
			if (leftPosition < position) {
				(*currentAutomaton).addPatternToTree(pattern.substr(leftPosition, position - leftPosition), position - 1);
				++(*numberOfParts);
			}
			leftPosition = position + 1;
		}
	}

	if (leftPosition < (int)pattern.length()) {
		(*currentAutomaton).addPatternToTree(pattern.substr(leftPosition), pattern.length() - 1);
		++(*numberOfParts);
	}
}

void findPatterns(Automaton* currentAutomaton, std::vector<int>* patternParts) {
	std::string text;
	std::cin >> text;
	(*patternParts).resize(text.length());
	currentAutomaton->lookThroughText(text, patternParts);
}

void printAnswer(const std::vector<int>& patternParts, const int& numberOfParts, const int& patternLength, const int& startMask) {
	bool start = true;
	if (startMask == patternLength) {
		for (int i = 0; i < (int)patternParts.size(); ++i) {
			if (i + patternLength <= (int)patternParts.size()) {
				if (!start) {
					std::cout << " ";
				}
				std::cout << i;
				start = false;
			}
		}
	} else {
		for (int i = 0; i < (int)patternParts.size(); ++i) {
			if ((patternParts[i] == numberOfParts) && (i + patternLength <= (int)patternParts.size()) && (i - startMask >= 0)) {
				if (!start) {
					std::cout << " ";
				}
				std::cout << i;
				start = false;
			}
		}
	}
}

int main() {
	Automaton* currentAutomaton = new Automaton();
	int numberOfParts = 0;
	int patternLength;
	int startMask = 0;
	addPattern(currentAutomaton, &numberOfParts, &patternLength, &startMask);

	std::vector<int> patternParts;
	findPatterns(currentAutomaton, &patternParts);

	printAnswer(patternParts, numberOfParts, patternLength, startMask);
	return 0;
}