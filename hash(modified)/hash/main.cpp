//Demidov Alexader, 296 group.

#include <iostream>
#include <vector>
#include <string>
#include <time.h>

const int BASE = 53;

void CountPower(int base, std::vector<long long>* result) {
	(*result)[0] = 1;
	for (size_t degree = 1; degree < (*result).size(); ++degree)
		(*result)[degree] = (*result)[degree - 1] * base;	
}

void CountPrefixesHashes(const std::string processedString, const std::vector<long long> &power, std::vector<long long>* hashes) {
	for (size_t position = 0; position < processedString.length(); ++position) {
		(*hashes)[position] = (processedString[position] - 'a' + 1) * power[position];
		if (position > 0) {
			(*hashes)[position] += (*hashes)[position - 1];
		}
	}
}

long long GetSubstringHash(int leftPosition, int length, const std::vector<long long> &hashes) {
	long long result = hashes[leftPosition + length - 1];
	if (leftPosition != 0) {
		result -= hashes[leftPosition - 1];
	}
	return result;
}

bool CompareHashes(int firstPosition, int secondPosition, long long firstHash, long long secondHash, const std::vector<long long> &power) {
	if (((firstPosition <= secondPosition) && (firstHash * power[secondPosition - firstPosition] == secondHash)) || 
		((firstPosition > secondPosition) && (firstHash == secondHash * power[firstPosition - secondPosition]))) {
		return true;
	} else {
		return false;
	}
}

void FindMaxMutualSubstring(std::string* result, const std::string & firstString, const std::string & secondString, int leftBorder, int rightBorder, 
		const std::vector<long long> &firstStringHashes, const std::vector<long long> &secondStringHashes, const std::vector<long long> &power) {
	
	if (rightBorder == 0) {
		(*result) = "";
		return;
	}

	int substringLength = (leftBorder + rightBorder) / 2;

	for (size_t firstPosition = 0; firstPosition + substringLength <= firstStringHashes.size(); ++ firstPosition) {
		for (size_t secondPosition = 0; secondPosition + substringLength <= secondStringHashes.size(); ++secondPosition) {
			if (substringLength == 0) {
				FindMaxMutualSubstring(result, firstString, secondString, substringLength + 1, rightBorder, firstStringHashes, secondStringHashes, power);
				return;
			}
			long long firstSubstringHash = GetSubstringHash(firstPosition, substringLength, firstStringHashes);
			long long secondSubstringHash = GetSubstringHash(secondPosition, substringLength, secondStringHashes);
			if (CompareHashes(firstPosition, secondPosition, firstSubstringHash, secondSubstringHash, power)) {
				std::string substring = firstString.substr(firstPosition, substringLength);
				if (substring == secondString.substr(secondPosition, substringLength)) {
					(*result) = substring;
					if (substringLength < rightBorder) {	
						FindMaxMutualSubstring(result, firstString, secondString, substringLength + 1, rightBorder, firstStringHashes, secondStringHashes, power);
					}
					return;
				}
			}
		}
	}
	if (substringLength != rightBorder) {
		FindMaxMutualSubstring(result, firstString, secondString, leftBorder, substringLength - 1, firstStringHashes, secondStringHashes, power);
	}
}

void GenerateString(std::string* processedString, int len) {
	for (int position = 0; position < len; ++position) {
		(*processedString) += 'a' + rand() % 26;
	}
}

int SimpleCheck(const std::string & firstString, const std::string & secondString, const std::string & maxMutualSubstring) {
	for (int subLength = std::min(firstString.length(), secondString.length()); subLength > 0; --subLength) {
		for  (int firstPosition = 0; firstPosition + subLength <= firstString.length(); ++firstPosition) {
			for  (int secondPosition = 0; secondPosition + subLength <= firstString.length(); ++secondPosition) {
				std::string substring = firstString.substr(firstPosition, subLength + 1);
				if (substring == secondString.substr(secondPosition, subLength + 1)) {
					if (maxMutualSubstring.length() == substring.length()) {
						return substring.length();
					}
				}
			}
		}
	}
	return 0;
}

int main() {
	srand(time(NULL));
	std::string firstString;
	std::string secondString;

	for (int pos = 1; pos < 20; ++pos) {
		GenerateString(&firstString, pos); 
		GenerateString(&secondString, pos);	
		std::cout << firstString << '\n';
		std::cout << secondString << '\n';

		std::vector<long long> power(std::max(firstString.length(), secondString.length()));
		CountPower(BASE, &power);

		std::vector<long long> firstStringHashes(firstString.length());
		std::vector<long long> secondStringHashes(secondString.length());
		CountPrefixesHashes(firstString, power, &firstStringHashes);
		CountPrefixesHashes(secondString, power, &secondStringHashes);

		std::string maxMutualSubstring;
		FindMaxMutualSubstring(&maxMutualSubstring, firstString, secondString, 0, std::min(firstString.length(), secondString.length()), firstStringHashes, secondStringHashes, power);

		if (SimpleCheck(firstString, secondString, maxMutualSubstring) == 0) {			
			std::cout << "fail" << "\n\n";
		} else {
			std::cout << maxMutualSubstring << "\n\n";
		}
	}

	std::cout << "true";
	return 0;
}