#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <list>
#include <string>

using namespace std;

class cell
{
public:
	int id;
	int wall;
	bool visited;
	int from;

	cell() {

	}

	cell(int id, int wall, bool visited, int from) {
		this->id = id;
		this->wall = wall;
		this->visited = visited;
		this->from = from;
	}
};

void DFS_iterative(vector<cell>& maze, int numC, int start, int end);
void BFS(vector<cell>& maze, int numC, int start, int end);
void DFS_recursive(vector<cell>& maze, int numC, int start, int end);



int main() {

	ifstream In("maze.txt");//the file maze.txt in in same folder as the C++ source file.

	int numR, numC;//number of rows, number of columns
	In >> numR >> numC;
	int Start, End;//beginning cell and target cell
	In >> Start >> End;

	vector<int> CellWalls(numR * numC);//CellWalls is a vector of size numR * numC .
	for (int i = 0; i < numR * numC; ++i) {
		In >> CellWalls[i];
	}
	In.close();
	cout << numR << " " << numC << endl;
	cout << Start << endl;
	cout << End << endl;
	for (auto i : CellWalls) cout << i << " ";
	cout << endl;

	vector<cell> maze(numR * numC);//vector maze with size same as cellWalls vector
	int j = 0;

	for (auto n : CellWalls) {
		//cout << "          here         == " << endl;
		//cout << n << endl;
		//cout << " maze index " << j << endl;
		cell eachCell(j, n, false, -1);
		maze[j] = eachCell;
		//cout << maze[j].wall << endl;
		j++;
	}

	DFS_iterative(maze, numC, Start, End);
	BFS(maze, numC, Start, End);
	DFS_recursive(maze, numC, Start, End);

	return 0;
}

void DFS_iterative(vector<cell>& maze, int numC, int start, int end) {
	
	cout << " In DFS Iterative " << endl;
	int s = 2, w = 1, n = 8, e = 4;
	bool checkN = 0, checkW = 0, checkS = 0, checkE = 0;
	unordered_map<int, string> neighbourMap;//to store mapping for index and its neighbours based on the given wall values
	for (int i = 0; i < maze.size(); i++) {//set flags for each direction where wall is present
		string neighbours = "";
		int input = maze[i].wall;
		//cout << input << endl;
		if (input >= n) {
			input -= n;
			checkN = 1;
		}
		if (input >= e) {
			input -= e;
			checkE = 1;
		}
		if (input >= s) {
			input -= s;
			checkS = 1;
		}
		if (input >= w) {
			input -= w;
			checkW = 1;
		}
		cout << "neighbours for : " << i << " are : ";
		/* based on the flags set neighbours in the pipe separated String where wall is not present.
		checked direction order - E-N-W-S as we have to follow S-W-N-E order for pushing the values in the stack. */
		if (!checkE) {
			neighbours += to_string(i + 1) + "|";
		}
		if (!checkN) {
			neighbours += to_string(i - numC) + "|";
		}
		if (!checkW) {
			neighbours += to_string(i - 1) + "|";
		}
		if (!checkS) {
			neighbours += to_string(i + numC) + "|";
		}
		neighbourMap[i] = neighbours;
		cout << neighbours << endl;
		checkE = checkN = checkW = checkS = 0;
		cout << endl;
	}

	size_t pos = 0;
	string delimiter = "|";
	string str;
	//cout << "delimiter =======" << delimiter << endl;
	stack<cell> S;
	cell t(start, maze[start].wall, false, start);
	//cout << "v.id =======" << t.id << endl;
	S.push(t);
	//cout << " size ==== " << S.size() << endl;
	cell v;
	//cout << " S top : " << t.id << endl;
	while (S.top().id != end) {
		v = S.top();
		//cout << "S.top == " << v.id << endl;
		S.pop();

		if (!v.visited) {
			maze[v.id].visited = true;
			cout << "visited node : " << v.id << "  = " << v.visited<< endl;
			str = neighbourMap[v.id];
			pos = 0;
			while ((pos = str.find(delimiter)) != std::string::npos) {
				int w = stoi(str.substr(0, pos));
				if (!maze[w].visited) {
					maze[w].from = v.id;
					S.push(maze[w]);
					cout << " Pushed element " << maze[w].id << endl;
				}
				str.erase(0, pos + delimiter.length());
			}
		}
	}
	for (int i = 0; i < maze.size(); i++) {
		maze[i].visited = false;
		maze[i].from = -1;
	}
}


void BFS(vector<cell>& maze, int numC, int start, int end) {

	cout << " In BFS " << endl;
	int s = 2, w = 1, n = 8, e = 4;
	bool checkN = 0, checkW = 0, checkS = 0, checkE = 0;
	unordered_map<int, string> neighbourMap;
	for (int i = 0; i < maze.size(); i++) {
		string neighbours = "";
		int input = maze[i].wall;
		//cout << input << endl;
		if (input >= n) {
			input -= n;
			checkN = 1;
		}
		if (input >= e) {
			input -= e;
			checkE = 1;
		}
		if (input >= s) {
			input -= s;
			checkS = 1;
		}
		if (input >= w) {
			input -= w;
			checkW = 1;
		}
		cout << "neighbours for : " << i << " are : ";
		if (!checkS) {
			neighbours += to_string(i + numC) + "|";
		}
		if (!checkW) {
			neighbours += to_string(i - 1) + "|";
		}
		if (!checkN) {
			neighbours += to_string(i - numC) + "|";
		}
		if (!checkE) {
			neighbours += to_string(i + 1) + "|";
		}
		neighbourMap[i] = neighbours;
		cout << neighbours << endl;
		checkE = checkN = checkW = checkS = 0;
		cout << endl;
	}

	size_t pos = 0;
	string delimiter = "|";
	string str;
	//cout << "delimiter =======" << delimiter << endl;
	queue<cell> Q;
	cell t(start, maze[start].wall, false, start);
	//cout << "v.id =======" << t.id << endl;
	Q.push(t);
	//cout << " size ==== " << S.size() << endl;
	cell v;
	//cout << " S top : " << t.id << endl;
	while (Q.front().id != end) {
		v = Q.front();
		//cout << "S.top == " << v.id << endl;
		Q.pop();

		if (!v.visited) {
			maze[v.id].visited = true;
			cout << "visited node : " << v.id << "  = " << v.visited << endl;
			str = neighbourMap[v.id];
			//cout << str << endl;
			pos = 0;
			while ((pos = str.find(delimiter)) != std::string::npos) {
				int w = stoi(str.substr(0, pos));
				if (!maze[w].visited) {
					maze[w].from = v.id;
					Q.push(maze[w]);
					cout << " Pushed element " << maze[w].id << endl;
				}
				str.erase(0, pos + delimiter.length());
			}
		}
	}
	for (int i = 0; i < maze.size(); i++) {
		maze[i].visited = false;
		maze[i].from = -1;
	}
}

void DFS_recursive(vector<cell>&maze, int numC, int start, int end) {
	cout << " In DFS_recursive " << endl;
	int s = 2, w = 1, n = 8, e = 4;
	bool checkN = 0, checkW = 0, checkS = 0, checkE = 0;
	string neighbours = "";
	int input = maze[start].wall;
	//cout << input << endl;
	if (input >= n) {
		input -= n;
		checkN = 1;
	}
	if (input >= e) {
		input -= e;
		checkE = 1;
	}
	if (input >= s) {
		input -= s;
		checkS = 1;
	}
	if (input >= w) {
		input -= w;
		checkW = 1;
	}
	if (!checkS) {
		neighbours += to_string(maze[start].id + numC) + "|";
	}
	if (!checkW) {
		neighbours += to_string(maze[start].id - 1) + "|";
	}
	if (!checkN) {
		neighbours += to_string(maze[start].id - numC) + "|";
	}
	if (!checkE) {
		neighbours += to_string(maze[start].id + 1) + "|";
	}

	size_t pos = 0;
	string delimiter = "|";

	maze[start].visited = true;
	cout << "visited node : " << maze[start].id << "  = " << maze[start].visited << endl;
	cout << neighbours << endl;
	pos = 0;
	while ((pos = neighbours.find(delimiter)) != std::string::npos) {
		int w = stoi(neighbours.substr(0, pos));
		if (!maze[w].visited) {
			maze[w].from = maze[start].id;
			cout << " Next element to be visited " << maze[w].id << endl;
			DFS_recursive(maze, numC, maze[w].id, end);
		}
		neighbours.erase(0, pos + delimiter.length());
	}
	checkE = checkN = checkW = checkS = 0;

}

