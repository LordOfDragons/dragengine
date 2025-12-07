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
	decVector2(float nx, float ny);
	
	/** \brief Create copy of vector. */
	decVector2(const decVector2 &v);
	
	/** \brief Create vector from point. */
	decVector2(const decPoint &p);
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
	void Snap(float grid);
	
	/** \brief Component of vector are equal to another one with respect to a threshold. */
	bool IsEqualTo(const decVector2 &v, float threshold = VECTOR_THRESHOLD) const;
	
	/** \brief Set all components to 0. */
	void SetZero();
	
	/** \brief Set components to the given values. */
	void Set(float nx, float ny);
	
	/** \brief Set components to the given vector. */
	void Set(const decVector2 &v);
	
	/** \brief Negate vector. */
	void Negate();
	
	/** \brief Set each component to the smallest value of this vector and another vector. */
	void SetSmallest(const decVector2 &v);
	
	/** \brief Vector with each component set to the smallest value of this vector and another vector. */
	decVector2 Smallest(const decVector2 &v) const;
	
	/** \brief Set each component to the largest value of this vector and another vector. */
	void SetLargest(const decVector2 &v);
	
	/** \brief Vector with each component set to the largest value of this vector and another vector. */
	decVector2 Largest(const decVector2 &v) const;
	
	/** \brief Clamp each component to the provided range. */
	void SetClamped(const decVector2 &minValue, const decVector2 &maxValue);
	
	/** \brief Vector with each component clamped to the provided range. */
	decVector2 Clamped(const decVector2 &minValue, const decVector2 &maxValue) const;
	
	/** \brief Rotate vector counter-clock-wise by 90 degrees. */
	decVector2 RotateLeft() const;
	
	/** \brief Rotate vector clock-wise by 90 degrees. */
	decVector2 RotateRight() const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Negation of this vector. */
	decVector2 operator-() const;
	
	/** \brief Set components of this vector to the values of another one. */
	decVector2 &operator=(const decVector2 &v);
	
	/** \brief Add components of another vector to this one. */
	decVector2 &operator+=(const decVector2 &v);
	
	/** \brief Subtract components of another vector from this vector. */
	decVector2 &operator-=(const decVector2 &v);
	
	/** \brief Multiply components of this vector with a value k. */
	decVector2 &operator*=(float k);
	
	/**
	 * \brief Divide components of this vector by a value k.
	 * \throws deeDivisionByZero Length of vector is 0.
	 */
	decVector2 &operator/=(float k);
	
	/** \brief Vector with the sum of this vector with another one. */
	decVector2 operator+(const decVector2 &v) const;
	
	/** \brief Vector with the difference of this vector to another one. */
	decVector2 operator-(const decVector2 &v) const;
	
	/** \brief Vector with this vector scaled by k. */
	decVector2 operator*(float k) const;
	
	/** \brief Calculate dot product of this vector with another one. */
	float operator*(const decVector2 &v) const;
	
	/**
	 * \brief Vector with this vector divided by k.
	 * \throws deeDivisionByZero Length of vector is 0.
	 */
	decVector2 operator/(float k) const;
	
	/** \brief All the components of this vector are less then the components of another one. */
	bool operator<(const decVector2 &v) const;
	
	/** \brief All the components of this vector are greater then the components of another one. */
	bool operator>(const decVector2 &v) const;
	
	/** \brief All the components of this vector are less then or equal to the components of another one. */
	bool operator<=(const decVector2 &v) const;
	
	/** \brief All the components of this vector are greater then or equal to the components of another one. */
	bool operator>=(const decVector2 &v) const;
	/*@}*/
};
