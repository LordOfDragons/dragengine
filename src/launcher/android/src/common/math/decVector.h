/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DECVECTOR_H_
#define _DECVECTOR_H_


/**
 * @brief 3 Component Vector.
 */
class decVector{
public:
	/** X Component of vector. */
	float x;
	/** Y Component of vector. */
	float y;
	/** Z Component of vector. */
	float z;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new vector initialized to ( 0,0,0 ). */
	decVector();
	/** Creates a new vector with the given values. */
	decVector( float nx, float ny, float nz );
	/** Creates a copy of a vector. */
	decVector( const decVector &v );
	/** Creates a copy of a double vector. */
	decVector( const decDVector &v );
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the length of the vector. */
	float Length() const;
	/** Retrieves the squared length of the vector. */
	float LengthSquared() const;
	/** Normalizes the vector. If the length of the vector is 0 an exception is thrown. */
	void Normalize();
	/** Retrieves normalized vector. If the length of the vector is 0 an exception is thrown. */
	const decVector Normalized() const;
	/** \brief Retrieves absolute vector. */
	const decVector Absolute() const;
	/** Snaps the components of the vector to a grid with the given size. */
	void Snap( float grid );
	/** Determines if the components of this vector are equal to another one with respect to a threshold. */
	bool IsEqualTo( const decVector &v, float threshold = VECTOR_THRESHOLD ) const;
	
	/** \brief All components are equal to zero with respect to a threshold. */
	bool IsZero( float threshold = VECTOR_THRESHOLD ) const;
	
	/** Sets all components to 0. */
	void SetZero();
	/** Sets the components to the given values. */
	void Set( float nx, float ny, float nz );
	/** Sets the components to the given vector. */
	void Set( const decVector &v );
	/** Negates the vector. */
	void Negate();
	/** \brief Set each component to the smallest value of this vector and another vector. */
	void SetSmallest( const decVector &v );
	/** \brief Retrieve vector with each component set to the smallest value of this vector and another vector. */
	decVector Smallest( const decVector &v ) const;
	/** \brief Set each component to the largest value of this vector and another vector. */
	void SetLargest( const decVector &v );
	/** \brief Retrieve vector with each component set to the largest value of this vector and another vector. */
	decVector Largest( const decVector &v ) const;
	/** \brief Clamp each component to the provided range. */
	void SetClamped( const decVector &minValue, const decVector &maxValue );
	/** \brief Retrieves vector with each component clamped to the provided range. */
	decVector Clamped( const decVector &minValue, const decVector &maxValue ) const;
	/*@}*/
	
	/** @name Operators */
	/*@{*/
	/** Retrieves the negation of this vector. */
	decVector operator-() const;
	/** Sets the components of this vector to the values of another one. */
	decVector &operator=( const decVector &v );
	/** Adds the components of another vector to this one. */
	decVector &operator+=( const decVector &v );
	/** Subtracts the components of another vector from this vector. */
	decVector &operator-=( const decVector &v );
	/** Multiplies the components of this vector with a value k. */
	decVector &operator*=( float k );
	/** Divides the components of this vector by a value k. If the value k is 0 an exception is thrown. */
	decVector &operator/=( float k );
	/** Retrieves a new vector with the sum of this vector with another one. */
	decVector operator+( const decVector &v ) const;
	/** Retrieves a new vector with the difference of this vector to another one. */
	decVector operator-( const decVector &v ) const;
	/** Retrieves a new vector with this vector scaled by k. */
	decVector operator*( float k ) const;
	/** Calculates the dot product of this vector with another one. */
	float operator*( const decVector &v ) const;
	/** Retrieves a new vector with this vector divided by k. If k is 0 an exception is thrown. */
	decVector operator/( float k ) const;
	/**
	 * \brief Retrieves a new vector with the cross product of this vector with another one.
	 * \details The length of the result vector is len(a)*len(b)*cos(alpha). Hence the length
	 *          of the result vector is only 1 if both vertices are of length 1 and orthogonal
	 *          to each other.
	 */
	decVector operator%( const decVector &v ) const;
	/** Determines if all the components of this vector are less then the components of another one. */
	bool operator<( const decVector &v ) const;
	/** Determines if all the components of this vector are greater then the components of another one. */
	bool operator>( const decVector &v ) const;
	/** Determines if all the components of this vector are less then or equal to the components of another one. */
	bool operator<=( const decVector &v ) const;
	/** Determines if all the components of this vector are greater then or equal to the components of another one. */
	bool operator>=( const decVector &v ) const;
	/*@}*/
};

#endif
