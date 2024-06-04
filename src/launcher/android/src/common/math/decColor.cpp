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
