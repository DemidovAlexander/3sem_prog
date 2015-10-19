//Demidov Alexander, 296

#include <iostream>
#include <vector>
#include <string>
#include <utility>

std::pair<int, int> FindMaxOddPalindrome(const std::string& observedString) { 
	std::vector<int> result(observedString.length());
	int maxResultPosition = 0;
	int leftBorderOfRightmostPal = 0;
	int	rightBorderOfRightmostPal = -1;

	for (int currentPosition = 0; currentPosition < observedString.length(); ++currentPosition) {
		int currentShift = 1;
		if (currentPosition <= rightBorderOfRightmostPal) {
			int previousResult = result[(rightBorderOfRightmostPal - currentPosition) + leftBorderOfRightmostPal];
			currentShift = std::min(previousResult, rightBorderOfRightmostPal - currentPosition) + 1;
		}
		while ((currentPosition >= currentShift) && (currentPosition + currentShift < observedString.length()) 
			&& (observedString[currentPosition + currentShift] == observedString[currentPosition - currentShift])) {
			++currentShift;
		}

		if (currentPosition + currentShift > rightBorderOfRightmostPal) {
			rightBorderOfRightmostPal = currentPosition + currentShift - 1;
			leftBorderOfRightmostPal = currentPosition - currentShift + 1;  
		}

		result[currentPosition] = currentShift - 1;

		if (result[currentPosition] >= result[maxResultPosition]) {
			maxResultPosition = currentPosition;
		}
	}
	
	return std::make_pair(result[maxResultPosition], maxResultPosition);
}

std::pair<int, int> FindMaxEvenPalindrome(const std::string& observedString) { 
	std::vector<int> result(observedString.length());
	int maxResultPosition = 0;
	int leftBorderOfRightmostPal = 0;
	int	rightBorderOfRightmostPal = -1;

	for (int currentPosition = 0; currentPosition < observedString.length(); ++currentPosition) {
		int currentShift = 1;
		if (currentPosition <= rightBorderOfRightmostPal) {
			int previousResult = result[(rightBorderOfRightmostPal - currentPosition + 1) + leftBorderOfRightmostPal];
			currentShift = std::min(previousResult, rightBorderOfRightmostPal - currentPosition + 1) + 1;
		}
		while ((currentPosition >= currentShift) && (currentPosition + currentShift - 1 < observedString.length()) 
			&& (observedString[currentPosition + currentShift - 1] == observedString[currentPosition - currentShift])) {
			++currentShift;
		}

		if (currentPosition + currentShift - 2 > rightBorderOfRightmostPal) {
			rightBorderOfRightmostPal = currentPosition + currentShift - 2;
			leftBorderOfRightmostPal = currentPosition - currentShift + 1;  
		}

		result[currentPosition] = currentShift - 1;

		if (result[currentPosition] >= result[maxResultPosition]) {
			maxResultPosition = currentPosition;
		}
	}

	return std::make_pair(result[maxResultPosition], maxResultPosition);
}


int main() {
	std::string readString;
	std::cin >> readString;

    std::pair<int, int> maxOddPalindromPositions = FindMaxOddPalindrome(readString);
	std::pair<int, int> maxEvenPalindromPositions = FindMaxEvenPalindrome(readString);
	
	if (maxOddPalindromPositions < maxEvenPalindromPositions) {
		std::cout << readString.substr(maxEvenPalindromPositions.second - maxEvenPalindromPositions.first, 2 * maxEvenPalindromPositions.first); 
	} else {
		std::cout << readString.substr(maxOddPalindromPositions.second - maxOddPalindromPositions.first, 2 * maxOddPalindromPositions.first + 1); 
	}
	
	return 0;
}