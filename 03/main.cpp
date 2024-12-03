#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include <regex>

using namespace std;

string cleanInput(string input);

int main() {
    ifstream file("input.txt");
    if (!file.is_open()) {
        cerr << "Error in file opening" << endl;
        return -1;
    }

    string input((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    file.close();

    int total = 0;
    regex matchMul(R"(mul\((\d{1,3}),(\d{1,3})\))");

    smatch match;
    string::const_iterator searchStart(input.cbegin());

    while (regex_search(searchStart, input.cend(), match, matchMul)) {
        total += (stoi(match[1])*stoi(match[2]));
        searchStart = match.suffix().first;
    }

    cout << "Result part one: " << total << endl;

    //reset iterator
    total = 0;
    input = cleanInput(input);
    searchStart = input.cbegin();

    while (regex_search(searchStart, input.cend(), match, matchMul)) {
        total += (stoi(match[1])*stoi(match[2]));
        searchStart = match.suffix().first;
    }

    cout << "Result part two: " << total << endl;

    return 0;
}

//Clean input removing everything between don't() and do()
string cleanInput(string input) {
    string cleanedInput;
    size_t pos = 0;

    while (pos < input.size()) {
        size_t dontPos = input.find("don't()", pos);
        if (dontPos == string::npos) {
            cleanedInput += input.substr(pos);
            break;
        }

        cleanedInput += input.substr(pos, dontPos - pos);

        size_t doPos = input.find("do()", dontPos);
        if (doPos == string::npos) {
            break;
        }

        pos = doPos + 4; // do() is 4 char long
    }

    return cleanedInput;
}