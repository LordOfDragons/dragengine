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
 * @brief 3 Component Integer Point.
 */
class decPoint3{
public:
	/** X Component of point. */
	int x;
	/** Y Component of point. */
	int y;
	/** Z Component of point. */
	int z;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new point initialized to ( 0,0,0 ). */
	decPoint3();
	/** Creates a new point with the given values. */
	decPoint3(int nx, int ny, int nz);
	/** Creates a new point with the values of another point. */
	decPoint3(const decPoint3 &p);
	/** Creates a new point with the values from a 2-component point augmented with a third value. */
	decPoint3(const decPoint3 &p, int nz);
	/** Creates a new point with the values of a 3-component vector discarding fractional parts of all components. */
	decPoint3(const decVector &p);
	/** Creates a new point with the values of a 3-component vector discarding fractional parts of all components. */
	decPoint3(const decDVector &p);
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the length of the point. */
	float Length() const;
	/** Sets all components to 0. */
	void SetZero();
	/** Sets the components to the given values. */
	void Set(int nx, int ny, int nz);
	/** \brief Retrieves absolute point. */
	const decPoint3 Absolute() const;
	/*@}*/
	
	/** @name Operators */
	/*@{*/
	/** Retrieves the negation of this point. */
	decPoint3 operator-() const;
	/** Sets the components of this point to the values of another one. */
	decPoint3 &operator=(const decPoint3 &p);
	/** Adds the components of another point to this one. */
	decPoint3 &operator+=(const decPoint3 &p);
	/** Subtracts the components of another point from this point. */
	decPoint3 &operator-=(const decPoint3 &p);
	/** Multiplies the components of this point with a value k. */
	decPoint3 &operator*=(int k);
	/** Divides the components of this point by a value k. If the value k is 0 an exception is thrown. */
	decPoint3 &operator/=(int k);
	/** Retrieves a new point with the sum of this point with another one. */
	decPoint3 operator+(const decPoint3 &p) const;
	/** Retrieves a new point with the difference of this point to another one. */
	decPoint3 operator-(const decPoint3 &p) const;
	/** Retrieves a new point with this point scaled by k. */
	decPoint3 operator*(int k) const;
	/** Calculates the dot product of this point with another one. */
	int operator*(const decPoint3 &p) const;
	/** Retrieves a new point with this point divided by k. If k is 0 an exception is thrown. */
	decPoint3 operator/(int k) const;
	/** Determines if this point is component wise equal to another point. */
	bool operator==(const decPoint3 &p) const;
	/** Determines if this point is component wise not equal to another point. */
	bool operator!=(const decPoint3 &p) const;
	/** Determines if all the components of this point are less then the components of another one. */
	bool operator<(const decPoint3 &p) const;
	/** Determines if all the components of this point are greater then the components of another one. */
	bool operator>(const decPoint3 &p) const;
	/** Determines if all the components of this point are less then or equal to the components of another one. */
	bool operator<=(const decPoint3 &p) const;
	/** Determines if all the components of this point are greater then or equal to the components of another one. */
	bool operator>=(const decPoint3 &p) const;
	/*@}*/
};
