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
 * \brief 4 Component Color.
 * 
 * RGBA style color in decimal range. The value of 0 indicates no luminance in that
 * color and 1 indicates full luminance. The color also contains an alpha component
 * where 0 indicates completly transparent and 1 indicates completly opaque.
 */
class decColor{
public:
	/** \brief Red Component of color. */
	float r;
	
	/** \brief Green Component of color. */
	float g;
	
	/** \brief Blue Component of color. */
	float b;
	
	/** \brief Alpha Component of color. */
	float a;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create color initialized to (0,0,0,1). */
	decColor();
	
	/** \brief Create color with initial values. */
	decColor( float red, float green, float blue );
	
	/** \brief Create color with the given values. */
	decColor( float red, float green, float blue, float alpha );
	
	/** \brief Creates copy of a color. */
	decColor( const decColor &copy );
	
	/** \brief Creates copy of a color with different alpha value. */
	decColor( const decColor &copy, float alpha );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Normalized color with each component clamped to the range of 0 to 1. */
	decColor Normalized() const;
	
	/** \brief Normalize color. Each component is clamped to the range of 0 to 1. */
	void Normalize();
	
	/** \brief Inverted color. Each component except alpha is 1-value. */
	decColor Inverted() const;
	
	/** \brief Invert color. Each component except alpha is set to 1-value. */
	void Invert();
	
	/** \brief Set red, green and blue to 0 and alpha to 1. */
	void SetZero();
	
	/** \brief Set values. */
	void Set( float red, float green, float blue );
	
	/** \brief Sets values. */
	void Set( float red, float green, float blue, float alpha );
	
	/** \brief Color is equal component wise to another color with respect to a threshold. */
	bool IsEqualTo( const decColor &color, float threshold = COLOR_THRESHOLD ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy color to this color. */
	decColor &operator=( const decColor &color );
	
	/** \brief Add components of another color to this one. */
	decColor &operator+=( const decColor &color );
	
	/** \brief Subtract components of another color from this color. */
	decColor &operator-=( const decColor &color );
	
	/** \brief Multiply components of this color with a value k. */
	decColor &operator*=( float scalar );
	
	/**
	 * \brief Divide components of this color by a scalar value.
	 * \throws deeDivisionByZero Scalar is 0.
	 */
	decColor &operator/=( float scalar );
	
	/** \brief Color with the sum of this color with another one. */
	decColor operator+( const decColor &color ) const;
	
	/** \brief Color with the difference of this color to another one. */
	decColor operator-( const decColor &color ) const;
	
	/** \brief Color with this color scaled by a scalar. */
	decColor operator*( float scalar ) const;
	
	/** \brief Color with this color multiplied component wise with another one. */
	decColor operator*( const decColor &color ) const;
	
	/**
	 * \brief Color with this color divided by a scalar.
	 * \throws deeDivisionByZero Scalar is 0.
	 */
	decColor operator/( float scalar ) const;
	/*@}*/
};
