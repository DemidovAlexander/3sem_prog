#include <iostream>
#include <set>
#include <vector>

int Mex(const std::vector<int>& forMex, size_t quantity) {
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

void ReadOctalNumbers(std::vector<int> * octalNumbers) {
	for (size_t position = 0; position < (*octalNumbers).size(); ++position) {
		std::cin >> (*octalNumbers)[position];
	}
}

int CountLosingGames(const std::vector<int> &octalNumbers, int quantity) {
	std::vector<int> forMex;
	std::vector<int> results(quantity + 1);
	int answer = 1;
	for (size_t currentQuantity = 1; currentQuantity <= quantity; ++currentQuantity) {
		for (size_t octalNumberIndex = 0; octalNumberIndex < octalNumbers.size(); ++octalNumberIndex) {
			if ((octalNumbers[octalNumberIndex] & 1) && (octalNumberIndex == currentQuantity)) {
				forMex.push_back(0);
			}

			if ((octalNumbers[octalNumberIndex] & 2) && (currentQuantity >= octalNumberIndex + 1)) {
				forMex.push_back(results[currentQuantity - octalNumberIndex]);
			}

			if ((octalNumbers[octalNumberIndex] & 4) && (currentQuantity >= octalNumberIndex + 2)) {
				int newQuantity = currentQuantity - octalNumberIndex;
				for (size_t firstPartSize = 1; firstPartSize <= newQuantity / 2; ++firstPartSize) {
					forMex.push_back(results[firstPartSize] ^ results[newQuantity - firstPartSize]);
				}
			}
		}
		results[currentQuantity] = Mex(forMex, currentQuantity);
		if (results[currentQuantity] == 0) {
			++answer;
		}
		forMex.clear();
	}

	return answer;
}

int main() {
	int k, n;
	std::cin >> k;

	std::vector<int> octalNumbers(k);
	ReadOctalNumbers(&octalNumbers);
	std::cin >> n;

	std::cout << CountLosingGames(octalNumbers, n);   

	return 0;
}