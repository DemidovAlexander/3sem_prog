//Demidov Alexander, 296 group

#include "iostream"
#include "string"
#include "vector"

int main() {
	std::string inputString;
	std::cin >> inputString;
	
	std::vector<std::vector<int> > dynamicResults;
	dynamicResults.assign(inputString.length(), std::vector<int>(inputString.length(), 0));

	for (int position = 0; position < inputString.length(); ++position) {
		dynamicResults[position][position] = 1; //start with 1 on the main diagonal
	}

	//dynamic count max palindrome length
	for (int column = 0; column < inputString.length(); ++column) {
		for (int row = inputString.length() - 1; row >= 0; --row) {
			if (row > column) {
				dynamicResults[row][column] = 0;
			} else if (row < column) {
				if (inputString[row] == inputString[column]) {
					dynamicResults[row][column] = std::max(dynamicResults[row + 1][column - 1] + 2, std::max(dynamicResults[row][column - 1], dynamicResults[row + 1][column]));
				} else {
					dynamicResults[row][column] = std::max(dynamicResults[row][column - 1], dynamicResults[row + 1][column]);
				}
			}
		}
	}
	
	//get palindrome
	int row = 0;
	int column = inputString.length() - 1;
	std::string answer = "";
	int position = 0;
	int offset = 0; //indicate a single character in the middle of our palindrome
	if (dynamicResults[0][inputString.length() - 1] % 2 != 0) {
		offset = 1;
	}
	int gotLetters = dynamicResults[0][inputString.length() - 1] / 2 + offset;

	while (position < gotLetters) {
		if (inputString[row] == inputString[column]) {
			answer += inputString[row];
			++row;
			--column;
			++position;
		} else if (dynamicResults[row][column - 1] == std::max(dynamicResults[row][column - 1], dynamicResults[row + 1][column])) {
			--column;
		} else {
			++row;
		}
	}

	//reflect the string
	for (int position = answer.length() - offset - 1; position >= 0; --position) {
		answer += answer[position];
	}

	std::cout << answer.length() << '\n';
	std::cout << answer << '\n';

	return 0;
}

