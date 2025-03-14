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
