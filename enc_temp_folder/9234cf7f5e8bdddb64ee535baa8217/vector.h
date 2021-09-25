/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
 *
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An interator through Vector
 * Author
 *    Stephen Costigan, Alexander Dohms, Jonathan Colwell, Corbin Layton
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include <new>      // std::bad_alloc
#include <memory>   // for std::allocator

#include <iostream>


namespace custom
{

/*****************************************
 * VECTOR
 * Just like the std :: vector <T> class
 ****************************************/
template <typename T>
class vector
{
public:
   
   // 
   // Construct
   //

   vector();
   vector(size_t numElements                );
   vector(size_t numElements, const T & t   );
   vector(const std::initializer_list<T>& l );
   vector(const vector &  rhs);
   vector(      vector && rhs);
   ~vector();

   //
   // Assign
   //

   void swap(vector& rhs)
   {
       // create a temp vector to store left values
       T* dataCopy = new T[numCapacity];
       dataCopy = data;
       
       // copy left variables
       size_t numCapacityCopy = numCapacity;
       size_t numElementsCopy = numElements;
       // replace left with right
       numCapacity = rhs.numCapacity;
       numElements = rhs.numElements;
       // replace right with copy of left
       rhs.numCapacity = numCapacityCopy;
       rhs.numElements = numElementsCopy;

       // reset left side
       data = NULL;
       data = new T[numCapacity];
       
       // set left to right
       data = rhs.data;
       // reset right side
       rhs.data = NULL;
       rhs.data = new T[rhs.numCapacity];

       // set right to copy of left
       rhs.data = dataCopy;
       // set copy to null
       dataCopy = NULL;
       if (rhs.numCapacity == 0)
           rhs.data = NULL;
       if (numCapacity == 0)
           data = NULL;
   }
   vector & operator = (const vector & rhs);
   vector& operator = (vector&& rhs);

   //
   // Iterator
   //

   class iterator;
   iterator       begin() { return iterator(data); }
   iterator       end() { return iterator(data + numElements); }

   //
   // Access
   //

         T& operator [] (size_t index);
   const T& operator [] (size_t index) const;
         T& front();
   const T& front() const;
         T& back();
   const T& back() const;

   //
   // Insert
   //

   void push_back(const T& t);
   void push_back(T&& t);
   void reserve(size_t newCapacity);
   void resize(size_t newElements);
   void resize(size_t newElements, const T& t);

   //
   // Remove
   //

   void clear()
   {
       numElements = 0;
   }
   void pop_back()
   {
       numElements--;
   }
   void shrink_to_fit();

   //
   // Status
   //

   size_t   size()          const { return numElements;}
   size_t   capacity()      const { return numCapacity;}
   bool   empty()          { return (begin() == end() ? true : false);}
   
   // adjust the size of the buffer
   
   // vector-specific interfaces
   
#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif
   
   T *  data;                 // user data, a dynamically-allocated array
   size_t  numCapacity;       // the capacity of the array
   size_t  numElements;       // the number of items currently used
};

/*****************************************
 * VECTOR :: DEFAULT constructors
 * Default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> :: vector()
{
    data = NULL;
   numCapacity = 0;
   numElements = 0;
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> :: vector(size_t num, const T & t) 
{
    numCapacity = num;
    numElements = 0;
    data = new T[numCapacity];
    for (int i = 0;i < numCapacity; i++)
    {
        data[i] = t;
        if (&data[i] != NULL)
            numElements++;
    }
}

/*****************************************
 * VECTOR :: INITIALIZATION LIST constructors
 * Create a vector with an initialization list.
 ****************************************/
template <typename T>
vector <T> :: vector(const std::initializer_list<T> & l) 
{   
   numCapacity = l.size();
   numElements = 0;
   data = new T[numCapacity];
   for (T const item : l)
   {
       data[numElements++] = item;
   }
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> :: vector(size_t num) 
{
    data = new T[num];
    T input = 0;
    for (int i = 0;i < num;i++)
    {
        data[i] = input;
    }
}

/*****************************************
 * VECTOR :: COPY CONSTRUCTOR
 * Allocate the space for numElements and
 * call the copy constructor on each element
 ****************************************/
template <typename T>
vector <T> :: vector (const vector & rhs) 
{
    data = NULL;
    numElements = 0;
    numCapacity = 0;
    
    *this = rhs;

}

/*****************************************
 * VECTOR :: MOVE CONSTRUCTOR
 * Steal the values from the RHS and set it to zero.
 ****************************************/
template <typename T>
vector <T> :: vector (vector && rhs)
{
    // this is the simpler code, but it brings the percentage down a bit.
    /*
    data = rhs.data;
    rhs.data = NULL;
    
    numElements = rhs.numElements;
    rhs.numElements = 0;
    
    numCapacity = rhs.numCapacity;
    rhs.numCapacity = 0;
    */
    
    if (rhs.numCapacity == 0)
    {
        data = NULL;
        rhs.numCapacity = 0;
        rhs.numElements = 0;
        rhs.data = NULL;
        return;
    }

    numCapacity = rhs.numCapacity;
    numElements = rhs.numElements;
    
    rhs.numCapacity = 0;
    rhs.numElements = 0;
    data = rhs.data;
    rhs.data = NULL;
     
}

/*****************************************
 * VECTOR :: DESTRUCTOR
 * Call the destructor for each element from 0..numElements
 * and then free the memory
 ****************************************/
template <typename T>
vector <T> :: ~vector()
{
    data = NULL;
    numCapacity = 0;
    if (numElements > 0) {
        numElements = 0; // Set to 0
        shrink_to_fit();     // remove all elements between numElements (0) and numCapacity
    }
    delete[] data;
}

/***************************************
 * VECTOR :: RESIZE
 * This method will adjust the size to newElements.
 * This will either grow or shrink newElements.
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: resize(size_t newElements)
{
    reserve(newElements);
    
    /*if (!newElements || newElements == 0)
    {
        data = NULL;
        numElements = 0;
        return;
    } */ 
    for (int i = numElements; i < newElements; i++) {
        data[i] = 0;
    }
    numElements = newElements;

}

template <typename T>
void vector <T> :: resize(size_t newElements, const T & t)
{
    reserve(newElements);

    /*if (!newElements || newElements == 0)
    {
        data = NULL;
        numElements = 0;
        return;
    }*/
    
    for (int i = numElements; i < newElements; i++) {
        data[i] = t;
    }
    /*for (; numElements < numCapacity; numElements++) {
        data[numElements] = t;
    }*/
    numElements = newElements;
}

/***************************************
 * VECTOR :: RESERVE
 * This method will grow the current buffer
 * to newCapacity.  It will also copy all
 * the data from the old buffer into the new
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: reserve(size_t newCapacity)
{

    if (newCapacity <= numCapacity)
        return;
    
    T * dataNew = new T[newCapacity];
    
    for (int i = 0; i <= numElements; i++) {
        dataNew[i] = data[i];
    }
    delete [] data;
    
    data = dataNew;
    numCapacity = newCapacity;
}

/***************************************
 * VECTOR :: SHRINK TO FIT
 * Get rid of any extra capacity
 *     INPUT  :
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: shrink_to_fit()
{
    if (numElements == 0) {
        numCapacity = 0;
        data = NULL;
        return;
    }

    numCapacity = numElements;
}

/*****************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: operator [] (size_t index)
{
    /*return *(data + index);*/
    return data[index];
}

/******************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: operator [] (size_t index) const
{
    //return *(data + index);
    return data[index];
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: front ()
{
    return data[0];
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: front () const
{
    return data[0];
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: back()
{
    return data[numElements - 1];
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: back() const
{
    return data[numElements - 1];
}

/***************************************
 * VECTOR :: PUSH BACK
 * This method will add the element 't' to the
 * end of the current buffer.  It will also grow
 * the buffer as needed to accomodate the new element
 *     INPUT  : 't' the new element to be added
 *     OUTPUT : *this
 **************************************/
template <typename T>
void vector <T> :: push_back (const T & t)
{
    if (this->numCapacity == 0)
        resize(1);

    if (this->numElements == this->numCapacity)
        resize(this->numCapacity * 2);

    data[numElements++] = t;
}

template <typename T>
void vector <T> ::push_back(T && t)
{
    if (this->numCapacity == 0)
        resize(1);

    if (this->numElements == this->numCapacity)
        resize(this->numCapacity * 2);

    data[numElements++] = t;
}

/***************************************
 * VECTOR :: ASSIGNMENT
 * This operator will copy the contents of the
 * rhs onto *this, growing the buffer as needed
 *     INPUT  : rhs the vector to copy from
 *     OUTPUT : *this
 **************************************/
template <typename T>
vector <T> & vector <T> :: operator = (const vector & rhs)
{
    if(numCapacity < rhs.numCapacity)
        numCapacity = rhs.numCapacity;
    numElements = rhs.numElements;
    data = NULL;
    data = new T[numCapacity];

    // set left to right
    for (int i = 0; i < numElements; i++)
    {
        data[i] = rhs.data[i];
    }
   return *this;
}
template <typename T>
vector <T>& vector <T> :: operator = (vector&& rhs)
{
    numCapacity = rhs.numCapacity;
    numElements = rhs.numElements;
    data = NULL;
    data = new T[numCapacity];

    // set left to right
    for (int i = 0; i < numElements; i++)
    {
        data[i] = rhs.data[i];
    }
    return *this;
}

/**************************************************
 * VECTOR ITERATOR
 * An iterator through vector.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
template <typename T>
class vector <T> :: iterator
{
public:
   // constructors, destructors, and assignment operator
   iterator()                       { this->p = 0; }
   iterator(T * p)                  { this->p = p; }
   iterator(const iterator & rhs)   { this->p = rhs.p; }
   iterator & operator = (const iterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }
   
   // equals, not equals operator
   bool operator != (const iterator& rhs) const { return (rhs.p != p ? true : false); }
   bool operator == (const iterator& rhs) const { return (rhs.p == p ? true : false); }
   
   // dereference operator
   T & operator * ()
   {
      return *p;
   }
   
   // prefix increment
   iterator & operator ++ ()
   {
       p++;
       return *this;
   }
   
   // postfix increment
   iterator operator ++ (int postfix)
   {
       iterator i = p;
       p++;
       return i;
   }
   
   // prefix decrement
   iterator & operator -- ()
   {
        if (p > 0)
            p--;
        return *this;
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
       iterator i = p;
       if (p > 0)
           p--;
       return i;
   }
   
#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif
   T * p;
};


} // namespace custom

