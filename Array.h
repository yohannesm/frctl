// --------------------------
// projects/c++/array/Array.h
// Copyright (C) 2009
// Glenn P. Downing
// --------------------------

#ifndef Array_h
#define Array_h

// --------
// includes
// --------

#include <algorithm> // copy, equal, fill, lexicographical_compare
#include <cstddef>   // ptrdiff_t, size_t
#include <stdexcept> // out_of_range
#include <utility>   // !=, <=, >, >=
#include <iostream> // cout

struct A{
	A(){
		std::cout << "A() ";}

	A(const A&){
		std::cout << "A(A) ";}

	~A(){
		std::cout << "~A() ";}
	A& operator = (const A&){
		std::cout << "=(A) ";
		return *this;
	}
};
template<typename A, typename II, typename BI>
BI uninit_copy(A& a, II b, II e, BI x){
   BI p = x;
   try{
   	while(b!=e){
	   a.construct(&*x, *b);
	   ++b;
	   ++x;}
	}
   catch(...){
      destroyer(a, p, x);
      throw;
      }
   return x;
   }


template <typename A, typename BI, typename U>
BI uninit_fill(A& a, BI b, BI e, const U& v){
	BI p = b;
	try{
		while(b!=e){
		a.construct(&*b, v);
		++b; 
		}
	}
	catch(...){
	destroyer(a, p, b);
	throw;
	}
	return e;
}

template<typename A, typename BI>
BI destroyer (A& a, BI b, BI e) {
	while(b!=e){
	--e;
	a.destroy(&*e);
	}
	return b;
}

// -----
// using
// -----

using namespace std::rel_ops;
// ----------
// namespaces
// ----------

namespace My {

// -----
// Array
// -----


template <typename T, std::size_t N, typename A = std::allocator<T> >
class Array {
    public:
        // --------
        // typedefs
        // --------
	typedef A		  allocator_type;
        typedef T                 value_type;

        typedef std::size_t       size_type;
        typedef std::ptrdiff_t    difference_type;

        typedef value_type*       pointer;
        typedef const value_type* const_pointer;

        typedef value_type&       reference;
        typedef const value_type& const_reference;

        typedef pointer           iterator;
        typedef const_pointer     const_iterator;

    public:
        // -----------
        // operator ==
        // -----------

        /**
         * Checking whether two Array object is equal to each other
	 * using the std::equal 
	 * O(N)
         */
        friend bool operator == (const Array& lhs, const Array& rhs) {
            // you must use std::equal()
	    return std::equal(lhs.begin(), lhs.end(), rhs.begin() );
            }
        
        // ----------
        // operator <
        // ----------

        /**
         * Determine whether lhs is lexicographically less than the rhs object
	 * O(N)
         */
        friend bool operator < (const Array& lhs, const Array& rhs) {
            // <your code>
            // you must use std::lexicographical_compare()
	    return std::lexicographical_compare(
	    	lhs.begin(), lhs.end(), rhs.begin(), rhs.end()
		);
            }

        // ----------
        // operator <<
        // ----------
/*
	friend std::ostream operator << (std::ostream& os, const Array& rhs){
		for(unsigned int i=0; i<rhs.size(); ++i){
		   os << rhs[i] << " ";
			}
		os << std::endl;
		return os;
	}
*/
    private:
    	allocator_type malc;
	char ca [ (sizeof(T) * N) / sizeof(char) ];
        //value_type a[N];
	pointer a;

    public:
        // ------------
        // constructors
        // ------------
	
        /**
         * initialize the array of size N
	 * with N element
         */
        explicit Array (const_reference v = value_type(), 
			const allocator_type& mlc = allocator_type()) : malc(mlc) {
            //std::fill(a, a+N, v);
	    //uninit_fill(this->malc, a, a+N, v); 
	    a = reinterpret_cast<pointer>(ca);
	    uninit_fill(this->malc, a, a+N, v); 
            }

        /**
         * Constructing the array by using some sequences from an 
	 * Input Iterator
         */
        template <typename II>
        Array (II b, II e, const allocator_type& mlc = allocator_type()): malc(mlc) {
	    size_type i = 0;
	    a = reinterpret_cast<pointer>(ca);
            while(b!=e){
	        a[i] = *b;
		++b; ++i;
	    	}
            }
	/*
	 *Copy constructor
	 */
	Array(const Array& that){
	malc = that.malc;
	a = reinterpret_cast<pointer>(ca);
	uninit_copy(malc, that.begin(), that.end(), a);
	}
	
	/*
	 *Assignment operator
	 */
	Array& operator = (const Array& that){
	malc = that.malc;
	std::copy(that.begin(), that.end(), a);
	return *this;
	}
	/*
	 *Destructor
	 */
	~Array(){
	   destroyer(this->malc, a, a + N);
	}
        // -----------
        // operator []
        // -----------

        /**
         * getting an element inside the array container
         */
        reference operator [] (size_type i) {
		reference result = a[i];
		return result;
            }

        /**
         * getting a constant reference of an element inside the array container
         */
        const_reference operator [] (size_type i) const {
            // you must call the non-const operator[]()
	     const_reference result = const_cast<Array&>(*this).operator[](i);
	     return result;
            }

        // --
        // at
        // --

        /**
         * a method to get the element of the Array at the specified index
	 * this method is very similar to the subscript operator, but we
	 * add in a check to check the bound of the Array object
	 * @throw std::out of range if the boundary limitation is violated
	 * @return returning a reference to the specified element
         */
        reference at (size_type i) {
            	if(i >= N || i < 0){
		    throw std::out_of_range("My::Array.at(i) >= size");
	    		}
		reference result =  a[i];
		return result;
            }
            

        /**
         * a method to get the element of the Array at the specified index
	 * this method is very similar to the subscript operator, but we
	 * add in a check to check the bound of the Array object
	 * @throw std::out of range if the boundary limitation is violated
	 * @return returning a constant reference to the specified element
         */
        const_reference at (size_type i) const {
            	if(i >= N || i < 0){
		    throw std::out_of_range("My::Array.at(i) >= size");
	    		}
		const_reference result =  const_cast<Array&>(*this).at(i);
		return result;
            // you must call the non-const at()
            }

        // -----
        // begin
        // -----

        /**
         * returning an iterator to the beginning of the Array
         */
        iterator begin () {
            iterator i = a;
	    return i;
            }

        /**
         * returning a constant  iterator to the beginning of the Array
         */
        const_iterator begin () const {
            // you must call the non-const begin()
	    const_iterator i = const_cast<Array&>(*this).begin();
	    return i;
            }

        // ---
        // end
        // ---

        /**
         * returning an iterator to the end of the Array
         */
        iterator end () {
            iterator i = a + N;
	    return i;
            }

        /**
         * returning a constant iterator to the end of the Array
         */
        const_iterator end () const {
            // you must call the non-const end()
	    const_iterator i = const_cast<Array&>(*this).end();
	    return i;
            }

        // ----
        // size
        // ----

        /**
         * return the size of the stack allocated array
         */
        size_type size () const {
            return N;
            }
	
	// ----
	// print
	// ----
	void print () const{
	for(std::size_t i=0; i<N; ++i){
		std::cout << a[i] << " ";
		}
		std::cout << std::endl;
	}
};

}//My
#endif // Array_h
