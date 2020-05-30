#ifndef MAIN_H
#define MAIN_H
#include<bits/stdc++.h>
#include<locale.h>
using namespace std;

enum class state {empty, obstacle, closed, path, start, finish};
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

void printBoard(vector<vector<state>> board);
void printEmojiBoard(vector<vector<state>> board);
vector<vector<state>> readBoard(string path);
vector<state> parseLine(string line);

int getCellInt(state s);
string getCellString(state s);

vector<vector<state>> search(vector<vector<state>> board, vector<int> init, vector<int> goal);
int heuristic(int x1, int y1, int x2, int y2);
void addToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<state>> &grid);
void cellSort(vector<vector<int>> &openlist);
bool compare(const vector<int> a, const vector<int> b);
void expandNeighbors(const vector<int> &current, vector<int> goal, vector<vector<int>> &openlist, vector<vector<state>> &grid);
bool checkValidCell(int x, int y, vector<vector<state>> &grid);


#endif
