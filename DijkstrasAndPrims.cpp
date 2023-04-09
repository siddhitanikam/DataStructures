//HW3 by Siddhita Nikam
//SU Net ID: snikam  SUID: 471330539
//HW3 Due: 11:59pm, Friday, October 7.

//HW2A: Dijkstra's algorithm with HEAP (20 points)
//HW2B: Use the same design for Prim's algorithm (5 points)
//In HW2B, for each x->y edges, also create a y->x edge with the same weight
// Note that printing results is also performed in the functions.

//You need to use the same function parameters as described.
//You are not allowed to use any extra functions
//You are not allowed to define any class.


//Note that even though we can pick any node as starting node for Prim's algorithm,
//for grading purpose, we will use the same source node as in Dijkstra's algorithm for Prim's algorithm.


#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <tuple>

using namespace std;


	void ShortestPath(vector<list<pair<int, int>>>& graph, vector<tuple<int, int, bool, int>>& table, vector<tuple<int, int>> & heap, int source);//20 points
	void MST(vector<list<pair<int, int>>>& graph, vector<tuple<int, int, bool, int>>& table, vector<tuple<int, int>> & heap, int source); //5 points

	void heapify(vector<tuple<int, int, bool, int>>& table, vector<tuple<int, int>>& heap, int rootIndex);

	int main() {
			
		//your code to read in graph from "graph.txt"

		ifstream In("graph.txt");
		int source, noOfNodes, noOfEdges;
		In >> source >> noOfNodes >> noOfEdges;
		//cout << source << " - " << noOfNodes << " - " << noOfEdges << endl;

		vector<list<pair<int, int>>> graph(noOfNodes);
		pair<int, int> edgeCostPair;
		int index;
		while (In >> index >> edgeCostPair.first >> edgeCostPair.second) {
			list<pair<int, int>> listOfPairs = graph[index];
			listOfPairs.push_back(edgeCostPair);
			graph[index] = listOfPairs;
		}
		In.close();
		//cout << "GRAPH CREATED!" << endl;
		
		int cost = 99999;
		int from = -1;
		bool visited = 0;
		vector<tuple<int, int, bool, int>> table(noOfNodes);//define your tuple
		vector<tuple<int, int>> heap(noOfNodes); //define your tuple
		tuple<int, int, bool, int> tableTuple;
		tuple<int, int> heapTuple;

		for (int i = 0; i < noOfNodes; i++) {
			tableTuple = make_tuple(cost, from, visited, i);
			heapTuple = make_tuple(i, cost);
			table[i] = tableTuple;
			heap[i] = heapTuple;
		}
		//cout << "TABLE and HEAP CREATED!" << endl;
		
		

		//You might need to make some initialization setting for table and heap

		ShortestPath(graph, table, heap, source);//HW2A
		//Printing the solution
		int in = 0;
		int fromNode = -1;
		cout << "For the shortest paths using Dijkstra's algorithm: " << endl;
		for (tuple<int, int, bool, int> path : table) {
			cout << "The cost from " << source << " to " << in << " is " << get<0>(path) << " and ";
			fromNode = in;
			if (get<0>(path) != cost) {
				cout << "path is ";
				while (fromNode != source) {
					cout << fromNode;
					fromNode = get<1>(table[fromNode]);
					cout << " <- ";
				}
				cout << fromNode;
			}
			else {
				cout << "no paths exist!";
			}
			in++;
			cout << endl;
		}

		//You might need to reset some data.
		ifstream In1("graph.txt");
		graph.clear();
		graph.resize(noOfNodes);
		heap.clear();
		heap.resize(noOfNodes);
		table.clear();
		table.resize(noOfNodes);
		In1 >> source >> noOfNodes >> noOfEdges;
		pair<int, int> revEdgeCostPair;
		while (In1 >> index >> edgeCostPair.first >> edgeCostPair.second) {
			list<pair<int, int>> listOfPairs = graph[index];
			listOfPairs.push_back(edgeCostPair);
			graph[index] = listOfPairs;
			revEdgeCostPair.first = index;
			revEdgeCostPair.second = edgeCostPair.second;
			listOfPairs = graph[edgeCostPair.first];
			listOfPairs.push_back(revEdgeCostPair);
			graph[edgeCostPair.first] = listOfPairs;

		}
		In1.close();
		for (int i = 0; i < noOfNodes; i++) {
			tableTuple = make_tuple(cost, from, visited, i);
			heapTuple = make_tuple(i, cost);
			table[i] = tableTuple;
			heap[i] = heapTuple;
		}

		MST(graph, table, heap, source);//HW2B
		//Printing the solution
		cout << "For the minimum spanning tree using Prim's algorithm: " << endl;
		cout << "The selected edges are : " << endl;

		int totalCost = 0;
		int ind = 0;
		for (tuple<int, int, bool, int> path : table) {
			if (get<2>(path) && get<0>(path) != 0) {
				cout << ind << " " << get<1>(path) << " " << get<0>(path) << endl;
				totalCost += get<0>(path);
			}
			ind++;
		}
		cout << "The overall cost is " << endl;
		cout << totalCost << endl;
		
	}
	void ShortestPath(vector<list<pair<int, int>>>& graph, vector<tuple<int, int, bool, int>>& table, vector<tuple<int, int>> & heap, int source) {

		//Your code  -- HW2A 20 pts
		int currNode;
		//currNode = source;
		//table tuple-- cost, from, visited, heapindex
		//updating the source parameters in heap and table
		get<0>(table[source]) = 0;
		get<1>(table[source]) = source;
		get<2>(table[source]) = 1;

		get<0>(heap[source]) = source;
		get<1>(heap[source]) = 0;

		while (!heap.empty()) {//all nodes are not visited
			int minCost = 999999;
			int minCostNode = -1;

			for (tuple<int, int> heapTuple : heap) {/*to define current node */
				if (minCost > get<1>(heapTuple)) {
					minCost = get<1>(heapTuple);
					minCostNode = get<0>(heapTuple);
				}
			}
			currNode = minCostNode;
			get<2>(table[currNode]) = 1;
			for (pair<int, int> edge : graph[currNode]) {
				//get the heapIndex
				int heapIndex = 0;
				while (heapIndex < heap.size() && get<0>(heap[heapIndex]) != edge.first) {
					heapIndex++;
				}
				//update min cost for all the neighbours of a selected node
				if (heapIndex < heap.size() && get<0>(table[currNode]) + edge.second < get<0>(table[edge.first])) {
					get<0>(table[edge.first]) = get<0>(table[currNode]) + edge.second;
					get<1>(table[edge.first]) = currNode;
					get<1>(heap[heapIndex]) = get<0>(table[edge.first]);
				}
			}
			heapify(table, heap, 0);
			get<3>(table[get<0>(heap[heap.size() - 1])]) = 0;//changing position of last node to the first
			heap[0] = heap[heap.size() - 1];
			heap.resize(heap.size() - 1);//size of heap reduced by 1
			heapify(table, heap, 0);//adjust to min-heap again
		}
	}
	void MST(vector<list<pair<int, int>>>& graph, vector<tuple<int, int, bool, int>>& table, vector<tuple<int, int>> & heap, int source) {

		//Your code -- HW2B 5 pts
		int currNode;
		//currNode = source;
		//table tuple-- cost, from, visited, heapindex
		//updating the source parameters in heap and table
		get<0>(table[source]) = 0;
		get<1>(table[source]) = source;
		//get<2>(table[source]) = 1;

		get<0>(heap[source]) = source;
		get<1>(heap[source]) = 0;

		while (!heap.empty()) {//all nodes are not visited
			int minCost = 999999;
			int minCostNode = -1;

			//for (tuple<int, int> heapTuple : heap) {/*to define current node */
			//	if (minCost > get<1>(heapTuple)) {
			//		minCost = get<1>(heapTuple);
			//		minCostNode = get<0>(heapTuple);
			//	}
			//}
			int inx = 0;
			for (tuple<int, int, bool, int> tableTpl : table) {
				if (!get<2>(tableTpl)) {
					if (minCost > get<0>(tableTpl)) {
						minCost = get<0>(tableTpl);
						minCostNode = inx;
					}
				}
				inx++;
			}
			currNode = minCostNode;
			get<2>(table[currNode]) = 1;
			for (pair<int, int> edge : graph[currNode]) {
				//get the heapIndex
				int heapIndex = 0;
				while (heapIndex < heap.size() && get<0>(heap[heapIndex]) != edge.first) {
					heapIndex++;
				}
				//update min cost for all the neighbours of a selected node
				if (heapIndex < heap.size() && edge.second < get<0>(table[edge.first]) && !get<2>(table[edge.first])) {
					get<0>(table[edge.first]) = edge.second;
					get<1>(table[edge.first]) = currNode;
					get<1>(heap[heapIndex]) = get<0>(table[edge.first]);
				}
			}
			int parent = 0;
			
			heapify(table, heap, 0);
			get<3>(table[get<0>(heap[heap.size() - 1])]) = 0;
			heap[0] = heap[heap.size() - 1];
			heap.resize(heap.size() - 1);
			heapify(table, heap, 0);
		}

	}

	void heapify(vector<tuple<int, int, bool, int>>& table, vector<tuple<int, int>>& heap, int rootIndex) {
		int src = rootIndex;
		int leftChild = 2 * rootIndex + 1;
		int rightChild = 2 * rootIndex + 2;
		if (leftChild > heap.size() && rightChild > heap.size()) {
			return;
		}
		if (leftChild < heap.size()) {
			heapify(table, heap, leftChild);
		}
		if (rightChild < heap.size()) {
			heapify(table, heap, rightChild);
		}
		if (leftChild < heap.size() && get<1>(heap[leftChild]) < get<1>(heap[src])) {
			src = leftChild;
		}
		if (rightChild < heap.size() && get<1>(heap[rightChild]) < get<1>(heap[src])) {
			src = rightChild;
		}
		if (src != rootIndex) {//changing the position of nodes to adjust it to min-heap
			int temp = src;
			tuple<int, int> tempTuple = heap[src];
			int tempHeapIndex = get<3>(table[get<0>(heap[temp])]);
			get<3>(table[get<0>(heap[src])]) = get<3>(table[get<0>(heap[rootIndex])]);
			get<3>(table[get<0>(heap[rootIndex])]) = tempHeapIndex;
			heap[src] = heap[rootIndex];
			heap[rootIndex] = tempTuple;
			src = rootIndex;
			rootIndex = temp;
		}
	}
		
