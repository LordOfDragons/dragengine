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
 * @brief 4 Component Vector.
 */
class decVector4{
public:
	/** X Component of vector. */
	float x;
	/** Y Component of vector. */
	float y;
	/** Z Component of vector. */
	float z;
	/** W Component of vector. */
	float w;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new vector initialized to ( 0,0,0,1 ). */
	decVector4();
	/** Creates a new vector with the given values. */
	decVector4(float nx, float ny, float nz);
	/** Creates a new vector with the given values. */
	decVector4(float nx, float ny, float nz, float nw);
	/** Creates a new vector with the values of another vector. */
	decVector4(const decVector4 &v);
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the length of the vector. */
	float Length();
	/**
	 * Normalizes the vector. If the length of the vector is
	 * 0 an exception is thrown.
	 */
	void Normalize();
	/** Retrieves normalized vector. If the length of the vector is 0 an exception is thrown. */
	const decVector4 Normalized() const;
	/** \brief Retrieves absolute vector. */
	const decVector4 Absolute() const;
	/** Sets all components to 0. */
	void SetZero();
	/** Sets the components to the given values. */
	void Set(float nx, float ny, float nz, float nw);
	/*@}*/
	
	/** @name Operators */
	/*@{*/
	/** Retrieves the negation of this vector. */
	decVector4 operator-();
	/** Sets the components of this vector to the values of another one. */
	decVector4 &operator=(const decVector4 &v);
	/** Adds the components of another vector to this one. */
	decVector4 &operator+=(const decVector4 &v);
	/** Subtracts the components of another vector from this vector. */
	decVector4 &operator-=(const decVector4 &v);
	/** Multiplies the components of this vector with a value k. */
	decVector4 &operator*=(float k);
	/** Divides the components of this vector by a value k. If the value k is 0 an exception is thrown. */
	decVector4 &operator/=(float k);
	/** Retrieves a new vector with the sum of this vector with another one. */
	decVector4 operator+(const decVector4 &v) const;
	/** Retrieves a new vector with the difference of this vector to another one. */
	decVector4 operator-(const decVector4 &v) const;
	/** Retrieves a new vector with this vector scaled by k. */
	decVector4 operator*(float k) const;
	/** Calculates the dor product of this vector with another one. */
	float operator*(const decVector4 &v) const;
	/** Retrieves a new vector with this vector divided by k. If k is 0 an exception is thrown. */
	decVector4 operator/(float k) const;
	/** Retrieves a new vector with the cross producr of this vector with another one. */
	decVector4 operator%(const decVector4 &v) const;
	/** Determines if all the components of this vector are less then the components of another one. */
	bool operator<(const decVector4 &v) const;
	/** Determines if all the components of this vector are greater then the components of another one. */
	bool operator>(const decVector4 &v) const;
	/** Determines if all the components of this vector are less then or equal to the components of another one. */
	bool operator<=(const decVector4 &v) const;
	/** Determines if all the components of this vector are greater then or equal to the components of another one. */
	bool operator>=(const decVector4 &v) const;
	/*@}*/
};
