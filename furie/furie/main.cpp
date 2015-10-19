#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <complex>
#include <utility>
#include <algorithm>

const double PI = 3.14159265359;

void FourierTransposition(std::vector< std::complex<double> >* polinomial, bool invert) {
	if ((*polinomial).size() == 1) {
        return;
	}

	std::vector<std::complex<double> > firstPolinomial((*polinomial).size() / 2); 
	std::vector<std::complex<double> > secondPolinomial((*polinomial).size() / 2);

	int newPosition = 0;
	for (size_t originalPosition = 0; originalPosition < (*polinomial).size(); originalPosition += 2) {
		firstPolinomial[newPosition] = (*polinomial)[originalPosition];
		secondPolinomial[newPosition] = (*polinomial)[originalPosition + 1]; 
		++newPosition;
	}

	FourierTransposition(&firstPolinomial, invert);
	FourierTransposition(&secondPolinomial, invert);

	double angle = 2 * PI / (*polinomial).size();
	std::complex<double> inversionFactor(1);
	if (invert) {
        angle = -angle;
		inversionFactor /= 2;
	} 

	std::complex<double> root(1);
	std::complex<double> multiplier(cos(angle), sin(angle));

	for (size_t position = 0; position < (*polinomial).size() / 2; ++position) {
		(*polinomial)[position] = (firstPolinomial[position] + root * secondPolinomial[position]) * inversionFactor;
		(*polinomial)[position + (*polinomial).size() / 2] = (firstPolinomial[position] - root * secondPolinomial[position]) * inversionFactor;
		root *= multiplier;
	}
}

size_t GetAppropriateSize(size_t polinomialSize) {
	size_t size = 1;
	while (size < polinomialSize) {
        size *= 2;   
	}
	return size * 2; 
}

void Multiply (const std::vector<size_t> &firstPolinomial, const std::vector<size_t> &secondPolinomial, std::vector<size_t>* result) {
	std::vector<std::complex<double> > firstFourierPolinomial(firstPolinomial.begin(), firstPolinomial.end());
	std::vector<std::complex<double> > secondFourierPolinomial(secondPolinomial.begin(), secondPolinomial.end());

	size_t size = GetAppropriateSize(std::max(firstPolinomial.size(), secondPolinomial.size()));

	firstFourierPolinomial.resize(size);
    secondFourierPolinomial.resize(size);
	(*result).resize(size);

	FourierTransposition(&firstFourierPolinomial, false);
	FourierTransposition(&secondFourierPolinomial, false);

	for (size_t position = 0; position < size; ++position) {
		firstFourierPolinomial[position] *= secondFourierPolinomial[position];
	}

	FourierTransposition(&firstFourierPolinomial, true);

	for (size_t position = 0; position < size; ++position) {
		(*result)[position] = int(firstFourierPolinomial[position].real() + 0.5);
	}
}

void Invert(const std::string &firstString, std::string &result) {
	result = "";
    for (size_t position = 0; position < firstString.length(); ++position) {
        result += 'a' + 'b' - firstString[position];
    }
}

void countCongruenceForShift(const std::string &firstString, const std::string &secondString, std::vector<size_t> &result) {
	std::vector<size_t> firstPolinom(2 * firstString.length());
	std::string copySecondString = secondString;
	std::reverse(copySecondString.begin(), copySecondString.end());
    std::vector<size_t> secondPolinom(2 * secondString.length());
    for (size_t position = 0; position < firstString.length(); ++position) {
        firstPolinom[position] = firstString[position] - 'a';
        firstPolinom[position + firstString.length()] = firstString[position] - 'a';
        secondPolinom[position] = copySecondString[position] - 'a';
        secondPolinom[position + secondString.length()] = 0;
    }
    Multiply(firstPolinom, secondPolinom, &result);
}

size_t GetAnswer(const std::string &firstString, const std::string &secondString) {
	std::vector<size_t> onesResult;
	std::vector<size_t> zerosResult;
	countCongruenceForShift(firstString, secondString, onesResult);

	std::string invertFirstString;
    std::string invertSecondString;
    Invert(firstString, invertFirstString);
    Invert(secondString, invertSecondString);
	countCongruenceForShift(invertFirstString, invertSecondString, zerosResult);

    size_t max = onesResult[0] + zerosResult[0];
    for (size_t position = 1; position < onesResult.size(); ++position) {
        if (max < onesResult[position] + zerosResult[position]) {
            max = onesResult[position] + zerosResult[position];
        }
    }

    return firstString.length() - max;
}

int main() {
    std::string firstString, secondString;
    std::getline(std::cin, firstString);
	std::getline(std::cin, secondString);

    std::cout << GetAnswer(firstString, secondString);

    return 0;
}
