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
 * \brief 3 Component Double precision Vector.
 * The difference to the other vector class is just that this class uses double instead of float.
 */
class decDVector{
public:
	/** \brief X Component of vector. */
	double x;
	
	/** \brief Y Component of vector. */
	double y;
	
	/** \brief Z Component of vector. */
	double z;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new vector initialized to ( 0,0,0 ). */
	decDVector();
	
	/** \brief Create new vector with the given values. */
	decDVector( double nx, double ny, double nz );
	
	/** \brief Creatses a new vector with the values of another vector. */
	decDVector( const decDVector &v );
	
	/** \brief Creatses a new vector with the values of another vector. */
	decDVector( const decVector &v );
	
	/** \brief Creatses a new vector with the values of another vector. */
	decDVector( const decPoint3 &p );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Length of the vector. */
	double Length() const;
	
	/** \brief Squared length of the vector. */
	double LengthSquared() const;
	
	/** \brief Normalizes the vector. If the length of the vector is 0 an exception is thrown. */
	void Normalize();
	
	/** \brief Retrieves normalized vector. If the length of the vector is 0 an exception is thrown. */
	const decDVector Normalized() const;
	
	/** \brief Retrieves absolute vector. */
	const decDVector Absolute() const;
	
	/** \brief Snaps the components of the vector to a grid with the given size. */
	void Snap( double grid );
	
	/** \brief Components of this vector are equal to another one with respect to a threshold. */
	bool IsEqualTo( const decDVector &v, double threshold = DVECTOR_THRESHOLD ) const;
	
	/** \brief All components are equal to zero with respect to a threshold. */
	bool IsZero( double threshold = DVECTOR_THRESHOLD ) const;
	
	/** \brief Sets all components to 0. */
	void SetZero();
	
	/** \brief Set components to the given values. */
	void Set( double nx, double ny, double nz );
	
	/** \brief Set components to the given vector. */
	void Set( const decDVector &v );
	
	/** \brief Set components to the given vector. */
	void Set( const decVector &v );
	
	/** \brief Negates the vector. */
	void Negate();
	
	/** \brief Set each component to the smallest value of this vector and another vector. */
	void SetSmallest( const decDVector &v );
	
	/** \brief Retrieve vector with each component set to the smallest value of this vector and another vector. */
	decDVector Smallest( const decDVector &v ) const;
	
	/** \brief Set each component to the largest value of this vector and another vector. */
	void SetLargest( const decDVector &v );
	
	/** \brief Retrieve vector with each component set to the largest value of this vector and another vector. */
	decDVector Largest( const decDVector &v ) const;
	
	/** \brief Clamp each component to the provided range. */
	void SetClamped( const decDVector &minValue, const decDVector &maxValue );
	
	/** \brief Retrieves vector with each component clamped to the provided range. */
	decDVector Clamped( const decDVector &minValue, const decDVector &maxValue ) const;
	
	/** \brief Concerts to a normal precision vector with possible precision loss. */
	decVector ToVector() const;
	
	/** \brief Convert to point rounding to the nearest integer value. */
	decPoint3 Round() const;
	
	/** \brief Mix vectors component wise. */
	decDVector Mix( const decDVector &vector, double blendFactor ) const;
	
	/** \brief Multiply component wise. */
	decDVector Multiply( const decDVector &vector ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Negation of this vector. */
	decDVector operator-() const;
	
	/** \brief Set components of this vector to the values of another one. */
	decDVector &operator=( const decDVector &v );
	
	/** \brief Adds the components of another vector to this one. */
	decDVector &operator+=( const decDVector &v );
	
	/** \brief Subtracts the components of another vector from this vector. */
	decDVector &operator-=( const decDVector &v );
	
	/** \brief Multiplies the components of this vector with a value k. */
	decDVector &operator*=( double k );
	
	/**
	 * \brief Divide components of this vector by a value k.
	 * 
	 * If the value k is 0 an exception is thrown.
	 */
	decDVector &operator/=( double k );
	
	/** \brief Vector with the sum of this vector with another one. */
	decDVector operator+( const decDVector &v ) const;
	
	/** \brief Vector with the difference of this vector to another one. */
	decDVector operator-( const decDVector &v ) const;
	
	/** \brief Vector with this vector scaled by k. */
	decDVector operator*( double k ) const;
	
	/** \brief Calculates the dor product of this vector with another one. */
	double operator*( const decDVector &v ) const;
	
	/**
	 * \brief Vector with this vector divided by k.
	 * 
	 * If k is 0 an exception is thrown.
	 */
	decDVector operator/( double k ) const;
	
	/**
	 * \brief Vector with the cross producr of this vector with another one.
	 * 
	 * The length of the result vector is len(a)*len(b)*cos(alpha). Hence the length
	 * of the result vector is only 1 if both vertices are of length 1 and orthogonal
	 * to each other.
	 */
	decDVector operator%( const decDVector &v ) const;
	
	/** \brief All components of this vector are less then the components of another one. */
	bool operator<( const decDVector &v ) const;
	
	/** \brief All components of this vector are greater then the components of another one. */
	bool operator>( const decDVector &v ) const;
	
	/** \brief All components of this vector are less then or equal to the components of another one. */
	bool operator<=( const decDVector &v ) const;
	
	/** \brief All components of this vector are greater then or equal to the components of another one. */
	bool operator>=( const decDVector &v ) const;
	/*@}*/
};
