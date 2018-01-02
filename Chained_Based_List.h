#include <iostream>
#include "List.h"

namespace cop3530
{
template <typename T>				//difference between class and typename??
class CBL: public List<T> 		//simple singly-linked list
{
	template <typename DataT>
	class CBL_Iter
	{
		public:
			// type aliases required for C++ iterator compatibility
		  	using value_type = DataT;
			using reference = DataT&;
			using pointer = DataT*;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
			
			// type aliases for prettier code
			using self_type = CBL_Iter;
			using self_reference = CBL_Iter&;
		      
		private:
			DataT* here;
		      
		public:
		  	explicit CBL_Iter( value_type *start) : here(start) {}
		 	CBL_Iter( const CBL_Iter& src ) : here( src.here ) {}
		      
			reference operator*() const //return a memory location
			{
				return *here;
			}
			
			pointer operator->() const
			{
				return &here;
			}
		      
			self_reference operator=(CBL_Iter<DataT> const& src )
			{
				this->here = src.here;
				return *this;
			}
		      
			self_reference operator++() // preincrement
			{
				++here;
				return *this;
			}
		      
			self_type operator++(int)	 // postincrement
			{
				self_type temp(*this);
				++(*this);
				return temp;
			}
		      
			bool operator==(CBL_Iter<DataT> const& rhs ) const
			{
				return (this->here == rhs.here);

			}
			
			bool operator!=(CBL_Iter<DataT> const& rhs) const
			{
				return (this->here != rhs.here);
			}
	};	 // end CBL_Iter
	
	public:
		T* data;
		int head;
		int tail;
		int size;
		int numSpotsUsed;
		CBL();
		CBL(int l);
		void make_new_data();
		void deallocate();
		void numSpots();
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
		bool contains(T, bool (*equals_function)(const T& x, const T& y));
		void print(std::ostream &out);
		T* contents();
		
		//Big 5
		CBL(const CBL& old)		//copy constructor
		{
			T* temp = new T [size];		//makes a completely different array for the new object.
			for(int i = 0; i <= tail; i++)
			{
				temp[i] = old.data[i];
			}
			this.data = temp;
			this.head = old.head;
			this.tail = old.tail;
			this.size = old.size;
			this.numSpotsUsed = old.numSpots();
		}
		~CBL()
		{
			delete [] data;
		}
		CBL& operator=(CBL& old)		//assignment operator
		{
			T* temp = new T [size];		//makes a completely different array for the new object.
			for(int i = 0; i <= tail; i++)
			{
				temp[i] = old.data[i];
			}
			this.data = temp;
			this.head = old.head;
			this.tail = old.tail;
			this.size = old.size;
			this.numSpotsUsed = old.numSpots();
		}
		
		CBL(CBL&& other)		//move constructor
		{
			data = other.data;		//this shoudld call the copy constructor.
			head = other.head;
			tail = other.tail;
			size = other.size;
			numSpotsUsed = other.numSpotsUsed;
			other.data = nullptr;
			other.head = nullptr;
			other.tail = nullptr;
			other.size = nullptr;
			other.numSpotsUsed = nullptr;
		}
		
		CBL& operator=(CBL&& other)		//move assignment operator
		{
			if (this == &other) { return this; }
			data = other.data;		//this shoudld call the copy constructor.
			head = other.head;
			tail = other.tail;
			size = other.size;
			numSpotsUsed = other.numSpotsUsed;
			other.data = nullptr;
			other.head = nullptr;
			other.tail = nullptr;
			other.size = nullptr;
			other.numSpotsUsed = nullptr;
		}
		
		//for iterators
		using iterator = CBL_Iter<T>;
		using const_iterator = CBL_Iter<T const>;
		
		iterator begin() { make_new_data(); return CBL_Iter<T>(&data[head-size+1]); } 
		iterator end() { return CBL_Iter<T>(&data[tail]); }
		const_iterator begin() const { return CBL_Iter<T const>(&data[head+1]); }
		const_iterator end() const { return CBL_Iter<T const>(&data[tail]); }
};

template <typename T>	
CBL<T>::CBL()
{
	data = new T [50];
	size = 50;
	head = 0;		//first empty index
	tail = 1;		//last empty index
	numSpotsUsed = 0;
}

template <typename T>	
CBL<T>::CBL(int len)
{
	try
	{
		if(len < 2)
		{
			throw(99);
		}
	}
	catch(int r)
	{
		std::cout<< "Your CBL must have a longer length.";
	}
	data = new T [len];
	size = len;
	head = 0;
	tail = 1;
	numSpotsUsed = 0;
}

template <typename T>
void CBL<T>::make_new_data()
{
	//should only be called if data is full.
	int s = (size * 1.5);
	T* array = new T [s];
	
	int index = 0;
	for(int counter = head+1; index <= numSpotsUsed; counter++, index++)
	{
		array[index] = data[counter%size];
	}
	size = s;
	head = size-1;
	tail = numSpotsUsed;
	delete [] data;
	data = array;
}

template <typename T>
void CBL<T>::deallocate()
{
	if(size > 99 && tail <= (size/2))		//This is the check to continue onto the rest of the method.
	{
		int s = size * .75;
		T* temp = new T [s];
		for(int counter = 0; counter <= tail; counter++)
		{
			temp[counter] = data[counter];
		}
		delete [] data;
		data = temp;
	}
}

template <typename T>
void CBL<T>::numSpots()
{
	if(head > tail)
	{
		numSpotsUsed = size - head + tail - 1;
	}
	if(tail > head)
	{
		numSpotsUsed = tail - head - 1;
	}
}

template <typename T>
void CBL<T>::insert(T x, int position)		//1st element has position 1.
{
	try
	{
		if(position < 0)
		{
			throw position;
		}
		if(position == 0)
		{
			push_front(x);
			return;
		}
		if(position >= length())
		{
			push_back(x);
			return;
		}
		if(numSpotsUsed >= size-1)	//You need one spot to be empty. In that spot when H=T, array is full.
		{
			make_new_data();
		}
		//Find that position
		int index = (head + position+1) % size;
		//Move everything down, starting with tail
		for(int counter = tail; counter != index; counter--)
		{
			if(counter == 0)
			{
				data[0] = data[size-1];
				counter = size;
			}
			data[counter] = data[counter-1];
		}
		
		//insert the new element
		data[index] = x;
		
		//adjust indexes
		tail++;
		if(tail == size)
			tail = 0;
		numSpotsUsed++;
	}
	catch(int t)
	{
		std::cout<< position << " is not a valid index position." << std::endl;
	}
}

template <typename T>
void CBL<T>::push_back(T x)
{
	if(head == tail) //make a separate array for transferring everything to a bigger array
	{
		make_new_data();
	}
	
	numSpotsUsed++;
	data[tail] = x;
	tail = (tail+1) % size;
}

template <typename T>
void CBL<T>::push_front(T x)
{
	if(head == tail) //make a separate array for transferring everything to a bigger array
	{ //size == numSpotsUsed-1
		make_new_data();
	}
	numSpotsUsed++;
	data[head] = x;
	head--;
	if(head == -1)
	{
		head = size-1;
	}
}

template <typename T>
T CBL<T>::replace(T x, int position)
{
	T hold = data[(position + head)%size];
	data[(position + head)%size] = x;
	return hold;
}

template <typename T>
T CBL<T>::remove(int position)
{
	T hold = data[(head + position+1) % size];	//find the spot that you want to remove.
	//move all elements down until it fills the empty spot, position to tail
	for(int counter = (head + position+1) % size; counter != tail; counter++)
	{
		if(counter == size)
		{
			counter = 0;
		}
		data[counter] = data[(counter+1) % size];
	}
	tail--;
	numSpotsUsed--;
	deallocate();
	return hold;
}

template <typename T>
T CBL<T>::pop_back()
{
	tail--;
	numSpotsUsed--;
	return data[tail];
}

template <typename T>
T CBL<T>::pop_front()
{
	head++;
	numSpotsUsed--;
	return data[head];
}

template <typename T>
T& CBL<T>::item_at(int position)		//1st element has position 1.
{
	try
	{
		if(position > numSpotsUsed || position < 1)
		{
			throw position;
		}
		return data[(head + position) % size];
	}
	catch(int x)
	{
		std::cout<< position << " is an invalid position." << std::endl;
	}
}

template <typename T>
T& CBL<T>::peek_back()
{
	if(tail == 0)
		return data[size-1];
	return data[tail-1];
}

template <typename T>
T& CBL<T>::peek_front()
{
	return data[(head+1) % size];
}

template <typename T>
bool CBL<T>::is_empty()
{
	if(numSpotsUsed == 0)
		return true;
	else
		return false;
}

template <typename T>
bool CBL<T>::is_full()
{
	return false;
}

template <class T>
int CBL<T>::length()
{
	return numSpotsUsed;
}

template <class T>
void CBL<T>::clear()
{
	head = 0;
	tail = 1;
	numSpotsUsed = 0;
	deallocate();
}

template <class T>
bool CBL<T>::contains(T x, bool (*equals_function)(const T& x, const T& y))
{
	for(int counter = head+1; counter != tail; counter++)
	{
		if(counter == size)
		{
			counter = 0;
		}
		if(equals_function(data[counter], x))
		{
			return true;
		}
	}
	return false;
}

template <class T>
T* CBL<T>::contents()
{
	T* array = new T[numSpotsUsed];
	int index = 0;
	for(int counter = head+1; counter != tail; counter++)
	{
		if(counter == size)			//counter corresponds to data, index to the new array.
		{
			counter = 0;
		}
		array[index] = data[counter];
		index++;
	}
	return array;
}

template <class T>
void CBL<T>::print(std::ostream &out)
{
	int index = 0;
	for(int counter = head+1; counter != tail && index < numSpotsUsed; counter++)
	{
		if(counter>=size)
		{
			out<< data[0] << ", ";
			counter = 0;
		}
		else
		{
			out<< data[counter] << ", ";
		}
			index++;
	}
	std::cout << std::endl;
}
}
