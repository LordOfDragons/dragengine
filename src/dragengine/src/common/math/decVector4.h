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
 * \brief 4 Component Vector.
 */
class DE_DLL_EXPORT decVector4{
public:
	/** \brief X Component of vector. */
	float x;
	
	/** \brief Y Component of vector. */
	float y;
	
	/** \brief Z Component of vector. */
	float z;
	
	/** \brief W Component of vector. */
	float w;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new vector initialized to ( 0,0,0,1 ). */
	decVector4();
	
	/** \brief Create new vector with the given values. */
	decVector4( float nx, float ny, float nz );
	
	/** \brief Create new vector with the given values. */
	decVector4( float nx, float ny, float nz, float nw );
	
	/** \brief Create new vector with the values of another vector. */
	decVector4( const decVector4 &v );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Length of the vector. */
	float Length();
	
	/**
	 * \brief Normalize vector.
	 * 
	 * If the length of the vector is 0 an exception is thrown.
	 */
	void Normalize();
	
	/** \brief Retrieves normalized vector. If the length of the vector is 0 an exception is thrown. */
	const decVector4 Normalized() const;
	
	/** \brief Retrieves absolute vector. */
	const decVector4 Absolute() const;
	
	/** \brief Sets all components to 0. */
	void SetZero();
	
	/** \brief Set components to the given values. */
	void Set( float nx, float ny, float nz, float nw );
	
	/** \brief Mix vectors component wise. */
	decVector4 Mix( const decVector4 &vector, float blendFactor ) const;
	
	/** \brief Multiply component wise. */
	decVector4 Multiply( const decVector4 &vector ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Negation of this vector. */
	decVector4 operator-();
	
	/** \brief Set components of this vector to the values of another one. */
	decVector4 &operator=( const decVector4 &v );
	
	/** \brief Adds the components of another vector to this one. */
	decVector4 &operator+=( const decVector4 &v );
	
	/** \brief Subtracts the components of another vector from this vector. */
	decVector4 &operator-=( const decVector4 &v );
	
	/** \brief Multiply components with a value k. */
	decVector4 &operator*=( float k );
	
	/** \brief Divice components by a value k. If the value k is 0 an exception is thrown. */
	decVector4 &operator/=( float k );
	
	/** \brief Vector with the sum of this vector with another one. */
	decVector4 operator+( const decVector4 &v ) const;
	
	/** \brief Vector with the difference of this vector to another one. */
	decVector4 operator-( const decVector4 &v ) const;
	
	/** \brief Vector with this vector scaled by k. */
	decVector4 operator*( float k ) const;
	
	/** \brief Calculates the dor product of this vector with another one. */
	float operator*( const decVector4 &v ) const;
	
	/** \brief Vector with this vector divided by k. If k is 0 an exception is thrown. */
	decVector4 operator/( float k ) const;
	
	/** \brief Vector with the cross producr of this vector with another one. */
	decVector4 operator%( const decVector4 &v ) const;
	
	/** \brief All components are less then the components of another one. */
	bool operator<( const decVector4 &v ) const;
	
	/** \brief All components are greater then the components of another one. */
	bool operator>( const decVector4 &v ) const;
	
	/** \brief All components are less then or equal to the components of another one. */
	bool operator<=( const decVector4 &v ) const;
	
	/** \brief All components are greater then or equal to the components of another one. */
	bool operator>=( const decVector4 &v ) const;
	/*@}*/
};
