#include <iostream>
#include "List.h"
namespace cop3530
{
template <typename T>				//difference between class and typename??
class PSLL: public List<T> 		//simple singly-linked list
{
	template <typename DataT>
	class PSLL_Iter
	{
		public:
			// type aliases required for C++ iterator compatibility
			using value_type = DataT;
			using reference = DataT&;
			using pointer = DataT*;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
		
			// type aliases for prettier code
			using self_type = PSLL_Iter;
			using self_reference = PSLL_Iter&;
		      
		private:
			Node<DataT>* here;
		      
		public:
		  	explicit PSLL_Iter( Node<DataT>* start = nullptr ) : here( start ) {}
		 	PSLL_Iter( const PSLL_Iter& src ) : here( src.here ) {}
		      
			reference operator*() const //return a memory location
			{
				return here->value;
			}
			
			pointer operator->() const
			{
				return &this;
			}
		      
			self_reference operator=( PSLL_Iter<DataT> const& src )
			{
				this->here = src.here;
				return *this;
			}
		      
			self_reference operator++() // preincrement
			{
				here = here->nextNodePointer;
				return *this;
			}
		      
			self_type operator++(int)	 // postincrement
			{
				self_type temp(*this);
				++(*this);
				return temp;
			}
		      
			bool operator==( PSLL_Iter<DataT> const& rhs ) const
			{
				return (this->here == rhs.here);

			}
			
			bool operator!=( PSLL_Iter<DataT> const& rhs) const
			{
				return (this->here != rhs.here);
			}
	};	 // end PSLL_Iter

	private:
		Node <T> *head;
		Node <T> *tail;
		Node <T> *free;
	
	public:
		PSLL();
		void deallocate();
		Node<T>* add(T obj);
		//all other methods should come from List class
		
		void insert(T, int position);
		void push_back(T);
		void push_front(T);
		T replace(T, int position);
		T remove(int position);
		T pop_back();
		T pop_front();
		T& item_at(int position);
		T& peek_back();
		T& peek_front();
		bool is_empty();
		bool is_full();
		int length();
		void clear();
		bool contains(T, bool (*equals_function)(const T& y, const T& z));
		void print(std::ostream &out);	
		T* contents();
		
		//BIG 5
		PSLL(const PSLL& old) //copy constructor
		{
			head = old.head;		//should call copy constructor for Node.
			Node<T>* temp = head;
			while(temp->nextNodePointer)
			{
				temp = temp->nextNodePointer;
			}
			tail = temp;
			free = nullptr;
		}
		~PSLL()	{	delete head;	delete free;}
		PSLL operator=(PSLL& old)
		{
			head = new Node<T>(); 		//create a new Node will recursively make new Nodes until nullptr.
			Node<T> temp = head;
			while(temp)
			{
				tail = temp;
				temp = temp->nextNodePointer;
			}
		}
		
		PSLL(PSLL&& other)		//move constructor
		{
			head = other.head;		//this shoudld call the copy constructor.
			tail = other.tail;
			other.head = nullptr;
			other.tail = nullptr;
		}
		
		PSLL& operator=(PSLL&& other)		//move assignment operator
		{
			if (this != &other) {}
			else
			{
				head = other.head;
				tail = other.tail;
				other.head = nullptr;
				other.tail = nullptr;
			}
			return *this;
		}
		
		using iterator = PSLL_Iter<T>;
		using const_iterator = PSLL_Iter<T const>;
		
		iterator begin() { return PSLL_Iter<T>(head); }
		iterator end() { return PSLL_Iter<T>(NULL); }
		const_iterator begin() const { return PSLL_Iter<T const>(head); }  
		const_iterator end() const { return PSLL_Iter<T const>(NULL); } 
};

template <typename T>	
PSLL<T>::PSLL()
{
	head = nullptr;
	tail = head;
	free = nullptr;
}

template <typename T>
void PSLL<T>::insert(T x, int position)
{
	Node<T>* temp = head;
	int counter = 0;
	if(position == 0)
	{
		push_front(x);
		return;
	}
	if(position == length())
	{
		push_back(x);
		return;
	}
	while(temp->nextNodePointer != nullptr)		//You have not reached the end of the LL
	{
		if(counter == (position))
		{
			Node<T>* hold = temp->nextNodePointer;
			Node<T> * added = new Node<T> (x);
			temp->nextNodePointer = added;
			added->nextNodePointer = hold;
			return;
		}
		temp= temp->nextNodePointer;
		counter++;
	}
	if(position>counter)
	{
		push_back(x);
	}
}

template <typename T>
void PSLL<T>::push_back(T x)
{
	if(tail)
	{
		tail->nextNodePointer = add(x);
		tail = tail->nextNodePointer;
		tail->nextNodePointer = nullptr;
	}
	else
	{
		tail = add(x);
		tail->nextNodePointer = nullptr;
	}
}

template <typename T>
void PSLL<T>::push_front(T x)
{
	Node<T> * temp = head;
	head = add(x);
	head->nextNodePointer = temp;
	if(length() == 1)
		tail = head;
}

template <typename T>
T PSLL<T>::replace(T x, int position)	//1st element is #1
{
	if(position >= length() || position <0)
	{
		throw "This position is not within the range.";
	}
	//changes the current value to the new one and returns the original value
	Node<T>* temp = head;
	int counter = 0;
	while(temp != nullptr)		//You have not reached the end of the LL
	{
		if(counter == (position-1))
		{
			T hold = temp->value;
			temp->value = x;
			return hold;
		}
		temp= temp->nextNodePointer;
		counter++;
	}
	
//	if(position>counter)			//if you just want to add it to the end.
//	{
//		push_back(x);
//	}
	return tail->value;		//Should never have to execute this line.
}

template <typename T>
T PSLL<T>::remove(int position)
{
	Node<T>* temp = head;
	
	if(position == 0)		//The very first value (head)
	{
		//Add it to the pool of free Nodes
		Node<T>* hold = free;	//hold has the free Nodes.
		free = head;
		T hold2 = head->value;
		head = head->nextNodePointer;
		free->nextNodePointer = hold;
		return hold2;
	}
	
	int counter = 1;
	while(temp != nullptr)	//You have not reached the end of the LL && the position is more than 0.
	{
		if(counter == (position-1))
		{
			Node<T>* hold = free;
			free = temp->nextNodePointer;
			T hold2 = temp->nextNodePointer->value;	
			temp->nextNodePointer = (temp->nextNodePointer)->nextNodePointer;
			free->nextNodePointer = hold;
			return hold2;
		}
		temp= temp->nextNodePointer;
		counter++;
	}
	return head->value;				//should never have to execute this.
}

template <typename T>
T PSLL<T>::pop_back()
{
	Node<T>* temp = head;				//if head == tail, does nullptr == nullptr?
	try
	{
		//if 0 items
		if(!tail)
		{
			throw 99;
		}
		
		//If only one item
		if(head == tail)		//should be able to replace with (temp->nextNodePointer == nullptr) 
		{
			T hold = head->value;		//head, tail and temp should all point to the same thing.
			
			//add to free pool
			Node<T>* hold2 = free;
			free = head;
			free->nextNodePointer = hold2;
			
			head = nullptr;
			tail = head;
			return hold;
		}
		
		while(temp->nextNodePointer->nextNodePointer != nullptr)
		{
			temp = temp->nextNodePointer;
		}
		
		Node<T>* holdFree = free;
		free = temp->nextNodePointer;
		free->nextNodePointer = holdFree;
		
		T hold = temp->nextNodePointer->value;
		tail = temp->nextNodePointer;
		temp->nextNodePointer = nullptr;				//tail->nextNodePointer = nullptr;
		return hold;
	}
	catch(int x)
	{
		std::cout << "Nothing to pop." << std::endl;
	}
}

template <typename T>
T PSLL<T>::pop_front()
{
	T hold = head->value; 	//to return later
	
	//adding head to the free list.
	Node<T>* holdFree = free;
	free = head;
	head = head->nextNodePointer;
	free->nextNodePointer = holdFree;
	return hold;
}

template <typename T>
T& PSLL<T>::item_at(int position)		//1st element is #1
{
	if(position >= length() || position <0)
	{
		throw "This position is not within the range.";
	}
	//returns without removing
	Node<T>* temp = head;
	int counter = 0;
	
	while(temp != nullptr)
	{
		if(counter == position-1)
		{
			return temp->value;
		}
		temp = temp->nextNodePointer;
		counter++;
	}
	return head->value;		//It should never come to this, however.
}

template <typename T>
T& PSLL<T>::peek_back()
{
	return tail->value;
}

template <typename T>
T& PSLL<T>::peek_front()
{
	return head->value;
}

template <typename T>
bool PSLL<T>::is_empty()
{
	if(head== tail && head == nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool PSLL<T>::is_full()
{
	return false;
}

template <typename T>
int PSLL<T>::length()
{
	Node<T>* temp = head;
	int count = 0;
	
	while(temp != nullptr)
	{
		count++;
		temp = temp->nextNodePointer;
	}
	return count;
}

template <typename T>
void PSLL<T>::clear()
{
	Node<T>* temp = head;
	while(temp != nullptr)
	{
		Node<T>* freeHold = free;
		free = temp;
		free->nextNodePointer = freeHold;
		temp = temp->nextNodePointer;
	}
	delete head;
	head = nullptr;
	tail = nullptr;
	deallocate();		//If too many Nodes in free, this will get rid of some of them.
}

template <typename T>
T* PSLL<T>::contents()
{
	T * array = new T[length()];
	Node<T>* temp = head;
	int counter = 0;
	
	while(temp != nullptr)
	{
		array[counter] = temp->value;
		counter++;
		temp = temp->nextNodePointer;
	}
	return array;
}

template <typename T>
void PSLL<T>::print(std::ostream &out)
{
	Node<T>* temp = head;
	
	while(temp != nullptr)
	{
		out<< temp->value << ", ";
		temp = temp->nextNodePointer;
	}
	std::cout<< std::endl;
}

template <typename T>
bool PSLL<T>::contains(T x, bool (*equals_function)(const T& y, const T& z))
{
	Node<T>* temp = head;
	
	while(temp != nullptr)
	{
		if(equals_function(temp->value, x))
		{
			return true;
		}
		temp = temp->nextNodePointer;
	}
	return false;
}

template <typename T>
void PSLL<T>::deallocate()
{
	int size = length();		//length of head to tail.
	
	if(size>99)		//size is 100 or more.
	{
		Node<T>* temp = free;
		int counter = 0;
		while(temp != nullptr)		//iterates through all of the free Nodes.
		{
			if(counter > (size/2))
			{
				delete temp->nextNodePointer;
				return;
			}
			counter++;
			temp = temp->nextNodePointer;
		}
	}
}

template <typename T>
Node<T>* PSLL<T>::add(T obj) 	//take something out of the Free stack and add to main.
{
	if(free != nullptr)
	{
		free->value = obj;
		Node<T>* hold = free;
		free = free->nextNodePointer;
		return hold; 		//hold now is a Node that has the object you want.
	}
	else
	{
		return new Node<T>(obj);	//nothing is in free.
	}
	
}
}
