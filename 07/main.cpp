#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

bool isResultAchivable(vector<long long int> elements, int index, long long int currentRes, long long int result);
long long int concatenateNumbers(long long int a, long long int b);

int main() {
    vector<vector<long long int>> equationsList;
    ifstream file("input.txt");
    if (!file.is_open()) {
        cerr << "Error in file opening" << endl;
        return -1;
    }

    string line;

    while (getline(file, line)) {
        //remove useless ':'
        line.erase(remove(line.begin(), line.end(), ':'), line.end());
        stringstream ss(line);
        vector<long long int> equation;
        long long int number;

        while (ss >> number) {
            equation.push_back(number);
        }

        equationsList.push_back(equation);
    }

    file.close();

    long long int fixableEquationsResult = 0;
    for (vector<long long int> equation : equationsList) {
        long long int result = equation[0];
        vector<long long int> elements(equation.begin() + 1, equation.end());
        if(isResultAchivable(elements, 1, elements[0], result)) {
            fixableEquationsResult += result;
        }
    }

    cout << "Fixable equations with + and * and || total: " << fixableEquationsResult << endl;

    return 0;
}

bool isResultAchivable(vector<long long int> elements, int index, long long int currentRes, long long int result) {
    if (index == elements.size()) {
        return currentRes == result;
    }

    long long int currentNumber = elements[index];

    if (isResultAchivable(elements, index + 1, currentRes + currentNumber, result)) {
        return true;
    }

    if (isResultAchivable(elements, index + 1, currentRes * currentNumber, result)) {
        return true;
    }

    long long int concatenatedNumber = currentNumber;
    for (size_t i = index + 1; i < elements.size(); i++) {
        if (i < elements.size()) {
            concatenatedNumber = concatenateNumbers(concatenatedNumber, elements[i]);
        }

        vector<long long int> newElements = elements;
        newElements[index] = concatenatedNumber;

        if (index + 1 < newElements.size() && i + 1 <= newElements.size()) {
            newElements.erase(newElements.begin() + index + 1, newElements.begin() + i + 1);
        }
        
        if (isResultAchivable(newElements, index + 1, currentRes, result)) {
            return true;
        }
    }

    return false;
}

long long int concatenateNumbers(long long int a, long long int b) {
    long long int numDigits = floor(log10(b)) + 1;
    return a * pow(10, numDigits) + b;
}