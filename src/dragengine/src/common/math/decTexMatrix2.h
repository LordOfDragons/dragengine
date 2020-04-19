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
 * \brief 3x2 component texture matrix in row major order.
 */
class decTexMatrix2{
public:
	float a11, a12, a13;
	float a21, a22, a23;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create identity matrix. */
	decTexMatrix2();
	/*@}*/
	
	
	
	/** \name Matrix Creation */
	/*@{*/
	/** \brief Create identity matrix. */
	static decTexMatrix2 CreateIdentity();
	
	/** \brief Create translation matrix. */
	static decTexMatrix2 CreateTranslation( float u, float v );
	
	/** \brief Create translation matrix. */
	static decTexMatrix2 CreateTranslation( const decVector2 &translation );
	
	/** \brief Create scaling matrix. */
	static decTexMatrix2 CreateScale( float su, float sv );
	
	/** \brief Create scaling matrix. */
	static decTexMatrix2 CreateScale( const decVector2 &scaling );
	
	/** \brief Create rotation matrix. */
	static decTexMatrix2 CreateRotation( float rotation );
	
	/** \brief Create shear matrix. */
	static decTexMatrix2 CreateShear( float shearU, float shearV );
	
	/** \brief Create shear matrix. */
	static decTexMatrix2 CreateShear( const decVector2 &shear );
	
	/** \brief Create scale-translation matrix. */
	static decTexMatrix2 CreateST( float su, float sv, float tu, float tv );
	
	/** \brief Create scale-translation matrix. */
	static decTexMatrix2 CreateST( const decVector2 &scaling, const decVector2 &translation );
	
	/** \brief Create scale-rotation-translation matrix. */
	static decTexMatrix2 CreateSRT( float su, float sv, float rotation, float tu, float tv );
	
	/** \brief Create scale-rotation-translation matrix. */
	static decTexMatrix2 CreateSRT( const decVector2 &scaling, float rotation, const decVector2 &translation );
	
	/** \brief Create center scale-rotation-translation matrix. */
	static decTexMatrix2 CreateCenterSRT( float su, float sv, float rotation, float tu, float tv );
	
	/** \brief Create center scale-rotation-translation matrix. */
	static decTexMatrix2 CreateCenterSRT( const decVector2 &scaling, float rotation, const decVector2 &translation );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set identity matrix. */
	void SetIdentity();
	
	/** \brief Set translation matrix. */
	void SetTranslation( float u, float v );
	
	/** \brief Set translation matrix. */
	void SetTranslation( const decVector2 &translation );
	
	/** \brief Set scaling matrix. */
	void SetScale( float su, float sv );
	
	/** \brief Set scaling matrix. */
	void SetScale( const decVector2 &scaling );
	
	/** \brief Set rotation matrix. */
	void SetRotation( float rotation );
	
	/** \brief Set shear matrix. */
	void SetShear( float shearU, float shearV );
	
	/** \brief Set shear matrix. */
	void SetShear( const decVector2 &shear );
	
	/** \brief Set scale-translation matrix. */
	void SetST( float su, float sv, float tu, float tv );
	
	/** \brief Set scale-translation matrix. */
	void SetST( const decVector2 &scaling, const decVector2 &translation );
	
	/** \brief Set scale-rotation-translation matrix. */
	void SetSRT( float su, float sv, float rotation, float tu, float tv );
	
	/** \brief Set scale-rotation-translation matrix. */
	void SetSRT( const decVector2 &scaling, float rotation, const decVector2 &translation );
	
	/** \brief Set scale-rotation-translation matrix. */
	void SetCenterSRT( float su, float sv, float rotation, float tu, float tv );
	
	/** \brief Set center scale-rotation-translation matrix. */
	void SetCenterSRT( const decVector2 &scaling, float rotation, const decVector2 &translation );
	
	/** \brief Determines if this matrix is cell wise equal to anither one in respect to a threshold. */
	bool IsEqualTo( const decTexMatrix2 &matrix, float threshold = TEXEL_THRESHOLD ) const;
	
	/** \brief Determinant. */
	float Determinant() const;
	
	/** \brief Inverse of matrix. */
	decTexMatrix Invert() const;
	
	/** \brief Convert to 3x3 texture matrix. */
	decTexMatrix ToTexMatrix() const;
	
	/** \brief Position part of matrix. */
	decVector2 GetPosition() const;
	
	/** \brief X axis part of matrix. */
	decVector2 GetAxisX() const;
	
	/** \brief Y axis part of matrix. */
	decVector2 GetAxisY() const;
	
	/** \brief Scaling of X and Y axis. */
	decVector2 GetScaling() const;
	
	/** \brief Rotation in degrees. */
	float GetRotation() const;
	
	/**
	 * \brief Shearing in X direction.
	 * 
	 * Due to rotation only X direction shearing can be retrieved. Y is 0.
	 */
	float GetShearing() const;
	
	/**
	 * \brief Rotation part of matrix.
	 * \returns Identity matrix with upper left 2x2 copied from this matrix.
	 */
	decTexMatrix2 GetRotationMatrix() const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set cells of this matrix to the values of another one. */
	decTexMatrix2 &operator=( const decTexMatrix2 &m );
	
	/** \brief Add cells of another matrix to this one. */
	decTexMatrix2 &operator+=( const decTexMatrix2 &m );
	
	/** \brief Subtract cells of another matrix from this vector. */
	decTexMatrix2 &operator-=( const decTexMatrix2 &m );
	
	/** \brief Multiply cells of this matrix with a value k. */
	decTexMatrix2 &operator*=( float k );
	
	/** \brief Divide cells of this matrix by a value k. If the value k is 0 an exception is thrown. */
	decTexMatrix2 &operator/=( float k );
	
	/** \brief Multiply matrix with another one. */
	decTexMatrix2 &operator*=( const decTexMatrix2 &m );
	
	/** \brief Matrix with the sum of this matrix with another one. */
	decTexMatrix2 operator+( const decTexMatrix2 &m ) const;
	
	/** \brief Matrix with the difference of this matrix to another one. */
	decTexMatrix2 operator-( const decTexMatrix2 &m ) const;
	
	/** \brief Matrix with this matrix scaled by k. */
	decTexMatrix2 operator*( float k ) const;
	
	/** \brief Matrix with this matrix divided by k. If k is 0 an exception is thrown. */
	decTexMatrix2 operator/( float k ) const;
	
	/** \brief Matrix with this matrix multiplied with another one. */
	decTexMatrix2 operator*( const decTexMatrix2 &m ) const;
	
	/** \brief Transform vector by this matrix. */
	decVector2 operator*( const decVector2 &v ) const;
	/*@}*/
};
