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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "decMath.h"
#include "../exceptions.h"



// Class decColor
///////////////////

// Constructor
////////////////

decColor::decColor() :
r( 0.0f ),
g( 0.0f ),
b( 0.0f ),
a( 1.0f ){
}

decColor::decColor( float red, float green, float blue ) :
r( red ),
g( green ),
b( blue ),
a( 1.0f ){
}

decColor::decColor( float red, float green, float blue, float alpha ) :
r( red ),
g( green ),
b( blue ),
a( alpha ){
}

decColor::decColor( const decColor &copy ) :
r( copy.r ),
g( copy.g ),
b( copy.b ),
a( copy.a ){
}

decColor::decColor( const decColor &copy, float alpha ) :
r( copy.r ),
g( copy.g ),
b( copy.b ),
a( alpha ){
}



// Management
///////////////

decColor decColor::Normalized() const{
	return decColor(
		decMath::clamp( r, 0.0f, 1.0f ),
		decMath::clamp( g, 0.0f, 1.0f ),
		decMath::clamp( b, 0.0f, 1.0f ),
		decMath::clamp( a, 0.0f, 1.0f ) );
}

void decColor::Normalize(){
	r = decMath::clamp( r, 0.0f, 1.0f );
	g = decMath::clamp( g, 0.0f, 1.0f );
	b = decMath::clamp( b, 0.0f, 1.0f );
	a = decMath::clamp( a, 0.0f, 1.0f );
}

decColor decColor::Inverted() const{
	return decColor( 1.0f - r, 1.0f - g, 1.0f - b, a );
}

void decColor::Invert(){
	r = 1.0f - r;
	g = 1.0f - g;
	b = 1.0f - b;
}

void decColor::SetZero(){
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
	a = 1.0f;
}

void decColor::Set( float red, float green, float blue ){
	r = red;
	g = green;
	b = blue;
}

void decColor::Set( float red, float green, float blue, float alpha ){
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

bool decColor::IsEqualTo( const decColor &copy, float threshold ) const{
	return fabsf( r - copy.r ) < threshold
		&& fabsf( g - copy.g ) < threshold
		&& fabsf( b - copy.b ) < threshold
		&& fabsf( a - copy.a ) < threshold;
}



// Operators
//////////////

decColor &decColor::operator=( const decColor &color ){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
	return *this;
}

decColor &decColor::operator+=( const decColor &color ){
	r += color.r;
	g += color.g;
	b += color.b;
	a += color.a;
	return *this;
}

decColor &decColor::operator-=( const decColor &color ){
	r -= color.r;
	g -= color.g;
	b -= color.b;
	a -= color.a;
	return *this;
}

decColor &decColor::operator*=( float scalar ){
	r *= scalar;
	g *= scalar;
	b *= scalar;
	a *= scalar;
	return *this;
}

decColor &decColor::operator/=( float scalar ){
	if( scalar == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	const float factor = 1.0f / scalar;
	r *= factor;
	g *= factor;
	b *= factor;
	a *= factor;
	return *this;
}

decColor decColor::operator+( const decColor &color ) const{
	return decColor( r + color.r, g + color.g, b + color.b, a + color.a );
}

decColor decColor::operator-( const decColor &color ) const{
	return decColor( r - color.r, g - color.g, b - color.b, a - color.a );
}

decColor decColor::operator*( float scalar ) const{
	return decColor( r * scalar, g * scalar, b * scalar, a * scalar );
}

decColor decColor::operator*( const decColor &color ) const{
	return decColor( r * color.r, g * color.g, b * color.b, a * color.a );
}

decColor decColor::operator/( float scalar ) const{
	if( scalar == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	const float factor = 1.0f / scalar;
	return decColor( r * factor, g * factor, b * factor, a * factor );
}
