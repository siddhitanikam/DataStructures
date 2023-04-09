//HW5 by Siddhita Nikam
//SU Net ID: snikam  SUID: 471330539
//HW5  AVL Tree
//Due: Monday (Nov. 14) at 11:59PM
//This homework requires more efforts. You should get started as soon as possible.
//You are not allowed to use any extra function.
//You are not allowed to add extra members to either class node or class avl_tree.

#include <iostream>

using namespace std;

//You need to use the following node class for each node of the AVL tree
class node {
public:
	int value;
	int height;//this is tree height. Leaf node is 1; empty node (i.e., NIL) is 0
	node* parent;//nullptr for root
	node* l_child;
	node* r_child;
	node() {}
	node(int i) { value = i; height = 1; parent = l_child = r_child = nullptr; }
};

class avl_tree {
public:
	node* root;
	avl_tree() {
		root = nullptr;
	}

	//************************************************************************************
	//Implement the following member functions

	//add method
	//in case of a tie, go to the left branch.
	void add_node(int i) {
		bool getPos = false;
		if (root == nullptr) {//for first node
			root = new node(i);
			getPos = true;
		}
		node* curr = root;
		while (!getPos) {
			if (i <= curr->value) {
				if (curr->l_child == nullptr) {//if position is found attach it to the left child
					curr->l_child = new node(i);
					curr->l_child->parent = curr;
					getPos = true;
					pair<node*, int> a = height_update(curr->l_child);
					if (a.second == 1) {
						L_Rotate(a.first->parent);
					}
					else if (a.second == 2) {
						node* parent = a.first->parent;
						R_Rotate(a.first);
						L_Rotate(parent);
					}
					else if (a.second == 3) {
						R_Rotate(a.first->parent);
					}
					else if (a.second == 4) {
						node* parent = a.first->parent;
						L_Rotate(a.first);
						R_Rotate(parent);
					}
				}
				else {//if value is less than root node keep on going to left side
					curr = curr->l_child;
				}
			}
			else {
				if (curr->r_child == nullptr) {//if position is found attach it to the right child
					curr->r_child = new node(i);
					curr->r_child->parent = curr;
					getPos = true;
					pair<node*, int> a = height_update(curr->r_child);
					if (a.second == 1) {
						L_Rotate(a.first->parent);
					}
					else if (a.second == 2) {
						node* parent = a.first->parent;
						R_Rotate(a.first);
						L_Rotate(parent);
					}
					else if (a.second == 3) {
						R_Rotate(a.first->parent);
					}
					else if (a.second == 4) {
						node* parent = a.first->parent;
						L_Rotate(a.first);
						R_Rotate(parent);
					}
				}
				else {//if value is greater than root node keep on going to right side
					curr = curr->r_child;
				}
			}
		}

	}

	//void delete_node(int i);
	//Delete the node with value i.  in case of multiple nodes with value i, delete the first node encountered.
	//If it is not a trivial case, always replace it with successor


	//If the node to delete is a leaf node, just delete it.
	//If the node is parent of only a left leaf node, replace it with the leaf node.
	//For all other cases, replace it with in-oreder successor
	void delete_node(int i) {
		if (nullptr == root) {
			return;
		}
		bool getPos = false;
		node* curr = root;//curr will point to the exact position of the node i
		while (!getPos) {
			if (curr->value == i) {
				getPos = true;
			}
			else if (i < curr->value) {
				curr = curr->l_child;
			}
			else {
				curr = curr->r_child;
			}
		}

		if (curr == root && curr->l_child == nullptr && curr->r_child == nullptr) {//deleting the only node in the tree
			root = nullptr;
			return;
		}

		node* parent = curr->parent;
		bool isLeft = false;
		if (nullptr != parent && parent->l_child == curr) {
			isLeft = true;
		}
		if (curr->l_child == nullptr && curr->r_child == nullptr) {//deleting leaf node
			if (isLeft) {
				parent->l_child = nullptr;
				curr->parent = nullptr;
			}
			else {
				parent->r_child = nullptr;
				curr->parent = nullptr;
			}
		}
		else {
			int lChildHeight = curr->l_child != nullptr ? curr->l_child->height : 0;
			int rChildHeight = curr->r_child != nullptr ? curr->r_child->height : 0;

			if (lChildHeight >= rChildHeight && lChildHeight != 0) {
				node* predecessor = curr->r_child;
				while (predecessor!= nullptr && nullptr != predecessor->l_child) {
					predecessor = predecessor->l_child;
				}
				if (nullptr == predecessor) {
					predecessor = curr->l_child;
				}
				
				curr->value = predecessor->value;//just changing the value by keeping the same connections with other nodes
				parent = predecessor->parent;//nodes with same value but different connections; parent is the node which we have to keep
				parent->r_child = predecessor->r_child;//we delete the predecessor here by connecting directly to its left child
				predecessor->r_child != nullptr ? predecessor->r_child->parent = parent : nullptr;

			}
			else if (lChildHeight < rChildHeight && rChildHeight != 0) {
				node* successor = curr->r_child;
				while (successor != nullptr && nullptr != successor->l_child) {
					successor = successor->l_child;
				}
				curr->value = successor->value;
				parent = successor->parent;
				parent->r_child = successor->r_child;
				successor->r_child != nullptr ? successor->r_child->parent = parent : nullptr;
			}

		}

		node* temp = nullptr;
		if (parent->l_child == nullptr && parent->r_child == nullptr) {
			temp = parent;
			temp->height = 1;
		}
		else if (parent->l_child != nullptr && parent->r_child == nullptr) {
			temp = parent->l_child;
		}
		else {
			temp = parent->r_child;
		}

		pair<node*, int> a = height_update(temp);
		if (a.second == 1) {
			L_Rotate(a.first->parent);
		}
		else if (a.second == 2) {
			node* parent = a.first->parent;
			R_Rotate(a.first);
			L_Rotate(parent);
		}
		else if (a.second == 3) {
			R_Rotate(a.first->parent);
		}
		else if (a.second == 4) {
			node* parent = a.first->parent;
			L_Rotate(a.first);
			R_Rotate(parent);
		}
	}


	//void in_order_traversal(node* p); //such as (2 5) (3 3) (5 2) (6 1) .....  (value height)
	//Height of leaf node is 1.
	void in_order_traversal(node* p) {
		if (nullptr == p) {
			return;
		}
		in_order_traversal(p->l_child);
		cout << "(" << p->value << " " << p->height << ") ";
		in_order_traversal(p->r_child);
	}

	//height update method;
	/*
	This function will be invoked by add_node and delete_node.
	p points to the first node that we need to check for possible height update.  We then need to check possible height update toward root.
	All nodes whose heights need to be updated will be performed in this function.
	In case of a violation, the function will return
	{address of Action Node, pattern}, where violation pattern = 1, 2, 3, 4 for RR, RL LL, LR, respectively.
	In case of no violation, the function will return {nullptr, 0}.
	*/
	pair<node*, int> height_update(node* p) {
		if (p == root) {
			return make_pair(nullptr, 0);
		}

		node* orig = p;
		node* pParent = p->parent;
		int lChildHeight;
		int rChildHeight;
		int heightDiff;
		node* actionNode = nullptr;
		bool actionNodeFound = false;

		while (!actionNodeFound && pParent != nullptr) {//updating height till root
			lChildHeight = pParent->l_child != nullptr ? pParent->l_child->height : 0;
			rChildHeight = pParent->r_child != nullptr ? pParent->r_child->height : 0;
			heightDiff = lChildHeight - rChildHeight;
			if (heightDiff > 1 || heightDiff < -1) {
				actionNode = pParent;
				actionNodeFound = true;
			}
			else {
				if (max(lChildHeight, rChildHeight) + 1 != pParent->height) {
					pParent->height = max(lChildHeight, rChildHeight) + 1;
					orig = pParent;
					pParent = pParent->parent;
				}
				else {
					break;
				}
			}
			
		}

		if (actionNode == nullptr) {
			return make_pair(nullptr, 0);
		}
		else {
			int pattern = 0;

			//to check violation
			lChildHeight = actionNode->l_child != nullptr ? actionNode->l_child->height : 0;
			rChildHeight = actionNode->r_child != nullptr ? actionNode->r_child->height : 0;

			if (lChildHeight > rChildHeight) {
				lChildHeight = actionNode->l_child->l_child != nullptr ? actionNode->l_child->l_child->height : 0;
				rChildHeight = actionNode->l_child->r_child != nullptr ? actionNode->l_child->r_child->height : 0;

				if (lChildHeight > rChildHeight) {//LL rotate
					return make_pair(actionNode->l_child, 3);
				}
				else {//LR rotate
					return make_pair(actionNode->l_child, 4);
				}
			}
			else if (rChildHeight > lChildHeight) {
				lChildHeight = actionNode->r_child->l_child != nullptr ? actionNode->r_child->l_child->height : 0;
				rChildHeight = actionNode->r_child->r_child != nullptr ? actionNode->r_child->r_child->height : 0;

				if (lChildHeight > rChildHeight) {//RL rotate
					return make_pair(actionNode->r_child, 2);
				}
				else {//RR rotate
					return make_pair(actionNode->r_child, 1);
				}
			}
		}		
		return make_pair(nullptr, 0);
	}

	//L rotate method;
	//p points to the node at which the rotation will be performed.
	void L_Rotate(node* p) {
		//cout << "In L rotate : " << p->value << endl;
		int lChildHeight = -1;
		int rChildHeight = -1;

		node* pRight = p->r_child;
		if (p == root) {//change the root to new value
			p->r_child = pRight->l_child;
			pRight->l_child != nullptr ? pRight->l_child->parent = p : nullptr;
			pRight->l_child = p;
			p->parent = pRight;
			root = pRight;
			root->parent = nullptr;

			lChildHeight = p->l_child != nullptr ? p->l_child->height : 0;
			rChildHeight = p->r_child != nullptr ? p->r_child->height : 0;
			p->height = max(lChildHeight, rChildHeight) + 1;

			lChildHeight = root->l_child != nullptr ? root->l_child->height : 0;
			rChildHeight = root->r_child != nullptr ? root->r_child->height : 0;
			root->height = max(lChildHeight, rChildHeight) + 1;
		}
		else {//if p is not a root node, p's right child will become parent and p's right child's left child will become p's right child. updating parents accordingly.
			node* parent = p->parent;
			bool isLeft = false;
			if (p == parent->l_child) {
				isLeft = true;
			}
			p->r_child = pRight->l_child;
			pRight->l_child != nullptr ? pRight->l_child->parent = p : nullptr;
			pRight->l_child = p;
			p->parent = pRight;
			if (isLeft) {
				parent->l_child = pRight;
			}
			else {
				parent->r_child = pRight;
			}
			pRight->parent = parent;

			//Update the heights again
			lChildHeight = p->l_child != nullptr ? p->l_child->height : 0;
			rChildHeight = p->r_child != nullptr ? p->r_child->height : 0;
			p->height = max(lChildHeight, rChildHeight) + 1;

			lChildHeight = pRight->l_child != nullptr ? pRight->l_child->height : 0;
			rChildHeight = pRight->r_child != nullptr ? pRight->r_child->height : 0;
			pRight->height = max(lChildHeight, rChildHeight) + 1;

			node* curr = pRight->parent;
			while (nullptr != curr) {
				lChildHeight = curr->l_child != nullptr ? curr->l_child->height : 0;
				rChildHeight = curr->r_child != nullptr ? curr->r_child->height : 0;
				curr->height = max(lChildHeight, rChildHeight) + 1;
				curr = curr->parent;
			}
		}
	}

	//R Rotate method
	//p points to the node at which the rotation will be performed.
	void R_Rotate(node* p) {
		int lChildHeight = -1;
		int rChildHeight = -1;

		node* pLeft = p->l_child;
		if (p == root) {//change the root to new value
			p->l_child = pLeft->r_child;
			pLeft->r_child != nullptr ? pLeft->r_child->parent = p : nullptr;
			pLeft->r_child = p;
			p->parent = pLeft;
			root = pLeft;
			root->parent = nullptr;

			lChildHeight = p->l_child != nullptr ? p->l_child->height : 0;
			rChildHeight = p->r_child != nullptr ? p->r_child->height : 0;
			p->height = max(lChildHeight, rChildHeight) + 1;

			lChildHeight = root->l_child != nullptr ? root->l_child->height : 0;
			rChildHeight = root->r_child != nullptr ? root->r_child->height : 0;
			root->height = max(lChildHeight, rChildHeight) + 1;
		}
		else {//based on the p's position update the tree
			node* parent = p->parent;
			bool isLeft = false;
			if (p == parent->l_child) {
				isLeft = true;
			}
			p->l_child = pLeft->r_child;
			pLeft->r_child != nullptr ? pLeft->r_child->parent = p : nullptr;
			pLeft->r_child = p;
			p->parent = pLeft;
			if (isLeft) {
				parent->l_child = pLeft;
			}
			else {
				parent->r_child = pLeft;
			}
			pLeft->parent = parent;

			//Update the heights again
			lChildHeight = p->l_child != nullptr ? p->l_child->height : 0;
			rChildHeight = p->r_child != nullptr ? p->r_child->height : 0;
			p->height = max(lChildHeight, rChildHeight) + 1;

			lChildHeight = pLeft->l_child != nullptr ? pLeft->l_child->height : 0;
			rChildHeight = pLeft->r_child != nullptr ? pLeft->r_child->height : 0;
			pLeft->height = max(lChildHeight, rChildHeight) + 1;

			node* curr = pLeft->parent;
			while (nullptr != curr) {
				lChildHeight = curr->l_child != nullptr ? curr->l_child->height : 0;
				rChildHeight = curr->r_child != nullptr ? curr->r_child->height : 0;
				curr->height = max(lChildHeight, rChildHeight) + 1;
				curr = curr->parent;
			}
		}
	}
};

int main() {
	cout << "avl_tree 1 (10pts)" << endl;
	avl_tree t1;
	t1.add_node(45);
	t1.add_node(50);
	t1.add_node(46);
	t1.add_node(30);
	t1.add_node(34);
	t1.add_node(48);
	t1.add_node(25);
	t1.add_node(60);
	t1.add_node(47);
	t1.in_order_traversal(t1.root); //2pts
	cout << "    2pts" << endl;
	t1.delete_node(60);
	t1.in_order_traversal(t1.root); //3pts
	cout << "    3pts" << endl;
	t1.add_node(70);
	t1.add_node(40);
	t1.add_node(32);
	t1.add_node(10);
	t1.in_order_traversal(t1.root); //2pts
	cout << "    2pts" << endl;
	t1.delete_node(70);
	t1.in_order_traversal(t1.root); //3pts
	cout << "    3pts" << endl;



	cout << endl;
	cout << "avl_tree 2 (10pts)" << endl;
	avl_tree t2;
	t2.add_node(100);
	t2.add_node(50);
	t2.add_node(200);
	t2.add_node(30);
	t2.add_node(80);
	t2.add_node(150);
	t2.add_node(300);
	t2.add_node(10);
	t2.add_node(40);
	t2.add_node(90);
	t2.add_node(170);
	t2.add_node(5);
	t2.in_order_traversal(t2.root); //3pts
	cout << "    3pts" << endl;
	t2.delete_node(300);
	t2.in_order_traversal(t2.root); //7pts
	cout << "    7pts" << endl;



	cout << endl;
	cout << "avl_tree 3 (15pts)" << endl;
	avl_tree t3;
	t3.add_node(100);
	t3.add_node(50);
	t3.add_node(200);
	t3.add_node(300);
	t3.add_node(100);
	t3.delete_node(100);
	t3.in_order_traversal(t3.root); // 2pts
	cout << "    2pts" << endl;
	t3.add_node(100);
	t3.add_node(100);
	t3.add_node(100);
	t3.add_node(100);
	t3.add_node(100);
	t3.in_order_traversal(t3.root); //3pts
	cout << "    3pts" << endl;
	t3.delete_node(200);
	t3.in_order_traversal(t3.root); //2pts
	cout << "    2pts" << endl;
	t3.delete_node(300);
	t3.in_order_traversal(t3.root); //2pts
	cout << "    2pts" << endl;
	t3.delete_node(100);
	t3.in_order_traversal(t3.root); //3pts
	cout << "    3pts" << endl;
	t3.delete_node(100);
	t3.in_order_traversal(t3.root); //3pts
	cout << "    3pts" << endl;

	return 0;
}