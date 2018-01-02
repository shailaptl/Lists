#include <iostream>
#include "List.h"

namespace cop3530
{
template <typename T>				//difference between class and typename??
class SDAL: public List<T> 		//simple singly-linked list
{
	template <typename DataT>
	class SDAL_Iter
	{
		public:
			// type aliases required for C++ iterator compatibility
		  	using value_type = DataT;
			using reference = DataT&;
			using pointer = DataT*;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
		
			// type aliases for prettier code
			using self_type = SDAL_Iter;
			using self_reference = SDAL_Iter&;
		      
		private:
			DataT* here;
		      
		public:
		  	explicit SDAL_Iter( value_type *start) : here(start) {}
		 	SDAL_Iter( const SDAL_Iter& src ) : here( src.here ) {}
		      
			reference operator*() const //return a memory location
			{
				return *here;
			}
			
			pointer operator->() const
			{
				return &here;
			}
		      
			self_reference operator=( SDAL_Iter<DataT> const& src )
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
		      
			bool operator==( SDAL_Iter<DataT> const& rhs ) const
			{
				return (this->here == rhs.here);

			}
			
			bool operator!=( SDAL_Iter<DataT> const& rhs) const
			{
				return (this->here != rhs.here);
			}
	};	 // end SDAL_Iter
	
	public:
		T* data;
		int tail;
		int size;
	public:
		SDAL();
		SDAL(int l);
		void make_new_data();
		void deallocate();
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
		SDAL<T>(const SDAL& old)		//copy constructor, new pointer from previous
		{
			T* temp = new T [size];		//makes a completely different array for the new object.
			std::cout << tail << std::endl;
			this->tail = old.tail;
			for(int i = 0; i <= tail; i++)
			{
				temp[i] = old.data[i];
			}
			this->data = temp;
			this->size = old.size;
		}
		~SDAL()
		{
			delete [] data;
		}
		SDAL<T> operator=(SDAL& old)		//assignment operator
		{
			T* temp = new T [size];		//makes a completely different array for the new object.
			for(int i = 0; i <= tail; i++)
			{
				temp[i] = old.data[i];
			}
			this->data = temp;
			this->tail = old.tail;
			this->size = old.size;
			return *this;
		}
		
		SDAL(SDAL&& other)		//move constructor
		{
			data = other.data;		//this shoudld call the copy constructor.
			tail = other.tail;
			size = other.size;
			other.tail = nullptr;
			other.size = nullptr;
		}
		
		SDAL& operator=(SDAL&& other)		//move assignment operator
		{
			//if (this != &other) { return this; }
			data = other.data;		//this shoudld call the copy constructor.
			tail = other.tail;
			size = other.size;
			other.tail = nullptr;
			other.size = nullptr;
			return *this;
		}
		
		//for iterators
		using iterator = SDAL_Iter<T>;
		using const_iterator = SDAL_Iter<T const>;
		
		iterator begin() { return SDAL_Iter<T>(&data[0]); }    
		iterator end() { return SDAL_Iter<T>(&data[tail+1]); }   
		const_iterator begin() const { return SDAL_Iter<T const>(&data[0]); }  
		const_iterator end() const { return SDAL_Iter<T const>(&data[tail+1]); }   
};

template <typename T>	
SDAL<T>::SDAL()
{
	data = new T [50];
	size = 50;
	tail = -1;
}

template <typename T>	
SDAL<T>::SDAL(int len)
{
	data = new T [len];
	size = len;
	tail = -1;
}

template <typename T>
void SDAL<T>::make_new_data()
{
	//should only be called if data is full.
	if(tail < size-1)		//if size is 50, tail should be 49 and its time to make another one.
	{
		return;
	}
	
	int s = (size * 1.5);
	T* array = new T [s];
	
	for(int counter = 0; counter < size; counter++)
	{
		array[counter] = data[counter];
	}
	delete [] data;		//This should remove the array from memory.
	data = array;
}

template <typename T>
void SDAL<T>::deallocate()
{
	if(size > 99 && tail <= (size/2))	//This is the check to continue onto the rest of the method.
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
void SDAL<T>::insert(T x, int position)		//1st element has position 1.
{
	try
	{
		if(position == 0)
		{
			this->push_front(x);
			return;
		}
		if(position < 0)
			throw(1);
	
		if(size == (tail+1))		 //make a separate array for transferring everything to a bigger array
		{
			make_new_data();
		}
		
		for(int counter = tail; counter >= position; counter--)
		{
			//should traverse backwards through the array
			data[counter+1] = data[counter];
		}
		//when counter equals position
		data[position] = x;
		tail++;
	}
	catch(int t)
	{
		std::cout<< position << " is not a valid index position." << std::endl;
	}
}

template <typename T>
void SDAL<T>::push_back(T x)
{
	if(size == (tail+1)) //make a separate array for transferring everything to a bigger array
	{
		make_new_data();
	}
	
	tail++;
	data[tail] = x;
}

template <typename T>
void SDAL<T>::push_front(T x)
{
	if(size == (tail+1)) //make a separate array for transferring everything to a bigger array
	{
		make_new_data();
	}
	
	int counter;
	for(counter = tail+1; counter > 0; counter--)
	{
		data[counter] = data[counter-1];
	}
	data[0] = x;
	tail++;
}

template <typename T>
T SDAL<T>::replace(T x, int position)
{
	T hold = data[position];
	data[position] = x;
	return hold;
}

template <typename T>
T SDAL<T>::remove(int position)
{
	int counter;
	T hold = data[position];
	for(counter = position; counter < tail; counter++)
	{
		data[counter] = data[counter+1];
	}
	tail--;
	deallocate();
	return hold;
}

template <typename T>
T SDAL<T>::pop_back()
{
	tail--;
	return data[tail+1];
}

template <typename T>
T SDAL<T>::pop_front()
{
	T hold = data[0];
	int counter;
	for(counter = 0; counter >tail; counter++)
	{
		data[counter] = data[counter+1];
	}
	tail--;
	deallocate();
	return hold;
}

template <typename T>
T& SDAL<T>::item_at(int position)		//1st element has position 1.
{
	try
	{
		if(position > tail || position < 0)
		{
			throw position;
		}
		return data[position];			//difference between putting it here and at the end
	}
	catch(int x)
	{
		std::cout << x << " is an invalid position." << std::endl;
	}
}

template <typename T>
T& SDAL<T>::peek_back()
{
	return data[tail];
}

template <typename T>
T& SDAL<T>::peek_front()
{
	return data[0];
}

template <typename T>
bool SDAL<T>::is_empty()
{
	if(tail == -1)
	{
		return true;
	}
	else{
		return false;
	}
}

template <typename T>
bool SDAL<T>::is_full()
{
	return false;
}

template <class T>
int SDAL<T>::length()
{
	return tail+1;
}

template <class T>
void SDAL<T>::clear()
{
	tail = -1;
	deallocate();
}

template <class T>
bool SDAL<T>::contains(T x, bool (*equals_function)(const T& y, const T& z))
{
	for(int counter = 0; counter <= tail; counter++)
	{
		if(equals_function(data[counter], x))
			return true;
	}
	return false;
}

template <class T>
T* SDAL<T>::contents()
{
	T* array = new T[tail+1];
	for(int counter = 0; counter <= tail; counter++)
	{
		array[counter] = data[counter];
	}
	return array;
}

template <class T>
void SDAL<T>::print(std::ostream &out)
{
	for(int counter = 0; counter <= tail; counter++)
	{
		out<< data[counter] << ", ";
	}
	out<<std::endl;
}
}
