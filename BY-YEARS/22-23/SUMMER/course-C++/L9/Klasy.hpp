#pragma once
#include <iostream>


/*
<done> (i) wstawienie elementu na zadaną pozycję;
<done> (ii) wstawienie elementu na początek listy (czyli na pozycję 0);
<done> (iii) wstawienie elementu na koniec listy;
<done> (iv) usunięcie elementu z określonej pozycji;
<done> (v) usunięcie elementu z początku listy (czyli z pozycji 0);
<done> (vi) usunięcie elementu z końca listy;
<done> (vii) usunięcie elementu o zadanej wartości (pierwszego od początku);
<done> (viii) usunięcie wszystkich elementów o zadanej wartości;
<done> (ix) określenie pozycji elementu o zadanej wartości (pierwszego od początku);
<done> (x) policzenie wszystkich elelemtów o zadanej wartości;
<done> (xi) zliczenie wszystkich elementów;
<done> (xii) sprawdzenie czy lista jest pusta.
*/

using namespace std;

namespace calc { 

	template<typename T>class LinkedList{
    
	class Node{
		public:    
			T arg;
		    Node* next;
		    //public:											
		    Node(){
		    	this->next = NULL;
		    }

			~Node() {
				delete next;
			}
	};

	public:
		Node* head;
	
        LinkedList(){
            this->head = NULL;
        }
	
		int length() const { //(xi)
			int L = 0;
			Node* p = head;
			while(p != NULL) { //do kiedny mozna isc dalej to zwiększamy L i idziemy dalej
				L ++;
				p = p->next;
			}
			return L;
		}

		bool Is_empty() const { //(xii)
			if(length() == 0) return true;
			return false;
		}

		void show() {
			if(Is_empty()) {
				throw ("pusta lista ;cc");
			}
			Node* tmp = head;
			while (tmp != NULL) {
				cout << tmp-> arg << "->";
				tmp = tmp -> next;
			}
			cout<<endl;
		}

		void add_end(T given_arg, bool on=true) { // w rozumieniu na pozycje ostatnia (iii)
			
			Node* node = new Node; // robimy nowego noda którego bedziemy doklejac
			node->arg = given_arg; //val
			if(head == NULL) { // jezeli lista jest pusta to pakujemy bo to jedyny argument
				head = node; //przepinamy pointer
				if(on) cout << "Done argument added!" << endl;
				return;
			}
			Node* tmp = head;
			while(tmp->next != NULL) { // jak mozemy isc dalej to idziemy
				tmp = tmp->next;
			}
			tmp->next = node; // jak dojdziemy do nula to dopinamy nasz element ^-^
			if(on) cout << "Done argument added!(end)" << endl;
		}

		void add_begi(T given_arg) { // w rozumieniu na pozycje 0 (ii)
			Node* node = new Node;
			node->arg = given_arg; //val
			if(head == NULL) {
				head = node;
				cout << "Done argument added!" << endl;
				return;
			}
			node->next = head;
			head = node;
			cout << "Done argument added! (begi)" << endl;
		}

		void add_index(int i, T given_arg) {
			if(i > length() || i < 0) { // easy error handling
				throw("Wrong index");
			}
			Node* node = new Node;
			node->arg = given_arg;
			int it = 0;
			Node* tmp = head;
			// do tego momentu analogicznie jak wczesniej
			while(tmp != NULL && it < i) {
				if(it == i - 1 ) {
					if(tmp->next != NULL) {
						node->next = tmp->next; // jezeli nie jestesmy na koncu to 
					}
					tmp->next = node;
					cout << "Node added at given index" << endl;
					break;
				}
				it++;
				tmp = tmp->next;
			}
		}

		void remove_end() {
			if(Is_empty()) {
				throw ("pusta lista ;cc");
			}
			switch(length()) {
				case 0:
					cout << "Empty list nothing to remove here!" << endl; //hehe
					return;
				case 1:
					head = NULL;
					cout << "List item removed" << endl;
					return;
				default:
					Node* tmp = head;
					while(tmp != NULL) {
						if(tmp->next->next == NULL) { //jestesmy na koncu to usuwamy elem
							tmp->next = NULL; //odpinamy
							cout << "List item removed (end)" << endl;
							return;
						}
						tmp = tmp->next; //tuptamy dalej
					}
			}
			
		}

		void remove_beg() {
			if(Is_empty()) {
				throw ("pusta lista ;cc");
			}
			switch(length()) {
				case 0:
					throw("Empty list nothing to remove here");
				case 1:
					head = NULL;
					cout << "List item removed" << endl;
					return;
				default:
					head = head->next;
					cout << "List item removed (begi)" << endl;
					return;
			}
		}

		void remove_index(int i) {
			if(Is_empty()) {
				throw ("pusta lista ;cc");
			}
			switch(length()) {
				case 0:
					throw("Empty list nothing to remove here");
				case 1:
					head = NULL;
					cout << "List item removed" << endl;
					return;
				default:
					int it = 0;
					Node* tmp = head;
					while(tmp != NULL) {
						if(it == i - 1) {
							tmp->next = tmp->next->next; //poprostu przepinamy 
							cout << "List item removed" << endl;
							return;
						}
						it++;
						tmp = tmp->next;
					}			
				}
		}

		void remove_val(T val) {
			if(Is_empty()) {
				throw ("pusta lista ;cc";)
			}
			switch(length()) {
				case 0:
					throw("Empty list nothing to remove here");
				case 1:
					if(head->arg == val) {
					head = NULL;
					cout << "List item removed" << endl;
					return;
					}
				default:
					if(head->arg == val) {
						head = head->next;
						return;
					}
					Node* tmp = head;
					while(tmp != NULL) {
						if(tmp->next->arg == val) {
							tmp->next = tmp->next->next; //poprostu przepinamy 
							cout << "List item removed (val)" << endl;
							return;
						}
						else tmp = tmp->next;
						
					}
			}
		}

		void search(T val) {
			if(Is_empty()) {
				throw ("pusta lista ;cc");
			}
			switch(length()) {
				case 0:
					throw("Empty list nothing here");
				case 1:
					if(head->arg == val) {
					cout << "List item found at index 0" << endl;
					return;
					}
				default:
					int it = 0;
					Node* tmp = head;
					while(tmp != NULL) {
						if(tmp->arg == val) { 
							cout << "List item found at index: " << it << endl;
							return;
						}
						it++;
						tmp = tmp->next;
					}			
				}
		}
		
		void count_them(T val) {
			if(Is_empty()) {
				throw ("pusta lista ;cc");
			}
			switch(length()) {
				case 0:
					throw("Empty list nothing here");
				case 1:
					if(head->arg == val) {
					cout << "List item found at index 0 and it is the only one" << endl;
					return;
					}
				default:
					int it = 0;
					int counter = 0;
					Node* tmp = head;
					while(tmp != NULL) {
						if(tmp->arg == val) { 
							counter++;
							cout << "List item found it is the" << counter << " time!" << endl;
						}
						tmp = tmp->next;
					}
					return;
				}
		}	
		
		void remove_val_all(T val) {
			if(Is_empty()) {
				throw ("pusta lista ;cc");
			}
			switch(length()) {
				case 0:
					throw("Empty list nothing to remove here");
				case 1:
					if(head->arg == val) {
					head = NULL;
					cout << "List item removed" << endl;
					return;
					}
				default:
					while(head->arg == val) head = head->next;
					Node* tmp = head;
					while(tmp->next != NULL) {
						if(tmp->next->arg == val) {
							tmp->next = tmp->next->next; //poprostu przepinamy 
							cout << "List item removed (all)" << endl;
						}
						else tmp = tmp->next;
					}
			}
		}

		T get_head() {
			if(Is_empty()) {
				throw ("nie mamy cos scinac ;c");
			}
			return head->arg;
		}

		LinkedList(LinkedList<T>&& list) { // przenoszący
			cout << "konstruktor przenoszący ;>" << endl;
			head = NULL;
			if(list.head != NULL) {
		    	while(list.head != NULL) {
					add_end(list.head->arg, false);
					list.head = list.head->next;
				}
				//list.head = NULL;	
		    }
		} 

		LinkedList(const LinkedList<T>& list) { // przenoszący
			cout << "konstruktor przenoszący ;>" << endl;
			head = NULL;
			Node* tmp = list.head;
			if(tmp != NULL) {
		    	while(tmp != NULL) {
					add_end(tmp->arg, false);
					tmp = tmp->next;
				}
				//list.head = NULL;	
		    }
		}
		
		LinkedList(std::initializer_list<T> tab) {
			cout << endl; // bez tego nie działa jak cos
			for(auto x : tab) { //przechodzimy po wszystkich elementach taki for x in xd 
				add_end(x, false);
			}
			
		}

		~LinkedList() {
			delete head;
		}

		friend ostream& operator<<(ostream &stream, const LinkedList<T> &l) {
			if(l.Is_empty()) {
				stream << "pusta lista ;cc" << endl;
				return stream;
			}
			Node* tmp = l.head;
			while (tmp != NULL) {
				stream << tmp-> arg << "->";
				tmp = tmp -> next;
			}
			return stream;
		}
	};

	template<typename T>class Queue : private LinkedList<T> {
			public:
			
				Queue() {
					this->head = NULL;
				}
				
				using LinkedList<T> :: add_end;
				using LinkedList<T> :: get_head;
				using LinkedList<T> :: remove_beg;
				using LinkedList<T> :: length;
				using LinkedList<T> :: Is_empty;
		};
	
	template<typename T>class Stack : private LinkedList<T> {
			public:
				
				Stack() {
					this->head = NULL;
				}
				
				// tutaj robie troche na odwrot wszystko ale dzięki temu 
				// nie musze trawersowac sie po calej liscie :D
				using LinkedList<T> :: add_begi;
				using LinkedList<T> :: get_head;
				using LinkedList<T> :: remove_beg;
				using LinkedList<T> :: length;
				using LinkedList<T> :: Is_empty;
		};
	
};
	
	
	
	
	
	
	
	
	