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
	return sqrtf( x * x + y * y + z * z );
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

const decPoint3 decPoint3::Absolute() const{
	return decPoint3( x < 0 ? -x : x, y < 0 ? -y : y, z < 0 ? -z : z );
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
