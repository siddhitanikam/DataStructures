//HW4 by Siddhita Nikam
//SU Net ID: snikam  SUID: 471330539

// assignment3_maxflowAndBipartite.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//HW4 maxflow and bipartite matching
//Due: 11:59pm, Saturday Oct. 29.
/*
When performing BFS for max flow or max matching, you need to explore neighbors in the order of increasing node IDs.
For example, if a node has unvisited neighbors 7, 3, 5, and 2.  Then the order for exploring neighbors will be 2, 3, 5, and 7.
This implies that you have to sort neighbors when forming the graph.

For simplicity, in the case of max flow problem, assume there will be at most one directed edge from a node to another node.


It is also assume that if there is a directed edge from n1 to n2, then there will be no edge from n2 to n1.

For max matching problem, since you are dealing with undirected graph, for simplicity, you can assume that there will at most one edge between
any two nodes.

*/
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <queue>
#include <tuple>
//#include <set>
#include <map>



using namespace std;

//You will implement the following three functions.  No additional functions are allowed.
void BipartiteMatching(vector<list<pair<int, int>>>& G, vector<tuple<int, int, bool, bool>>& T, int s, int t);//Graph, Table, source, sink
void MaxFlow(vector<list<pair<int, int>>>& G, vector<tuple<int, int, bool, bool>>& T, int s, int t);//Graph, Table, source, sink
int BFS(vector<list<pair<int, int>>>& G, vector<tuple<int, int, bool, bool>>& T, int s, int t);
//Both BipartiteMatching and MaxFlow will call BFS.
//Return the flow on the path found by BFS or 0 if no path found

int main() {

	//Read in graph from graph1.txt
	//Data initialization
	ifstream In("graph1.txt");
	int source, sink, Nodes, noOfEdges;;
	In >> source >> sink >> Nodes >> noOfEdges;
	vector<list<pair<int, int>>> Graph1(Nodes);
	vector<list<pair<int, int>>> origGraph(Nodes);

	vector<tuple<int, int, bool, bool>> Table1(Nodes);//  Design the tuple. Remember that you will perform BFS
	int index;
	pair<int, int> edgeCostPair;
	//int cost = 99999;
	int from = -1;
	bool visited = 0;
	bool pushed = 0;
	while (In >> index >> edgeCostPair.first >> edgeCostPair.second) {
		Graph1[index].push_back(edgeCostPair);
		origGraph[index].push_back(edgeCostPair);
		Graph1[edgeCostPair.first].push_back(make_pair(index, 0));
	}
	In.close();
	
	for (int i = 0; i < Nodes; i++) {
		Table1[i] = make_tuple(0, from, visited, pushed);
	}
	//cout << "GRAPH AND TABLE CREATED" << endl;
	MaxFlow(Graph1, Table1, source, sink);

	//Printing the results
	cout << "The following edges have scheduled flow as indicated: " << endl;
	for (int i = 0; i <= sink; i++) {
		for (pair<int, int> p : origGraph[i]) {
			for (pair<int, int> p1 : Graph1[i]) {
				if (p.first == p1.first && p.second - p1.second!=0) {
					cout << "The scheduled flow on edge (" << i << " , " << p.first << ") = " << p.second-p1.second << " out of capacity = " << p.second << endl;
				}
			}
		}
	}
	cout << endl;
	//Read in graph from graph2.txt
	//Data initialization
	//cout << " Checking Bipartite now" << endl;
	ifstream In1("graph2.txt");
	int LNodes, RNodes;
	In1 >> Nodes >> noOfEdges >> LNodes >> RNodes;
	vector<list<pair<int, int>>> Graph2(Nodes + 2);
	vector<list<pair<int, int>>> origGraph2(Nodes + 2);

	vector<tuple<int, int, bool, bool>> Table2(Nodes + 2);
	//Read in graph from graph2.txt
	//Data initialization
	while (In1 >> index >> edgeCostPair.first) {
		edgeCostPair.second = 1;
		Graph2[index].push_back(edgeCostPair);
		origGraph2[index].push_back(edgeCostPair);
		Graph2[edgeCostPair.first].push_back(make_pair(index, 0));
	}
	for (int i = 0; i < LNodes; i++) {
		Graph2[Nodes].push_back(make_pair(i, 1));
		Graph2[i].push_back(make_pair(Nodes, 0));
	}
	for (int i = LNodes; i < Nodes; i++) {
		Graph2[i].push_back(make_pair(Nodes + 1, 1));
		Graph2[Nodes + 1].push_back(make_pair(i, 0));
	}
	In1.close();

	for (int i = 0; i < Nodes + 2; i++) {
		Table2[i] = make_tuple(0, from, visited, pushed);
	}

	BipartiteMatching(Graph2, Table2, Nodes, Nodes + 1);

	//Printing the results
	cout << "The following matchings are selected : " << endl;
	
	for (int i = 0; i <= sink; i++) {
		for (pair<int, int> p : origGraph2[i]) {
			for (pair<int, int> p1 : Graph2[i]) {
				if (p.first == p1.first && p1.second == 0) {
					cout << "(" << i << "," << p1.first << ")" << endl;
				}
			}
		}
	}
	
	return 0;
}

int BFS(vector<list<pair<int, int>>>& G, vector<tuple<int, int, bool, bool>>& T, int s, int t) {
	//cout << "================= In BFS ========================" << endl;
	size_t pos = 0;
	string str;
	queue<int> Q;
	Q.push(s);
	get<0>(T[s]) = 0;
	get<1>(T[s]) = s;
	get<3>(T[s]) = true;

	int curr;
	while (!Q.empty() && Q.back() != t) {//until the sink node is not reached at the back of the queue
		curr = Q.front();
		Q.pop();
		if (!get<2>(T[curr])) {
			get<2>(T[curr]) = true;
			//cout << "visited node : " << curr << "  = " << get<2>(T[curr]) << endl;
			std::map<int, int> neighbours;
			for (pair<int, int> edge : G[curr]) {//get the ndeighbours of current node and store it in the map[node, cost pair]
				if(edge.second != 0)//adding condition to ignore neighbours with 0 weight
					neighbours[edge.first] = edge.second;
			}
			for (auto i = neighbours.begin(); i != neighbours.end(); i++) {//push neighbours to the queue and update the table
				if (!get<2>(T[i->first]) && !get<3>(T[i->first])) {
					get<0>(T[i->first]) = i->second;
					get<1>(T[i->first]) = curr;
					get<3>(T[i->first]) = true;
					Q.push(i->first);
					//cout << " Pushed element " << i->first << endl;
				}
			}
		}
	}

	int path = t;
	int minCost = 9999;
	if (Q.empty()) {
		return 0;
	}
	//getting min flow for the s to t path
	while (path != s) {
		if (minCost > get<0>(T[path])) {
			minCost = get<0>(T[path]);
		}
		path = get<1>(T[path]);
	}

	//updating min flow in graph for the s to t path
	int revPath = t;
	pair<int, int> updEdge;
	int revNode;
	while (revPath != s) {
		revNode = get<1>(T[revPath]);
		for (pair<int, int> edge : G[revPath]) {//add cost to the rev path
			if (edge.first == revNode) {
				G[revPath].remove(edge);
				updEdge.first = edge.first;
				updEdge.second = edge.second + minCost;
				G[revPath].push_back(updEdge);
				break;
			}
		}
		for (pair<int, int> revEdge : G[revNode]) {//subtract cost from the current forward path
			if (revEdge.first == revPath) {
				G[revNode].remove(revEdge);
				updEdge.first = revEdge.first;
				updEdge.second = revEdge.second - minCost;
				G[revNode].push_back(updEdge);
				break;
			}
		}
		revPath = get<1>(T[revPath]);
	}

	//cout << "minCost : " << minCost << endl;
	return minCost;
	
}

void MaxFlow(vector<list<pair<int, int>>>& G, vector<tuple<int, int, bool, bool>>& T, int s, int t) {
	int flow = 0;
	int res = -1;
	while (res!=0) {
		res = BFS(G, T, s, t);
		flow = flow + res;
		for (int i = 0; i < T.size(); i++) {//reset table
			T[i] = make_tuple(0, -1, 0, 0);
		}
	}
	cout << "The maximum flow = " << flow << endl;
}
void BipartiteMatching(vector<list<pair<int, int>>>& G, vector<tuple<int, int, bool, bool>>& T, int s, int t) {
	int matching = 0;
	int res = -1;
	while (res != 0) {
		res = BFS(G, T, s, t);
		matching = matching + res;
		for (int i = 0; i < T.size(); i++) {//reset table
			T[i] = make_tuple(0, -1, 0, 0);
		}
	}
	cout << "The maximum matching = " << matching << endl;
}

//The following is a sample screenshot.
/*
The maximum flow = 19
The following edges have scheduled flow as indicated:
The scheduled flow on edge (0, 1) = 10 out of capacity = 10
The scheduled flow on edge (0, 2) = 9 out of capacity = 10
The scheduled flow on edge (1, 3) = 4 out of capacity = 4
The scheduled flow on edge (1, 4) = 6 out of capacity = 8
The scheduled flow on edge (2, 4) = 9 out of capacity = 9
The scheduled flow on edge (3, 5) = 9 out of capacity = 10
The scheduled flow on edge (4, 3) = 5 out of capacity = 5
The scheduled flow on edge (4, 5) = 10 out of capacity = 10

The maximum matching = 5
The following matchings are selected:
(0, 8)
(1, 5)
(2, 6)
(3, 9)
(4, 7)
*/

//Input format for max flow problem in graph1.txt
/*
0 //source node
5 //sink node
6 //number of nodes
9 //number of edges
0 1 10 //A directed edge from node 0 to node 1 with weight 10 .
0 2 10
1 3 4
1 4 8
1 2 2
2 4 9
3 5 10
4 3 5
4 5 10
*/

//input format for bipartite matching in graph2.txt
/*
10 //Number of Nodes 0 ... 9
9 //Number of Edges
5 //L -- number of nodes on the left side;  0 ...L-1 are on the left side
5 //R -- number of nodes on the right side; L ... L+R-1 are on the right side
0 5 //An undirected edge connecting 0 to 5
0 7
0 8
1 5
2 6
2 7
3 8
3 9
4 7
*/

