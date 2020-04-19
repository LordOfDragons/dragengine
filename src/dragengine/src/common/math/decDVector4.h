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
 * \brief 4 Component Vector.
 */
class decDVector4{
public:
	/** \brief X Component of vector. */
	double x;
	
	/** \brief Y Component of vector. */
	double y;
	
	/** \brief Z Component of vector. */
	double z;
	
	/** \brief W Component of vector. */
	double w;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new vector initialized to ( 0,0,0,1 ). */
	decDVector4();
	
	/** \brief Create new vector with the given values. */
	decDVector4( double nx, double ny, double nz );
	
	/** \brief Create new vector with the given values. */
	decDVector4( double nx, double ny, double nz, double nw );
	
	/** \brief Create new vector with the values of another vector. */
	decDVector4( const decDVector4 &v );
	
	/** \brief Create new vector with the values of another vector. */
	decDVector4( const decVector4 &v );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Length of the vector. */
	double Length();
	
	/** \brief Normalizes the vector. If the length of the vector is 0 an exception is thrown. */
	void Normalize();
	
	/** \brief Retrieves normalized vector. If the length of the vector is 0 an exception is thrown. */
	const decDVector4 Normalized() const;
	
	/** \brief Retrieves absolute vector. */
	const decDVector4 Absolute() const;
	
	/** \brief Sets all components to 0. */
	void SetZero();
	
	/** \brief Set components to the given values. */
	void Set( double nx, double ny, double nz, double nw );
	
	/** \brief Convert to a single precision vector with possible loss of precision. */
	decVector4 ToVector4() const;
	
	/** \brief Mix vectors component wise. */
	decDVector4 Mix( const decDVector4 &vector, double blendFactor ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Negation of this vector. */
	decDVector4 operator-();
	
	/** \brief Set components of this vector to the values of another one. */
	decDVector4 &operator=( const decDVector4 &v );
	
	/** \brief Adds the components of another vector to this one. */
	decDVector4 &operator+=( const decDVector4 &v );
	
	/** \brief Subtracts the components of another vector from this vector. */
	decDVector4 &operator-=( const decDVector4 &v );
	
	/** \brief Multiplies the components of this vector with a value k. */
	decDVector4 &operator*=( double k );
	
	/** \brief Divides the components of this vector by a value k. If the value k is 0 an exception is thrown. */
	decDVector4 &operator/=(double k);
	
	/** \brief Vector with the sum of this vector with another one. */
	decDVector4 operator+( const decDVector4 &v ) const;
	
	/** \brief Vector with the difference of this vector to another one. */
	decDVector4 operator-( const decDVector4 &v ) const;
	
	/** \brief Vector with this vector scaled by k. */
	decDVector4 operator*( double k ) const;
	
	/** \brief Calculates the dor product of this vector with another one. */
	double operator*( const decDVector4 &v ) const;
	
	/** \brief Vector with this vector divided by k. If k is 0 an exception is thrown. */
	decDVector4 operator/( double k ) const;
	
	/** \brief Vector with the cross producr of this vector with another one. */
	decDVector4 operator%( const decDVector4 &v ) const;
	
	/** \brief All components of this vector are less then the components of another one. */
	bool operator<( const decDVector4 &v ) const;
	
	/** \brief All components of this vector are greater then the components of another one. */
	bool operator>( const decDVector4 &v ) const;
	
	/** \brief All components of this vector are less then or equal to the components of another one. */
	bool operator<=( const decDVector4 &v ) const;
	
	/** \brief All components of this vector are greater then or equal to the components of another one. */
	bool operator>=( const decDVector4 &v ) const;
	/*@}*/
};
