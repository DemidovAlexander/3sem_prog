#include <iostream>
#include <vector>
#include <utility>

const int UNDEFINED = -1;

int Mex(const std::vector<int>& forMex, int quantity) {
	std::vector<bool> used(quantity + 1);
 
	for (size_t position = 0; position < forMex.size(); ++position) {
		used[forMex[position]] = true;
	}
 
	int result;
	size_t position = 0;
	while (position < used.size()) {
		if (!(used[position])) {
			result = position;
			break;
		}
		++position;
	}
 
	return result;
}

class Graph {
public:
	std::vector<std::pair<int, std::vector<int> > > children;
	int verticesNumber;

	void initialize(int size) {
		children.resize(size);
		for (int position = 0; position < size; ++position) {
			children[position].first = UNDEFINED;
		}
	}

	void addEdge(int parent, int child) {
		children[parent].second.push_back(child);
	}

	void readGraph() {
		int edgesNumber;
		std::cin >> verticesNumber >> edgesNumber;
		initialize(verticesNumber);

		for (int edge = 0; edge < edgesNumber; ++edge) {
			int parent, child;
			std::cin >> parent >> child;
			addEdge(parent - 1, child - 1);
		}
	}

	void analizeGame() {
		bool madeUpdates = true;
		while(madeUpdates) {
			madeUpdates = false;
			for (int currentVertex = 0; currentVertex < verticesNumber; ++currentVertex) {
				if (children[currentVertex].first == UNDEFINED) {
					std::vector<int> forMex;
					for (size_t child = 0; child < children[currentVertex].second.size(); ++child) {
						if (children[children[currentVertex].second[child]].first != UNDEFINED) {
							forMex.push_back(children[children[currentVertex].second[child]].first);
						}
					}
					int nimSum = Mex(forMex, verticesNumber);

					bool haveGoodGrandsons = true;
					for (size_t child = 0; child < children[currentVertex].second.size(); ++child) {
						if (children[children[currentVertex].second[child]].first == UNDEFINED) {
							bool haveGoodChild = false;
							for (size_t grandson = 0; grandson < children[children[currentVertex].second[child]].second.size(); ++grandson) {
									if (children[children[children[currentVertex].second[child]].second[grandson]].first == nimSum) {
										haveGoodChild = true;
									}
							}
							if (!haveGoodChild) {
								haveGoodGrandsons = false;
								break;
							}
						}
					}

					if (haveGoodGrandsons) {
						children[currentVertex].first = nimSum;
						madeUpdates = true;
					}
				}
			}
		}
	}
};

int CheckFirstWin(const Graph &firstGraph, const Graph &secondGraph, int firstStartPosition, int secondStartPosition) {
	if (firstGraph.children[firstStartPosition].first == UNDEFINED) {
		if (secondGraph.children[secondStartPosition].first == UNDEFINED) {
			return 0;
		}
		for (size_t child = 0; child < firstGraph.children[firstStartPosition].second.size(); ++child) {
			if (firstGraph.children[firstGraph.children[firstStartPosition].second[child]].first == secondGraph.children[secondStartPosition].first) {
				return 1;
			}
		}
		return 0;
	}

	if (secondGraph.children[secondStartPosition].first == UNDEFINED) {
		for (size_t child = 0; child < secondGraph.children[secondStartPosition].second.size(); ++child) {
			if (secondGraph.children[secondGraph.children[secondStartPosition].second[child]].first == firstGraph.children[firstStartPosition].first) {
				return 1;
			}
		}
		return 0;
	}

	if ((firstGraph.children[firstStartPosition].first ^ secondGraph.children[secondStartPosition].first) == 0) {
		return -1;
	} else {
		return 1;
	}

	return -1;
}

void PrintAnswers(const Graph &firstGraph, const Graph &secondGraph) {
	int gamesNumber;
	std::cin >> gamesNumber;

	for (int currentGame = 0; currentGame < gamesNumber; ++currentGame) {
		int firstStartPosition, secondStartPosition;
		std::cin >> firstStartPosition >> secondStartPosition;
		int result = CheckFirstWin(firstGraph, secondGraph, firstStartPosition - 1, secondStartPosition - 1);
		if (result == 1) {
			std::cout << "first" << '\n';
		} else if (result == -1) { 
			std::cout << "second" << '\n';
		} else {
			std::cout << "draw" << '\n';
		}
	}
}

int main() {
	Graph firstGraph, secondGraph;
	firstGraph.readGraph();
	secondGraph.readGraph();

	firstGraph.analizeGame();
	secondGraph.analizeGame();

	PrintAnswers(firstGraph, secondGraph);

	return 0;
}
