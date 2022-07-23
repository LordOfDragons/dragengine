/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


/**
 * \brief 3 Component Integer Point.
 */
class DE_DLL_EXPORT decPoint3{
public:
	/** \brief X Component of point. */
	int x;
	
	/** \brief Y Component of point. */
	int y;
	
	/** \brief Z Component of point. */
	int z;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new point initialized to ( 0,0,0 ). */
	decPoint3();
	
	/** \brief Create new point with the given values. */
	decPoint3( int nx, int ny, int nz );
	
	/** \brief Create new point with the values of another point. */
	decPoint3( const decPoint3 &p );
	
	/** \brief Create new point with the values from a 2-component point augmented with a third value. */
	decPoint3( const decPoint &p, int nz );
	
	/** \brief Create new point with the values of a 3-component vector discarding fractional parts of all components. */
	decPoint3( const decVector &p );
	
	/** \brief Create new point with the values of a 3-component vector discarding fractional parts of all components. */
	decPoint3( const decDVector &p );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Length of the point. */
	float Length() const;
	
	/** \brief Sets all components to 0. */
	void SetZero();
	
	/** \brief Set components to the given values. */
	void Set( int nx, int ny, int nz );
	
	/** \brief Retrieves absolute point. */
	decPoint3 Absolute() const;
	
	/** \brief Set each component to the smallest value of this point and another point. */
	void SetSmallest( const decPoint3 &p );
	
	/** \brief Retrieve point with each component set to the smallest value of this point and another point. */
	decPoint3 Smallest( const decPoint3 &p ) const;
	
	/** \brief Set each component to the largest value of this point and another point. */
	void SetLargest( const decPoint3 &p );
	
	/** \brief Retrieve point with each component set to the largest value of this point and another point. */
	decPoint3 Largest( const decPoint3 &p ) const;
	
	/** \brief Clamp each component to the provided range. */
	void SetClamped( const decPoint3 &minValue, const decPoint3 &maxValue );
	
	/** \brief Retrieves point with each component clamped to the provided range. */
	decPoint3 Clamped( const decPoint3 &minValue, const decPoint3 &maxValue ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Negation of this point. */
	decPoint3 operator-() const;
	
	/** \brief Set components of this point to the values of another one. */
	decPoint3 &operator=( const decPoint3 &p );
	
	/** \brief Adds the components of another point to this one. */
	decPoint3 &operator+=( const decPoint3 &p );
	
	/** \brief Subtracts the components of another point from this point. */
	decPoint3 &operator-=( const decPoint3 &p );
	
	/** \brief Multiplies the components of this point with a value k. */
	decPoint3 &operator*=( int k );
	
	/** \brief Divides the components of this point by a value k. If the value k is 0 an exception is thrown. */
	decPoint3 &operator/=( int k );
	
	/** \brief Retrieves a new point with the sum of this point with another one. */
	decPoint3 operator+( const decPoint3 &p ) const;
	
	/** \brief Retrieves a new point with the difference of this point to another one. */
	decPoint3 operator-( const decPoint3 &p ) const;
	
	/** \brief Retrieves a new point with this point scaled by k. */
	decPoint3 operator*( int k ) const;
	
	/** \brief Calculates the dot product of this point with another one. */
	int operator*( const decPoint3 &p ) const;
	
	/** \brief Retrieves a new point with this point divided by k. If k is 0 an exception is thrown. */
	decPoint3 operator/( int k ) const;
	
	/** \brief Determines if this point is component wise equal to another point. */
	bool operator==( const decPoint3 &p ) const;
	
	/** \brief Determines if this point is component wise not equal to another point. */
	bool operator!=( const decPoint3 &p ) const;
	
	/** \brief Determines if all the components of this point are less then the components of another one. */
	bool operator<( const decPoint3 &p ) const;
	
	/** \brief Determines if all the components of this point are greater then the components of another one. */
	bool operator>( const decPoint3 &p ) const;
	
	/** \brief Determines if all the components of this point are less then or equal to the components of another one. */
	bool operator<=( const decPoint3 &p ) const;
	
	/** \brief Determines if all the components of this point are greater then or equal to the components of another one. */
	bool operator>=( const decPoint3 &p ) const;
	/*@}*/
};
