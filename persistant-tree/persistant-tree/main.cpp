#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

struct Node {
	Node * leftSon, * rightSon;
	int sum;
 
	Node (int value) : leftSon(NULL), rightSon(NULL), sum(value) {}
 
	Node (Node * left, Node * right) : leftSon(left), rightSon(right), sum(0) {
		if (leftSon) {
			sum += leftSon->sum;
		}
		if (rightSon) {
			sum += rightSon->sum;
		}
	}
};

class SegmentTree {
private:
	std::vector<Node*> roots;
	int treeLeftBound, treeRightBound;

	Node* buildTree(const std::vector<int>& data, int leftBound, int rightBound) {
		if (leftBound == rightBound) {
			return new Node(data[leftBound]);
		}
		int mid = (leftBound + rightBound) / 2;
		return new Node(buildTree(data, leftBound, mid), buildTree(data, mid + 1, rightBound));
	}

	Node* getRoot() {
		return *(roots.rbegin());
	}

	Node* updateTree(Node* currentNode, int leftBound, int rightBound, int targetPosition, int value) {
		if (leftBound == rightBound) {
			return new Node(value);
		}
		int mid = (leftBound + rightBound) / 2;
		if (targetPosition <= mid) {
			return new Node(updateTree(currentNode->leftSon, leftBound, mid, targetPosition, value), currentNode->rightSon);
		} else {
			return new Node(currentNode->leftSon, updateTree(currentNode->rightSon, mid + 1, rightBound, targetPosition, value));
		}
	}

	int getSum(Node* currentNode, int leftBound, int rightBound, int leftPosition, int rightPosition) {
		if ((rightBound < leftPosition) || (leftBound > rightPosition)) {
			return 0;
		}
		if ((leftPosition <= leftBound) && (rightBound <= rightPosition)) {
			return currentNode->sum;
		}
		int mid = (leftBound + rightBound) / 2;
		return getSum(currentNode->leftSon, leftBound, mid, leftPosition, rightPosition) + getSum(currentNode->rightSon, mid + 1, rightBound, leftPosition, rightPosition);
	}

public:
	SegmentTree(const std::vector<int>& data, int leftBound, int rightBound) {
		treeLeftBound = leftBound;
		treeRightBound = rightBound;
		roots.push_back(buildTree(data, treeLeftBound, treeRightBound));
	}

	int getRootsNumber() {
		return roots.size();
	}

	int getSumForRoot(int currentRootNumber, int leftPosition, int rightPosition) {
		return getSum(roots[currentRootNumber], treeLeftBound, treeRightBound, leftPosition, rightPosition);
	}

	void updatePosition(int targetPosition, int value) {
		roots.push_back(updateTree(getRoot(), treeLeftBound, treeRightBound, targetPosition, value));
	}
};

void CountStatistics(const std::vector<std::pair<int, int> > &values) {
	std::vector<std::pair<int, int> > sortedValues(values);
	std::sort(sortedValues.begin(), sortedValues.end());

	std::vector<int> initializationData(values.size(), 0); 
	SegmentTree tree(initializationData, 0, initializationData.size() - 1);

    for (size_t position = 0; position < values.size(); ++position) {
        tree.updatePosition(sortedValues[position].second, 1);
    }

    int requestsNumber;
    std::cin >> requestsNumber;
    for (int position = 0; position < requestsNumber; ++position) {
        int leftPosition, rightPosition, statNumber;
        std::cin >> leftPosition >> rightPosition >> statNumber;

        int left = 0;
        int right = tree.getRootsNumber();
        while (left < right) {
			int mid = (left + right) / 2;
            int sum = tree.getSumForRoot(mid, leftPosition, rightPosition);
            if (sum < statNumber) { 
				left = mid + 1;
            } else {
                right = mid;
            }
        }

		std::cout << values[sortedValues[right - 1].second].first << '\n';
    }
}

void ReadData(std::vector<std::pair<int, int> >* values) {
	int elementsNumber;
	std::cin >> elementsNumber;
	for (int position = 0; position < elementsNumber; ++position) {
		int currentElement;
		std::cin >> currentElement;
		(*values).push_back(std::make_pair(currentElement, position));
	}
}

int main() {
	std::vector<std::pair<int, int> > values;
    ReadData(&values);

	CountStatistics(values);

	return 0;
}