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



// Class decDVector
/////////////////////

// Constructor
////////////////

decDVector::decDVector() :
x( 0.0 ),
y( 0.0 ),
z( 0.0 ){
}

decDVector::decDVector( double nx, double ny, double nz ) :
x( nx ),
y( ny ),
z( nz ){
}

decDVector::decDVector( const decDVector &v ) :
x( v.x ),
y( v.y ),
z( v.z ){
}

decDVector::decDVector( const decVector &v ) :
x( ( double )v.x ),
y( ( double )v.y ),
z( ( double )v.z ){
}

decDVector::decDVector( const decPoint3 &p ) :
x( ( int )p.x ),
y( ( int )p.y ),
z( ( int )p.z ){
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

decPoint3 decDVector::Round() const{
	return decPoint3(
		( int )( x < 0.0 ? x - 0.5 : x + 0.5 ),
		( int )( y < 0.0 ? y - 0.5 : y + 0.5 ),
		( int )( z < 0.0 ? z - 0.5 : z + 0.5 ) );
}

decDVector decDVector::Mix( const decDVector &vector, double blendFactor ) const{
	const double factor1 = decMath::clamp( blendFactor, 0.0, 1.0 );
	const double factor2 = 1.0 - factor1;
	return decDVector(
		x * factor2 + vector.x * factor1,
		y * factor2 + vector.y * factor1,
		z * factor2 + vector.z * factor1 );
}

decDVector decDVector::Multiply( const decDVector &vector ) const{
	return decDVector( x * vector.x, y * vector.y, z * vector.z );
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
