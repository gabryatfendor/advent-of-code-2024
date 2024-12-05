#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

bool isUpdateOrdered(vector<int> update, vector<vector<int>> orderingRules);
int middleFixedUpdate(vector<int> update, vector<vector<int>> orderingRules);

int main() {
    vector<vector<int>> orderingRules;
    vector<vector<int>> updates;
    int updateMiddleSum = 0;
    int fixedUpdateMiddleSum = 0;

    //import data
    ifstream file("input.txt");
    if (!file.is_open()) {
        cerr << "Error in file opening" << endl;
        return -1;
    }

    string line;
    while (getline(file, line)) {
        if (line.find('|') != string::npos) {
            stringstream ss(line);
            string page;
            vector<int> numberedPages;
            while (getline(ss, page, '|')) {
                numberedPages.push_back(stoi(page));
            }
            orderingRules.push_back(numberedPages);
        } else if (line.find(',') != string::npos) {
            stringstream ss(line);
            string page;
            vector<int> numberedPages;
            while (getline(ss, page, ',')) {
                numberedPages.push_back(stoi(page));
            }
            updates.push_back(numberedPages);
        }
    }

    file.close();

    for (const auto& update : updates) {
        if (isUpdateOrdered(update, orderingRules)) {
            updateMiddleSum += update[(update.size()/2)];
        } else {
            fixedUpdateMiddleSum += middleFixedUpdate(update, orderingRules);
        }
    }

    cout << updateMiddleSum << endl;
    cout << fixedUpdateMiddleSum << endl;

    return 0;
}

bool isUpdateOrdered(vector<int> update, vector<vector<int>> orderingRules) {
    //stupid algorithm
    //read every rule, check if is correct
    for (const auto& rule : orderingRules) {
        auto first = find(update.begin(), update.end(), rule[0]);
        auto second = find(update.begin(), update.end(), rule[1]);

        if (first != update.end() && second != update.end()) {
            if (second < first) {
                return false;
            }
        }
    }

    return true;
}

int middleFixedUpdate(vector<int> update, vector<vector<int>> orderingRules) {
    vector<int> modifiableUpdate = update;
    bool stillToFix = true;
    //stupid algorithm
    //read every rule, when it's not correct switch places between the 2 elements
    //use stillToFix bool to exit loop
    while (stillToFix) {
        stillToFix = false;
        for (const auto& rule : orderingRules) {
                auto first = find(modifiableUpdate.begin(), modifiableUpdate.end(), rule[0]);
                auto second = find(modifiableUpdate.begin(), modifiableUpdate.end(), rule[1]);

                if (first != modifiableUpdate.end() && second != modifiableUpdate.end()) {
                    if (second < first) {
                        int index1 = distance(modifiableUpdate.begin(), first);
                        int index2 = distance(modifiableUpdate.begin(), second);
                        modifiableUpdate[index1] = rule[1];
                        modifiableUpdate[index2] = rule[0];
                        stillToFix = true;
                    }
                }
            }
    }

    return modifiableUpdate[(modifiableUpdate.size()/2)];;
}