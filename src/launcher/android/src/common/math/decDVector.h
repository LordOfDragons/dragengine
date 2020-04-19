/* 
 * Drag[en]gine Android Launcher
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
 * @brief 3 Component Double precision Vector.
 * The difference to the other vector class is just that this class uses double instead of float.
 */
class decDVector{
public:
	/** X Component of vector. */
	double x;
	/** Y Component of vector. */
	double y;
	/** Z Component of vector. */
	double z;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new vector initialized to ( 0,0,0 ). */
	decDVector();
	/** Creates a new vector with the given values. */
	decDVector( double nx, double ny, double nz );
	/** Creatses a new vector with the values of another vector. */
	decDVector( const decDVector &v );
	/** Creatses a new vector with the values of another vector. */
	decDVector( const decVector &v );
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the length of the vector. */
	double Length() const;
	/** Retrieves the squared length of the vector. */
	double LengthSquared() const;
	/** Normalizes the vector. If the length of the vector is 0 an exception is thrown. */
	void Normalize();
	/** Retrieves normalized vector. If the length of the vector is 0 an exception is thrown. */
	const decDVector Normalized() const;
	/** \brief Retrieves absolute vector. */
	const decDVector Absolute() const;
	/** Snaps the components of the vector to a grid with the given size. */
	void Snap( double grid );
	/** Determines if the components of this vector are equal to another one with respect to a threshold. */
	bool IsEqualTo( const decDVector &v, double threshold = DVECTOR_THRESHOLD ) const;
	
	/** \brief All components are equal to zero with respect to a threshold. */
	bool IsZero( double threshold = DVECTOR_THRESHOLD ) const;
	
	/** Sets all components to 0. */
	void SetZero();
	/** Sets the components to the given values. */
	void Set( double nx, double ny, double nz );
	/** Sets the components to the given vector. */
	void Set( const decDVector &v );
	/** Sets the components to the given vector. */
	void Set( const decVector &v );
	/** Negates the vector. */
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
	/** Concerts to a normal precision vector with possible precision loss. */
	decVector ToVector() const;
	/*@}*/
	
	/** @name Operators */
	/*@{*/
	/** Retrieves the negation of this vector. */
	decDVector operator-() const;
	/** Sets the components of this vector to the values of another one. */
	decDVector &operator=( const decDVector &v );
	/** Adds the components of another vector to this one. */
	decDVector &operator+=( const decDVector &v );
	/** Subtracts the components of another vector from this vector. */
	decDVector &operator-=( const decDVector &v );
	/** Multiplies the components of this vector with a value k. */
	decDVector &operator*=( double k );
	/**
	 * Divides the components of this vector by a value k. If the value k
	 * is 0 an exception is thrown.
	 */
	decDVector &operator/=( double k );
	/** Retrieves a new vector with the sum of this vector with another one. */
	decDVector operator+( const decDVector &v ) const;
	/** Retrieves a new vector with the difference of this vector to another one. */
	decDVector operator-( const decDVector &v ) const;
	/** Retrieves a new vector with this vector scaled by k. */
	decDVector operator*( double k ) const;
	/** Calculates the dor product of this vector with another one. */
	double operator*( const decDVector &v ) const;
	/**
	 * Retrieves a new vector with this vector divided by k. If k is 0 an
	 * exception is thrown.
	 */
	decDVector operator/( double k ) const;
	/**
	 * \brief Retrieves a new vector with the cross producr of this vector with another one.
	 * \details The length of the result vector is len(a)*len(b)*cos(alpha). Hence the length
	 *          of the result vector is only 1 if both vertices are of length 1 and orthogonal
	 *          to each other.
	 */
	decDVector operator%( const decDVector &v ) const;
	/** Determines if all the components of this vector are less then the components of another one. */
	bool operator<( const decDVector &v ) const;
	/** Determines if all the components of this vector are greater then the components of another one. */
	bool operator>( const decDVector &v ) const;
	/** Determines if all the components of this vector are less then or equal to the components of another one. */
	bool operator<=( const decDVector &v ) const;
	/** Determines if all the components of this vector are greater then or equal to the components of another one. */
	bool operator>=( const decDVector &v ) const;
	/*@}*/
};
