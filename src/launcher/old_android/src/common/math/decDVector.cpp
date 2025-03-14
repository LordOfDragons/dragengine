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



// Class decDVector
/////////////////////

// Constructor
////////////////

decDVector::decDVector(){
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

decDVector::decDVector( double nx, double ny, double nz ){
	x = nx;
	y = ny;
	z = nz;
}

decDVector::decDVector( const decDVector &v ){
	x = v.x;
	y = v.y;
	z = v.z;
}

decDVector::decDVector( const decVector &v ){
	x = v.x;
	y = v.y;
	z = v.z;
}



// Management
///////////////

double decDVector::Length() const{
	return sqrt( x * x + y * y + z * z );
}

double decDVector::LengthSquared() const{
	return x * x + y * y + z * z;
}

void decDVector::Normalize(){
	const double len = sqrt( x * x + y * y + z * z );
	
	if( len == 0.0 ){
		DETHROW( deeDivisionByZero );
	}
	
	x /= len;
	y /= len;
	z /= len;
}

const decDVector decDVector::Normalized() const{
	const double len = sqrt( x * x + y * y + z * z );
	
	if( len == 0.0 ){
		DETHROW( deeDivisionByZero );
	}
	
	return decDVector( x / len, y / len, z / len );
}

const decDVector decDVector::Absolute() const{
	return decDVector( fabs( x ), fabs( y ), fabs( z ) );
}

void decDVector::Snap( double grid ){
	if( grid == 0.0 ){
		DETHROW( deeDivisionByZero );
	}
	
	const double halfGrid = grid * 0.5;
	double integral;
	
	// x
	integral = grid * floor( x / grid );
	
	if( x - integral < halfGrid ){
		x = integral;
		
	}else{
		x = integral + grid;
	}
	
	// y
	integral = grid * floor( y / grid );
	
	if( y - integral < halfGrid ){
		y = integral;
		
	}else{
		y = integral + grid;
	}
	
	// z
	integral = grid * floor( z / grid );
	
	if( z - integral < halfGrid ){
		z = integral;
	}else{
		z = integral + grid;
	}
}

decVector decDVector::ToVector() const{
	return decVector( ( float )x, ( float )y, ( float )z );
}

bool decDVector::IsEqualTo( const decDVector &v, double threshold ) const{
	return fabs( x - v.x ) < threshold && fabs( y - v.y ) < threshold && fabs( z - v.z ) < threshold;
}

bool decDVector::IsZero( double threshold ) const{
	return fabs( x ) < threshold && fabs( y ) < threshold && fabs( z ) < threshold;
}

void decDVector::SetZero(){
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

void decDVector::Set( double nx, double ny, double nz ){
	x = nx;
	y = ny;
	z = nz;
}

void decDVector::Set( const decDVector &v ){
	x = v.x;
	y = v.y;
	z = v.z;
}

void decDVector::Set( const decVector &v ){
	x = v.x;
	y = v.y;
	z = v.z;
}

void decDVector::Negate(){
	x = -x;
	y = -y;
	z = -z;
}

void decDVector::SetSmallest( const decDVector &v ){
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

decDVector decDVector::Smallest( const decDVector &v ) const{
	return decDVector(
		( x < v.x ) ? x : v.x,
		( y < v.y ) ? y : v.y,
		( z < v.z ) ? z : v.z
	);
}

void decDVector::SetLargest( const decDVector &v ){
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

decDVector decDVector::Largest( const decDVector &v ) const{
	return decDVector(
		( x > v.x ) ? x : v.x,
		( y > v.y ) ? y : v.y,
		( z > v.z ) ? z : v.z
	);
}

void decDVector::SetClamped( const decDVector &minValue, const decDVector &maxValue ){
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

decDVector decDVector::Clamped( const decDVector &minValue, const decDVector &maxValue ) const{
	return decDVector(
		( x < minValue.x ) ? minValue.x : ( ( x > maxValue.x ) ? maxValue.x : x ),
		( y < minValue.y ) ? minValue.y : ( ( y > maxValue.y ) ? maxValue.y : y ),
		( z < minValue.z ) ? minValue.z : ( ( z > maxValue.z ) ? maxValue.z : z )
	);
}




// Operators
//////////////

decDVector decDVector::operator-() const{
	return decDVector( -x, -y, -z );
}

decDVector &decDVector::operator=( const decDVector &v ){
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

decDVector &decDVector::operator+=( const decDVector &v ){
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

decDVector &decDVector::operator-=( const decDVector &v ){
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

decDVector &decDVector::operator*=( double k ){
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

decDVector &decDVector::operator/=( double k ){
	if( k == 0.0 ){
		DETHROW( deeDivisionByZero );
	}
	
	x /= k;
	y /= k;
	z /= k;
	
	return *this;
}

decDVector decDVector::operator+( const decDVector &v ) const{
	return decDVector( x + v.x, y + v.y, z + v.z );
}

decDVector decDVector::operator-( const decDVector &v ) const{
	return decDVector( x - v.x, y - v.y, z - v.z );
}

decDVector decDVector::operator*( double k ) const{
	return decDVector( x * k, y * k, z * k );
}

double decDVector::operator*( const decDVector &v ) const{
	return x * v.x + y * v.y + z * v.z;
}

decDVector decDVector::operator/( double k ) const{
	if( k == 0.0 ){
		DETHROW( deeDivisionByZero );
	}
	return decDVector( x / k, y / k, z / k );
}

decDVector decDVector::operator%( const decDVector &v ) const{
	return decDVector( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
}

bool decDVector::operator<( const decDVector &v ) const{
	return x < v.x && y < v.y && z < v.z;
}

bool decDVector::operator>( const decDVector &v ) const{
	return x > v.x && y > v.y && z > v.z;
}

bool decDVector::operator<=( const decDVector &v ) const{
	return x <= v.x && y <= v.y && z <= v.z;
}

bool decDVector::operator>=( const decDVector &v ) const{
	return x >= v.x && y >= v.y && z >= v.z;
}
