#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream file("input.txt");

    if (!file.is_open()) {
        cerr << "Error in file opening" << endl;
        return -1;
    }

    vector<int> firstList;
    vector<int> secondList;
    
    int num1, num2;
    while (file >> num1 >> num2) {
        firstList.push_back(num1);
        secondList.push_back(num2);
    }

    file.close();

    stable_sort(firstList.begin(), firstList.end());
    stable_sort(secondList.begin(), secondList.end());

    int totalDistance = 0;
    int totalSimilarity = 0;

    int index = 0;
    for (const int& i : firstList) {
        //calculate distance
        totalDistance += max(firstList[index], secondList[index]) - min(firstList[index], secondList[index]);
        //calculate similarity
        totalSimilarity += i * count(secondList.begin(), secondList.end(), i);
        index++;
    }
    
    cout << "Distance: " << totalDistance << endl;
    cout << "Similarity: " << totalSimilarity << endl;

    return 0;
}