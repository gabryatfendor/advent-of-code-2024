#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

bool isReportSafe(vector<int> numbers);

int main() {
    int safeReports = 0;

    ifstream file("input.txt");
    if (!file.is_open()) {
        cerr << "Error in file opening" << endl;
        return -1;
    }

    string line;
    while (getline(file, line)) {        
        stringstream ss(line);
        vector<int> numbers;

        int num;
        while (ss >> num) {
            numbers.push_back(num);
        }

        if (isReportSafe(numbers)) {
            safeReports++;
        } else { //part 2: dampener
            for (size_t i = 0; i < numbers.size(); ++i) {
                //copy with removed element
                vector<int> numbersPermutation = numbers;
                numbersPermutation.erase(numbersPermutation.begin() + i);

                //call funcion on permutation
                if (isReportSafe(numbersPermutation)) {
                    safeReports++;
                    break;
                }
            }
        }
    }

    file.close();

    cout << safeReports << endl;
    return 0;
}

bool isReportSafe(vector<int> numbers) {
    bool isIncreasing = true;
    for (size_t i = 0; i < numbers.size() - 1; i++) {
        //first check: increasing/decresing
        if (i == 0) {
            //set increasing/decreasing
            numbers[0] > numbers[1] ? isIncreasing = false : isIncreasing = true; 
        } else {
            if ((isIncreasing && numbers[i] > numbers[i+1]) || (!isIncreasing && numbers[i] < numbers[i+1])) {
                //not safe since it changed increasing/decreasing trend, we skip
                return false;
            }
        }

        //second check: difference between element between 1 and 3
        int difference = abs(numbers[i]-numbers[i+1]);
        if (difference < 1 || difference > 3) {
            //difference too high, we skip
            return false;
        }
    }

    return true;
}