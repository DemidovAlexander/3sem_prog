//Demidov Alexander, 296 group

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

void ZFunctionFromString(const std::string& processedString, std::vector<size_t>* zFunction) {
	(*zFunction).resize(processedString.length());
	(*zFunction)[0] = 0;
	size_t leftBorder = 0;
	size_t rightBorder = 0;

	for (size_t currentPosition = 1; currentPosition < processedString.length(); ++currentPosition) {
		if (currentPosition <= rightBorder) {
			(*zFunction)[currentPosition] = std::min(rightBorder - currentPosition + 1, (*zFunction)[currentPosition - leftBorder]);
		}
		while (((*zFunction)[currentPosition] + currentPosition < processedString.length()) 
			&& (processedString[(*zFunction)[currentPosition]] == processedString[(*zFunction)[currentPosition] + currentPosition])) {
			++(*zFunction)[currentPosition];
		}
		if ((*zFunction)[currentPosition] + currentPosition - 1 > rightBorder) {
			leftBorder = currentPosition;
			rightBorder = (*zFunction)[currentPosition] + currentPosition - 1;
		}
	}
}

void PrefixFunctionFromString(const std::string& processedString, std::vector<size_t>* prefixFunction) {
	(*prefixFunction).resize(processedString.length());
	(*prefixFunction)[0] = 0;

	for (size_t currentPosition = 1; currentPosition < processedString.length(); ++currentPosition) {
		size_t sampleLength = (*prefixFunction)[currentPosition - 1];
		while (sampleLength > 0 && processedString[currentPosition] != processedString[sampleLength]) {
			sampleLength = (*prefixFunction)[sampleLength - 1];
		}
		if (processedString[currentPosition] == processedString[sampleLength]) {
			++sampleLength;
		}
		(*prefixFunction)[currentPosition] = sampleLength;
	}
}

void StringFromPrefixFunction(const std::vector<size_t>& prefixFunction, std::string& processedString) {
	processedString = "a";
	for (size_t currentPosition = 1; currentPosition < prefixFunction.size(); ++currentPosition) { 
		if (prefixFunction[currentPosition] > 0) {
			processedString += processedString[prefixFunction[currentPosition] - 1];
		} else {
			std::vector<bool> availableLetters;
			availableLetters.assign(26, false);
			size_t position = currentPosition;
			while (position != 0) {
				availableLetters[processedString[prefixFunction[position - 1]] - 'a'] = true;
				position = prefixFunction[position - 1];
			}
			size_t currentLetter = 0;
			while (availableLetters[currentLetter]) {
				++currentLetter;
			}
			processedString += ('a' + currentLetter);
		}
	}
}

void PrefixFromZFunction(std::vector<size_t>* function) {
	std::vector<size_t> zFunction(*function);
	(*function).assign((*function).size(), 0);
	for (size_t currentPosition = 1; currentPosition < zFunction.size(); ++currentPosition) {
		size_t prefixFunctionPosition = currentPosition + zFunction[currentPosition] - 1;
		(*function)[prefixFunctionPosition] = std::max((*function)[prefixFunctionPosition], zFunction[currentPosition]);
	}

	if ((*function).size() > 1) { 
		for (size_t currentPosition = (*function).size() - 2; currentPosition > 0; --currentPosition) {
			if ((*function)[currentPosition + 1] != 0) {
				(*function)[currentPosition] = std::max((*function)[currentPosition], (*function)[currentPosition + 1] - 1);
			}
		}
	}
}

void InputFunction(std::vector<size_t>* function) {
	size_t length;
	std::cin >> length;
	(*function).resize(length);
	for (size_t position = 0; position < length; ++position) {
		std::cin >> (*function)[position];
	}
}

void PrintFunctionResult(const std::vector<size_t>& functionResult) {
	for (size_t position = 0; position < functionResult.size(); ++position) {
		std::cout << functionResult[position] << ' ';
	}
}

int main() {
	int mode;
	std::cin >> mode;

	std::string processedString;
	std::vector<size_t> functionResult;

	switch (mode) {
	case 1: //z-function
		std::cin >> processedString;
		ZFunctionFromString(processedString, &functionResult);
		PrintFunctionResult(functionResult);
		break;
	case 2: //prefix function
		std::cin >> processedString;
		PrefixFunctionFromString(processedString, &functionResult);
		PrintFunctionResult(functionResult);
		break;
	case 3: //str from prefix function
		InputFunction(&functionResult);
		StringFromPrefixFunction(functionResult, processedString);
		std::cout << processedString;
		break;
	case 4: //str from z-function
		InputFunction(&functionResult);		
		PrefixFromZFunction(&functionResult);
		StringFromPrefixFunction(functionResult, processedString);
		std::cout << processedString;
		break;
	case 5: //prefix function from z-function
		InputFunction(&functionResult);
		PrefixFromZFunction(&functionResult);
		PrintFunctionResult(functionResult);
		break;
	case 6: //z-function from prefix function
		InputFunction(&functionResult);
		StringFromPrefixFunction(functionResult, processedString);
		ZFunctionFromString(processedString, &functionResult);
		PrintFunctionResult(functionResult);
		break;
	}

	return 0;
}