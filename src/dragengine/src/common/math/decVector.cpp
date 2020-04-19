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

#include <stdio.h>
#include <string.h>

#include "decMath.h"
#include "../exceptions.h"



// Class decVector
////////////////////

// Constructor
////////////////

decVector::decVector() :
x( 0.0f ),
y( 0.0f ),
z( 0.0f ){
}

decVector::decVector( float nx, float ny, float nz ) :
x( nx ),
y( ny ),
z( nz ){
}

decVector::decVector( const decVector &v ) :
x( v.x ),
y( v.y ),
z( v.z ){
}

decVector::decVector( const decDVector &v ) :
x( ( float )v.x ),
y( ( float )v.y ),
z( ( float )v.z ){
}

decVector::decVector( const decPoint3 &p ) :
x( ( float )p.x ),
y( ( float )p.y ),
z( ( float )p.z ){
}



// Management
///////////////

float decVector::Length() const{
	return sqrtf( x * x + y * y + z * z );
}

float decVector::LengthSquared() const{
	return x * x + y * y + z * z;
}

void decVector::Normalize(){
	const float len = sqrtf( x * x + y * y + z * z );
	
	if( len == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	x /= len;
	y /= len;
	z /= len;
}

const decVector decVector::Normalized() const{
	const float len = sqrtf( x * x + y * y + z * z );
	
	if( len == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	return decVector( x / len, y / len, z / len );
}

const decVector decVector::Absolute() const{
	return decVector( fabsf( x ), fabsf( y ), fabsf( z ) );
}

void decVector::Snap( float grid ){
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
	
	// z
	integral = grid * ( float )floorf( z / grid );
	
	if( z - integral < halfGrid ){
		z = integral;
		
	}else{
		z = integral + grid;
	}
}

bool decVector::IsEqualTo( const decVector &v, float threshold ) const{
	return fabsf( x - v.x ) < threshold && fabsf( y - v.y ) < threshold && fabsf( z - v.z ) < threshold;
}

bool decVector::IsZero( float threshold ) const{
	return fabsf( x ) < threshold && fabsf( y ) < threshold && fabsf( z ) < threshold;
}

void decVector::SetZero(){
	x = 0;
	y = 0;
	z = 0;
}

void decVector::Set( float nx, float ny, float nz ){
	x = nx;
	y = ny;
	z = nz;
}

void decVector::Set( const decVector &v ){
	x = v.x;
	y = v.y;
	z = v.z;
}

void decVector::Negate(){
	x = -x;
	y = -y;
	z = -z;
}

void decVector::SetSmallest( const decVector &v ){
	if( v.x < x ){
		x = v.x;
	}
	if( v.y < y ){
		y = v.y;
	}
	if( v.z < z ){
		z = v.z;
	}
}

decVector decVector::Smallest( const decVector &v ) const{
	return decVector(
		( x < v.x ) ? x : v.x,
		( y < v.y ) ? y : v.y,
		( z < v.z ) ? z : v.z
	);
}

void decVector::SetLargest( const decVector &v ){
	if( v.x > x ){
		x = v.x;
	}
	if( v.y > y ){
		y = v.y;
	}
	if( v.z > z ){
		z = v.z;
	}
}

decVector decVector::Largest( const decVector &v ) const{
	return decVector(
		( x > v.x ) ? x : v.x,
		( y > v.y ) ? y : v.y,
		( z > v.z ) ? z : v.z
	);
}

void decVector::SetClamped( const decVector &minValue, const decVector &maxValue ){
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
	
	if( z < minValue.z ){
		z = minValue.z;
		
	}else if( z > maxValue.z ){
		z = maxValue.z;
	}
}

decVector decVector::Clamped( const decVector &minValue, const decVector &maxValue ) const{
	return decVector(
		( x < minValue.x ) ? minValue.x : ( ( x > maxValue.x ) ? maxValue.x : x ),
		( y < minValue.y ) ? minValue.y : ( ( y > maxValue.y ) ? maxValue.y : y ),
		( z < minValue.z ) ? minValue.z : ( ( z > maxValue.z ) ? maxValue.z : z )
	);
}

decPoint3 decVector::Round() const{
	return decPoint3(
		( int )( x < 0.0f ? x - 0.5f : x + 0.5f ),
		( int )( y < 0.0f ? y - 0.5f : y + 0.5f ),
		( int )( z < 0.0f ? z - 0.5f : z + 0.5f ) );
}

decVector decVector::Mix( const decVector &vector, float blendFactor ) const{
	const float factor1 = decMath::clamp( blendFactor, 0.0f, 1.0f );
	const float factor2 = 1.0f - factor1;
	return decVector(
		x * factor2 + vector.x * factor1,
		y * factor2 + vector.y * factor1,
		z * factor2 + vector.z * factor1 );
}



// Operators
//////////////

decVector decVector::operator-() const{
	return decVector( -x, -y, -z );
}

decVector &decVector::operator=( const decVector &v ){
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

decVector &decVector::operator+=( const decVector &v ){
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

decVector &decVector::operator-=( const decVector &v ){
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

decVector &decVector::operator*=( float k ){
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

decVector &decVector::operator/=( float k ){
	if( k == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	x /= k;
	y /= k;
	z /= k;
	return *this;
}

decVector decVector::operator+( const decVector &v ) const{
	return decVector( x + v.x, y + v.y, z + v.z );
}

decVector decVector::operator-( const decVector &v ) const{
	return decVector( x - v.x, y - v.y, z - v.z );
}

decVector decVector::operator*( float k ) const{
	return decVector( x * k, y * k, z * k );
}

float decVector::operator*( const decVector &v ) const{
	return x * v.x + y * v.y + z * v.z;
}

decVector decVector::operator/( float k ) const{
	if( k == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	return decVector( x / k, y / k, z / k );
}

decVector decVector::operator%( const decVector &v ) const{
	return decVector( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
}

bool decVector::operator<( const decVector &v ) const{
	return x < v.x && y < v.y && z < v.z;
}

bool decVector::operator>( const decVector &v ) const{
	return x > v.x && y > v.y && z > v.z;
}

bool decVector::operator<=( const decVector &v ) const{
	return x <= v.x && y <= v.y && z <= v.z;
}

bool decVector::operator>=( const decVector &v ) const{
	return x >= v.x && y >= v.y && z >= v.z;
}
