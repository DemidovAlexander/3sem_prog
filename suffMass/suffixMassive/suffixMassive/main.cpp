#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

const int NUMBER_OF_SYMBOLS = 27;
const char SPECIAL_SYMBOL = '`';

void PrepareForBuild(const std::string& processedString, std::vector<int>* suffixPermutations, std::vector<int>* equivalenceClasses, int* classesNumber) {
	std::vector<int> countingSymbols(NUMBER_OF_SYMBOLS, 0);
	for (int position = 0; position < (int)processedString.length(); ++position) {
		++countingSymbols[processedString[position] - SPECIAL_SYMBOL];
	}
	for (int position = 1; position < NUMBER_OF_SYMBOLS; ++position) {
		countingSymbols[position] += countingSymbols[position - 1];
	}
	for (int position = 0; position < (int)processedString.length(); ++position) {
		--countingSymbols[processedString[position] - SPECIAL_SYMBOL];
		(*suffixPermutations)[countingSymbols[processedString[position] - SPECIAL_SYMBOL]] = position;
	}

	(*equivalenceClasses)[(*suffixPermutations)[0]] = 0;
	for (int position = 1; position < (int)processedString.length(); ++position) {
		if (processedString[(*suffixPermutations)[position]] != processedString[(*suffixPermutations)[position - 1]]) {
			++(*classesNumber);
		}
		(*equivalenceClasses)[(*suffixPermutations)[position]] = (*classesNumber) - 1;
	}
}

void StableCountingSort(const int length, std::vector<int>* suffixPermutations, std::vector<int>* newSuffixPermutations, const std::vector<int>& equivalenceClasses, const int classesNumber) {
		std::vector<int> countingSymbols(length, 0);
		for (int position = 0; position < length; ++position) {
			++countingSymbols[equivalenceClasses[(*newSuffixPermutations)[position]]];
		}
		for (int position = 1; position < classesNumber; ++position) {
			countingSymbols[position] += countingSymbols[position - 1];
		}
		for (int position = length - 1; position >= 0; --position) {
			--countingSymbols[equivalenceClasses[(*newSuffixPermutations)[position]]];
			(*suffixPermutations)[countingSymbols[equivalenceClasses[(*newSuffixPermutations)[position]]]] = (*newSuffixPermutations)[position];
		}
}

void UpdateEquivalenceClasses(const int length, std::vector<int>* suffixPermutations, std::vector<int>* equivalenceClasses, int* classesNumber, const int currrentShift) {
	std::vector<int> newEquivalenceClasses(length, 0);
	newEquivalenceClasses[(*suffixPermutations)[0]] = 0;
	(*classesNumber) = 1;
	for (int position = 1; position < length; ++position) {
		int element1 = ((*suffixPermutations)[position] + currrentShift) % length;
		int element2 = ((*suffixPermutations)[position - 1] + currrentShift) % length;
		if ((*equivalenceClasses)[(*suffixPermutations)[position]] != (*equivalenceClasses)[(*suffixPermutations)[position - 1]] 
			|| (*equivalenceClasses)[element1] != (*equivalenceClasses)[element2]) {
			++(*classesNumber);
		}
		newEquivalenceClasses[(*suffixPermutations)[position]] = (*classesNumber) - 1;
	}
	(*equivalenceClasses) = newEquivalenceClasses;
}

void BuildSuffixArray(const std::string& processedString, std::vector<int>* suffixPermutations) {	
	std::vector<int> equivalenceClasses(processedString.length());
	(*suffixPermutations).resize(processedString.length());

	int classesNumber = 1;
	PrepareForBuild(processedString, suffixPermutations, &equivalenceClasses, &classesNumber);
	for (int currentShift = 1; currentShift < (int)processedString.length(); currentShift *= 2) {
		std::vector<int> newSuffixPermutations(processedString.length(), 0);
		for (int position = 0; position < (int)processedString.length(); ++position) {
			newSuffixPermutations[position] = (*suffixPermutations)[position] - currentShift;
			if (newSuffixPermutations[position] < 0) {
				newSuffixPermutations[position] += processedString.length();
			}
		}
		StableCountingSort(processedString.length(), suffixPermutations, &newSuffixPermutations, equivalenceClasses, classesNumber);
		UpdateEquivalenceClasses(processedString.length(), suffixPermutations, &equivalenceClasses, &classesNumber, currentShift);
	}
}

int compareSuffixAndSubstring(const std::string& prString, const std::string& substring, int shift, int suffixNumber) {
	while ((suffixNumber + shift < (int)prString.length()) && (shift < (int)substring.length()) && (prString[suffixNumber + shift] == substring[shift])) {
		++shift;
	}
	return shift;
}

void FindSubstring(std::vector<int>* answer, const std::string& prString, const std::string& substring, const std::vector<int>& suffixes, int lBorder, int rBorder, int lMatch, int rMatch) {
	if ((lMatch >= (int)substring.length()) && (rMatch >= (int)substring.length())) {
		for (int position = lBorder; position <= rBorder; ++position) {
			(*answer).push_back(suffixes[position]);
		}
	} else if (lBorder <= rBorder) {
		int middle = (lBorder + rBorder) / 2;
		int comparedSymbols = compareSuffixAndSubstring(prString, substring, (std::min(lMatch, rMatch)), suffixes[middle]);
		if (comparedSymbols == (int)substring.length()) {
			(*answer).push_back(suffixes[middle]);
			FindSubstring(answer, prString, substring, suffixes, middle + 1, rBorder, (std::min(comparedSymbols, rMatch)), rMatch);
			FindSubstring(answer, prString, substring, suffixes, lBorder, middle - 1, lMatch, (std::min(lMatch, comparedSymbols)));
		} else if ((suffixes[middle] + comparedSymbols == (int)prString.length()) || (prString[suffixes[middle] + comparedSymbols] < substring[comparedSymbols])) {
			FindSubstring(answer, prString, substring, suffixes, middle + 1, rBorder, (std::min(comparedSymbols, rMatch)), rMatch);
		} else {
			FindSubstring(answer, prString, substring, suffixes, lBorder, middle - 1, lMatch, (std::min(lMatch, comparedSymbols)));
		}
	}
}

void PrintAnswer(const std::vector<int>& answer) {
	for (int position = 0; position < (int)answer.size(); ++position) {
		std::cout << answer[position] << ' ';
	}
}

int main() {
	std::string processedString;
	std::cin >> processedString;
	std::vector<int> suffixPermutations;
	processedString += SPECIAL_SYMBOL;
	BuildSuffixArray(processedString, &suffixPermutations);

	std::string substring;
	std::cin >> substring;
	std::vector<int> answer;
	int lMatch = compareSuffixAndSubstring(processedString, substring, 0, suffixPermutations[0]);
	FindSubstring(&answer, processedString, substring, suffixPermutations, 0, (int)processedString.length() - 1, lMatch, 0);

	std::sort(answer.begin(), answer.end());
	PrintAnswer(answer);
	return 0;
}