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
 * @brief 4x3 Component Matrix.
 * Provides matrix calculation on a 3x3 matrix in row major order.
 * This kind of matrix is useful for transforming 3 component
 * color values.
 */
class decColorMatrix3{
public:
	/**
	 * @name Cells
	 * The first number indicates the row the second the column.
	 */
	float a11, a12, a13, a14;
	float a21, a22, a23, a24;
	float a31, a32, a33, a34;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new color matrix initialized to identity. */
	decColorMatrix3();
	/** Creates a copy of a color matrix. */
	decColorMatrix3( const decColorMatrix3 &m );
	/*@}*/
	
	/** @name Matrix Creation */
	/*@{*/
	/** Creates a new identity color matrix. */
	static decColorMatrix3 CreateIdentity();
	/** Creates a new color translation matrix. */
	static decColorMatrix3 CreateTranslation( float red, float green, float blue );
	/** Creates a new color translation matrix. */
	static decColorMatrix3 CreateTranslation( const decColor &color );
	/** Creates a new color scaling matrix. */
	static decColorMatrix3 CreateScaling( float sred, float sgreen, float sblue );
	/** Creates a new color scaling matrix. */
	static decColorMatrix3 CreateScaling( const decColor &color );
	/**
	 * Creates a color matrix with the given colors as transformation base.
	 * The red color fills the values in the first column, the green of
	 * the second, the blue of the third, the alpha of the fourth and the
	 * translation the fifth.
	 */
	static decColorMatrix3 CreateFrom( const decColor &red, const decColor &green, const decColor &blue, const decColor &translation );
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Sets identity color matrix. */
	void SetIdentity();
	/** Sets color translation matrix. */
	void SetTranslation( float red, float green, float blue );
	/** Sets color translation matrix. */
	void SetTranslation( const decColor &color );
	/** Sets color scaling matrix. */
	void SetScaling( float sred, float sgreen, float sblue );
	/** Sets color scaling matrix. */
	void SetScaling( const decColor &color );
	/**
	 * Sets color matrix with the given colors as transformation base.
	 * The red color fills the values in the first column, the green of
	 * the second, the blue of the third, the alpha of the fourth and the
	 * translation the fifth.
	 */
	void SetFrom( const decColor &red, const decColor &green, const decColor &blue, const decColor &translation );
	
	/**
	 * Calculates the determinant of the color matrix. Mathematically this only
	 * works on a 5x5 matrix. Hence the color matrix is first expanded with an
	 * identity line at the bottom to become a 5x5 matrix to calculate the
	 * determinant from.
	 */
	float Determinant() const;
	/**
	 * Retrieves the inverse of this color matrix if the inverse is possible.
	 * Mathematically this only works for a 4x4 matrix. Hence we expand the
	 * color matrix with an identity line to become a 4x4 matrix. Thereof the
	 * inverse is calculated and only the 4x3 part is returned.
	 */
	decColorMatrix3 Invert() const;
	/** Retrieves the red color base. */
	decColor GetRedBase() const;
	/** Retrieves the green color base. */
	decColor GetGreenBase() const;
	/** Retrieves the blue color base. */
	decColor GetBlueBase() const;
	/** Retrieves the translation color base. */
	decColor GetTranslationBase() const;
	/** Sets the red color base. */
	void SetRedBase( const decColor &color );
	/** Sets the green color base. */
	void SetGreenBase( const decColor &color );
	/** Sets the blue color base. */
	void SetBlueBase( const decColor &color );
	/** Sets the translation color base. */
	void SetTranslationBase( const decColor &color );
	/** Transforms color by this matrix. */
	decColor Transform( const decColor &color ) const;
	
	/**
	 * Determines if this matrix is cell wise equal to another one in respect to a threshold.
	 */
	bool IsEqualTo( const decColorMatrix3 &matrix, float threshold = VECTOR_THRESHOLD ) const;
	/*@}*/
	
	/** @name Operators */
	/*@{*/
	/** Sets the cells of this matrix to the values of another one. */
	decColorMatrix3 &operator=( const decColorMatrix3 &matrix );
	/** Adds the cells of another matrix to this one. */
	decColorMatrix3 &operator+=( const decColorMatrix3 &matrix );
	/** Subtracts the cells of another matrix from this vector. */
	decColorMatrix3 &operator-=( const decColorMatrix3 &matrix );
	/** Multiplies the cells of this matrix with a value k. */
	decColorMatrix3 &operator*=( float scalar );
	/**
	 * Divides the cells of this matrix by a value k. If the value k
	 * is 0 an exception is thrown.
	 */
	decColorMatrix3 &operator/=( float scalar );
	/**
	 * Multiplies this matrix with another one. Mathematically this would yield
	 * a 5x5 matrix but as a color matrix has no use for the last line only the
	 * 5x4 part is returned.
	 */
	decColorMatrix3 &operator*=( const decColorMatrix3 &matrix );
	/** Retrieves a new matrix with the sum of this matrix with another one. */
	decColorMatrix3 operator+( const decColorMatrix3 &matrix ) const;
	/** Retrieves a new matrix with the difference of this matrix to another one. */
	decColorMatrix3 operator-( const decColorMatrix3 &matrix ) const;
	/** Retrieves a new matrix with this matrix scaled by k. */
	decColorMatrix3 operator*( float scalar ) const;
	/**
	 * Retrieves a new matrix with this matrix divided by k. If k is 0 an
	 * exception is thrown.
	 */
	decColorMatrix3 operator/( float scalar ) const;
	/**
	 * Retrieves a new matrix with this matrix multiplied with another one.
	 * Mathematically this would yield a 5x5 matrix but as a color matrix has no
	 * use for the last line only the 5x4 part is returned.
	 */
	decColorMatrix3 operator*( const decColorMatrix3 &matrix ) const;
	/** Transforms color by this matrix. */
	decColor operator*( const decColor &color ) const;
	/*@}*/
};
