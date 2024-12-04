#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

const string XMAS = "XMAS";
const string SAMX = "SAMX";
const string MAS = "MAS";

int searchInLine(string line, string target);
int searchDiagonally(vector<string> matrix, string target);
int checkDiagonal(vector<string> input, string target, int rows, int columns, int startRow, int startCol, int dRow, int dCol);
int searchXShaped(vector<string> input, string target); 

int main() {
    //import data
    ifstream file("input.txt");
    if (!file.is_open()) {
        cerr << "Error in file opening" << endl;
        return -1;
    }

    vector<string> input;
    string line;

    while (getline(file, line)) {
        input.push_back(line);
    }

    file.close();

    int totalXmas = 0;
    int totalXShapedMas = 0;

    //we have to search in 8 ways
    //the two horizontal is the easiest, we simply search line by line
    //for the word XMAS and SAMX (reversed)
    for (const auto& row : input) {
        totalXmas += searchInLine(row, XMAS);
        totalXmas += searchInLine(row, SAMX);
    }

    //to search vertically we have to do the same thing but by column
    //to do this the most immediate way is to transform every "column" in a separate string
    //and pass it to the same method as before. We first have to know the number of columns
    int numberOfColumns = input[0].size();
    for (int i = 0;i < numberOfColumns;i++) {
        string composedLine = "";
        for (const auto& row : input) {
            //extract the nth element of the row and add it to the composed line
            composedLine += row[i];
        }
        //we pas the composed line to the same function
        totalXmas += searchInLine(composedLine, XMAS);
        totalXmas += searchInLine(composedLine, SAMX);
    }

    //now for the 4 diagonal ways, in this case we surely have to pass the whole 
    //matrix, since there is no easy transformation that can help us
    totalXmas += searchDiagonally(input, XMAS);
    totalXmas += searchDiagonally(input, SAMX);

    //part 2: we have to find X shaped MAS in any direction
    totalXShapedMas += searchXShaped(input, MAS);

    cout << totalXmas << endl;
    cout << totalXShapedMas << endl;

    return 0;
}

int searchInLine(string line, string target) {
    int matches = 0;
    string::size_type pos = 0;

    while ((pos = line.find(target, pos)) != string::npos) {
        matches++;
        pos += target.length();
    }

    return matches;
}

int searchDiagonally(vector<string> input, string target) {
    int matches = 0;

    int rows = input.size();
    int columns = input[0].size();

    for (int col = 0; col < columns; ++col)
        matches += checkDiagonal(input, target, rows, columns, 0, col, 1, 1); // ↘ (dRow=1, dCol=1)
    for (int row = 1; row < rows; ++row)
        matches += checkDiagonal(input, target, rows, columns, row, 0, 1, 1); // ↘ (dRow=1, dCol=1)

    // Diagonali secondarie: partono dalla prima riga e dall'ultima colonna
    for (int col = 0; col < columns; ++col)
        matches += checkDiagonal(input, target, rows, columns, 0, col, 1, -1); // ↙ (dRow=1, dCol=-1)
    for (int row = 1; row < rows; ++row)
        matches += checkDiagonal(input, target, rows, columns, row, columns - 1, 1, -1); // ↙ (dRow=1, dCol=-1)

    return matches;
}

int checkDiagonal(vector<string> input, string target, int rows, int columns, int startRow, int startCol, int dRow, int dCol) {
    int matches = 0;
    string diagonalWord;

    for (int i = 0; ; i++) {
        int r = startRow + i * dRow;
        int c = startCol + i * dCol;
        
        if (r < 0 || r >= rows || c < 0 || c >= columns)
            break;

        diagonalWord += input[r][c];

        if (diagonalWord.size() > target.length())
            diagonalWord.erase(0, 1); 

        if (diagonalWord == target)
            matches++;
    }

    return matches;
}

int searchXShaped(vector<string> input, string target) {
    string reverseTarget = target;
    reverse(reverseTarget.begin(), reverseTarget.end());

    int matches = 0;

    int rows = input.size();
    int columns = input[0].size();

    //iterate through the possible crossing points
    for (size_t row = 1; row < rows - 1; ++row)
    {
        for (size_t column = 1; column < columns - 1; ++column)
        {
            //for every center we check the needed values to create a x-shaped MAS populating the vector with every word found
            vector<string> words;

            words.push_back(string{input.at(row - 1)[column - 1], input.at(row)[column], input.at(row + 1)[column + 1]}); //top-left, bottom-right
            words.push_back(string{input.at(row - 1)[column + 1], input.at(row)[column], input.at(row + 1)[column - 1]}); //top-right, bottom-left

            matches += (words[0] == MAS || words[0] == reverseTarget) && (words[1] == MAS || words[1] == reverseTarget);
        }
    }

    return matches;
}