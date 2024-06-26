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
 * \brief 3x3 Component Texture Matrix.
 * Provides matrix calculation on a 3x3 matrix in row major order.
 * This kind of matrix is intended to be used for texture coordinate
 * transformations and composes of a single rotation part and a
 * translation part. For more complex transformations a 4x4 matrix
 * can be used too.
 */
class DE_DLL_EXPORT decTexMatrix{
public:
	/**
	 * \name Cells
	 * The first number indicates the row the second the column.
	 */
	float a11, a12, a13;
	float a21, a22, a23;
	float a31, a32, a33;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new matrix initialized to identity. */
	decTexMatrix();
	/*@}*/
	
	
	
	/** \name Matrix Creation */
	/*@{*/
	/** \brief Create new identity matrix. */
	static decTexMatrix CreateIdentity();
	
	/** \brief Create new translation matrix. */
	static decTexMatrix CreateTranslation( float u, float v );
	
	/** \brief Create new translation matrix. */
	static decTexMatrix CreateTranslation( const decVector2 &t );
	
	/** \brief Create new scaling matrix. */
	static decTexMatrix CreateScale( float su, float sv );
	
	/** \brief Create new scaling matrix. */
	static decTexMatrix CreateScale( const decVector2 &s );
	
	/** \brief Create new rotation matrix. */
	static decTexMatrix CreateRotation( float arc );
	
	/** \brief Create shear matrix. */
	static decTexMatrix CreateShear( float shearU, float shearV );
	
	/** \brief Create shear matrix. */
	static decTexMatrix CreateShear( const decVector2 &shear );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set matrix to the identity matrix. */
	void SetIdentity();
	
	/** \brief Matrix is cell wise equal to anither one in respect to a threshold. */
	bool IsEqualTo( const decTexMatrix &matrix, float threshold = TEXEL_THRESHOLD ) const;
	
	/** \brief Determinant. */
	float Determinant() const;
	
	/** \brief Inverse of texture matrix. */
	decTexMatrix Invert() const;
	
	/** \brief Convert to 3x2 texture matrix dropping the last row. */
	decTexMatrix2 ToTexMatrix2() const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set cells of this matrix to the values of another one. */
	decTexMatrix &operator=( const decTexMatrix &m );
	
	/** \brief Adds the cells of another matrix to this one. */
	decTexMatrix &operator+=( const decTexMatrix &m );
	
	/** \brief Subtracts the cells of another matrix from this vector. */
	decTexMatrix &operator-=( const decTexMatrix &m );
	
	/** \brief Multiplies the cells of this matrix with a value k. */
	decTexMatrix &operator*=( float k );
	
	/**
	 * \brief Divide cells of this matrix by a value k.
	 * 
	 * If the value k is 0 an exception is thrown.
	 */
	decTexMatrix &operator/=( float k );
	
	/** \brief Multiplies this matrix with another one. */
	decTexMatrix &operator*=( const decTexMatrix &m );
	
	/** \brief Retrieves a new matrix with the sum of this matrix with another one. */
	decTexMatrix operator+( const decTexMatrix &m ) const;
	
	/** \brief Retrieves a new matrix with the difference of this matrix to another one. */
	decTexMatrix operator-( const decTexMatrix &m ) const;
	
	/** \brief Retrieves a new matrix with this matrix scaled by k. */
	decTexMatrix operator*( float k ) const;
	
	/**
	 * \brief Matrix with this matrix divided by k.
	 * 
	 * If k is 0 an exception is thrown.
	 */
	decTexMatrix operator/( float k ) const;
	
	/** \brief Retrieves a new matrix with this matrix multiplied with another one. */
	decTexMatrix operator*( const decTexMatrix &m ) const;
	
	/** \brief Transforms vector by this matrix. */
	decVector2 operator*( const decVector2 &v ) const;
	/*@}*/
};
