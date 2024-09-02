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



// Class decVector2
/////////////////////

// Constructor
////////////////

decVector2::decVector2() :
x( 0.0f ),
y( 0.0f ){
}

decVector2::decVector2( float nx, float ny ) :
x( nx ),
y( ny ){
}

decVector2::decVector2( const decVector2 &v ) :
x( v.x ),
y( v.y ){
}

decVector2::decVector2( const decPoint &p ) :
x( ( float )p.x ),
y( ( float )p.y ){
}



// Management
///////////////

float decVector2::Length() const{
	return sqrtf( x * x + y * y );
}

void decVector2::Normalize(){
	const float len = sqrtf( x * x + y * y );
	if( len == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	x /= len;
	y /= len;
}

decVector2 decVector2::Normalized() const{
	const float len = sqrtf( x * x + y * y );
	if( len == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	return decVector2( x / len, y / len );
}

const decVector2 decVector2::Absolute() const{
	return decVector2( fabsf( x ), fabsf( y ) );
}

void decVector2::Snap( float grid ){
	if( grid == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	const float halfGrid = grid * 0.5f;
	float integral;
	
	// x
	integral = grid * ( float )floorf( x / grid );
	
	if( x - integral < halfGrid ){
		x = integral;
		
	}else{
		x = integral + grid;
	}
	
	// y
	integral = grid * ( float )floorf( y / grid );
	
	if( y - integral < halfGrid ){
		y = integral;
		
	}else{
		y = integral + grid;
	}
}

bool decVector2::IsEqualTo( const decVector2 &v, float threshold ) const{
	return fabsf( x - v.x ) < threshold && fabsf( y - v.y ) < threshold;
}

void decVector2::SetZero(){
	x = 0.0f;
	y = 0.0f;
}

void decVector2::Set( float nx, float ny ){
	x = nx;
	y = ny;
}

void decVector2::Set( const decVector2 &v ){
	x = v.x;
	y = v.y;
}

void decVector2::Negate(){
	x = -x;
	y = -y;
}

void decVector2::SetSmallest( const decVector2 &v ){
	if( v.x < x ){
		x = v.x;
	}
	if( v.y < y ){
		y = v.y;
	}
}

decVector2 decVector2::Smallest( const decVector2 &v ) const{
	return decVector2(
		( x < v.x ) ? x : v.x,
		( y < v.y ) ? y : v.y
	);
}

void decVector2::SetLargest( const decVector2 &v ){
	if( v.x > x ){
		x = v.x;
	}
	if( v.y > y ){
		y = v.y;
	}
}

decVector2 decVector2::Largest( const decVector2 &v ) const{
	return decVector2(
		( x > v.x ) ? x : v.x,
		( y > v.y ) ? y : v.y
	);
}

void decVector2::SetClamped( const decVector2 &minValue, const decVector2 &maxValue ){
	if( x < minValue.x ){
		x = minValue.x;
		
	}else if( x > maxValue.x ){
		x = maxValue.x;
	}
	
	if( y < minValue.y ){
		y = minValue.y;
		
	}else if( y > maxValue.y ){
		y = maxValue.y;
	}
}

decVector2 decVector2::Clamped( const decVector2 &minValue, const decVector2 &maxValue ) const{
	return decVector2(
		( x < minValue.x ) ? minValue.x : ( ( x > maxValue.x ) ? maxValue.x : x ),
		( y < minValue.y ) ? minValue.y : ( ( y > maxValue.y ) ? maxValue.y : y )
	);
}

decVector2 decVector2::RotateLeft() const{
	return decVector2( -y, x );
}

decVector2 decVector2::RotateRight() const{
	return decVector2( y, -x );
}



// Operators
//////////////

decVector2 decVector2::operator-() const{
	return decVector2( -x, -y );
}

decVector2 &decVector2::operator=( const decVector2 &v ){
	x = v.x;
	y = v.y;
	return *this;
}

decVector2 &decVector2::operator+=( const decVector2 &v ){
	x += v.x;
	y += v.y;
	return *this;
}

decVector2 &decVector2::operator-=( const decVector2 &v ){
	x -= v.x;
	y -= v.y;
	return *this;
}

decVector2 &decVector2::operator*=( float k ){
	x *= k;
	y *= k;
	return *this;
}

decVector2 &decVector2::operator/=( float k ){
	if( k == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	x /= k;
	y /= k;
	return *this;
}

decVector2 decVector2::operator+( const decVector2 &v ) const{
	return decVector2( x + v.x, y + v.y );
}

decVector2 decVector2::operator-( const decVector2 &v ) const{
	return decVector2( x - v.x, y - v.y );
}

decVector2 decVector2::operator*( float k ) const{
	return decVector2( x * k, y * k );
}

float decVector2::operator*( const decVector2 &v ) const{
	return x * v.x + y * v.y;
}

decVector2 decVector2::operator/( float k ) const{
	if( k == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	return decVector2( x / k, y / k );
}

bool decVector2::operator<( const decVector2 &v ) const{
	return x < v.x && y < v.y;
}

bool decVector2::operator>( const decVector2 &v ) const{
	return x > v.x && y > v.y;
}

bool decVector2::operator<=( const decVector2 &v ) const{
	return x <= v.x && y <= v.y;
}

bool decVector2::operator>=( const decVector2 &v ) const{
	return x >= v.x && y >= v.y;
}
