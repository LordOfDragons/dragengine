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
 * @brief 3x3 Component Texture Matrix.
 * Provides matrix calculation on a 3x3 matrix in row major order.
 * This kind of matrix is intended to be used for texture coordinate
 * transformations and composes of a single rotation part and a
 * translation part. For more complex transformations a 4x4 matrix
 * can be used too.
 */
class decTexMatrix{
public:
	/**
	 * @name Cells
	 * The first number indicates the row the second the column.
	 */
	float a11, a12, a13;
	float a21, a22, a23;
	float a31, a32, a33;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new matrix initialized to identity. */
	decTexMatrix();
	/*@}*/
	
	/** @name Matrix Creation */
	/*@{*/
	/** Creates a new identity matrix. */
	static decTexMatrix CreateIdentity();
	/** Creates a new translation matrix. */
	static decTexMatrix CreateTranslation( float u, float v );
	/** Creates a new translation matrix. */
	static decTexMatrix CreateTranslation( const decVector2 &t );
	/** Creates a new scaling matrix. */
	static decTexMatrix CreateScale( float su, float sv );
	/** Creates a new scaling matrix. */
	static decTexMatrix CreateScale( const decVector2 &s );
	/** Creates a new rotation matrix. */
	static decTexMatrix CreateRotation( float arc );
	
	/** \brief Create shear matrix. */
	static decTexMatrix CreateShear( float shearU, float shearV );
	
	/** \brief Create shear matrix. */
	static decTexMatrix CreateShear( const decVector2 &shear );
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Sets the matrix to the identity matrix. */
	void SetIdentity();
	/**
	 * Determines if this matrix is cell wise equal to anither one in respect to a threshold.
	 */
	bool IsEqualTo( const decTexMatrix &matrix, float threshold = TEXEL_THRESHOLD ) const;
	
	/** \brief Determinant. */
	float Determinant() const;
	
	/** \brief Inverse of texture matrix. */
	decTexMatrix Invert() const;
	
	/** \brief Convert to 3x2 texture matrix dropping the last row. */
	decTexMatrix2 ToTexMatrix2() const;
	/*@}*/
	
	/** @name Operators */
	/*@{*/
	/** Sets the cells of this matrix to the values of another one. */
	decTexMatrix &operator=( const decTexMatrix &m );
	/** Adds the cells of another matrix to this one. */
	decTexMatrix &operator+=( const decTexMatrix &m );
	/** Subtracts the cells of another matrix from this vector. */
	decTexMatrix &operator-=( const decTexMatrix &m );
	/** Multiplies the cells of this matrix with a value k. */
	decTexMatrix &operator*=( float k );
	/**
	 * Divides the cells of this matrix by a value k. If the value k
	 * is 0 an exception is thrown.
	 */
	decTexMatrix &operator/=( float k );
	/** Multiplies this matrix with another one. */
	decTexMatrix &operator*=( const decTexMatrix &m );
	/** Retrieves a new matrix with the sum of this matrix with another one. */
	decTexMatrix operator+( const decTexMatrix &m ) const;
	/** Retrieves a new matrix with the difference of this matrix to another one. */
	decTexMatrix operator-( const decTexMatrix &m ) const;
	/** Retrieves a new matrix with this matrix scaled by k. */
	decTexMatrix operator*( float k ) const;
	/**
	 * Retrieves a new matrix with this matrix divided by k. If k is 0 an
	 * exception is thrown.
	 */
	decTexMatrix operator/( float k ) const;
	/** Retrieves a new matrix with this matrix multiplied with another one. */
	decTexMatrix operator*( const decTexMatrix &m ) const;
	/** Transforms vector by this matrix. */
	decVector2 operator*( const decVector2 &v ) const;
	/*@}*/
};
