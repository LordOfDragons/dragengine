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



/**
 * @brief 2 Component Integer Point.
 */
class decPoint{
public:
	/** X Component of point. */
	int x;
	/** Y Component of point. */
	int y;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new point initialized to ( 0,0 ). */
	decPoint();
	/** Creates a new point with the given values. */
	decPoint( int nx, int ny );
	/** Creates a new point with the values of another point. */
	decPoint( const decPoint &p );
	/** Creates a new point from a vector. Fractional parts of all components are discarded. */
	decPoint( const decVector2 &v );
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the length of the point. */
	float Length() const;
	/** Sets all components to 0. */
	void SetZero();
	/** Sets the components to the given values. */
	void Set( int nx, int ny );
	/** \brief Retrieves absolute point. */
	const decPoint Absolute() const;
	/** \brief Set each component to the smallest value of this point and another point. */
	void SetSmallest( const decPoint &p );
	/** \brief Retrieve point with each component set to the smallest value of this point and another point. */
	decPoint Smallest( const decPoint &p ) const;
	/** \brief Set each component to the largest value of this point and another point. */
	void SetLargest( const decPoint &p );
	/** \brief Retrieve point with each component set to the largest value of this point and another point. */
	decPoint Largest( const decPoint &p ) const;
	/** \brief Clamp each component to the provided range. */
	void SetClamped( const decPoint &minValue, const decPoint &maxValue );
	/** \brief Retrieves point with each component clamped to the provided range. */
	decPoint Clamped( const decPoint &minValue, const decPoint &maxValue ) const;
	/*@}*/
	
	/** @name Operators */
	/*@{*/
	/** Retrieves the negation of this point. */
	decPoint operator-() const;
	/** Sets the components of this point to the values of another one. */
	decPoint &operator=( const decPoint &p );
	/** Adds the components of another point to this one. */
	decPoint &operator+=( const decPoint &p );
	/** Subtracts the components of another point from this point. */
	decPoint &operator-=( const decPoint &p );
	/** Multiplies the components of this point with a value k. */
	decPoint &operator*=( int k );
	/** Divides the components of this point by a value k. If the value k is 0 an exception is thrown. */
	decPoint &operator/=( int k );
	/** Retrieves a new point with the sum of this point with another one. */
	decPoint operator+( const decPoint &p ) const;
	/** Retrieves a new point with the difference of this point to another one. */
	decPoint operator-( const decPoint &p ) const;
	/** Retrieves a new point with this point scaled by k. */
	decPoint operator*( int k ) const;
	/** Calculates the dot product of this point with another one. */
	int operator*( const decPoint &p ) const;
	/** Retrieves a new point with this point divided by k. If k is 0 an exception is thrown. */
	decPoint operator/( int k ) const;
	/** Determines if this point is component wise equal to another point. */
	bool operator==( const decPoint &p ) const;
	/** Determines if this point is component wise not equal to another point. */
	bool operator!=( const decPoint &p ) const;
	/** Determines if all the components of this point are less then the components of another one. */
	bool operator<( const decPoint &p ) const;
	/** Determines if all the components of this point are greater then the components of another one. */
	bool operator>( const decPoint &p ) const;
	/** Determines if all the components of this point are less then or equal to the components of another one. */
	bool operator<=( const decPoint &p ) const;
	/** Determines if all the components of this point are greater then or equal to the components of another one. */
	bool operator>=( const decPoint &p ) const;
	/*@}*/
};
