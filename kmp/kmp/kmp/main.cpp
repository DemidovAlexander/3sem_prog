//Demidov Alexander, 296 group

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

//output the position of the substring in the string
void PrintPosition(size_t position) {
	std::cout << position << ' ';
}

//count prefix function for the substring
void CountPrefixFunction(const std::string& substring, std::vector<size_t>* prefixFunction) {
	(*prefixFunction)[0] = 0;

	for (size_t currentPosition = 1; currentPosition < substring.length(); ++currentPosition) {
		size_t sampleLength = (*prefixFunction)[currentPosition - 1];
		while (sampleLength > 0 && substring[currentPosition] != substring[sampleLength]) {
			sampleLength = (*prefixFunction)[sampleLength - 1];
		}
		if (substring[currentPosition] == substring[sampleLength]) {
			++sampleLength;
		}
		(*prefixFunction)[currentPosition] = sampleLength;
	}
}

//search positions of the substring in the string
void FindSubstringPositions(std::string& substring) {
	substring += '#';
	
	std::vector<size_t> prefixFunction(substring.length());
	CountPrefixFunction(substring, &prefixFunction);

	size_t sampleLength = prefixFunction[substring.length() - 1];
	size_t currentPosition = 0;

	char currentSymbol = getchar();
	while (currentSymbol != '\n') {	
		while (sampleLength > 0 && currentSymbol != substring[sampleLength]) {
			sampleLength = prefixFunction[sampleLength - 1];
		}
		if (currentSymbol == substring[sampleLength]) {
			++sampleLength;
		}
		if (sampleLength == substring.length() - 1) {
			PrintPosition(currentPosition + 2 - substring.length());
		}
		++currentPosition;
		currentSymbol = std::getchar();
	}
}

int main() {
	std::string substring;
	std::getline(std::cin, substring);

	FindSubstringPositions(substring);

	return 0;
}