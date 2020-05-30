//============================================================================
// Name        : main.cpp
// Author      : rewrihitesh
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : Reading Filein C++, Ansi-style
//============================================================================
#include "main.hpp"

using namespace std;
int main(){
	setlocale(LC_ALL, "en_US.utf8"); // make unicode string work;
	vector<vector<state>> board=readBoard("./1.board");
	vector<int> init={0,0};
	vector<int> goal={9,11};
	cout<<"Raw Maze Board\n";
	printBoard(board);
	auto solution = search(board, init, goal);
	cout<<"\nA* algorithm path searching..\n\n";
	cout<<"Start Point: (0,0)\t Finish Point: (9,11)\n";
	printEmojiBoard(solution);
	return 0;
}

void printEmojiBoard(vector<vector<state>> board){
	for(vector<state> row:board){
			for(auto cols:row){
				cout<<getCellString(cols)<<" ";
			}
			cout<<endl;
		}
}

void printBoard(vector<vector<state>> board){

	for(vector<state> row:board){
		for(auto cols:row){
			cout<<getCellInt(cols)<<" ";
		}
		cout<<endl;
	}
}

vector<vector<state>> readBoard(string path){
	ifstream ifs(path,std::ifstream::in);
	string line;
	vector<state> row;
	vector<vector<state>> board;
	if(ifs){
		while(getline(ifs,line)){
			row=parseLine(line);
			board.push_back(row);
		}
	}else{
		cerr<<"File can't be opened";
	}
	return board;
}
vector<state> parseLine(string line){
	vector<state> row;
	istringstream ss(line);
	int i=-1;
	char c;
	while(ss>>i>>c && c==','){
		if(i==0)
		row.push_back(state::empty);
		else
		row.push_back(state::obstacle);
	}
	return row;
}

int getCellInt(state s){
	if(state::empty==s) return 0;
	return 1;
}

string getCellString(state s){
	switch(s){
	case state::finish: return "\xF0\x9F\x93\xAA";
	case state::obstacle: return "\xE2\x9B\x94";
	case state::path : return "\xF0\x9F\x9A\x94";
	case state::start: return "\xE2\x9B\xAA";
	default : return "0 ";
	}
}

// ######### Search Implementation ##########

vector<vector<state>> search(vector<vector<state>> board, vector<int> init, vector<int> goal){
	vector<vector<int>> openlist;
	int x = init[0];
	int y = init[1];
	int g = 0;
	int h = heuristic(x, y, goal[0],goal[1]);
	addToOpen(x, y, g, h, openlist, board);
	while (openlist.size() > 0) {
	    // Get the next node
	    cellSort(openlist);
	    auto current = openlist.back();
	    openlist.pop_back();
	    x = current[0];
	    y = current[1];
	    board[x][y] = state::path;

	    // Check if we're done.
	    if (x == goal[0] && y == goal[1]) {
	      board[init[0]][init[1]] = state::start;
	      board[goal[0]][goal[1]] = state::finish;
	      return board;
	    }

	    // If we're not done, expand search to current node's neighbors.
	    expandNeighbors(current, goal, openlist, board);
	  }

	  // We've run out of new nodes to explore and haven't found a path.
	  cout << "No path found!" << "\n";

	return {{}};
}

int heuristic(int x1, int y1, int x2, int y2) {
  return abs(x2 - x1) + abs(y2 - y1);
}
void addToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<state>> &board) {
  // Add node to open vector, and mark board cell as closed.
  openlist.push_back(vector<int>{x, y, g, h});
  board[x][y] = state::closed;
}

bool compare(const vector<int> a, const vector<int> b) {
  int f1 = a[2] + a[3]; // f1 = g1 + h1
  int f2 = b[2] + b[3]; // f2 = g2 + h2
  return f1 > f2;
}
void cellSort(vector<vector<int>> &openlist) {
  sort(openlist.begin(), openlist.end(), compare);
}
void expandNeighbors(const vector<int> &current, vector<int> goal, vector<vector<int>> &openlist, vector<vector<state>> &grid) {
  // Get current node's data.
  int x = current[0];
  int y = current[1];
  int g = current[2];

  // Loop through current node's potential neighbors.
  for (int i = 0; i < 4; i++) {
    int x2 = x + delta[i][0];
    int y2 = y + delta[i][1];

    // Check that the potential neighbor's x2 and y2 values are on the grid and not closed.
    if (checkValidCell(x2, y2, grid)) {
      // Increment g value and add neighbor to open list.
      int g2 = g + 1;
      int h2 = heuristic(x2, y2, goal[0], goal[1]);
      addToOpen(x2, y2, g2, h2, openlist, grid);
    }
  }
}
/**
 * Check that a cell is valid: on the grid, not an obstacle, and clear.
 */
bool checkValidCell(int x, int y, vector<vector<state>> &grid) {
  bool on_grid_x = (x >= 0 && x < (int)grid.size());
  bool on_grid_y = (y >= 0 && y < (int)grid[0].size());
  if (on_grid_x && on_grid_y)
    return grid[x][y] == state::empty;
  return false;
}
