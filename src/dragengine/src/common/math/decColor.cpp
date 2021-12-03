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

decColor decColor::CreateHSV( float hue, float saturation, float value ){
	// https://www.rapidtables.com/convert/color/index.html
	hue = decMath::normalize( hue, 0.0f, 1.0f ) * 360.0f;
	saturation = decMath::clamp( saturation, 0.0f, 1.0f );
	value = decMath::clamp( value, 0.0f, 1.0f );
	
	const float c = value * saturation;
	const float x = c * ( 1.0f - fabsf( fmodf( hue / 60.0f, 2.0f ) - 1.0f ) );
	const float m = value - c;
	
	float tr = 0.0f, tg = 0.0f, tb = 0.0f;
	
	if( hue < 60.0f ){
		tr = c;
		tg = x;
		
	}else if( hue < 120.0f ){
		tr = x;
		tg = c;
		
	}else if( hue < 180.0f ){
		tg = c;
		tb = x;
		
	}else if( hue < 240.0f ){
		tg = x;
		tb = c;
		
	}else if( hue < 300.0f ){
		tr = x;
		tb = c;
		
	}else{
		tr = c;
		tb = x;
	}
	
	return decColor( tr + m, tg + m, tb + m );
}

decColor decColor::CreateHSL( float hue, float saturation, float lightness ){
	// https://www.rapidtables.com/convert/color/index.html
	hue = decMath::normalize( hue, 0.0f, 1.0f ) * 360.0f;
	saturation = decMath::clamp( saturation, 0.0f, 1.0f );
	lightness = decMath::clamp( lightness, 0.0f, 1.0f );
	
	const float c = ( 1.0f - fabsf( lightness * 2.0f - 1.0f ) ) * saturation;
	const float x = c * ( 1.0f - fabsf( fmodf( hue / 60.0f, 2.0f ) - 1.0f ) );
	const float m = lightness - c / 2.0f;
	
	float tr = 0.0f, tg = 0.0f, tb = 0.0f;
	
	if( hue < 60.0f ){
		tr = c;
		tg = x;
		
	}else if( hue < 120.0f ){
		tr = x;
		tg = c;
		
	}else if( hue < 180.0f ){
		tg = c;
		tb = x;
		
	}else if( hue < 240.0f ){
		tg = x;
		tb = c;
		
	}else if( hue < 300.0f ){
		tr = x;
		tb = c;
		
	}else{
		tr = c;
		tb = x;
	}
	
	return decColor( tr + m, tg + m, tb + m );
}

decColor decColor::CreateCMYK( float cyan, float magenta, float yellow, float black ){
	const float tc = 1.0f - decMath::clamp( cyan, 0.0f, 1.0f );
	const float tm = 1.0f - decMath::clamp( magenta, 0.0f, 1.0f );
	const float ty = 1.0f - decMath::clamp( yellow, 0.0f, 1.0f );
	const float tk = 1.0f - decMath::clamp( black, 0.0f, 1.0f );
	return decColor( tc * tk, tm * tk, ty * tk );
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

void decColor::ToHSV( float &hue, float &saturation, float &value ) const{
	const float tr = decMath::clamp( r, 0.0f, 1.0f );
	const float tg = decMath::clamp( g, 0.0f, 1.0f );
	const float tb = decMath::clamp( b, 0.0f, 1.0f );
	const float cmax = decMath::max( tr, tg, tb );
	const float cmin = decMath::min( tr, tg, tb );
	const float delta = cmax - cmin;
	
	if( delta == 0.0f ){
		hue = 0.0f;
		
	}else if( cmax == tr ){
		hue = decMath::normalize( ( tg - tb ) / delta, 6.0f ) / 6.0f;
		
	}else if( cmax == tg ){
		hue = ( ( tb - tr ) / delta + 2.0f ) / 6.0f;
		
	}else{ // cmax == tb
		hue = ( ( tr - tg ) / delta + 4.0f ) / 6.0f;
	}
	
	if( cmax == 0.0f ){
		saturation = 0;
		
	}else{
		saturation = delta / cmax;
	}
	
	value = cmax;
}

void decColor::ToHSL( float &hue, float &saturation, float &lightness ) const{
	const float tr = decMath::clamp( r, 0.0f, 1.0f );
	const float tg = decMath::clamp( g, 0.0f, 1.0f );
	const float tb = decMath::clamp( b, 0.0f, 1.0f );
	const float cmax = decMath::max( tr, tg, tb );
	const float cmin = decMath::min( tr, tg, tb );
	const float delta = cmax - cmin;
	
	if( delta == 0.0f ){
		hue = 0.0f;
		
	}else if( cmax == tr ){
		hue = decMath::normalize( ( tg - tb ) / delta, 6.0f ) / 6.0f;
		
	}else if( cmax == tg ){
		hue = ( ( tb - tr ) / delta + 2.0f ) / 6.0f;
		
	}else{ // cmax == tb
		hue = ( ( tr - tg ) / delta + 4.0f ) / 6.0f;
	}
	
	lightness = ( cmax + cmin ) / 2.0f;
	
	if( delta == 0.0f ){
		saturation = 0;
		
	}else{
		saturation = delta / ( 1.0f - fabsf( lightness * 2.0f - 1.0f ) );
	}
}

void decColor::ToCMYK( float &cyan, float &magenta, float &yellow, float &black ) const{
	const float tr = decMath::clamp( r, 0.0f, 1.0f );
	const float tg = decMath::clamp( g, 0.0f, 1.0f );
	const float tb = decMath::clamp( b, 0.0f, 1.0f );
	
	black = 1.0f - decMath::max( tr, tg, tb );
	
	if( black == 1.0f ){
		cyan = 0.0f;
		magenta = 0.0f;
		yellow = 0.0f;
		
	}else{
		cyan = ( 1.0f - tr - black ) / ( 1.0f - black );
		magenta = ( 1.0f - tg - black ) / ( 1.0f - black );
		yellow = ( 1.0f - tb - black ) / ( 1.0f - black );
	}
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

decColor &decColor::operator*=( const decColor &color ){
	r *= color.r;
	g *= color.g;
	b *= color.b;
	a *= color.a;
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
