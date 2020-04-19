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



// class decDVector4
//////////////////////

// Constructor
////////////////

decDVector4::decDVector4(){
	x = 0.0;
	y = 0.0;
	z = 0.0;
	w = 1.0;
}

decDVector4::decDVector4( double nx, double ny, double nz ){
	x = nx;
	y = ny;
	z = nz;
	w = 1.0;
}

decDVector4::decDVector4( double nx, double ny, double nz, double nw ){
	x = nx;
	y = ny;
	z = nz;
	w = nw;
}

decDVector4::decDVector4( const decDVector4 &v ){
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

decDVector4::decDVector4( const decVector4 &v ){
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}



// Management
///////////////

double decDVector4::Length(){
	return sqrt( x * x + y * y + z * z );
}

void decDVector4::Normalize(){
	const double len = sqrt( x * x + y * y + z * z );
	
	if( len == 0.0 ){
		DETHROW( deeDivisionByZero );
	}
	
	x /= len;
	y /= len;
	z /= len;
}

const decDVector4 decDVector4::Normalized() const{
	const double len = sqrt( x * x + y * y + z * z );
	
	if( len == 0.0 ){
		DETHROW( deeDivisionByZero );
	}
	
	return decDVector4( x / len, y / len, z / len );
}

const decDVector4 decDVector4::Absolute() const{
	return decDVector4( fabs( x ), fabs( y ), fabs( z ), fabs( w ) );
}

void decDVector4::SetZero(){
	x = 0.0;
	y = 0.0;
	z = 0.0;
	w = 1.0;
}

void decDVector4::Set( double nx, double ny, double nz, double nw ){
	x = nx;
	y = ny;
	z = nz;
	w = nw;
}

decVector4 decDVector4::ToVector4() const{
	return decVector4( ( double )x, ( double )y, ( double )z, ( double )w );
}



// Operators
//////////////

decDVector4 decDVector4::operator-(){
	return decDVector4( -x, -y, -z );
}

decDVector4 &decDVector4::operator=( const decDVector4 &v ){
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

decDVector4 &decDVector4::operator+=( const decDVector4 &v ){
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

decDVector4 &decDVector4::operator-=( const decDVector4 &v ){
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

decDVector4 &decDVector4::operator*=( double k ){
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

decDVector4 &decDVector4::operator/=(double k){
	if( k == 0.0 ){
		DETHROW( deeDivisionByZero );
	}
	
	x /= k;
	y /= k;
	z /= k;
	
	return *this;
}

decDVector4 decDVector4::operator+( const decDVector4 &v ) const{
	return decDVector4( x + v.x, y + v.y, z + v.z );
}

decDVector4 decDVector4::operator-( const decDVector4 &v ) const{
	return decDVector4( x - v.x, y - v.y, z - v.z );
}

decDVector4 decDVector4::operator*( double k ) const{
	return decDVector4( x * k, y * k, z * k );
}

double decDVector4::operator*( const decDVector4 &v ) const{
	return x * v.x + y * v.y + z * v.z;
}

decDVector4 decDVector4::operator/( double k ) const{
	if( k == 0.0 ){
		DETHROW( deeDivisionByZero );
	}
	return decDVector4( x / k, y / k, z / k );
}

decDVector4 decDVector4::operator%( const decDVector4 &v ) const{
	return decDVector4( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
}

bool decDVector4::operator<( const decDVector4 &v ) const{
	return x < v.x && y < v.y && z < v.z;
}

bool decDVector4::operator>( const decDVector4 &v ) const{
	return x > v.x && y > v.y && z > v.z;
}

bool decDVector4::operator<=( const decDVector4 &v ) const{
	return x <= v.x && y <= v.y && z <= v.z;
}

bool decDVector4::operator>=( const decDVector4 &v ) const{
	return x >= v.x && y >= v.y && z >= v.z;
}
