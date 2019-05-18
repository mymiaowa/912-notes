#ifndef LIST_H_
#define LIST_H_

#include "ListNode.h"

template <typename T>
class List{
private:
	ListNodePosi(T) head;
	ListNodePosi(T) tail;
	int             size;

protected:// internal methods


public:
	//constructors
	List();									// empty List
	List(List const &L);					// copy from another List
	List(List const &L, int rank, int n);	// copy n elements from L[rank]
	List(ListNodePosi(T) p, int n);			// copy n elements from position p

	//deconstructor
	~List();

	//read-only interfaces
	ListNodePosi(T) first() const { return head->succ; }
	ListNodePosi(T) last() const { return tail->prev; }
	ListNodePosi(T) find(T const &val) const { return find(val, size, tail); }		//find val from all elements of the list
	ListNodePosi(T) find(T const &val, int n, ListNodePosi(T) p) const;				//find val from a range of n elements before p
	T& operator[](int rank);
	int getSize() const { return size; }
	bool empty() const { return size == 0; }

	//writable interfaces

	//insert and delete operations
	ListNodePosi(T) push_back(T const &val);
	ListNodePosi(T) push_front(T const & val);
	ListNodePosi(T) insert_before(int rank, T const &val);
	ListNodePosi(T) insert_before(ListNodePosi(T) p, T const &val);
	ListNodePosi(T) pop_back();
	ListNodePosi(T) pop_front();
	ListNodePosi(T) pop(int rank);
	ListNodePosi(T) pop(ListNodePosi(T) p);

	//deduplicate & uniquify
	int deduplicate();
	int uniquify();
};

/*----------implementations of class List----------*/

//constructors
template <typename T>
List<T>::List(){
	tail = new ListNode<T>();
	head = new ListNode<T>();
	tail->prev = head;
	head->succ = tail;
	size       = 0;
}

template <typename T>
List<T>::List(List const &L) {
	tail = new ListNode<T>();
	head = new ListNode<T>();
	tail->prev = head;
	head->succ = tail;
	size = 0;

	ListNodePosi(T) p = L.head->succ;
	for (int ix = 0, size = L.getSize(); ix != size; ++ix, p = p->succ)
		push_back(p->val);
}

template <typename T>
List<T>::List(List const &L, int rank, int n){
	ListNodePosi(T) p = L.head->succ;
	while (rank--) p = p->succ;
	
	tail = new ListNode<T>();
	head = new ListNode<T>();
	tail->prev = head;
	head->succ = tail;
	size = 0;

	for (int ix = 0; ix != n; ++ix, p = p->succ)
		push_back(p->val);
}

template <typename T>
List<T>::List(ListNodePosi(T) p, int n){
	tail = new ListNode<T>();
	head = new ListNode<T>();
	tail->prev = head;
	head->succ = tail;
	size = 0;

	for (int ix = 0; ix != n; ++ix, p = p->succ)
		push_back(p->val);
}

//deconstructor
template <typename T>
List<T>::~List(){
	while (size) pop_back();
}

//read-only interfaces

template <typename T>
ListNodePosi(T) List<T>::find(T const &val, int n, ListNodePosi(T) p) const {
	while ((p = p->prev) != head && n--)
		if (p->val == val) return p;
	return nullptr;
}

template <typename T>
T& List<T>::operator[](int rank){
	ListNodePosi(T) p = head->succ;
	while (rank--) p = p->succ;
	return p->val;
}

// writable interfaces

//insert and delete operations
template <typename T>
ListNodePosi(T) List<T>::push_back(T const &val){
	++size;
	return tail->insertAsPred(val);
}

template <typename T>
ListNodePosi(T) List<T>::push_front(T const &val) {
	++size;
	return head->insertAsSucc(val);
}

template <typename T>
ListNodePosi(T) List<T>::pop_back() {
	if (empty()) return nullptr;
	
	ListNodePosi(T) res = tail->prev;
	res->prev->succ     = tail;
	tail->prev          = res->prev;
	--size;
	return res;
}

template <typename T>
ListNodePosi(T) List<T>::pop_front() {
	if (empty()) return nullptr;

	ListNodePosi(T) res = head->succ;
	head->succ          = res->succ;
	res->succ->prev     = head;
	--size;
	return res;
}

template <typename T>
ListNodePosi(T) List<T>::insert_before(int rank, T const &val){
	ListNodePosi(T) p = head->succ;
	while (rank--) p = p->succ;
	return insert_before(p, val);
}

template <typename T>
ListNodePosi(T) List<T>::insert_before(ListNodePosi(T) p, T const &val){
	++size;
	return p->insertAsPred(val);
}

template <typename T>
ListNodePosi(T) List<T>::pop(int rank){
	if (rank >= size || rank < 0) return nullptr;

	ListNodePosi(T) p = head->succ;
	while (rank--) p = p->succ;
	return pop(p);
}

template <typename T>
ListNodePosi(T) List<T>::pop(ListNodePosi(T) p){
	p->prev->succ = p->succ;
	p->succ->prev = p->prev;
	--size;
	return p;
}

//deduplicate & uniquify
template <typename T>
int List<T>::deduplicate(){
	int oldSize = size;
	ListNodePosi(T) p = head->succ;
	ListNodePosi(T) tmp;
	for (; p != tail; p = p->succ){
		for(tmp = head->succ; tmp != p; tmp = tmp->succ){
			if(tmp->val == p->val){
				pop(tmp);
				break;
			}
		}
	}
	return oldSize - size;
}

template <typename T>
int List<T>::uniquify(){
	if (empty()) return 0;

	int oldSize = size;
	ListNodePosi(T) p = head->succ;
	while ((p = p->succ) != tail)
		if (p->val == p->prev->val) pop(p);
	return oldSize - size;
}

#endif
