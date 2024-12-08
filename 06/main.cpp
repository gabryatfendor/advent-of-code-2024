#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <set>

using namespace std;


//TODO:
// the second part is VERY slow (like it takes 30 minutes to run) because
// it's nearly a graphical solution in its inception. Think of a faster algorithm
char changeDirection(char currentDirection);
tuple<int, bool, tuple<int, int>> checkNearestBlock(tuple<int, int> pos, char direction, vector<tuple<int, int>> blockPositions, int maxX, int maxY);
tuple<bool, bool, tuple<int, int>> checkNearestBlockForLoop(tuple<int, int> pos, char direction, vector<tuple<int, int>> blockPositions, int maxX, int maxY);
bool simulationLooping(vector<string> updatedMap);

set<tuple<int, int>> visitedPositions; //since sets only store unique values we use this to save visited positions
set<tuple<int, int, char>> visitedPositionsWithDirection; 

int main() {
    //import data
    ifstream file("input.txt");
    if (!file.is_open()) {
        cerr << "Error in file opening" << endl;
        return -1;
    }

    vector<string> map;
    string line;

    while (getline(file, line)) {
        map.push_back(line);
    }

    file.close();

    int totalDistance = 0;
    char currentDirection = 'N';
    tuple<int, int> guardPosition;
    vector<tuple<int, int>> blockPositions;

    //initial setup
    int y = 0;
    for (string row : map) {
        int x = 0;
        for(char c : row) {
            switch(c) {
                case '#':
                    blockPositions.push_back(make_tuple(x,y));
                    break;
                case '^':
                    guardPosition = make_tuple(x,y);
                    break;
                default:
                    break;
            }
            x++;
        }
        y++;
    }

    visitedPositions.insert(guardPosition);
    bool outOfBounds = false;
    while (!outOfBounds) {
        tuple<int, bool, tuple<int, int>> haveWeWalkedAndHowLong = checkNearestBlock(guardPosition, currentDirection, blockPositions, map[0].size() , y);
        if(get<0>(haveWeWalkedAndHowLong) == -1) {
            cerr << "Something went wrong" << endl;
            return -1;
        }

        currentDirection = changeDirection(currentDirection);
        if (!get<1>(haveWeWalkedAndHowLong)) {
            outOfBounds = true;
        }
        guardPosition = get<2>(haveWeWalkedAndHowLong);
        totalDistance += get<0>(haveWeWalkedAndHowLong);
    }

    cout << "Total distance: " << totalDistance << endl;
    cout << "Unique positions visited " << visitedPositions.size() << endl;

    //part 2
    //for every dot in the map
    //we substitute with a #
    //we simulate a run
    //if the guard is in a loop we increase the counter
    //initial setup
    int totalLoopingConfig = 0;
    vector<string> updatedMap;
    int y2 = 0;
    int counter = 0;
    for (string row : map) {
        int x2 = 0;
        for(char c : row) {
            switch(c) {
                case '.':
                    updatedMap = map;
                    updatedMap.at(y2)[x2] = '#';
                    visitedPositionsWithDirection.clear();
                    counter++;
                    if (simulationLooping(updatedMap)) {
                        totalLoopingConfig++;
                    }
                    break;
                default:
                    break;
            }
            x2++;
        }
        y2++;
    }

    cout << "Total looping config " << totalLoopingConfig << endl;

    return 0;
}

char changeDirection(char currentDirection) {
    switch(currentDirection) {
        case 'N':
            return 'E';
        case 'E':
            return 'S';
        case 'S':
            return 'W';
        case 'W':
        default:
            return 'N';
    }
}

tuple<int, bool, tuple<int, int>> checkNearestBlock(tuple<int, int> pos, char direction, vector<tuple<int, int>> blockPositions, int maxX, int maxY) {
    tuple<int, int> posToCheck = pos;
    int distance = 0;
    switch (direction) {
        case 'N':
            while (get<1>(posToCheck) >= 0) {
                get<1>(posToCheck)--;
                if (get<1>(posToCheck) < 0) {
                    //we are out of bounds
                    get<1>(posToCheck)++;
                    return make_tuple(distance, false, posToCheck);
                }
                distance++;
                visitedPositions.insert(posToCheck);
                if (find(blockPositions.begin(), blockPositions.end(), posToCheck) != blockPositions.end()) {
                    //found block
                    distance--;
                    visitedPositions.erase(posToCheck);
                    get<1>(posToCheck)++;
                    return make_tuple(distance, true, posToCheck);
                }
            }
            break;
        case 'E':
            while (get<0>(posToCheck) <= maxX) {
                get<0>(posToCheck)++;
                if (get<0>(posToCheck) > maxX) {
                    //we are out of bounds
                    get<0>(posToCheck)--;
                    return make_tuple(distance, false, posToCheck);
                }
                distance++;
                visitedPositions.insert(posToCheck);
                if (find(blockPositions.begin(), blockPositions.end(), posToCheck) != blockPositions.end()) {
                    //found block
                    distance--;
                    visitedPositions.erase(posToCheck);
                    get<0>(posToCheck)--;
                    return make_tuple(distance, true, posToCheck);
                }
            }
            break;
        case 'S':
            while (get<1>(posToCheck) <= maxY) {
                get<1>(posToCheck)++;
                if (get<1>(posToCheck) > maxY) {
                    //we are out of bounds
                    get<1>(posToCheck)--;
                    return make_tuple(distance, false, posToCheck);
                }
                distance++;
                visitedPositions.insert(posToCheck);
                if (find(blockPositions.begin(), blockPositions.end(), posToCheck) != blockPositions.end()) {
                    //found block
                    distance--;
                    visitedPositions.erase(posToCheck);
                    get<1>(posToCheck)--;
                    return make_tuple(distance, true, posToCheck);
                }
            }
            break;
        case 'W':
            while (get<0>(posToCheck) >= 0) {
                get<0>(posToCheck)--;
                if (get<0>(posToCheck) < 0) {
                    //we are out of bounds
                    get<0>(posToCheck)++;
                    return make_tuple(distance, false, posToCheck);
                }
                distance++;
                visitedPositions.insert(posToCheck);
                if (find(blockPositions.begin(), blockPositions.end(), posToCheck) != blockPositions.end()) {
                    //found block
                    distance--;
                    visitedPositions.erase(posToCheck);
                    get<0>(posToCheck)++;
                    return make_tuple(distance, true, posToCheck);
                }
            }
            break;
        default:
            //we should never come here
            return make_tuple(-1, false, posToCheck);
    }
    //we should never come here
    return make_tuple(-1, false, posToCheck);
}

tuple<bool, bool, tuple<int, int>> checkNearestBlockForLoop(tuple<int, int> pos, char direction, vector<tuple<int, int>> blockPositions, int maxX, int maxY) {
    tuple<int, int> posToCheck = pos;
    switch (direction) {
        case 'N':
            while (get<1>(posToCheck) >= 0) {
                get<1>(posToCheck)--;
                if (get<1>(posToCheck) < 0) {
                    //we are out of bounds
                    get<1>(posToCheck)++;
                    return make_tuple(false, false, posToCheck);
                }

                //check for loop
                if (visitedPositionsWithDirection.find(make_tuple(get<0>(posToCheck), get<1>(posToCheck), direction)) != visitedPositionsWithDirection.end()) {
                    //loop
                    return make_tuple(true, false, posToCheck);
                } else {
                    visitedPositionsWithDirection.insert(make_tuple(get<0>(posToCheck), get<1>(posToCheck), direction));                
                }

                if (find(blockPositions.begin(), blockPositions.end(), posToCheck) != blockPositions.end()) {
                    //found block
                    visitedPositionsWithDirection.erase(make_tuple(get<0>(posToCheck), get<1>(posToCheck), direction));
                    get<1>(posToCheck)++;
                    return make_tuple(false, true, posToCheck);
                }
            }
            break;
        case 'E':
            while (get<0>(posToCheck) <= maxX) {
                get<0>(posToCheck)++;
                if (get<0>(posToCheck) > maxX) {
                    //we are out of bounds
                    get<0>(posToCheck)--;
                    return make_tuple(false, false, posToCheck);
                }
                
                //check for loop
                if (visitedPositionsWithDirection.find(make_tuple(get<0>(posToCheck), get<1>(posToCheck), direction)) != visitedPositionsWithDirection.end()) {
                    //loop
                    return make_tuple(true, false, posToCheck);
                } else {
                    visitedPositionsWithDirection.insert(make_tuple(get<0>(posToCheck), get<1>(posToCheck), direction));                
                }

                if (find(blockPositions.begin(), blockPositions.end(), posToCheck) != blockPositions.end()) {
                    //found block
                    visitedPositionsWithDirection.erase(make_tuple(get<0>(posToCheck), get<1>(posToCheck), direction));
                    get<0>(posToCheck)--;
                    return make_tuple(false, true, posToCheck);
                }
            }
            break;
        case 'S':
            while (get<1>(posToCheck) <= maxY) {
                get<1>(posToCheck)++;
                if (get<1>(posToCheck) > maxY) {
                    //we are out of bounds
                    get<1>(posToCheck)--;
                    return make_tuple(false, false, posToCheck);
                }
                
                //check for loop
                if (visitedPositionsWithDirection.find(make_tuple(get<0>(posToCheck), get<1>(posToCheck), direction)) != visitedPositionsWithDirection.end()) {
                    //loop
                    return make_tuple(true, false, posToCheck);
                } else {
                    visitedPositionsWithDirection.insert(make_tuple(get<0>(posToCheck), get<1>(posToCheck), direction));                
                }

                if (find(blockPositions.begin(), blockPositions.end(), posToCheck) != blockPositions.end()) {
                    //found block
                    visitedPositionsWithDirection.erase(make_tuple(get<0>(posToCheck), get<1>(posToCheck), direction));
                    get<1>(posToCheck)--;
                    return make_tuple(false, true, posToCheck);
                }
            }
            break;
        case 'W':
            while (get<0>(posToCheck) >= 0) {
                get<0>(posToCheck)--;
                if (get<0>(posToCheck) < 0) {
                    //we are out of bounds
                    get<0>(posToCheck)++;
                    return make_tuple(false, false, posToCheck);
                }
                
                //check for loop
                if (visitedPositionsWithDirection.find(make_tuple(get<0>(posToCheck), get<1>(posToCheck), direction)) != visitedPositionsWithDirection.end()) {
                    //loop
                    return make_tuple(true, false, posToCheck);
                } else {
                    visitedPositionsWithDirection.insert(make_tuple(get<0>(posToCheck), get<1>(posToCheck), direction));                
                }

                if (find(blockPositions.begin(), blockPositions.end(), posToCheck) != blockPositions.end()) {
                    //found block
                    visitedPositionsWithDirection.erase(make_tuple(get<0>(posToCheck), get<1>(posToCheck), direction));
                    get<0>(posToCheck)++;
                    return make_tuple(false, true, posToCheck);
                }
            }
            break;
        default:
            //we should never come here
            return make_tuple(false, false, posToCheck);
    }
    //we should never come here
    return make_tuple(false, false, posToCheck);
}

bool simulationLooping(vector<string> updatedMap) {
    vector<string> modifiableMap = updatedMap;
    char currentDirection = 'N';
    tuple<int, int> guardPosition;
    vector<tuple<int, int>> blockPositions;

    //initial setup
    int y = 0;
    for (string row : modifiableMap) {
        int x = 0;
        for(char c : row) {
            switch(c) {
                case '#':
                    blockPositions.push_back(make_tuple(x,y));
                    break;
                case '^':
                    guardPosition = make_tuple(x,y);
                    break;
                default:
                    break;
            }
            x++;
        }
        y++;
    }

    visitedPositions.insert(guardPosition);
    bool outOfBounds = false;
    while (!outOfBounds) {
        tuple<bool, bool, tuple<int, int>> haveWeWalkedAndHaveWeLooped = checkNearestBlockForLoop(guardPosition, currentDirection, blockPositions, modifiableMap[0].size() , y);
        if (get<0>(haveWeWalkedAndHaveWeLooped)) {
            return true;
        }
        currentDirection = changeDirection(currentDirection);
        if (!get<1>(haveWeWalkedAndHaveWeLooped)) {
            outOfBounds = true;
        }
        guardPosition = get<2>(haveWeWalkedAndHaveWeLooped);
    }
    return false;
}
