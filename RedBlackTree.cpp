//CSE674 HW6  Due: 11:59PM, Dec. 1 Thursday
#include <iostream>
using namespace std;

class node {
public:
	node* p_parent;
	node* p_Lchild;
	node* p_Rchild;
	int value;
	bool color; //true: red, false: black
	node() { p_parent = p_Lchild = p_Rchild = nullptr; }
	node(int i) { value = i; p_parent = p_Lchild = p_Rchild = nullptr; }
};

class tree {
public:
	node* root;
	tree() { root = nullptr; }
	void R_rotate(node* p) {//R-rotate at node pointed by p
		node* pLeft = p->p_Lchild;
		if (p == root) {//change the root to new value
			p->p_Lchild = pLeft->p_Rchild;
			pLeft->p_Rchild != nullptr ? pLeft->p_Rchild->p_parent = p : nullptr;
			pLeft->p_Rchild = p;
			p->p_parent = pLeft;
			root = pLeft;
			root->p_parent = nullptr;
		}
		else {//based on the p's position update the tree
			node* parent = p->p_parent;
			bool isLeft = false;
			if (p == parent->p_Lchild) {
				isLeft = true;
			}
			p->p_Lchild = pLeft->p_Rchild;
			pLeft->p_Rchild != nullptr ? pLeft->p_Rchild->p_parent = p : nullptr;
			pLeft->p_Rchild = p;
			p->p_parent = pLeft;
			if (isLeft) {
				parent->p_Lchild = pLeft;
			}
			else {
				parent->p_Rchild = pLeft;
			}
			pLeft->p_parent = parent;
		}
	}

	void L_rotate(node* p) {//L-rotate at node pointed by p
		node* pRight = p->p_Rchild;
		if (p == root) {//change the root to new value
			p->p_Rchild = pRight->p_Lchild;
			pRight->p_Lchild != nullptr ? pRight->p_Lchild->p_parent = p : nullptr;
			pRight->p_Lchild = p;
			p->p_parent = pRight;
			root = pRight;
			root->p_parent = nullptr;
		}
		else {//if p is not a root node, p's right child will become parent and p's right child's left child will become p's right child. updating parents accordingly.
			node* parent = p->p_parent;
			bool isLeft = false;
			if (p == parent->p_Lchild) {
				isLeft = true;
			}
			p->p_Rchild = pRight->p_Lchild;
			pRight->p_Lchild != nullptr ? pRight->p_Lchild->p_parent = p : nullptr;
			pRight->p_Lchild = p;
			p->p_parent = pRight;
			if (isLeft) {
				parent->p_Lchild = pRight;
			}
			else {
				parent->p_Rchild = pRight;
			}
			pRight->p_parent = parent;
		}
	};
	//R_rotate and L_rotate will be called by insert_node, delete_node, delete_1, delete_2A, delete_2B, and delete_3.

	void add_node(int i) {//same policy as HW5 for avl trees.
		bool getPos = false;
		if (root == nullptr) {
			root = new node(i);
			root->color = false;
			getPos = true;
		}
		node* curr = root;
		while (!getPos) {
			if (i <= curr->value) {
				if (curr->p_Lchild == nullptr) {
					curr->p_Lchild = new node(i);
					curr->p_Lchild->color = true;
					curr->p_Lchild->p_parent = curr;
					getPos = true;
					//while (curr != root) {}
					if (!curr->color) {//case I - parent is black. it's done
						//getPos = true;
						break;
					}
					else {
						curr = curr->p_Lchild;
						node* parent = curr->p_parent;
						node* uncleNode = nullptr;
						bool isLchild = false;

						while (nullptr != parent && parent->color) {//case II - parent is red
							if (nullptr != parent) {
								if (parent == parent->p_parent->p_Lchild) {
									isLchild = true;
									uncleNode = parent->p_parent->p_Rchild;
								}
								else {
									uncleNode = parent->p_parent->p_Lchild;
								}
							}
							if (nullptr == uncleNode || (nullptr != uncleNode && !uncleNode->color)) {//II-A uncle node is black
								if (isLchild) {
									parent->color = false;
									parent->p_parent->color = true;
									R_rotate(parent->p_parent);
								}
								else {
									R_rotate(parent);
									parent = parent->p_parent;
									parent->p_parent->color = true;
									parent->color = false;
									L_rotate(parent->p_parent);
									curr = parent->p_parent;
									parent = curr->p_parent;
								}
							}
							else if (nullptr != uncleNode && uncleNode->color) {//II-B uncle node is red
								//step1 - change colors of parent, uncle and grandparent
								parent->color = !parent->color;//parent
								uncleNode->color = !uncleNode->color;//uncle
								if (nullptr != parent->p_parent) {//grandparent
									parent->p_parent->color = !parent->p_parent->color;
								}
								if (nullptr != parent->p_parent && parent->p_parent == root) {
									parent->p_parent->color = false;
									//getPos = true;
									break;
								}
								else if (nullptr != parent->p_parent) {
									curr = parent->p_parent;
									parent = curr->p_parent;
								}
							}
						}
					}
				}
				else {
					curr = curr->p_Lchild;
				}
			}
			else {
				if (curr->p_Rchild == nullptr) {
					curr->p_Rchild = new node(i);
					curr->p_Rchild->color = true;
					curr->p_Rchild->p_parent = curr;
					getPos = true;
					//while (curr != root) {}
					if (!curr->color) {//case I - parent is black. it's done
						//getPos = true;
						break;
					}
					else {
						curr = curr->p_Rchild;
						node* parent = curr->p_parent;
						node* uncleNode = nullptr;
						bool isLchild = false;

						while (nullptr != parent && parent->color) {//case II - parent is red
							if (nullptr != parent) {
								if (parent == parent->p_parent->p_Lchild) {
									isLchild = true;
									uncleNode = parent->p_parent->p_Rchild;
								}
								else {
									uncleNode = parent->p_parent->p_Lchild;
								}
							}
							if (nullptr == uncleNode || (nullptr != uncleNode && !uncleNode->color)) {//II-A uncle node is black
								if (isLchild) {
									L_rotate(parent);
									parent = parent->p_parent;
									parent->p_parent->color = true;
									parent->color = false;
									R_rotate(parent->p_parent);
									curr = parent->p_parent;
									parent = curr!= nullptr ? curr->p_parent : nullptr;
								}
								else {
									parent->color = false;
									parent->p_parent->color = true;
									L_rotate(parent->p_parent);
								}
							}
							else if (nullptr != uncleNode && uncleNode->color) {//II-B uncle node is red
								//step1 - change colors of parent, uncle and grandparent
								parent->color = !parent->color;//parent
								uncleNode->color = !uncleNode->color;//uncle
								if (nullptr != parent->p_parent) {//grandparent
									parent->p_parent->color = !parent->p_parent->color;
								}
								if (nullptr != parent->p_parent && parent->p_parent == root) {
									parent->p_parent->color = false;
									//getPos = true;
									break;
								}
								else if (nullptr != parent->p_parent) {
									curr = parent->p_parent;
									parent = curr != nullptr ? curr->p_parent : nullptr;
								}

							}
						}
					}
				}
				else {
					curr = curr->p_Rchild;
				}
			}
		}
	}

	//If the node to delete is a leaf node, just delete it.
	//If the node is parent of only a left leaf node, replace it with the leaf node.
	//For all other cases, replace it with in-oreder successor
	void delete_node(int i) {//same policy as HW5 for avl trees.
		if (root == nullptr) {
			return;
		}
		node* curr = root;//curr will point to the exact position of the node i
		bool getPos = false;

		while (!getPos) {
			if (curr->value == i) {
				getPos = true;
			}
			else if (i < curr->value) {
				curr = curr->p_Lchild;
			}
			else {
				curr = curr->p_Rchild;
			}
		}
		if (!getPos) {
			return;
		}

		if (curr == root && curr->p_Lchild == nullptr && curr->p_Rchild == nullptr) {//deleting the only node in the tree
			root = nullptr;
			return;
		}

		bool isLeafNode = false;
		
		do {
			if (curr->p_Lchild == nullptr && curr->p_Rchild == nullptr) {//check if the curr node(node to be deleted) is leaf node
				isLeafNode = true;
			}
			else if(curr->p_Lchild != nullptr || curr->p_Rchild != nullptr) {
				isLeafNode = false;
			}

			if (isLeafNode) {
				if (curr->color) {//deleting leaf node if it is red
					if (curr == curr->p_parent->p_Lchild) {
						curr->p_parent->p_Lchild = nullptr;
						curr->p_parent = nullptr;
					}
					else {
						curr->p_parent->p_Rchild = nullptr;
						curr->p_parent = nullptr;
					}
					break;
				}
				else {//black node will get converted to the double black node and then will need to check the cases
					node* siblingNode = nullptr;
					bool isLChild = false;
					if (curr == curr->p_parent->p_Lchild) {
						siblingNode = curr->p_parent->p_Rchild;
						isLChild = true;
					}
					else {
						siblingNode = curr->p_parent->p_Lchild;
						isLChild = false;
					}

					//starting different cases
					if (siblingNode == nullptr || !siblingNode->color) {//if sibling node is black for case I and II
						if(siblingNode != nullptr && ((siblingNode->p_Lchild != nullptr && siblingNode->p_Lchild->color) || (siblingNode->p_Rchild != nullptr && siblingNode->p_Rchild->color))) {//Case I - Sibling of DB is black and has at least one Red child node
							delete_1(curr, isLChild);
							break;
						}
						else if ((siblingNode->p_Lchild == nullptr || !siblingNode->p_Lchild->color) && (siblingNode->p_Rchild == nullptr || !siblingNode->p_Rchild->color)) {//Case II - Sibling of DB is black & it doesnt have any Red child nodes
							if (siblingNode->p_parent && !siblingNode->p_parent->color) {//Case II-A - Parent of sibling is black
								delete_2A(curr, isLChild);
								if (curr == curr->p_parent->p_Lchild) {
									curr->p_parent->p_Lchild = nullptr;
								}
								else {
									curr->p_parent->p_Rchild = nullptr;
								}
								curr->p_parent = nullptr;
								break;
							}
							else if (siblingNode->p_parent && siblingNode->p_parent->color) {//Case II-B - Parent of sibling is red
								delete_2B(curr, isLChild);
								break;
							}
						}
					}
					else if (siblingNode->color) {//Case III - Sibling of DB is Red
						delete_3(curr, isLChild);
						break;
					}
				}
			}
			else {
				if (nullptr == curr->p_Lchild || nullptr == curr->p_Rchild && ((curr->p_Lchild && curr->p_Lchild->color != curr->color) || (curr->p_Rchild && curr->p_Rchild->color != curr->color)))//node with only child
				{
					if (nullptr != curr->p_Lchild) {
						curr->value = curr->p_Lchild->value;
						curr->color = false;
						curr->p_Lchild->p_parent = nullptr;
						curr->p_Lchild = nullptr;
						break;
					}
					else {
						curr->value = curr->p_Rchild->value;
						curr->color = false;
						curr->p_Rchild->p_parent = nullptr;
						curr->p_Rchild = nullptr;
						break;
					}
				}
				else {
					node* predecessor = curr;
					node* checkNode = curr->p_Rchild;
					while (checkNode) {
						predecessor = checkNode;
						checkNode = checkNode->p_Lchild;
					}
					curr->value = predecessor->value;
					curr = predecessor;
					isLeafNode = true;
				}
			}
			
		} while (isLeafNode);
	};



	void delete_1(node* p, bool b) {//Case 1 of delete_node; p points to the parent of double black node; b is true if double black is the left child of its parent; else b is false.
		node* siblingNode = nullptr;
		node* parent = p->p_parent;
		if (p == p->p_parent->p_Lchild) {
			siblingNode = p->p_parent->p_Rchild;
			p->p_parent->p_Lchild = nullptr;
		}
		else {
			siblingNode = p->p_parent->p_Lchild;
			p->p_parent->p_Rchild = nullptr;
		}
		p->p_parent = nullptr;

		if (siblingNode == parent->p_Lchild) {//deciding rotations
			if (siblingNode != nullptr && siblingNode->p_Rchild != nullptr && siblingNode->p_Rchild->color && siblingNode->p_Lchild != nullptr && siblingNode->p_Lchild->color) {//LL pattern
				R_rotate(parent);
				siblingNode->p_Lchild->color = !siblingNode->p_Lchild->color;
				siblingNode->color = parent->color;
				parent->color = false;
			}
			else if (siblingNode != nullptr  && (siblingNode->p_Rchild != nullptr && siblingNode->p_Rchild->color && (siblingNode->p_Lchild != nullptr && !siblingNode->p_Lchild->color))) {//RL pattern
				L_rotate(siblingNode);
				R_rotate(parent);
				parent->p_parent->color = parent->color;
				parent->color = false;
			}
		}
		else {
			if (siblingNode != nullptr && siblingNode->p_Rchild != nullptr && siblingNode->p_Rchild->color && siblingNode->p_Lchild != nullptr && siblingNode->p_Lchild->color) {
				L_rotate(parent);//RR pattern
				siblingNode->p_Rchild->color = !siblingNode->p_Rchild->color;
				siblingNode->color = parent->color;
				parent->color = false;
			}
			else if (siblingNode != nullptr && siblingNode->p_Rchild != nullptr && siblingNode->p_Rchild->color && (siblingNode->p_Lchild != nullptr && !siblingNode->p_Lchild->color)) {//LR pattern
				R_rotate(siblingNode);
				L_rotate(parent);
				parent->p_parent->color = parent->color;
				parent->color = false;
			}
		}
	};
	

	void delete_2A(node* p, bool b) {//Case 2A of delete_node; p points to the parent of double black node; b is true if double black is the left child of its parent; else b is false.
		if (p == nullptr || p->p_parent == nullptr) {
			return;
		}
		node* siblingNode = nullptr;
		node* parent = p->p_parent;
		bool isLChild = false;

		if (p == p->p_parent->p_Lchild) {
			siblingNode = p->p_parent->p_Rchild;
			isLChild = true;
		}
		else {
			siblingNode = p->p_parent->p_Lchild;
			isLChild = false;
		}

		if (!p->p_parent->color) {//if parent is black -- parent to db and start all over
			siblingNode->color = true;//change sibling to red
			delete_2A(parent, isLChild);
		}
		else if (p->p_parent->color) {
			parent->color = false;//change parent to black
			siblingNode->color = true;//change sibling to red
			return;
		}
	};
	

	void delete_2B(node* p, bool b) {//Case 2B of delete_node; p points to the parent of double black node; b is true if double black is the left child of its parent; else b is false.
		node* siblingNode = nullptr;
		node* parent = p->p_parent;

		if (p == p->p_parent->p_Lchild) {
			siblingNode = p->p_parent->p_Rchild;
		}
		else {
			siblingNode = p->p_parent->p_Lchild;
		}

		if (p->p_parent->p_Lchild == p) {
			p->p_parent->p_Lchild = nullptr;
		}
		else {
			p->p_parent->p_Rchild = nullptr;
		}
		siblingNode->color = true;//change sibling to Red
		parent->color = false;//change parent to Black
		p->p_parent = nullptr;
	};
	


	void delete_3(node* p, bool b) {//Case 3 of delete_node; p points to the parent of double black node; b is true if double black is the left child of its parent; else b is false.
		node* curr = p;
		node* parent = p->p_parent;
		node* grandparent = parent->p_parent;

		bool isLeft = false;
		if (grandparent!= nullptr && parent == grandparent->p_Lchild) {
			grandparent->p_Lchild = nullptr;
			isLeft = true;		
		}			
		else if (grandparent != nullptr && parent == grandparent->p_Rchild) {
			grandparent->p_Rchild = nullptr;
		}
		else {
			root = nullptr;
		}			
		parent->p_parent = nullptr;

		if (p == parent->p_Lchild) {
			node* actioNode = parent;
			node* rightChild = parent->p_Rchild;//like in avl tree - rotation
			actioNode->p_Rchild = rightChild->p_Lchild;

			if (rightChild->p_Lchild) {
				rightChild->p_Lchild->p_parent = actioNode;
			}
			rightChild->p_Lchild = actioNode;
			actioNode->p_parent = rightChild;

			//changing colors
			actioNode->color = !actioNode->color;
			rightChild->color = !rightChild->color;

			if (grandparent == nullptr) {//if grandparent is root
				root = rightChild;
				rightChild->p_parent = nullptr;
			}
			else if (isLeft) {//if parent is the left child
				grandparent->p_Lchild = rightChild;
				rightChild->p_parent = grandparent;
			}
			else {
				grandparent->p_Rchild = rightChild;
				rightChild->p_parent = grandparent;
			}
		}
		else {
			node* actioNode = parent;
			node* leftChild = parent->p_Lchild;
			actioNode->p_Lchild = leftChild->p_Rchild;
			if (leftChild->p_Rchild) {
				leftChild->p_Rchild->p_parent = actioNode;
			}
			leftChild->p_Rchild = actioNode;
			actioNode->p_parent = leftChild;

			actioNode->color = !actioNode->color;
			leftChild->color = !leftChild->color;
		

			if (grandparent == nullptr) {
				root = leftChild;
				leftChild->p_parent = nullptr;
			}
			else if (isLeft) {
				grandparent->p_Lchild = leftChild;
				leftChild->p_parent = grandparent;
			}
			else {
				grandparent->p_Rchild = leftChild;
				leftChild->p_parent = grandparent;
			}
		}


		//As we just rotate based on the pattern alongwith DB, to remove DB node - checking same as written in the delete function when started.
		node* siblingNode = nullptr;
		bool isLChild = false;
		if (curr == curr->p_parent->p_Lchild) {
			siblingNode = curr->p_parent->p_Rchild;
			isLChild = true;
		}
		else {
			siblingNode = curr->p_parent->p_Lchild;
			isLChild = false;
		}

		if (siblingNode == nullptr || !siblingNode->color) {//if sibling node is black for case I and II
			if (siblingNode != nullptr && ((siblingNode->p_Lchild != nullptr && siblingNode->p_Lchild->color) || (siblingNode->p_Rchild != nullptr && siblingNode->p_Rchild->color))) {//Case I - Sibling of DB is black and has at least one Red child node
				delete_1(curr, isLChild);
			}
			else if (siblingNode != nullptr && ((siblingNode->p_Lchild == nullptr || !siblingNode->p_Lchild->color) && (siblingNode->p_Rchild == nullptr || !siblingNode->p_Rchild->color))) {//Case II - Sibling of DB is black & it doesnt have any Red child nodes
				if (siblingNode->p_parent && !siblingNode->p_parent->color) {//Case II-A - Parent of sibling is black
					delete_2A(curr, isLChild);
					if (curr == curr->p_parent->p_Lchild) {
						curr->p_parent->p_Lchild = nullptr;
					}
					else {
						curr->p_parent->p_Rchild = nullptr;
					}
					curr->p_parent = nullptr;
				}
				else if (siblingNode->p_parent && siblingNode->p_parent->color) {//Case II-B - Parent of sibling is red
					delete_2B(curr, isLChild);
				}
			}
		}
		else if (siblingNode->color) {//Case III - Sibling of DB is Red
			delete_3(curr, isLChild);
		}

	};
	


	//For the following, during traveral, print (value, color) of each node.
	void InorderPrint(node* p) {//Print all nodes of a subtree rooted at node pointed by p; Example: (12 R) (7 B) (11 R) ...
		if (nullptr == p) {
			return;
		}
		InorderPrint(p->p_Lchild);
		string color = "B";
		if (p->color) {
			color = "R";
		}
		cout << "(" << p->value << " " << color << ") ";
		InorderPrint(p->p_Rchild);
	}

};

int main() {
	cout << " Red black tree 3  (8pts)" << endl;
	tree T3;
	T3.add_node(50);
	T3.add_node(100);
	T3.add_node(30);
	T3.add_node(200);
	T3.add_node(300);
	T3.add_node(250);
	T3.add_node(280);
	T3.InorderPrint(T3.root);
	cout << "    4pts" << endl;
	T3.delete_node(30);

	T3.InorderPrint(T3.root);
	cout << "    4pts" << endl;

	cout << endl;
	cout << " Red black tree 4 (9pts)" << endl;
	tree T4;
	T4.add_node(47);
	T4.add_node(32);
	T4.add_node(71);
	T4.add_node(20);
	T4.add_node(35);
	T4.add_node(65);
	T4.add_node(85);



	T4.add_node(34);
	T4.add_node(60);
	T4.add_node(82);
	T4.add_node(92);

	T4.InorderPrint(T4.root);
	cout << "    3pts" << endl;
	T4.delete_node(47);

	T4.InorderPrint(T4.root);
	cout << "    3pts" << endl;

	T4.delete_node(60);
	T4.delete_node(85);
	T4.InorderPrint(T4.root);
	cout << "    3pts" << endl;

	cout << endl;
	cout << endl;
	cout << " Red black tree 5 (12pts)" << endl;
	tree T5;
	T5.add_node(100);
	T5.add_node(200);
	T5.add_node(300);

	T5.add_node(400);
	T5.add_node(50);
	T5.add_node(150);

	T5.add_node(160);
	T5.add_node(130);
	T5.add_node(155);
	T5.InorderPrint(T5.root);
	cout << "    2pts" << endl;
	T5.delete_node(150);
	T5.InorderPrint(T5.root);
	cout << "    2pts" << endl;
	T5.delete_node(155);
	T5.InorderPrint(T5.root);
	cout << "    2pts" << endl;
	T5.delete_node(160);
	T5.InorderPrint(T5.root);
	cout << "    2pts" << endl;
	T5.delete_node(200);
	T5.InorderPrint(T5.root);
	cout << "    2pts" << endl;
	T5.delete_node(300);
	T5.InorderPrint(T5.root);
	cout << "    2pts" << endl;

	cout << endl;
	cout << endl;
	cout << " Red black tree 6 (13pts)" << endl;
	tree t6;
	t6.add_node(100);
	t6.add_node(50);
	t6.add_node(200);
	t6.add_node(300);
	t6.add_node(100);
	t6.delete_node(100);
	t6.InorderPrint(t6.root);
	cout << "    2pts" << endl;

	t6.add_node(100);
	t6.add_node(100);
	t6.add_node(100);
	t6.add_node(100);
	t6.add_node(100);

	t6.InorderPrint(t6.root);
	cout << "    3pts" << endl;
	t6.delete_node(200);
	t6.InorderPrint(t6.root);
	cout << "    2pts" << endl;

	t6.delete_node(300);
	t6.InorderPrint(t6.root);
	cout << "    2pts" << endl;


	t6.delete_node(100);
	t6.InorderPrint(t6.root);
	cout << "    2pts" << endl;


	t6.delete_node(100);
	t6.InorderPrint(t6.root);
	cout << "    2pts" << endl;

	return 0;
}