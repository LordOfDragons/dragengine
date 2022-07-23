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
 * \brief 2 Component Integer Point.
 */
class DE_DLL_EXPORT decPoint{
public:
	/** \brief X Component of point. */
	int x;
	
	/** \brief Y Component of point. */
	int y;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new point initialized to ( 0,0 ). */
	decPoint();
	
	/** \brief Create new point with the given values. */
	decPoint( int nx, int ny );
	
	/** \brief Create new point with the values of another point. */
	decPoint( const decPoint &p );
	
	/** \brief Create new point from a vector. Fractional parts of all components are discarded. */
	decPoint( const decVector2 &v );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Length of the point. */
	float Length() const;
	
	/** \brief Sets all components to 0. */
	void SetZero();
	
	/** \brief Set components to the given values. */
	void Set( int nx, int ny );
	
	/** \brief Retrieves absolute point. */
	decPoint Absolute() const;
	
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
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Negation of this point. */
	decPoint operator-() const;
	
	/** \brief Set components of this point to the values of another one. */
	decPoint &operator=( const decPoint &p );
	
	/** \brief Adds the components of another point to this one. */
	decPoint &operator+=( const decPoint &p );
	
	/** \brief Subtracts the components of another point from this point. */
	decPoint &operator-=( const decPoint &p );
	
	/** \brief Multiplies the components of this point with a value k. */
	decPoint &operator*=( int k );
	
	/** \brief Divides the components of this point by a value k. If the value k is 0 an exception is thrown. */
	decPoint &operator/=( int k );
	
	/** \brief Retrieves a new point with the sum of this point with another one. */
	decPoint operator+( const decPoint &p ) const;
	
	/** \brief Retrieves a new point with the difference of this point to another one. */
	decPoint operator-( const decPoint &p ) const;
	
	/** \brief Retrieves a new point with this point scaled by k. */
	decPoint operator*( int k ) const;
	
	/** \brief Calculates the dot product of this point with another one. */
	int operator*( const decPoint &p ) const;
	
	/** \brief Retrieves a new point with this point divided by k. If k is 0 an exception is thrown. */
	decPoint operator/( int k ) const;
	
	/** \brief Determines if this point is component wise equal to another point. */
	bool operator==( const decPoint &p ) const;
	
	/** \brief Determines if this point is component wise not equal to another point. */
	bool operator!=( const decPoint &p ) const;
	
	/** \brief Determines if all the components of this point are less then the components of another one. */
	bool operator<( const decPoint &p ) const;
	
	/** \brief Determines if all the components of this point are greater then the components of another one. */
	bool operator>( const decPoint &p ) const;
	
	/** \brief Determines if all the components of this point are less then or equal to the components of another one. */
	bool operator<=( const decPoint &p ) const;
	
	/** \brief Determines if all the components of this point are greater then or equal to the components of another one. */
	bool operator>=( const decPoint &p ) const;
	/*@}*/
};
