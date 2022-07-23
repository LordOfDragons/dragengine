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



// Class decPoint3
////////////////////

// Constructor
////////////////

decPoint3::decPoint3(){
	x = 0;
	y = 0;
	z = 0;
}

decPoint3::decPoint3( int nx, int ny, int nz ){
	x = nx;
	y = ny;
	z = nz;
}

decPoint3::decPoint3( const decPoint3 &p ){
	x = p.x;
	y = p.y;
	z = p.z;
}

decPoint3::decPoint3( const decPoint &p, int nz ){
	x = p.x;
	y = p.y;
	z = nz;
}

decPoint3::decPoint3( const decVector &p ){
	x = ( int )p.x;
	y = ( int )p.y;
	z = ( int )p.z;
}

decPoint3::decPoint3( const decDVector &p ){
	x = ( int )p.x;
	y = ( int )p.y;
	z = ( int )p.z;
}



// Management
///////////////

float decPoint3::Length() const{
	return sqrtf( ( float )( x * x + y * y + z * z ) );
}

void decPoint3::SetZero(){
	x = 0;
	y = 0;
	z = 0;
}

void decPoint3::Set( int nx, int ny, int nz ){
	x = nx;
	y = ny;
	z = nz;
}

decPoint3 decPoint3::Absolute() const{
	return decPoint3( x < 0 ? -x : x, y < 0 ? -y : y, z < 0 ? -z : z );
}

void decPoint3::SetSmallest( const decPoint3 &p ){
	if( p.x < x ){
		x = p.x;
	}
	if( p.y < y ){
		y = p.y;
	}
	if( p.z < z ){
		z = p.z;
	}
}

decPoint3 decPoint3::Smallest( const decPoint3 &p ) const{
	return decPoint3(
		decMath::min( x, p.x ),
		decMath::min( y, p.y ),
		decMath::min( z, p.z )
	);
}

void decPoint3::SetLargest( const decPoint3 &p ){
	if( p.x > x ){
		x = p.x;
	}
	if( p.y > y ){
		y = p.y;
	}
	if( p.z > z ){
		z = p.z;
	}
}

decPoint3 decPoint3::Largest( const decPoint3 &p ) const{
	return decPoint3(
		decMath::max( x, p.x ),
		decMath::max( y, p.y ),
		decMath::max( z, p.z )
	);
}

void decPoint3::SetClamped( const decPoint3 &minValue, const decPoint3 &maxValue ){
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

decPoint3 decPoint3::Clamped( const decPoint3 &minValue, const decPoint3 &maxValue ) const{
	return decPoint3(
		decMath::clamp( x, minValue.x, maxValue.x ),
		decMath::clamp( y, minValue.y, maxValue.y ),
		decMath::clamp( z, minValue.z, maxValue.z )
	);
}



// Operators
//////////////

decPoint3 decPoint3::operator-() const{
	return decPoint3( -x, -y, -z );
}

decPoint3 &decPoint3::operator=( const decPoint3 &p ){
	x = p.x;
	y = p.y;
	z = p.z;
	return *this;
}

decPoint3 &decPoint3::operator+=( const decPoint3 &p ){
	x += p.x;
	y += p.y;
	z += p.z;
	return *this;
}

decPoint3 &decPoint3::operator-=( const decPoint3 &p ){
	x -= p.x;
	y -= p.y;
	z -= p.z;
	return *this;
}

decPoint3 &decPoint3::operator*=( int k ){
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

decPoint3 &decPoint3::operator/=( int k ){
	if( k == 0 ){
		DETHROW( deeDivisionByZero );
	}
	x /= k;
	y /= k;
	z /= k;
	return *this;
}

decPoint3 decPoint3::operator+( const decPoint3 &p ) const{
	return decPoint3( x + p.x, y + p.y, z + p.z );
}

decPoint3 decPoint3::operator-( const decPoint3 &p ) const{
	return decPoint3( x - p.x, y - p.y, z - p.z );
}

decPoint3 decPoint3::operator*( int k ) const{
	return decPoint3( x * k, y * k, z * k );
}

int decPoint3::operator*( const decPoint3 &p ) const{
	return x * p.x + y * p.y + z * p.z;
}

decPoint3 decPoint3::operator/( int k ) const{
	if( k == 0 ){
		DETHROW( deeDivisionByZero );
	}
	return decPoint3( x / k, y / k, z / k );
}

bool decPoint3::operator==( const decPoint3 &p ) const{
	return x == p.x && y == p.y && z == p.z;
}

bool decPoint3::operator!=( const decPoint3 &p ) const{
	return x != p.x || y != p.y || z != p.z;
}

bool decPoint3::operator<( const decPoint3 &p ) const{
	return x < p.x && y < p.y && z < p.z;
}

bool decPoint3::operator>( const decPoint3 &p ) const{
	return x > p.x && y > p.y && z > p.z;
}

bool decPoint3::operator<=( const decPoint3 &p ) const{
	return x <= p.x && y <= p.y && z <= p.z;
}

bool decPoint3::operator>=( const decPoint3 &p ) const{
	return x >= p.x && y >= p.y && z >= p.z;
}
