#include <iostream>
#include "List.h"
namespace cop3530
{
template <typename W>
class NodeCDAL
{
	public:
		NodeCDAL* nextNodePointer;
		W* arr;
		
		NodeCDAL();
		NodeCDAL(const NodeCDAL& old);
		~NodeCDAL();
};

template <typename T>				//difference between class and typename??
class CDAL: public List<T> 		//simple singly-linked list
{
	template <typename DataT>
	class CDAL_Iter
	{
		public:
			// type aliases required for C++ iterator compatibility
		  	using value_type = DataT;
			using reference = DataT&;
			using pointer = DataT*;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
		
			// type aliases for prettier code
			using self_type = CDAL_Iter;
			using self_reference = CDAL_Iter&;
		      
		private:
			NodeCDAL <DataT>* here;
			int index;
		      
		public:
		  	explicit CDAL_Iter( NodeCDAL<DataT>* start, int i) : here( start ), index(i) {}
		 	CDAL_Iter( const CDAL_Iter& src ) : here( src.here ), index(src.index) {}
		      
			reference operator*() const //return a memory location
			{
				return here->arr[index];
			}
			
			pointer operator->() const
			{
				return &(here->dataArray[index]);
			}
		      
			self_reference operator=( CDAL_Iter<DataT> const& src )
			{
				this->here = src.here;
				this->index = src.index;
				return *this;
			}
		      
			self_reference operator++() // preincrement
			{
				++index;
				if (index % 50 == 0){
					here = here->nextNodePointer;
					index = 0;
				}
				return *this;
			}
		      
			self_type operator++(int)	 // postincrement
			{
				self_type temp(*this);
				++(*this);
				return temp;
			}
		      
			bool operator==( CDAL_Iter<DataT> const& rhs ) const
			{
				return (this->here == rhs.here && this->index == rhs.index);
			}
			
			bool operator!=( CDAL_Iter<DataT> const& rhs) const
			{
				return (this->here != rhs.here && this->index != rhs.index);
			}
	};	 // end CDAL_Iter
	
	NodeCDAL <T> *data;		//essentially, the head
	int tail;
	public:
		CDAL();
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
		
		//big 5
		CDAL(const CDAL& old);		//copy constructor
		~CDAL();					//deconstructor
		CDAL& operator=(CDAL& old)		//assignment operator
		{
    		data = new NodeCDAL<T>(*old.data);		//recreate that Node and a copy of it by calling Node's copy constructor
  			tail = old.tail;						//data is a NodeCDAL
		}
		
		CDAL(CDAL&& other)		//move constructor
		{
			data = other.data;
			tail = other.tail;
			~data;
			//other.data = nullptr;
			other.tail = nullptr;
		}
		
		CDAL& operator=(CDAL&& other)		//move assignment operator
		{
			if (this != &other) {}
			else
			{
				data = other.data;
				tail = other.tail;
				~data;
				//other.data = nullptr;
				other.tail = nullptr;
			}
			return *this;
		}
		
		//for iterators
		using iterator = CDAL_Iter<T>;
		using const_iterator = CDAL_Iter<T const>;
		
		iterator begin(){	return CDAL_Iter<T>(data, 0);	}
		iterator end()	{	return CDAL_Iter<T>(NULL, tail);	}
		const_iterator begin() const { return CDAL_Iter<T const>(data, 0); }
		const_iterator end() const { return CDAL_Iter<T const>(NULL, tail); }
};

template <typename W>
NodeCDAL<W>::NodeCDAL()			//array of ambiguous W's
{
	nextNodePointer = nullptr;
	arr = new W [50]; 						//should create a new NodeCDAL of W, of length 50.
};

template <typename W>
NodeCDAL<W>::NodeCDAL(const NodeCDAL& old)		//copy constructor, takes in a NodeCDAL
{
	if(old->nextNodePointer == nullptr)
	{
		return;
	}
	
	//old->nextNodePointer
	nextNodePointer = new NodeCDAL<W>(old->nextNodePointer);
	//create a new array
	W* temp = new W [50];
	for(int i = 0; i < 50; i++)
	{
		temp[i] = old.arr[i];
	}
	this.arr = temp;
}

template <typename W>
NodeCDAL<W>::~NodeCDAL()
{
	delete [] arr;
	delete nextNodePointer;
}

template <class T>				//difference between class and typename??
CDAL<T>::CDAL()				//should define what T is in declaration.
{
	data = new NodeCDAL<T>;
	tail = -1;
}

template <typename T>
CDAL<T>::CDAL(const CDAL& old)					//copy constructor
{
    data = new NodeCDAL<T>(*old.data);		//recreate that Node and a copy of it by calling Node's copy constructor
    tail = old.tail;						//data is a NodeCDAL
}

template <typename T>
CDAL<T>::~CDAL()
{
	delete data;		//data is a Node so it should call the Node deconstructor.
}

template <typename T>
void CDAL<T>::insert(T x, int position)
{
	NodeCDAL<T>* temp = data;
	T hold = data->arr[position];
	T hold2;
	temp->arr[position] = x;
	int counter = position+1;
	while(temp != nullptr)
	{
		hold2 = data->arr[counter];
		data->arr[counter] = hold;
		hold = hold2;
		counter++;
		if(counter == 50)
		{
			counter = 0;
			temp = temp->nextNodePointer;
		}
	}
	tail++;
}

template <typename T>
void CDAL<T>::push_back(T x)
{
	tail++;
	NodeCDAL<T>* n = data;
	while(n->nextNodePointer != nullptr)
	{
		n= n->nextNodePointer;
	}
	if(tail%50 == 0)
	{
		n->nextNodePointer = new NodeCDAL<T>();		//new array added if necessary. Now just go ahead and add the value...
		n = n->nextNodePointer;
		n->arr [0] = x;
		return;
	}
	else
	{
		n->arr[tail%50] = x;
	}
	
}

template <typename T>
void CDAL<T>::push_front(T x)
{
	NodeCDAL<T>* temp = data;
	T hold = data->arr[0];
	T hold2;
	temp->arr[0] = x;
	int counter = 1;
	while(temp != nullptr)
	{
		hold2 = data->arr[counter];
		data->arr[counter] = hold;
		hold = hold2;
		counter++;
		if(counter == 50)
		{
			counter = 0;
			temp = temp->nextNodePointer;
		}
	}
	tail++;
}

template <typename T>
T CDAL<T>::replace(T x, int position)
{
	//if(position > tail) throw exception
	int whichNode = position/50;
	int whichSpot = position%50;
	NodeCDAL<T>* temp = data;
	
	for(int counter = 0; counter < whichNode; counter++)
	{
		temp = temp->nextNodePointer;
	}
	
	T hold = temp->arr[whichSpot];
	temp->arr[whichSpot] = x;
	return hold;
}

template <typename T>
T CDAL<T>::remove(int position)
{
	int counter = position%50;
	T hold = data->arr[counter];
	NodeCDAL<T>* temp = data;
	int overallCounter = position;
	while(temp != nullptr && overallCounter < tail)
	{
		temp->arr[counter] = temp->arr[counter+1];
		counter++;
		overallCounter++;
		if(counter == 50)
		{
			temp = temp->nextNodePointer;
			counter = 0;
		}
	}
	tail--;
	return hold;
}

template <typename T>
T CDAL<T>::pop_back()
{
	NodeCDAL<T>* temp = data;
	while(temp->nextNodePointer != nullptr)
	{
		temp = temp->nextNodePointer;
	}
	tail--;
	return temp->arr[(tail+1)%50];
}

template <typename T>
T& CDAL<T>::item_at(int position)
{
	int whichNode = position/50;
	NodeCDAL<T>* temp = data;
	
	for(int i = 0; i < whichNode; i++)
	{
		temp = temp->nextNodePointer;
	}
	return temp->arr[position%50];
}

template <typename T>
T& CDAL<T>::peek_back()
{
	NodeCDAL<T>* temp = data;
	while(temp->nextNodePointer != nullptr)
	{
		temp = temp->nextNodePointer;
	}
	return temp->arr[tail%50];
}

template <typename T>
T& CDAL<T>::peek_front()
{
	return data->arr[0];
}

template <typename T>
bool CDAL<T>::is_empty()
{
	if(tail == -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool CDAL<T>::is_full()
{
	return false;
}

template <typename T>
int CDAL<T>::length()
{
	return tail+1;
}

template <typename T>
void CDAL<T>::clear()
{
	data= new NodeCDAL<T>();
	tail = -1;
}

template <typename T>
bool CDAL<T>::contains(T x, bool (*equals_function)(const T& y, const T& z))
{
	NodeCDAL<T>* temp = data;
	int counter = 0;
	while(temp != nullptr)
	{
		if(equals_function(temp->arr[counter], x))
		{
			return true;
		}
		counter++;
		if(counter == 50)
		{
			temp = temp->nextNodePointer;
			counter = 0;
		}
	}
	return false;
}

template <typename T>
void CDAL<T>::print(std::ostream &out)
{
	int counter = 0;
	int counter2 = 0;
	NodeCDAL<T>* temp = data;
	
	while(temp != nullptr && counter2 <= tail)
	{
		out<<temp->arr[counter] << ", ";
		counter++;
		counter2++;
		if(counter== 50)
		{
			temp = temp->nextNodePointer;
			counter = 0;
		}
	}
	out<< std::endl;
}

template <typename T>
T* CDAL<T>::contents()
{
	T* arr = new T [length()];
	for(int i = 0; i < length(); i++)
	{
		arr[i] = data->arr[i%50];
	}
	return arr;
}

template <typename T>
T CDAL<T>::pop_front()
{
	T hold = data->arr[0];
	int counter = 0;
	NodeCDAL<T>* temp = data;
	int position = 0;
	while(temp != nullptr && position < tail)
	{
		temp->arr[counter] = temp->arr[counter+1];
		counter++;
		position++;
		if(counter == 50)
		{
			temp = temp->nextNodePointer;
			counter = 0;
		}
	}
	tail--;
	return hold;
}
}
