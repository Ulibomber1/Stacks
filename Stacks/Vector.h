// Adopted from M.A. Weiss, DSACC++ textbook
// by KV, AY 2020/21, for CSE 2020

#ifndef VECTOR_H
#define VECTOR_H

#include <cstdlib> // for swap
#include <iostream>
#include <cassert>

template <typename T>
class Vector
{
public:
	/*Constructors*/
	// defines how to initialize vectors without any parameters passed
	explicit Vector(int initSize = 0)
		:theSize(initSize),
		theCapacity(initSize + SPARE_CAPACITY) // alternate way of initializing variables for constructors
	{data = new T[theCapacity];}

	// added by KV for lab2 ... good to have this one ...
	// defines how to intitialize vectors of certain size and/or with an initial value
	Vector(int initSize, int initVal)
		:theSize(initSize),
		theCapacity(initSize + SPARE_CAPACITY)
	{
		data = new T[theCapacity];
		for (int i = 0; i < theCapacity; i++)
			data[i] = initVal;
	}

	// defines how "right hand side" vector declarations are initialized 
	Vector(const Vector& rhs)
		:theSize(rhs.theSize),
		theCapacity(rhs.theCapacity),
		data(nullptr)
	{
		data = new T[theCapacity];
		for (int k = 0; k < theSize; ++k)
			data[k] = rhs.data[k];
	}

	// added by KV
	Vector(Vector&& rhs)
		:theSize( rhs.theSize ),
		theCapacity( rhs.theCapacity ),
		data( rhs.data )
	{
		rhs.data = nullptr;
		rhs.theSize = 0;
		rhs.theCapacity = 0;
	}

	// defines behavior of the '=' operator for vectors
	Vector& operator= (Vector&& rhs)
	{
		std::swap(theSize, rhs.theSize);
		std::swap(theCapacity, rhs.theCapacity);
		std::swap(data, rhs.data);

		return *this;
	}

	// defines behavior of the '=' operator for const vectors
	Vector& operator= (const Vector& rhs)
	{
		Vector copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	// defines the behavior of the '[]' operator with vectors
	T& operator[] (int index)
	{
		assert(index >= 0 && index < theSize); // check if index exceeds the size
		return data[index]; // return the data in element at index 
	}

	// defines the behavior of the '[]' operator w/ vectors when they are const
	const T& operator[] (int index) const
	{
		assert(index >= 0 && index < theSize);
		return data[index];
	}


	/*Destructor*/
	~Vector()
	{
		delete[] data;
	}


	/*Iterators(new concept)*/
	// as implemented here, the iterators are simply pointers to elements in the data array
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef T* reverse_iterator;
	typedef const T* const_reverse_iterator;

	iterator begin()
	{
		return &data[0];
	}

	const_iterator begin() const
	{
		return &data[0];
	}

	reverse_iterator rbegin()
	{
		return &data[size()-1];
	}

	const_reverse_iterator rbegin() const
	{
		return &data[size()];
	}

	iterator end()
	{
		return &data[size()];
	}

	const_iterator end() const
	{
		return &data[size()];
	}

	reverse_iterator rend()
	{
		return &data[0];
	}

	const_reverse_iterator rend() const
	{
		return &data[0];
	}


	/*Accessors*/
	// returns bool for whether the vector is empty or not
	bool empty() const
	{
		return size() == 0;
	}

	// returns the size of the vector
	int size() const
	{
		return theSize;
	}

	// returns the capacity of the vector
	int capacity() const
	{
		return theCapacity;
	}

	// returns the data in the last element
	const T& back() const
	{
		assert(theSize >= 1);
		return data[theSize - 1];
	}

	// returns the data in the first element
	const T& front() const
	{
		assert(theSize >= 1);
		return data[0];
	}


	/*Mutators*/
	// resizes the vector if called
	void resize(int newSize)
	{
		if (newSize > theCapacity)
			reserve(newSize * 2);
		theSize = newSize;
	}

	// reserves new memory for the vector and moves it there
	void reserve(int newCapacity)
	{
		if (newCapacity < theSize)
			return;
		T* newArray = new T[newCapacity];
		for (int k = 0; k < theSize; ++k)
			newArray[k] = std::move(data[k]);

		theCapacity = newCapacity;
		std::swap(data, newArray);
		delete[] newArray; // discard the helper var newArray
	}

	// adds new element data to the 'back' of the vector. Reserves memory if necessary
	void push_back(const T& x)
	{
		if (theSize == theCapacity)
			reserve(2 * theCapacity + 1);
		data[theSize++] = x;
	}

	// removes the element at the 'back' of the vector by reducing theSize by one
	void pop_back()
	{
		assert(theSize >= 1);
		--theSize;
	}

	// clears a Vector of all its elements;
	void clear()
	{
		theSize = 0;
	}

	// Reverses the vector elements
	void reverse()
	{
		assert(theSize >= 1);
		iterator itr = begin();
		iterator ritr = rbegin() - 1;

		while (itr != ritr && itr < ritr)
		{
			std::swap(*itr, *ritr);
			itr++, ritr--;
		}

		return;
	}

	// Replaces any element with a specified value with a new specified value
	void replace(T oldval, T newval)
	{
		for (iterator itr = begin(); itr != end(); itr++)
		{
			if (*itr == oldval)
				*itr = newval;
		}
		return;
	}

	// Inserts a value at an index or iterator (pointer)
	void insert(iterator itr, T value)
	{
		if (itr == end())
		{
			push_back(value);
			return;
		}
		assert(itr >= begin() && itr < end());
		push_back(back());
		iterator itr1 = end() - 2;
		iterator itr2 = end() - 1;
		while (itr1 >= itr)
		{
			*itr2 = *itr1;
			itr1--;
			itr2--;
		}
		*itr = value;
		return;
	}

	void insert(int index, T value)
	{
		if (index == theSize)
		{
			push_back(value);
			return;
		}
		assert(index >= 0 && index < theSize);

		push_back(back()); // double up on last value

		// starting at back, move each item one position
		// to right, making space for new value at index
		for (int i = theSize; i >= index; i--)
			data[i] = data[i - 1];
		data[index] = value; // place new value
		return;
	}

	// Erases a value at index or iterator (pointer)
	void erase(iterator itr)
	{
		assert(itr >= begin() && itr < end());

		if (itr == end() - 1)
		{
			pop_back();
			return;
		}

		iterator itr2 = itr;
		itr2++;
		while (itr2 != end())
		{
			*itr = *itr2;
			itr++;
			itr2++;
		}
		pop_back();
		return;
	}

	void erase(int index)
	{
		assert(index >= 0 && index < theSize);
		if (index == theSize - 1)
		{
			pop_back();
			return;
		}
		// copy items one position to left
		// to overwrite value at index;
		for (int i = index; i < theSize; i++)
			data[i] = data[i + 1];
		pop_back(); // get rid of last duplicate item
		return;
	}

private:
	static const int SPARE_CAPACITY = 2;
	int theSize;
	int theCapacity;
	T* data;
};
#endif