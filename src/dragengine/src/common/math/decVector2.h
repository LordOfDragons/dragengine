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
 * \brief 2 component vector.
 */
class decVector2{
public:
	float x;
	float y;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create zero vector. */
	decVector2();
	
	/** \brief Create vector with initial values. */
	decVector2( float nx, float ny );
	
	/** \brief Create copy of vector. */
	decVector2( const decVector2 &v );
	
	/** \brief Create vector from point. */
	decVector2( const decPoint &p );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Length. */
	float Length() const;
	
	/**
	 * \brief Normalize vector.
	 * \throws deeDivisionByZero Length of vector is 0.
	 */
	void Normalize();
	
	/**
	 * \brief Normalized vector.
	 * \throws deeDivisionByZero Length of vector is 0.
	 */
	decVector2 Normalized() const;
	
	/** \brief Absolute vector. */
	const decVector2 Absolute() const;
	
	/** \brief Snaps the components of the vector to a grid with the given size. */
	void Snap( float grid );
	
	/** \brief Component of vector are equal to another one with respect to a threshold. */
	bool IsEqualTo( const decVector2 &v, float threshold = VECTOR_THRESHOLD ) const;
	
	/** \brief Set all components to 0. */
	void SetZero();
	
	/** \brief Set components to the given values. */
	void Set( float nx, float ny );
	
	/** \brief Set components to the given vector. */
	void Set( const decVector2 &v );
	
	/** \brief Negate vector. */
	void Negate();
	
	/** \brief Set each component to the smallest value of this vector and another vector. */
	void SetSmallest( const decVector2 &v );
	
	/** \brief Vector with each component set to the smallest value of this vector and another vector. */
	decVector2 Smallest( const decVector2 &v ) const;
	
	/** \brief Set each component to the largest value of this vector and another vector. */
	void SetLargest( const decVector2 &v );
	
	/** \brief Vector with each component set to the largest value of this vector and another vector. */
	decVector2 Largest( const decVector2 &v ) const;
	
	/** \brief Clamp each component to the provided range. */
	void SetClamped( const decVector2 &minValue, const decVector2 &maxValue );
	
	/** \brief Vector with each component clamped to the provided range. */
	decVector2 Clamped( const decVector2 &minValue, const decVector2 &maxValue ) const;
	
	/** \brief Rotate vector counter-clock-wise by 90 degrees. */
	decVector2 RotateLeft() const;
	
	/** \brief Rotate vector clock-wise by 90 degrees. */
	decVector2 RotateRight() const;
	
	/** \brief Convert to point rounding to the nearest integer value. */
	decPoint Round() const;
	
	/** \brief Mix vectors component wise. */
	decVector2 Mix( const decVector2 &vector, float blendFactor ) const;
	
	/** \brief Multiply component wise. */
	decVector2 Multiply( const decVector2 &vector ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Negation of this vector. */
	decVector2 operator-() const;
	
	/** \brief Set components of this vector to the values of another one. */
	decVector2 &operator=( const decVector2 &v );
	
	/** \brief Add components of another vector to this one. */
	decVector2 &operator+=( const decVector2 &v );
	
	/** \brief Subtract components of another vector from this vector. */
	decVector2 &operator-=( const decVector2 &v );
	
	/** \brief Multiply components of this vector with a value k. */
	decVector2 &operator*=( float k );
	
	/**
	 * \brief Divide components of this vector by a value k.
	 * \throws deeDivisionByZero Length of vector is 0.
	 */
	decVector2 &operator/=( float k );
	
	/** \brief Vector with the sum of this vector with another one. */
	decVector2 operator+( const decVector2 &v ) const;
	
	/** \brief Vector with the difference of this vector to another one. */
	decVector2 operator-( const decVector2 &v ) const;
	
	/** \brief Vector with this vector scaled by k. */
	decVector2 operator*( float k ) const;
	
	/** \brief Calculate dot product of this vector with another one. */
	float operator*( const decVector2 &v ) const;
	
	/**
	 * \brief Vector with this vector divided by k.
	 * \throws deeDivisionByZero Length of vector is 0.
	 */
	decVector2 operator/( float k ) const;
	
	/** \brief All the components of this vector are less then the components of another one. */
	bool operator<( const decVector2 &v ) const;
	
	/** \brief All the components of this vector are greater then the components of another one. */
	bool operator>( const decVector2 &v ) const;
	
	/** \brief All the components of this vector are less then or equal to the components of another one. */
	bool operator<=( const decVector2 &v ) const;
	
	/** \brief All the components of this vector are greater then or equal to the components of another one. */
	bool operator>=( const decVector2 &v ) const;
	/*@}*/
};
