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
 * @brief 4 Component Quaternion.
 */
class decQuaternion{
public:
	/** X Component of quaternion. */
	float x;
	/** Y Component of quaternion. */
	float y;
	/** Z Component of quaternion. */
	float z;
	/** W Component of quaternion. */
	float w;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new quaternion initialized to no rotation. */
	decQuaternion();
	/** Creates a new quaternion with the given values. */
	decQuaternion( float nx, float ny, float nz, float nw );
	/** Creates a new quaternion with the values of another quaternion. */
	decQuaternion( const decQuaternion &q );
	/*@}*/
	
	/** @name Quaternion Creation */
	/*@{*/
	/** \brief Creates a new quaternion from euler angles. */
	static decQuaternion CreateFromEuler( float rx, float ry, float rz );
	/** \brief Creates a new quaternion from euler angles. */
	static decQuaternion CreateFromEuler( const decVector &angles );
	/** \brief Creates a new quaternion from an euler angle around the x axis. */
	static decQuaternion CreateFromEulerX( float angle );
	/** \brief Creates a new quaternion from an euler angle around the y axis. */
	static decQuaternion CreateFromEulerY( float angle );
	/** \brief Creates a new quaternion from an euler angle around the z axis. */
	static decQuaternion CreateFromEulerZ( float angle );
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the length of the quaternion. */
	float Length() const;
	/**
	 * Normalizes the quaternion. If the length of the quaternion is
	 * 0 an exception is thrown.
	 */
	void Normalize();
	/**
	 * \brief Retrieves the normalized quaternion.
	 * \details If the length of the quaterion is 0 an exception is thrown.
	 */
	decQuaternion Normalized() const;
	/**
	 * \brief Retrieves the conjugation of this quaternion.
	 * \details This yields the inverse rotation of the quaterion.
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
	/** Calculates the slerp of this quaternion with another given an interpolation factor. */
	decQuaternion Slerp( const decQuaternion &other, float factor ) const;
	
	/** \brief Two quaternions are equal component wise with respect to a threshold. */
	bool IsEqualTo( const decQuaternion &q, float threshold = QUATERNION_THRESHOLD ) const;
	
	/**
	 * \brief Two quaternions represent the same rotation with respect to a threshold.
	 * \details Same as <code>a.IsEqualTo(b) || a.IsEqualTo(-b)</code>. In particular
	 *          quaternions of the same rotation are not required to be equal.
	 */
	bool SameRotation( const decQuaternion &q, float threshold = QUATERNION_THRESHOLD ) const;
	
	/** Sets all components to 0 and w to 1. */
	void SetZero();
	/** Sets the components to the given values. */
	void Set( float nx, float ny, float nz, float nw );
	/** Sets the quaternion from another one. */
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
	/*@}*/
	
	/** @name Operators */
	/*@{*/
	/** Retrieves the negation of this quaternion. */
	decQuaternion operator-() const;
	/** Sets the components of this quaternion to the values of another one. */
	decQuaternion &operator=( const decQuaternion &q );
	/** Adds the components of another quaternion to this one. */
	decQuaternion &operator+=( const decQuaternion &q );
	/** Subtracts the components of another quaternion from this quaternion. */
	decQuaternion &operator-=( const decQuaternion &q );
	/** Multiplies the components of this quaternion with a value k. */
	decQuaternion &operator*=( float k );
	/** Multiply this quaternion with another quaternion. */
	decQuaternion &operator*=( const decQuaternion &q );
	/** Divides the components of this quaternion by a value k. If the value k is 0 an exception is thrown. */
	decQuaternion &operator/=( float k );
	/** Retrieves a new quaternion with the sum of this quaternion with another one. */
	decQuaternion operator+( const decQuaternion &q ) const;
	/** Retrieves a new quaternion with the difference of this quaternion to another one. */
	decQuaternion operator-( const decQuaternion &q ) const;
	/** Retrieves a new quaternion with this quaternion scaled by k. */
	decQuaternion operator*( float k ) const;
	/** Retrieves a new quaternion with this quaternion multiplied by another quaternion. */
	decQuaternion operator*( const decQuaternion &q ) const;
	/** Retrieves a new quaternion with this quaternion divided by k. If k is 0 an exception is thrown. */
	decQuaternion operator/( float k ) const;
	/** Rotate a vector by this quaternion. */
	decVector operator*( const decVector &v ) const;
	/** Rotate a vector by this quaternion. */
	decDVector operator*( const decDVector &v ) const;
	/** Determines if all the components of this quaternion are less then the components of another one. */
	bool operator<( const decQuaternion &q ) const;
	/** Determines if all the components of this quaternion are greater then the components of another one. */
	bool operator>( const decQuaternion &q ) const;
	/** Determines if all the components of this quaternion are less then or equal to the components of another one. */
	bool operator<=( const decQuaternion &q ) const;
	/** Determines if all the components of this quaternion are greater then or equal to the components of another one. */
	bool operator>=( const decQuaternion &q ) const;
	/*@}*/
};
