#ifndef LIST_H
#define LIST_H
#include <iostream>
//an abstract class
namespace cop3530
{
template <typename E>
class List
{
	public:
		//all the methods that all the List classes should have
		virtual void insert(E, int position) = 0;
		virtual void push_back(E) = 0;
		virtual void push_front(E) = 0;
		virtual E replace(E, int position) = 0;
		virtual E remove(int position) = 0;
		virtual E pop_back() = 0;
		virtual E pop_front() = 0;
		virtual E& item_at(int position) = 0;
		virtual E& peek_back() = 0;
		virtual E& peek_front() = 0;
		virtual bool is_empty() = 0;
		virtual bool is_full() = 0;
		virtual int length() = 0;
		virtual void clear() = 0;
		virtual bool contains(E, bool (*equals_function)(const E& x, const E& y)) = 0;	
		virtual void print(std::ostream &out) = 0;	
		virtual E* contents() = 0;
};

template <typename W>
class Node 		//to be used by all of the linked list classes
{
	public:
		Node* nextNodePointer;
		W value;
		Node(W val);		//put in a E value here?
		Node(const Node& old);		//copy constructor
		~Node();
};

template <typename W>
Node<W>::Node(W val)
{
	value = val; 								//Set node value to n
};

template <typename W>
Node<W>::~Node()
{
	delete nextNodePointer;
}

template <typename W>
Node<W>::Node(const Node& old)		//copy constructor
{
	nextNodePointer = new Node(old.nextNodePointer);
	value = old.value;
	//recursively calls to make more Nodes.
}
}
#endif /*LIST_H*/
