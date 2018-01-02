#include "List.h"
#include <iostream>

namespace cop3530
{
template <typename T>				//difference between class and typename??
class SSLL: public List<T> 		//simple singly-linked list
{
	template <typename DataT>
	class SSLL_Iter
	{
		public:
			// type aliases required for C++ iterator compatibility
		  	using value_type = DataT;
			using reference = DataT&;
			using pointer = DataT*;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
		
			// type aliases for prettier code
			using self_type = SSLL_Iter;
			using self_reference = SSLL_Iter&;
		      
		private:
			Node<DataT>* here;
		      
		public:
		  	explicit SSLL_Iter( Node<DataT>* start = nullptr ) : here( start ) {}
		 	SSLL_Iter( const SSLL_Iter& src ) : here( src.here ) {}
		    
			reference operator*() const //return a memory location
			{
				return here->value;
			}
			
			pointer operator->() const
			{
				return &this;
			}
		      
			self_reference operator=( SSLL_Iter<DataT> const& src )
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
		      
			bool operator==( SSLL_Iter<DataT> const& rhs ) const
			{
				return (this->here == rhs.here);

			}
			
			bool operator!=( SSLL_Iter<DataT> const& rhs) const
			{
				return (this->here != rhs.here);
			}
	};	 // end SSLL_Iter
	
	private:
	public:
		Node <T> *head;
		Node <T> *tail;
		SSLL();
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
		void tryPosition(int pos);
		
		//big 5
		SSLL(const SSLL& old);		//copy constructor
		~SSLL();
		SSLL& operator=(SSLL& old)
		{
			head = new Node<T>(); 		//create a new Node will recursively make new Nodes until nullptr.
			Node<T> temp = head;
			while(temp)
			{
				tail = temp;
				temp = temp->nextNodePointer;
			}
			return *this;
		}
		
		SSLL(SSLL&& other)		//move constructor
		{
			head = other.head;		//this shoudld call the copy constructor.
			tail = other.tail;
			other.head = nullptr;
			other.tail = nullptr;
		}
		
		SSLL& operator=(SSLL&& other)		//move assignment operator
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
		
		
		using iterator = SSLL_Iter<T>;
		using const_iterator = SSLL_Iter<T const>;
		
		iterator begin(){	return SSLL_Iter<T>(head);	}
		iterator end(){	return SSLL_Iter<T>(nullptr);}    //This returns the iterator's pointer to end
		const_iterator begin() const{	return SSLL_Iter<T const>(head);	}  		
		const_iterator end() const{	return SSLL_Iter<T const>(nullptr);}
};

template <typename T>	
SSLL<T>::SSLL()
{
	tail = nullptr;
	head = tail;
}

template <typename T>
SSLL<T>::SSLL(const SSLL<T>& old)		//copy constructor
{
	head = new Node<T>(); 		//create a new Node will recursively make new Nodes until nullptr.
	Node<T> temp = head;
	while(temp)
	{
		tail = temp;
		temp = temp->nextNodePointer;
	}
}

template <typename T>
SSLL<T>::~SSLL()
{
	delete head;		//should call Node deconstructor for these Nodes
	//delete tail;		//WIll be done through recursive call for Node deconstructor.
}
		
template <typename T>
void SSLL<T>::insert(T x, int position)
{
	Node<T>* temp = head;
	int counter = 0;
	if(position == 0)
	{
		push_front(x);
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
void SSLL<T>::push_back(T x)
{
	tail->nextNodePointer = new Node<T>(x);
	this->tail = tail->nextNodePointer;
	tail->nextNodePointer = nullptr;
}

template <typename T>
void SSLL<T>::push_front(T x)
{
	Node<T> * temp = head;
	head = new Node<T>(x);
	head->nextNodePointer = temp;
	
	if(length() == 1) 
	{
		tail = head;
	}
}

template <typename T>
T SSLL<T>::replace(T x, int position)
{
	tryPosition(position);
	//changes the current value to the new one and returns the original value
	Node<T>* temp = head;
	int counter = 0;
	while(temp != nullptr)		//You have not reached the end of the LL
	{
		if(counter == (position))
		{
			T hold = temp->value;
			temp->value = x;
			return hold;
		}
		temp= temp->nextNodePointer;
		counter++;
	}
	
	if(position>counter)
	{
		push_back(x);
	}
	return head->value;		//added to remove error, should never have to execut this statement
}

template <typename T>
void SSLL<T>::tryPosition(int pos)
{
	try
	{
		if(pos < 0)
		{
			throw(1);
		}
	}
	catch(int y)
	{
		std::cout<< pos << " is not a valid position." << std::endl;
	}
}

template <typename T>
T SSLL<T>::remove(int position)		//1st element is position 1
{
	//First check if this position is valid
	tryPosition(position);
	Node<T>* temp = head;
	
	if(position == 0)
	{
		Node<T>* hold = head;
		head = head->nextNodePointer;
		hold->nextNodePointer = nullptr;
		delete hold;
		return temp->value;
	}
	
	int counter = 1;
	while(temp != nullptr)			//You have not reached the end of the LL && the position is more than 0.
	{
		if(counter == (position-1))
		{
			Node<T>* hold = temp->nextNodePointer;
			temp->nextNodePointer = (temp->nextNodePointer)->nextNodePointer;		//This works if you try to remove the last one.
			Node<T>* hold2 = hold;
			hold2->nextNodePointer = nullptr;
			delete hold2;
			return hold->value;
		}
		temp= temp->nextNodePointer;
		counter++;
	}
	return head->value;		//Just to get rid of error, but should never have to execute this statement.
}

template <typename T>
T SSLL<T>::pop_back()
{
	Node<T>* temp = head;
	
	//If only one item
	if(head == tail)		//should be able to replace with (temp->nextNodePointer == nullptr) 
	{
		T hold = head->value;		//head, tail and temp should all point to the same thing.
		Node<T>* hold2 = head;		
		head = nullptr;
		delete hold2;
		tail = head;
		return hold;
	}
	
	while(temp->nextNodePointer->nextNodePointer != nullptr)
	{
		temp = temp->nextNodePointer;
	}
	
	T hold = temp->nextNodePointer->value;
	Node<T>* hold2 = temp->nextNodePointer;
	delete hold2;
	temp->nextNodePointer = nullptr;
	return hold;
}

template <typename T>
T SSLL<T>::pop_front()
{
	T hold = head->value;
	Node<T>* hold2 = head;
	head = head->nextNodePointer;
	hold2->nextNodePointer = nullptr;
	delete hold2;
	return hold;
}

template <typename T>
T& SSLL<T>::item_at(int position)
{
	tryPosition(position);
	//returns without removing
	Node<T> * temp = head;
	int counter = 0;
	
	while(temp != nullptr)
	{
		if(counter == position)
		{
			return temp->value;
		}
		temp = temp->nextNodePointer;
		counter++;
	}
	return head->value;		//to get rid of error
}

template <typename T>
T& SSLL<T>::peek_back()
{
	return tail->value;
}

template <typename T>
T& SSLL<T>::peek_front()
{
	return head->value;
}

template <typename T>
bool SSLL<T>::is_empty()
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
bool SSLL<T>::is_full()
{
	return false;
}

template <typename T>
int SSLL<T>::length()
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
void SSLL<T>::clear()
{
	delete head;
	head = nullptr;
	tail = head;
}

template <typename T>
bool SSLL<T>::contains(T x, bool (*equals_function)(const T& y, const T& z))
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
T* SSLL<T>::contents()
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
void SSLL<T>::print(std::ostream &out)
{
	Node<T>* temp = head;
	while(temp != nullptr)
	{
		out<< temp->value << ", ";
		temp = temp->nextNodePointer;
	}
	out<< std::endl;
}

bool equals(int y, int z)
{
	return y==z;
}
}
