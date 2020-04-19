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
 * \brief 5x4 component color matrix in row major order.
 * 
 * The colons are (1 to 5): red, green, blue, alpha and white. The rows
 * are (1 to 4): red, green, blue and alpha. Color transformation has
 * to be done in linear color space. With gamma corrected colors certain
 * transformation functions can yield wrong results.
 */
class decColorMatrix{
public:
	float a11, a12, a13, a14, a15;
	float a21, a22, a23, a24, a25;
	float a31, a32, a33, a34, a35;
	float a41, a42, a43, a44, a45;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create color matrix initialized to identity. */
	decColorMatrix();
	
	/** \brief Create copy of a color matrix. */
	decColorMatrix( const decColorMatrix &m );
	/*@}*/
	
	
	
	/** \name Matrix Creation */
	/*@{*/
	/** \brief Create identity color matrix. */
	static decColorMatrix CreateIdentity();
	
	/** \brief Create color translation matrix. */
	static decColorMatrix CreateTranslation( float red, float green, float blue, float alpha );
	
	/** \brief Create color translation matrix. */
	static decColorMatrix CreateTranslation( const decColor &color );
	
	/** \brief Create color scaling matrix. */
	static decColorMatrix CreateScaling( float sred, float sgreen, float sblue, float salpha );
	
	/** \brief Create color scaling matrix. */
	static decColorMatrix CreateScaling( const decColor &color );
	
	/** \brief Create scaling then translation color matrix. */
	static decColorMatrix CreateST( const decColor &scale, const decColor &translate );
	
	/**
	 * \brief Create color matrix with colors as transformation base.
	 * 
	 * The parameters fill the colons from 1 to 5 in the appropriate order..
	 */
	static decColorMatrix CreateFrom( const decColor &red, const decColor &green,
		const decColor &blue, const decColor &alpha, const decColor &white );
	
	/**
	 * \brief Create brightness adjustment matrix.
	 * 
	 * Brightness can be positive or negative.
	 */
	static decColorMatrix CreateBrightness( float brightness );
	
	/**
	 * \brief Create brightness adjustment matrix.
	 * 
	 * Brightness can be positive or negative.
	 */
	static decColorMatrix CreateBrightness( float red, float green, float blue );
	
	/**
	 * \brief Create brightness adjustment matrix.
	 * 
	 * Brightness can be positive or negative.
	 */
	static decColorMatrix CreateBrightness( const decColor &brightness );
	
	/**
	 * \brief Create contrast adjustment matrix.
	 * 
	 * Contrast can be positive or negative.
	 */
	static decColorMatrix CreateContrast( float contrast );
	
	/**
	 * \brief Create contrast adjustment matrix.
	 * 
	 * Contrast can be positive or negative.
	 */
	static decColorMatrix CreateContrast( float red, float green, float blue );
	
	/**
	 * \brief Create contrast adjustment matrix.
	 * 
	 * Contrast can be positive or negative.
	 */
	static decColorMatrix CreateContrast( const decColor &contrast );
	
	/**
	 * \brief Create saturation adjustment matrix.
	 * 
	 * Saturation can be positive or negative.
	 */
	static decColorMatrix CreateSaturation( float saturation );
	
	/**
	 * \brief Create saturation adjustment matrix.
	 * 
	 * Saturation can be positive or negative.
	 */
	static decColorMatrix CreateSaturation( float red, float green, float blue );
	
	/**
	 * \brief Create saturation adjustment matrix.
	 * 
	 * Saturation can be positive or negative.
	 */
	static decColorMatrix CreateSaturation( const decColor &saturation );
	
	/** \brief Create color inversion matrix. */
	static decColorMatrix CreateColorInversion();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set identity color matrix. */
	void SetIdentity();
	
	/** \brief Set translation color matrix. */
	void SetTranslation( float red, float green, float blue, float alpha );
	
	/** \brief Set translation color matrix. */
	void SetTranslation( const decColor &color );
	
	/** \brief Set scaling color matrix. */
	void SetScaling( float sred, float sgreen, float sblue, float salpha );
	
	/** \brief Set scaling color matrix. */
	void SetScaling( const decColor &color );
	
	/** \brief Set scaling then translation color matrix. */
	void SetST( const decColor &scale, const decColor &translate );
	
	/**
	 * \brief Set color matrix transformation base.
	 * 
	 * The parameters fill the colons from 1 to 5 in the appropriate order..
	 */
	void SetFrom( const decColor &red, const decColor &green, const decColor &blue,
		const decColor &alpha, const decColor &white );
	
	/**
	 * \brief Set brightness adjustment matrix.
	 * 
	 * Brightness can be positive or negative.
	 */
	void SetBrightness( float brightness );
	
	/**
	 * \brief Set brightness adjustment matrix.
	 * 
	 * Brightness can be positive or negative.
	 */
	void SetBrightness( float red, float green, float blue );
	
	/**
	 * \brief Set brightness adjustment matrix.
	 * 
	 * Brightness can be positive or negative.
	 */
	void SetBrightness( const decColor &brightness );
	
	/**
	 * \brief Set contrast adjustment matrix.
	 * 
	 * Contrast can be positive or negative.
	 */
	void SetContrast( float contrast );
	
	/**
	 * \brief Set contrast adjustment matrix.
	 * 
	 * Contrast can be positive or negative.
	 */
	void SetContrast( float red, float green, float blue );
	
	/**
	 * \brief Set contrast adjustment matrix.
	 * 
	 * Contrast can be positive or negative.
	 */
	void SetContrast( const decColor &contrast );
	
	/**
	 * \brief Set saturation adjustment matrix.
	 * 
	 * Saturation can be positive or negative.
	 */
	void SetSaturation( float saturation );
	
	/**
	 * \brief Set saturation adjustment matrix.
	 * 
	 * Saturation can be positive or negative.
	 */
	void SetSaturation( float red, float green, float blue );
	
	/**
	 * \brief Set saturation adjustment matrix.
	 * 
	 * Saturation can be positive or negative.
	 */
	void SetSaturation( const decColor &saturation );
	
	/** \brief Set color inversion matrix. */
	void SetColorInversion();
	
	
	
	/**
	 * \brief Determinant of the color matrix.
	 * 
	 * Mathematically this only works on a 5x5 matrix. The color matrix
	 * is first expanded with an identity line at the bottom to become a
	 * 5x5 matrix. From this matrix the determinant is calculated from.
	 */
	float Determinant() const;
	
	/**
	 * \brief Inverse of color matrix if the inverse is possible.
	 * 
	 * Mathematically this only works for a 5x5 matrix. The color matrix
	 * is expanded with an identity line to become a 5x5 matrix. Form
	 * this matrix the inverse is calculated and the 5x4 part returned.
	 */
	decColorMatrix Invert() const;
	
	/** \brief Red color base. */
	decColor GetRedBase() const;
	
	/** \brief Green color base. */
	decColor GetGreenBase() const;
	
	/** \brief Blue color base. */
	decColor GetBlueBase() const;
	
	/** \brief Alpha color base. */
	decColor GetAlphaBase() const;
	
	/** \brief White color base. */
	decColor GetWhiteBase() const;
	
	/** \brief Set red color base. */
	void SetRedBase( const decColor &color );
	
	/** \brief Set green color base. */
	void SetGreenBase( const decColor &color );
	
	/** \brief Set blue color base. */
	void SetBlueBase( const decColor &color );
	
	/** \brief Set alpha color base. */
	void SetAlphaBase( const decColor &color );
	
	/** \brief Set white color base. */
	void SetWhiteBase( const decColor &color );
	
	/** \brief Transform color by this matrix. */
	decColor Transform( const decColor &color ) const;
	
	/** \brief Transform color by this matrix. */
	decColor Transform( float red, float green, float blue ) const;
	
	/** \brief Transform color by this matrix. */
	decColor Transform( float red, float green, float blue, float alpha ) const;
	
	/**
	 * \brief Transform only color component of color by this matrix.
	 * 
	 * \em color alpha is assumed to be one. Result alpha is set to 1.
	 */
	decColor TransformColor( const decColor &color ) const;
	
	/** \brief Matrix is cell wise equal to another one in respect to a threshold. */
	bool IsEqualTo( const decColorMatrix &matrix, float threshold = VECTOR_THRESHOLD ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another matrix to this matrix. */
	decColorMatrix &operator=( const decColorMatrix &matrix );
	
	/** \brief Cell-wise add another matrix this this. */
	decColorMatrix &operator+=( const decColorMatrix &matrix );
	
	/** \brief Cell-wise subtract another matrix from this matrix. */
	decColorMatrix &operator-=( const decColorMatrix &matrix );
	
	/** \brief Cell-wise multiply matrix with a scalar. */
	decColorMatrix &operator*=( float scalar );
	
	/**
	 * \brief Cell-wise divide matrix by a scalar.
	 * \throws deeDivisionByZero \em scalar is 0.
	 */
	decColorMatrix &operator/=( float scalar );
	
	/**
	 * \brief Matrix-multiply this matrix with another matrix.
	 * 
	 * Mathematically this results in a 5x5 matrix. The last row is discarded.
	 */
	decColorMatrix &operator*=( const decColorMatrix &matrix );
	
	/** \brief Cell-wise addition this matrix and another matrix. */
	decColorMatrix operator+( const decColorMatrix &matrix ) const;
	
	/** \brief Cell-wise subtraction of another matrix from this matrix. */
	decColorMatrix operator-( const decColorMatrix &matrix ) const;
	
	/** \brief Cell-wise multiplication of matrix with a scalar. */
	decColorMatrix operator*( float scalar ) const;
	
	/**
	 * \brief Cell-wise division of matrix by a scalar.
	 * \throws deeDivisionByZero \em scalar is 0.
	 */
	decColorMatrix operator/( float scalar ) const;
	
	/**
	 * \brief Matrix-multiplication of this matrix with another matrix.
	 * 
	 * Mathematically this results in a 5x5 matrix. The last row is discarded.
	 */
	decColorMatrix operator*( const decColorMatrix &matrix ) const;
	
	/** \brief Transform color by matrix. */
	decColor operator*( const decColor &color ) const;
	/*@}*/
};
