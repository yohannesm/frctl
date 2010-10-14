// ----------------------------
// projects/c++/matrix/Matrix.h
// Copyright (C) 2009
// Glenn P. Downing
// ----------------------------

#ifndef Matrix_h
#define Matrix_h

// --------
// includes
// --------

#include <algorithm> // copy, equal, fill, lexicographical_compare
#include <cassert>   // assert
#include <cstddef>   // ptrdiff_t, size_t
#include <stdexcept> // out_of_range
#include <utility>   // !=, <=, >, >=
#include "Array.h"
// -----
// using
// -----

using namespace std::rel_ops;

// ----------
// namespaces
// ----------

namespace My {

// ------
// Matrix
// ------

template <typename T, std::size_t R, std::size_t C, typename A = std::allocator<T> >
class Matrix {
    public:
        // --------
        // typedefs
        // --------
	typedef A		  allocator_type;
        typedef Array<T, C, A>        value_type;

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

        /**O(N) average case
         * == operator 
	 * to check whether two matrix object is equaivalent to each other
         */
        friend bool operator == (const Matrix& lhs, const Matrix& rhs) {
            size_type row = lhs.end() - lhs.begin();
	    	for(unsigned int i=0; i<row; ++i){
			if(lhs[i]!=rhs[i]) return false;
	    		}
	    return true;
	    }
        
        // ----------
        // operator <
        // ----------

        /**O(N)
         * to check whether the lhs Matrix object is less than the
	 * rhs matrix object
         */
        friend bool operator < (const Matrix& lhs, const Matrix& rhs) {
            size_type row = lhs.end() - lhs.begin();
	    	for(unsigned int i=0; i<row; ++i){
			if(lhs[i]>=rhs[i]) return false;
	    		}
	    return true;
	    }
        // ----------
        // operator +
        // ----------

        /**
         * adding all the element of the matrix  by a T value of the rhs 
	 * of the operator
         */
        friend Matrix operator + (Matrix lhs, const T& rhs) {
            return lhs += rhs;}

        // ----------
        // operator +
        // ----------

        /**
         * Adding a lhs matrix by another matrix on the rhs of the operator 
	 * returning a new matrix with the added value
         */
        friend Matrix operator + (Matrix lhs, const Matrix& rhs) {
            return lhs += rhs;}

        // ----------
        // operator -
        // ----------

        /**
         * subtracting  each element of the matrix  by a T value of the rhs 
	 * of the operator
         */
        friend Matrix operator - (Matrix lhs, const T& rhs) {
            return lhs -= rhs;}

        // ----------
        // operator -
        // ----------

        /**
         * Subtracting a lhs matrix by another matrix on the rhs of the operator
	 * returning a new matrix with the added value
         */
        friend Matrix operator - (Matrix lhs, const Matrix& rhs) {
            return lhs -= rhs;}

        // ----------
        // operator *
        // ----------

        /**
         * multiplying  each element of the matrix  by a T value of the rhs 
	 * of the operator
         */
        friend Matrix operator * (Matrix lhs, const T& rhs) {
            return lhs *= rhs;}

        // ----------
        // operator /
        // ----------

        /*
         *dividing each element of the matrix  by a T value of the rhs 
	 * of the operator
         * @throws invalid_argument if (rhs == 0)
         */
        friend Matrix operator / (Matrix lhs, const T& rhs) {
            return lhs /= rhs;}

        // ----------
        // operator %
        // ----------

        /**
         *modding  each element of the matrix  by a T value of the rhs 
	 * of the operator
         * @throws invalid_argument if (rhs <= 0)
         */
        friend Matrix operator % (Matrix lhs, const T& rhs) {
            return lhs %= rhs;}

//template <typename T, std::size_t R, std::size_t C, typename A = std::allocator<T> >
    private:
    	typedef typename allocator_type::template rebind<value_type>::other rebinded_alc_type;
	rebinded_alc_type mal;
	char co[sizeof (value_type) * R / sizeof(char)];
	pointer m;

    public:
        // ------------
        // constructors
        // ------------
	// takes in a default value of T
	// and then initialize all the elements inside that matrix with that value
	explicit Matrix(const T& v = T()){
	    m = reinterpret_cast<pointer>(co);
	    uninit_fill(this->mal, m, m + R, value_type(v));
	    }
        /**
	 * Constructor that takes in an Array as default value
         */
	#if 0
        explicit Matrix (const value_type& v = value_type()) : mal() {
	    m = reinterpret_cast<pointer>(co);
	    uninit_fill(this->mal, m, m + R, v);
	    }
	 #endif
	/*
	 *Copy Constructor
	 */
        Matrix (const Matrix& that){
	   this->mal = that.mal;
	   m = reinterpret_cast<pointer>(co);
	   uninit_copy(this->mal, that.begin(), that.end(), m);
	}
	/*
	 *Copy Assignment
	 */
	 Matrix& operator= (const Matrix& that){
	 mal = that.mal;
	 std::copy(that.begin(), that.end(), m);
	 return *this;
	 }
	
	/*
	 *Destructor
	 */
	 ~Matrix(){
	    destroyer(mal, m, m + R);
	 }

        // -----------
        // operator []
        // -----------

        /**
         * Subscript operation on matrix
	 * @return: returning the specified row of this matrix
         */
        reference operator [] (size_type i) {
	   return m[i];
	}
        /**
         * const Subscript operation on matrix
	 * @return: returning the specified row of this matrix
         */
        const_reference operator [] (size_type i) const {
            // you must call the non-const operator[]()
            return const_cast<Matrix&>(*this)[i];}

        // -----------
        // operator +=
        // -----------

        /**
         * += operation on this matrix
	 * adding each value of this matrix by a value of T on the rhs
         */
        Matrix& operator += (const T& rhs) {
            for(unsigned int i=0; i < R; i++){
	        for(unsigned int j=0; j < C; j++){
			m[i][j] += rhs;
			}
	    	}
            return *this;}

        // -----------
        // operator +=
        // -----------

        /**
         * += operation on this matrix
	 * adding each value of this matrix by a value of another matrix on the rhs
         */
        Matrix& operator += (const Matrix& rhs) {
            for(unsigned int i=0; i < R; i++){
	        for(unsigned int j=0; j < C; j++){
			m[i][j] += rhs[i][j];
			}
	    	}
            return *this;}
        // -----------
        // operator -=
        // -----------

        /**
         * -= operation on this matrix
	 * subtracting each value of this matrix by a value of T on the rhs
         */
        Matrix& operator -= (const T& rhs) {
            for(unsigned int i=0; i < R; i++){
	        for(unsigned int j=0; j < C; j++){
			m[i][j] -= rhs;
			}
	    	}
            return *this;}

        // -----------
        // operator -=
        // -----------

        /**
         * -= operation on this matrix
	 * subtracting each value of this matrix by a value of another matrix on the rhs
         */
        Matrix& operator -= (const Matrix& rhs) {
            for(unsigned int i=0; i < R; i++){
	        for(unsigned int j=0; j < C; j++){
			m[i][j] -= rhs[i][j];
			}
	    	}
            return *this;}

        // -----------
        // operator *=
        // -----------

        /**
         * *= operation on this matrix
	 * multiplying each value of this matrix by a value of T on the rhs
         */
        Matrix& operator *= (const T& rhs) {
            for(unsigned int i=0; i < R; i++){
	        for(unsigned int j=0; j < C; j++){
			m[i][j] *= rhs;
			}
	    	}
            return *this;}

        // -----------
        // operator /=
        // -----------

        /**
         * /= operation on this matrix
	 * dividing each value of this matrix by a value of T on the rhs
         * @throws invalid_argument if (rhs == 0)
         */
        Matrix& operator /= (const T& rhs) {
	  if(rhs == 0)
	  	throw std::invalid_argument("Matrix /= operator::invalid args");
            for(unsigned int i=0; i < R; i++){
	        for(unsigned int j=0; j < C; j++){
			m[i][j] /= rhs;
			}
	    	}
            return *this;}

        // -----------
        // operator %=
        // -----------

        /**
         * %= operation on this matrix
	 * modding each value of this matrix by a value of T on the rhs
         * @throws invalid_argument if (rhs <= 0)
         */
        Matrix& operator %= (const T& rhs) {
	  if(rhs == 0)
	  	throw std::invalid_argument("Matrix %= operator::invalid args");
            for(unsigned int i=0; i < R; i++){
	        for(unsigned int j=0; j < C; j++){
			m[i][j] %= rhs;
			}
	    	}
            return *this;}

        // -----
        // apply
        // -----

        /**
         * apply a function or function object to each element
         * UF must be a function or act liek a function and have
	 * operator () overloaded
	 * f can't be null or zero or the program will crash
         */
        template <typename UF>
        Matrix& apply (UF f) {
            for(unsigned int i=0; i < R; i++){
	        for(unsigned int j=0; j < C; j++){
			m[i][j] = f(m[i][j]);
			}
		}
            return *this;}

        // --
        // at
        // --

        /** modifiable .at() method
         * getting the content of the array inside a row of this matrix
	 * @return reference to the specified array element
	 * @throw std::out_of_range if size_type i is invalid
         */
        reference at (size_type i) {
	    if(i<0 || i>= R){
	    	throw std::out_of_range("My::Matrix.at(i) invalid args");
		}
            return m[i];}

        /**
        * const .at() method
         * getting the content of the array inside a row of this matrix
	 * @return reference to the specified array element
	 * @throw std::out_of_range if size_type i is invalid
         */
        const_reference at (size_type i) const {
            // you must call the non-const at()a
            return const_cast<Matrix&>(*this).at(i);}

        // -----
        // begin
        // -----

        /**
         * return an iterator to the beginning row of the matrix
         */
        iterator begin () {
            return m;}

        /**
         * return a const iterator to the beginning row of the matrix
         */
        const_iterator begin () const {
            // you must call the non-const begin()
            return const_cast<Matrix&>(*this).begin();}

        // ---
        // end
        // ---

        /**
         * return an iterator to the one past the end of the row of the matrix
         */
        iterator end () {
            return m + R;}

        /**
         * return a const iterator to the one past the end of the row of the matrix
         */
        const_iterator end () const {
            // you must call the non-const end()
            return const_cast<Matrix&>(*this).end();}

        // ----
        // size
        // ----

        /**
         * Return the row or column size of the matrix 
         */
        size_type size () const {
            return R * C;}
	    
	size_type rows() const{
	    return R;
	}

	size_type cols() const{
	    return C;
	}
	  };// end class Matrix
// -----
// apply
// -----

/**
 * apply a function or function object to each element
 * Matrix copy constructor have to be working correctly for this function to runs
 */
template <typename T, std::size_t R, std::size_t C, typename A, typename UF>
Matrix<T, R, C> apply (Matrix<T, R, C, A> x, UF f) {
    return x.apply(f);}

} // My

#endif // Matrix_h
