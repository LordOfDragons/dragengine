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



// Class decPoint
///////////////////

// Constructor
////////////////

decPoint::decPoint() :
x( 0 ),
y( 0 ){
}

decPoint::decPoint( int nx, int ny ) :
x( nx ),
y( ny ){
}

decPoint::decPoint( const decPoint &p ) :
x( p.x ),
y( p.y ){
}

decPoint::decPoint( const decVector2 &v ) :
x( ( int )v.x ),
y( ( int )v.y ){
}



// Management
///////////////

float decPoint::Length() const{
	return sqrtf( ( float )( x * x + y * y ) );
}

void decPoint::SetZero(){
	x = 0;
	y = 0;
}

void decPoint::Set( int nx, int ny ){
	x = nx;
	y = ny;
}

decPoint decPoint::Absolute() const{
	return decPoint( x < 0 ? -x : x, y < 0 ? -y : y );
}

void decPoint::SetSmallest( const decPoint &p ){
	if( p.x < x ){
		x = p.x;
	}
	if( p.y < y ){
		y = p.y;
	}
}

decPoint decPoint::Smallest( const decPoint &p ) const{
	return decPoint(
		( x < p.x ) ? x : p.x,
		( y < p.y ) ? y : p.y
	);
}

void decPoint::SetLargest( const decPoint &p ){
	if( p.x > x ){
		x = p.x;
	}
	if( p.y > y ){
		y = p.y;
	}
}

decPoint decPoint::Largest( const decPoint &p ) const{
	return decPoint(
		( x > p.x ) ? x : p.x,
		( y > p.y ) ? y : p.y
	);
}

void decPoint::SetClamped( const decPoint &minValue, const decPoint &maxValue ){
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

decPoint decPoint::Clamped( const decPoint &minValue, const decPoint &maxValue ) const{
	return decPoint(
		( x < minValue.x ) ? minValue.x : ( ( x > maxValue.x ) ? maxValue.x : x ),
		( y < minValue.y ) ? minValue.y : ( ( y > maxValue.y ) ? maxValue.y : y )
	);
}



// Operators
//////////////

decPoint decPoint::operator-() const{
	return decPoint( -x, -y );
}

decPoint &decPoint::operator=( const decPoint &p ){
	x = p.x;
	y = p.y;
	return *this;
}

decPoint &decPoint::operator+=( const decPoint &p ){
	x += p.x;
	y += p.y;
	return *this;
}

decPoint &decPoint::operator-=( const decPoint &p ){
	x -= p.x;
	y -= p.y;
	return *this;
}

decPoint &decPoint::operator*=( int k ){
	x = x * k;
	y = x * k;
	return *this;
}

decPoint &decPoint::operator/=( int k ){
	if( k == 0 ){
		DETHROW( deeDivisionByZero );
	}
	x /= k;
	y /= k;
	return *this;
}

decPoint decPoint::operator+( const decPoint &p ) const{
	return decPoint( x + p.x, y + p.y );
}

decPoint decPoint::operator-( const decPoint &p ) const{
	return decPoint( x - p.x, y - p.y );
}

decPoint decPoint::operator*( int k ) const{
	return decPoint( x * k, y * k );
}

int decPoint::operator*( const decPoint &p ) const{
	return x * p.x + y * p.y;
}

decPoint decPoint::operator/( int k ) const{
	if( k == 0 ){
		DETHROW( deeDivisionByZero );
	}
	return decPoint( x / k, y / k );
}

bool decPoint::operator==( const decPoint &p ) const{
	return x == p.x && y == p.y;
}

bool decPoint::operator!=( const decPoint &p ) const{
	return x != p.x || y != p.y;
}

bool decPoint::operator<( const decPoint &p ) const{
	return x < p.x && y < p.y;
}

bool decPoint::operator>( const decPoint &p ) const{
	return x > p.x && y > p.y;
}

bool decPoint::operator<=( const decPoint &p ) const{
	return x <= p.x && y <= p.y;
}

bool decPoint::operator>=( const decPoint &p ) const{
	return x >= p.x && y >= p.y;
}
