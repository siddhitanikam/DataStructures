//HW2 by Siddhita Nikam
//SU Net ID: snikam  SUID: 471330539
//CSE674 HW2
//Due: 11:59PM, Thursday, September 22
//Do not modify main function.
//Do not introduce new functions.
//In-place implementation is required.  External structures are not allowed.
//For example, you are not allowed to copy linked list values to outside, and then process the data and copy them
//back to linked list.
//Need to use the same parameters of functions as stated.



#include <iostream>
using namespace std;

class Node {
public:
	int value;
	Node* next;
	Node* previous;
	Node(int i) { value = i; next = previous = nullptr; }
	Node() { next = previous = nullptr; }
};

class DoublyLinkedList {
public:
	Node* head;
	Node* tail;
	DoublyLinkedList() { head = tail = nullptr; }
	DoublyLinkedList(int n, int m);//constructor for an n-node DoublyLinkedList with random values in 0 ... m-1  
	void printForward();
	void printBackward();
	//implement the following member function:
	void MergeSortIte(Node* p1, Node* p2);//Implement MergeSort iteratively. Sort a portion of a DoublyLinkedList, from the node pointed by p1 to the node pointed by p2.
	void MergeSortRec(Node* p1, Node* p2); //Implement MergeSort recursively. Sort a portion of DoublyLinkedList, from the node pointed by p1 to the node pointed by p2.
	void Merge(Node* p1, Node* p2, Node* p3, Node* p4); //Merge two portions of a DoublyLinkedList, one from the node pointed by p1 to the node pointed by p2 and
	//the other from the node pointed by p3 to the node pointed by p4.
	//The function Merge can be called by functions MergeSortIte and MergeSortRec;
};

void DoublyLinkedList::MergeSortIte(Node* p1, Node* p2) {
	//Your code
	cout << endl << " in Merge Iterative " << endl;
	int size = 1;
	int checkEnd = 0;
	cout << "\n";
	Node* slow = p1;
	Node* fast = p1;
	Node* mid = p1;
	Node* temp = p1;
	Node* temp1 = nullptr;

	int count = 0;
	Node* p1Bkp = p1;
	Node* prevNode = p1->previous;
	Node* nextNode = p2->next;
	Node* head1 = nullptr;
	Node* tail1 = nullptr;
	Node* head2 = nullptr;
	Node* tail2 = nullptr;
	p2->next = nullptr;
	while (p1 != NULL) {
		count++;
		p1 = p1->next;
	}
	if (p1 == p2) {
		//cout << "P1 and p2 are same " << endl;
		return;
	}
	for (size = 1; size < count; size *= 2) {
		//cout << "In For " << size << endl;
		/*for (int i = 0; i < count; i += 2 * size) {
			cout << "In i loop " << i << endl;
			for (int j = i + 1; j < i + size; j++) {
				slow = slow->next;
			}
			for (int k = i + 1; k < i + 2 * size; k++) {
				fast = fast->next;
			}
			//cout << temp->value << "  " << slow->value << endl;
			temp = slow;
			while (temp != fast) {
				cout << " in while ";
				temp = temp->next;
				//cout << " in while after last next 1st time " << endl;
				if (temp != fast) {
					temp = temp->next;
					mid = mid->next;
					cout << "In if to check" << endl;
				}
			}
			cout << "slow - " << slow->value << "  fast - " << fast->value << " mid - " << mid->value << endl;
			Merge(p1Bkp, slow, slow->next, fast);*/

		head1 = p1;
		while (head1) {

			bool forFirstitr = 0;
			if (head1 == p1)
				forFirstitr = 1;

			int count = size;
			tail1 = head1;
			while (--count && tail1->next)
				tail1 = tail1->next;

			head2 = tail1->next;
			if (!head2)
				break;
			count = size;
			tail2 = head2;
			while (--count && tail2->next)
				tail2 = tail2->next;

			Node* temp = tail2->next;

			DoublyLinkedList dl;
			head1->previous = nullptr;
			tail2->next = nullptr;
			dl.Merge(head1, tail1, head2, tail2);
			head1 = dl.head;
			tail2 = dl.tail;


			if (forFirstitr)
				p1 = head1;
			else
				temp1->next = head1;

			temp1 = tail2;
			head1 = temp;
		}
		temp1->next = head1;
	}
	temp1->next = nullptr;


	Node* current = p1, * previousPart = nullptr;
	while (current) {
		current->previous = previousPart;
		previousPart = current;
		current = current->next;
	}
	

	if (prevNode) {
		prevNode->next = p1;
		p1->previous = prevNode;
	}
	else {
		head = p1;
	}
	if (nextNode) {
		nextNode->previous = previousPart;
		previousPart->next = nextNode;
	}
	else {
		tail = previousPart;
	}
}
void DoublyLinkedList::MergeSortRec(Node* p1, Node* p2) {
	//Your code
	//cout << endl << " in Merge Recursive ";
	Node* prevNode = p1->previous;
	Node* nextNode = p2->next;
	p2->next = nullptr;
	Node* temp = nullptr;
	Node* mid = p1;
	Node* last = p1;

	DoublyLinkedList dl1;
	DoublyLinkedList dl2;

	//cout << " Before : " << " p1 value " << p1->value << " mid value " << mid->value << " mid next " << mid->next->value << " p2 value " << p2->value << "  last value " << last->value <<endl;
	if (p1 == p2) {
		//cout << "P1 and p2 are same " << endl;
		return;
	}
	while (last != nullptr && last->next != nullptr) {
		temp = mid;
		last = last->next->next;
		mid = mid->next;
	}
	temp->next = nullptr;
	mid->previous = nullptr;

	dl1.head = p1;
	dl1.head->previous = nullptr;
	dl1.tail = temp;
	dl1.tail->next = nullptr;
	dl1.MergeSortRec(dl1.head, dl1.tail);

	/*cout << " After : " << "p1 value " << p1->value << " mid value " << mid->value << " mid next " << mid->next->value << " p2 value " << p2->value << "  ";*/
	dl2.head = mid;
	dl2.head->previous = nullptr;
	dl2.tail = p2;
	dl2.tail->next = nullptr;
	dl2.MergeSortRec(dl2.head, dl2.tail);

	dl1.head->previous = prevNode;
	dl2.tail->next = nextNode;

	Merge(dl1.head, dl1.tail, dl2.head, dl2.tail);
	//mid = p1->next;
	//cout << " *****DONE!!" << endl;
	/*cout << " AFTER merge : " << "p1 value " << p1->value << " mid value " << mid->value << " mid next " << mid->next->value << " p2 value " << p2->value << "  ";*/
}

void DoublyLinkedList::Merge(Node* p1, Node* p2, Node* p3, Node* p4) {
	//Your code
	Node* p1bkp = p1;
	Node* prev = p1->previous;
	Node temp(-1);
	Node* curr = &temp;
	Node* last = p4->next;
	//cout << " In Merge Function  " << endl;
	//cout << "prev value " << prev->value << " last value " << last->value << endl;
	//cout << " p1 value " << p1->value << " p2 value " << p2->value << " p3 Value  " << p3->value << " p4 value " << p4->value << endl;
	p2->next = nullptr;
	p4->next = nullptr;
	//cout << "BEFORE";
	/*while (p1 != p4->next) {
		cout << p1->value << "->";
		p1 = p1->next;
	}*/
	while (p1 != p2->next && p3 != p4->next) {
		if (p1->value < p3->value) {
			if (curr != nullptr) {//for 1st element
				curr->next = p1;
				//p1->previous = prev;
				//prev != nullptr ? prev->next = p1 : nullptr;

			}
			p1->previous = curr;
			curr = p1;
			p1 = p1->next;
		}
		else {
			if (curr != nullptr) {
				curr->next = p3;
				//p3->previous = prev;
				//prev != nullptr ? prev->next = p3 : nullptr;
			}
			p3->previous = curr;
			curr = p3;
			p3 = p3->next;
		}
		//cout << "curr value " << curr->value << endl;

	}

	if (p1 == p2->next) {
		while (p3 != p4->next) {
			//p3->next = curr->next;
			curr->next = p3;
			p3->previous = curr;
			curr = p3;
			p3 = p3->next;
			//p3->previous = curr;
		}
	}
	else if (p3 == p4->next) {
		while (p1 != p2->next) {
			//p1->next=curr->next;
			curr->next = p1;
			p1->previous = curr;
			curr = p1;
			p1 = p1->next;
			//p1->previous = curr;
		}
	}

	curr->next = nullptr;
	if (temp.next) {
		temp.next->previous = nullptr;
	}

	if (prev) {
		prev->next = temp.next;
		prev->next->previous = prev;
	}
	else {
		head = temp.next;
	}

	if (last) {
		last->previous = curr;
		curr->next = last;
	}
	else {
		tail = curr;
	}

	/*curr->next = last;
	last->previous = curr;*/
	//cout << "curr value " << curr->value << endl;

	//cout << endl;
}



DoublyLinkedList::DoublyLinkedList(int n, int m) {
	head = tail = nullptr;
	for (int i = 0; i < n; ++i) {
		Node* p1 = new Node{ rand() % m };
		if (!head) {
			head = tail = p1;
		}
		else {
			tail->next = p1;
			p1->previous = tail;
			tail = p1;
		}
	}
}


void DoublyLinkedList::printForward() {
	cout << endl;
	Node* p1 = head;
	while (p1) {
		cout << p1->value << " ";
		p1 = p1->next;
	}
}

void DoublyLinkedList::printBackward() {
	cout << endl;
	Node* p1 = tail;
	while (p1) {
		cout << p1->value << " ";
		p1 = p1->previous;
	}
}

int main() {
	int a{ 50 }, b{ 30 };
	DoublyLinkedList d1{ a,b }, d2{ a,b };

	d1.printForward();
	d1.printBackward();

	//d1.MergeSortIte(d1.head, d1.tail);
	d1.printForward();
	d1.printBackward();

	d2.printForward();
	d2.printBackward();

	d2.MergeSortRec(d2.head, d2.tail);
	d2.printForward();
	d2.printBackward();

	return 0;
}