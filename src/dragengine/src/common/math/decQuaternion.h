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
 * \brief 4 Component Quaternion.
 */
class DE_DLL_EXPORT decQuaternion{
public:
	/** \brief X Component of quaternion. */
	float x;
	
	/** \brief Y Component of quaternion. */
	float y;
	
	/** \brief Z Component of quaternion. */
	float z;
	
	/** \brief W Component of quaternion. */
	float w;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new quaternion initialized to no rotation. */
	decQuaternion();
	
	/** \brief Create new quaternion with the given values. */
	decQuaternion( float nx, float ny, float nz, float nw );
	
	/** \brief Create new quaternion with the values of another quaternion. */
	decQuaternion( const decQuaternion &q );
	/*@}*/
	
	
	
	/** \name Quaternion Creation */
	/*@{*/
	/** \brief Create new quaternion from euler angles. */
	static decQuaternion CreateFromEuler( float rx, float ry, float rz );
	
	/** \brief Create new quaternion from euler angles. */
	static decQuaternion CreateFromEuler( const decVector &angles );
	
	/** \brief Create new quaternion from an euler angle around the x axis. */
	static decQuaternion CreateFromEulerX( float angle );
	
	/** \brief Create new quaternion from an euler angle around the y axis. */
	static decQuaternion CreateFromEulerY( float angle );
	
	/** \brief Create new quaternion from an euler angle around the z axis. */
	static decQuaternion CreateFromEulerZ( float angle );
	
	/** \brief Create from axis and rotation angle around axis. */
	static decQuaternion CreateFromAxis( const decVector &axis, float angle );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Length of the quaternion. */
	float Length() const;
	
	/**
	 * \breief Normalize quaternion.
	 * 
	 * If the length of the quaternion is 0 an exception is thrown.
	 */
	void Normalize();
	
	/**
	 * \brief Normalized quaternion.
	 * 
	 * If the length of the quaterion is 0 an exception is thrown.
	 */
	decQuaternion Normalized() const;
	
	/**
	 * \brief Conjugation of this quaternion.
	 * 
	 * This yields the inverse rotation of the quaterion.
	 */
	decQuaternion Conjugate() const;
	
	/** \brief Retrieves absolute quaternion. */
	const decQuaternion Absolute() const;
	
	/** \brief Dot-product. */
	float Dot( const decQuaternion &q ) const;
	
	/**
	 * \brief Retrieves quaternion as euler angles.
	 * \warning The result is only correct if the quaternion is normalized.
	 */
	const decVector GetEulerAngles() const;
	
	/** \brief Calculates the slerp of this quaternion with another given an interpolation factor. */
	decQuaternion Slerp( const decQuaternion &other, float factor ) const;
	
	/** \brief Two quaternions are equal component wise with respect to a threshold. */
	bool IsEqualTo( const decQuaternion &q, float threshold = QUATERNION_THRESHOLD ) const;
	
	/**
	 * \brief Two quaternions represent the same rotation with respect to a threshold.
	 * 
	 * Same as \code{.cpp}a.IsEqualTo(b) || a.IsEqualTo(-b)\endcode. In particular
	 * quaternions of the same rotation are not required to be equal.
	 */
	bool SameRotation( const decQuaternion &q, float threshold = QUATERNION_THRESHOLD ) const;
	
	/** \brief Sets all components to 0 and w to 1. */
	void SetZero();
	
	/** \brief Set components to the given values. */
	void Set( float nx, float ny, float nz, float nw );
	
	/** \brief Set quaternion from another one. */
	void Set( const decQuaternion &q );
	
	/** \brief Set from euler angles. */
	void SetFromEuler( float rx, float ry, float rz );
	
	/** \brief Set from euler angles. */
	void SetFromEuler( const decVector &angles );
	
	/** \brief Set from an euler angle around the x axis. */
	void SetFromEulerX( float angle );
	
	/** \brief Set from an euler angle around the y axis. */
	void SetFromEulerY( float angle );
	
	/** \brief Set from an euler angle around the z axis. */
	void SetFromEulerZ( float angle );
	
	/** \brief Set from axis and rotation angle around axis. */
	void SetFromAxis( const decVector &axis, float angle );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Negation of this quaternion. */
	decQuaternion operator-() const;
	
	/** \brief Set components of this quaternion to the values of another one. */
	decQuaternion &operator=( const decQuaternion &q );
	
	/** \brief Adds the components of another quaternion to this one. */
	decQuaternion &operator+=( const decQuaternion &q );
	
	/** \brief Subtracts the components of another quaternion from this quaternion. */
	decQuaternion &operator-=( const decQuaternion &q );
	
	/** \brief Multiplies the components of this quaternion with a value k. */
	decQuaternion &operator*=( float k );
	
	/** \brief Multiply this quaternion with another quaternion. */
	decQuaternion &operator*=( const decQuaternion &q );
	
	/** \brief Divides components by a value k. If the value k is 0 an exception is thrown. */
	decQuaternion &operator/=( float k );
	
	/** \brief Quaternion with the sum of this quaternion with another one. */
	decQuaternion operator+( const decQuaternion &q ) const;
	
	/** \brief Quaternion with the difference of this quaternion to another one. */
	decQuaternion operator-( const decQuaternion &q ) const;
	
	/** \brief Quaternion scaled by k. */
	decQuaternion operator*( float k ) const;
	
	/** \brief Quaternion multiplied by another quaternion. */
	decQuaternion operator*( const decQuaternion &q ) const;
	
	/** \brief Quaternion divided by k. If k is 0 an exception is thrown. */
	decQuaternion operator/( float k ) const;
	
	/** \brief Rotate a vector by this quaternion. */
	decVector operator*( const decVector &v ) const;
	
	/** \brief Rotate a vector by this quaternion. */
	decDVector operator*( const decDVector &v ) const;
	
	/** \brief All components are less then the components of another one. */
	bool operator<( const decQuaternion &q ) const;
	
	/** \brief All components are greater then the components of another one. */
	bool operator>( const decQuaternion &q ) const;
	
	/** \brief All components are less then or equal to the components of another one. */
	bool operator<=( const decQuaternion &q ) const;
	
	/** \brief All components are greater then or equal to the components of another one. */
	bool operator>=( const decQuaternion &q ) const;
	/*@}*/
};
