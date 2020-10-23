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



// class decVector4
/////////////////////

// Constructor
////////////////

decVector4::decVector4(){
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

decVector4::decVector4( float nx, float ny, float nz ){
	x = nx;
	y = ny;
	z = nz;
	w = 1.0f;
}

decVector4::decVector4( float nx, float ny, float nz, float nw ){
	x = nx;
	y = ny;
	z = nz;
	w = nw;
}

decVector4::decVector4( const decVector4 &v ){
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}



// Management
///////////////

float decVector4::Length(){
	return sqrtf( x * x + y * y + z * z );
}

void decVector4::Normalize(){
	const float len = sqrtf( x * x + y * y + z * z );
	
	if( len == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	x /= len;
	y /= len;
	z /= len;
}

const decVector4 decVector4::Normalized() const{
	const float len = sqrtf( x * x + y * y + z * z );
	
	if( len == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	return decVector4( x / len, y / len, z / len );
}

const decVector4 decVector4::Absolute() const{
	return decVector4( fabsf( x ), fabsf( y ), fabsf( z ), fabsf( w ) );
}

void decVector4::SetZero(){
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

void decVector4::Set( float nx, float ny, float nz, float nw ){
	x = nx;
	y = ny;
	z = nz;
	w = nw;
}

decVector4 decVector4::Mix (const decVector4 &vector, float blendFactor ) const{
	const float factor1 = decMath::clamp( blendFactor, 0.0f, 1.0f );
	const float factor2 = 1.0f - factor1;
	return decVector4(
		x * factor2 + vector.x * factor1,
		y * factor2 + vector.y * factor1,
		z * factor2 + vector.z * factor1,
		w * factor2 + vector.w * factor1 );
}

decVector4 decVector4::Multiply( const decVector4 &vector ) const{
	return decVector4( x * vector.x, y * vector.y, z * vector.z, w * vector.w );
}



// Operators
//////////////

decVector4 decVector4::operator-(){
	return decVector4( -x, -y, -z );
}

decVector4 &decVector4::operator=( const decVector4 &v ){
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

decVector4 &decVector4::operator+=( const decVector4 &v ){
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

decVector4 &decVector4::operator-=( const decVector4 &v ){
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

decVector4 &decVector4::operator*=( float k ){
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

decVector4 &decVector4::operator/=( float k ){
	if( k == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	x /= k;
	y /= k;
	z /= k;
	return *this;
}

decVector4 decVector4::operator+( const decVector4 &v ) const{
	return decVector4( x + v.x, y + v.y, z + v.z );
}

decVector4 decVector4::operator-( const decVector4 &v ) const{
	return decVector4( x - v.x, y - v.y, z - v.z );
}

decVector4 decVector4::operator*( float k ) const{
	return decVector4( x * k, y * k, z * k );
}

float decVector4::operator*( const decVector4 &v ) const{
	return x * v.x + y * v.y + z * v.z;
}

decVector4 decVector4::operator/( float k ) const{
	if( k == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	return decVector4( x / k, y / k, z / k );
}

decVector4 decVector4::operator%( const decVector4 &v ) const{
	return decVector4( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
}

bool decVector4::operator<( const decVector4 &v ) const{
	return x < v.x && y < v.y && z < v.z;
}

bool decVector4::operator>( const decVector4 &v ) const{
	return x > v.x && y > v.y && z > v.z;
}

bool decVector4::operator<=( const decVector4 &v ) const{
	return x <= v.x && y <= v.y && z <= v.z;
}

bool decVector4::operator>=( const decVector4 &v ) const{
	return x >= v.x && y >= v.y && z >= v.z;
}
