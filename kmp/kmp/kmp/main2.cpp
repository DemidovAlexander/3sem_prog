#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

vector<size_t> CountPrefixFunction(string substring) {
	vector<size_t> prefixFunction(substring.length());
	prefixFunction[0] = 0;

	for (size_t currentPosition = 1; currentPosition < substring.length(); ++currentPosition) {
		size_t sampleLength = prefixFunction[currentPosition - 1];
		while (sampleLength > 0 && substring[currentPosition] != substring[sampleLength]) {
			sampleLength = prefixFunction[sampleLength - 1];
		}
		if (substring[currentPosition] == substring[sampleLength]) {
			++sampleLength;
		}
		prefixFunction[currentPosition] = sampleLength;
	}

	return prefixFunction;
}

void FindSubstringPositions(string substring, size_t strLength) {
	substring += '#';
	
	char currentSymbol;
	vector<size_t> prefixFunction = CountPrefixFunction(substring);

	size_t sampleLength = prefixFunction[substring.length() - 1];

	for (size_t currentPosition = 0; currentPosition < strLength; ++currentPosition) {
		cin >> currentSymbol;		
		while (sampleLength > 0 && currentSymbol != substring[sampleLength]) {
			sampleLength = prefixFunction[sampleLength - 1];
		}
		if (currentSymbol == substring[sampleLength]) {
			++sampleLength;
		}
		if (sampleLength == substring.length() - 1) {
			cout << currentPosition + 2 - substring.length() << ' ';
		}
	}
}

int main() {
	string substring;
	cin >> substring;
	
	size_t strLength;
	cin >> strLength;

	FindSubstringPositions(substring, strLength);

	return 0;
}