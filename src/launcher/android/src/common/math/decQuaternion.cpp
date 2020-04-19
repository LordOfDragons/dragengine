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



// Class decQuaternion
////////////////////////

// Constructor
////////////////

decQuaternion::decQuaternion() :
x( 0.0f ),
y( 0.0f ),
z( 0.0f ),
w( 1.0f ){
}

decQuaternion::decQuaternion( float nx, float ny, float nz, float nw ) :
x( nx ),
y( ny ),
z( nz ),
w( nw ){
}

decQuaternion::decQuaternion( const decQuaternion &q ) :
x( q.x ),
y( q.y ),
z( q.z ),
w( q.w ){
}



// Quaternion creation
////////////////////////

decQuaternion decQuaternion::CreateFromEuler( float rx, float ry, float rz ){
	decQuaternion q;
	q.SetFromEuler( rx, ry, rz );
	return q;
}

decQuaternion decQuaternion::CreateFromEuler( const decVector &angles ){
	decQuaternion q;
	q.SetFromEuler( angles );
	return q;
}

decQuaternion decQuaternion::CreateFromEulerX( float angle ){
	decQuaternion q;
	q.SetFromEulerX( angle );
	return q;
}

decQuaternion decQuaternion::CreateFromEulerY( float angle ){
	decQuaternion q;
	q.SetFromEulerY( angle );
	return q;
}

decQuaternion decQuaternion::CreateFromEulerZ( float angle ){
	decQuaternion q;
	q.SetFromEulerZ( angle );
	return q;
}



// Management
///////////////

float decQuaternion::Length() const{
	return sqrtf( x * x + y * y + z * z + w * w );
}

void decQuaternion::Normalize(){
	const float len = sqrtf( x * x + y * y + z * z + w * w );
	
	if( len == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	const float invLen = 1.0f / len;
	x *= invLen;
	y *= invLen;
	z *= invLen;
	w *= invLen;
}

decQuaternion decQuaternion::Normalized() const{
	const float len = sqrtf( x * x + y * y + z * z + w * w );
	
	if( len == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	const float invLen = 1.0f / len;
	return decQuaternion( x * invLen, y * invLen, z * invLen, w * invLen );
}

decQuaternion decQuaternion::Conjugate() const{
	return decQuaternion( -x, -y, -z, w );
}

const decQuaternion decQuaternion::Absolute() const{
	return decQuaternion( fabsf( x ), fabsf( y ), fabsf( z ), fabsf( w ) );
}

float decQuaternion::Dot( const decQuaternion &q ) const{
	return x * q.x + y * q.y + z * q.z + w * q.w;
}

const decVector decQuaternion::GetEulerAngles() const{
	const float sqnx = x * x;
	const float sqny = y * y;
	const float sqnz = z * z;
	const float sqnw = w * w;
	
	const float a11 =  sqnx - sqny - sqnz + sqnw;
	const float a22 = -sqnx + sqny - sqnz + sqnw;
	const float a33 = -sqnx - sqny + sqnz + sqnw;
	
	const float t1 = x * y;
	const float t2 = z * w;
	const float a21 = 2.0f * ( t1 + t2 );
	
	const float t3 = x * z;
	const float t4 = y * w;
	const float a31 = 2.0f * ( t3 - t4 );
	const float a13 = 2.0f * ( t3 + t4 );
	
	const float a23 = 2.0f * ( y * z - x * w );
	
	if( a23 < 1.0f ){
		if( a23 > -1.0f ){
			return decVector( asinf( a23 ), atan2f( -a13, a33 ), atan2f( -a21, a22 ) );
			
		}else{ // undefined
			return decVector( -PI * 0.5f, 0.0f, -atan2f( a31, a11 ) );
		}
		
	}else{ // undefined
		return decVector( PI * 0.5f, 0.0f, atan2f( a31, a11 ) );
	}
}

decQuaternion decQuaternion::Slerp( const decQuaternion &other, float factor ) const{
	float omega, cosom, sinom, scale0, scale1;
	float qx, qy, qz, qw;
	
	cosom = x * other.x + y * other.y + z * other.z + w * other.w;
	
	if( cosom < 0.0f ){
		cosom = -cosom;
		qx = -other.x;
		qy = -other.y;
		qz = -other.z;
		qw = -other.w;
		
	}else{
		qx = other.x;
		qy = other.y;
		qz = other.z;
		qw = other.w;
	}
	
	if( ( 1.0f - cosom ) > 0.001f ){
		omega = acosf( cosom );
		sinom = 1.0f / sinf( omega );
		scale0 = sinf( omega * ( 1.0f - factor ) ) * sinom;
		scale1 = sinf( omega * factor ) * sinom;
		
	}else{
		scale0 = 1.0f - factor;
		scale1 = factor;
	}
	
	return decQuaternion( x * scale0 + qx * scale1, y * scale0 + qy * scale1, z * scale0 + qz * scale1, w * scale0 + qw * scale1 );
}

bool decQuaternion::IsEqualTo( const decQuaternion &q, float threshold ) const{
	return fabs( x - q.x ) < threshold
		&& fabs( y - q.y ) < threshold
		&& fabs( z - q.z ) < threshold
		&& fabs( w - q.w ) < threshold;
}

bool decQuaternion::SameRotation( const decQuaternion &q, float threshold ) const{
	return IsEqualTo( q, threshold ) || IsEqualTo( -q, threshold );
}

void decQuaternion::SetZero(){
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

void decQuaternion::Set( float nx, float ny, float nz, float nw ){
	x = nx;
	y = ny;
	z = nz;
	w = nw;
}

void decQuaternion::Set( const decQuaternion &q ){
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
}



void decQuaternion::SetFromEuler( float rx, float ry, float rz ){
	const float a = sinf( rx );
	const float b = cosf( rx );
	const float c = sinf( ry );
	const float d = cosf( ry );
	const float e = sinf( rz );
	const float f = cosf( rz );
	const float g = d * f;
	const float h = c * f;
	const float i = c * e;
	const float j = d * e;
	
	const float a11 = g - a * i;
	const float a12 = a * h + j;
	const float a13 = -c * b;
	const float a21 = -b * e;
	const float a22 = b * f;
	const float a23 = a;
	const float a31 = h + a * j;
	const float a32 = -a * g + i;
	const float a33 = b * d;
	
	const float trace = a11 + a22 + a33 + 1.0f;
	
	if( trace > 0.0001f ) { // do not choose this too small or the numerical errors explode the result
		const float s = 0.5f / sqrtf( trace );
		x = ( a32 - a23 ) * s;
		y = ( a13 - a31 ) * s;
		z = ( a21 - a12 ) * s;
		w = 0.25f / s;
		
	}else if( a11 > a22 && a11 > a33 ){
		const float s = 0.5f / sqrtf( 1.0f + a11 - a22 - a33 );
		x = 0.25f / s;
		y = ( a12 + a21 ) * s;
		z = ( a13 + a31 ) * s;
		w = ( a23 - a32 ) * s;
		
	}else if( a22 > a33 ){
		const float s = 0.5f / sqrtf( 1.0f + a22 - a11 - a33 );
		x = ( a12 + a21 ) * s;
		y = 0.25f / s;
		z = ( a23 + a32 ) * s;
		w = ( a13 - a31 ) * s;
		
	}else{
		const float s = 0.5f / sqrtf( 1.0f + a33 - a11 - a22 );
		x = ( a13 + a31 ) * s;
		y = ( a23 + a32 ) * s;
		z = 0.25f / s;
		w = ( a12 - a21 ) * s;
	}
}

void decQuaternion::SetFromEuler( const decVector &angles ){
	SetFromEuler( angles.x, angles.y, angles.z );
}

void decQuaternion::SetFromEulerX( float angle ){
	const float s = sinf( angle );
	const float c = cosf( angle );
	const float trace = c + c + 2.0f;
	
	if( trace > 0.0001f ) { // do not choose this too small or the numerical errors explode the result
		const float t = sqrtf( trace );
		x = -s / t;
		y = 0.0f;
		z = 0.0f;
		w = 0.5f * t;
		
	}else{
		const float t = sqrtf( 2.0f - c - c ); // 4.0f - trace
		x = 0.5f * t;
		y = 0.0f;
		z = 0.0f;
		w = s / t;
	}
}

void decQuaternion::SetFromEulerY( float angle ){
	const float s = sinf( angle );
	const float c = cosf( angle );
	const float trace = c + c + 2.0f;
	
	if( trace > 0.0001f ) { // do not choose this too small or the numerical errors explode the result
		const float t = sqrtf( trace );
		x = 0.0f;
		y = -s / t;
		z = 0.0f;
		w = 0.5f * t;
		
	}else{
		const float t = sqrtf( 2.0f - c - c ); // 4.0f - trace
		x = 0.0f;
		y = 0.5f * t;
		z = 0.0f;
		w = -s / t;
	}
}

void decQuaternion::SetFromEulerZ( float angle ){
	const float s = sinf( angle );
	const float c = cosf( angle );
	const float trace = c + c + 2.0f;
	
	if( trace > 0.0001f ) { // do not choose this too small or the numerical errors explode the result
		const float t = sqrtf( trace );
		x = 0.0f;
		y = 0.0f;
		z = -s / t;
		w = 0.5f * t;
		
	}else{
		const float t = sqrtf( 2.0f - c - c ); // 4.0f - trace
		x = 0.0f;
		y = 0.0f;
		z = 0.5f * t;
		w = s / t;
	}
}



// Operators
//////////////

decQuaternion decQuaternion::operator-() const{
	return decQuaternion( -x, -y, -z, -w );
}

decQuaternion &decQuaternion::operator=( const decQuaternion &q ){
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
	return *this;
}

decQuaternion &decQuaternion::operator+=( const decQuaternion &q ){
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

decQuaternion &decQuaternion::operator-=( const decQuaternion &q ){
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}

decQuaternion &decQuaternion::operator*=( float k ){
	x *= k;
	y *= k;
	z *= k;
	w *= k;
	return *this;
}

decQuaternion &decQuaternion::operator*=( const decQuaternion &q ){
	const float nx = q.x * w + q.y * z - q.z * y + q.w * x;
	const float ny = -q.x * z + q.y * w + q.z * x + q.w * y;
	const float nz = q.x * y - q.y * x + q.z * w + q.w * z;
	const float nw = -q.x * x - q.y * y - q.z * z + q.w * w;
	
	x = nx;
	y = ny;
	z = nz;
	w = nw;
	
	return *this;
}

decQuaternion &decQuaternion::operator/=( float k ){
	if( k == 0.0 ){
		DETHROW( deeDivisionByZero );
	}
	
	x /= k;
	y /= k;
	z /= k;
	w /= k;
	
	return *this;
}

decQuaternion decQuaternion::operator+( const decQuaternion &q ) const{
	return decQuaternion( x + q.x, y + q.y, z + q.z, w + q.w );
}

decQuaternion decQuaternion::operator-( const decQuaternion &q ) const{
	return decQuaternion( x - q.x, y - q.y, z - q.z, w - q.w );
}

decQuaternion decQuaternion::operator*( float k ) const{
	return decQuaternion( x * k, y * k, z * k, w * k );
}

decQuaternion decQuaternion::operator*( const decQuaternion &q ) const{
	return decQuaternion(
			q.x * w + q.y * z - q.z * y + q.w * x,
		-q.x * z + q.y * w + q.z * x + q.w * y,
			q.x * y - q.y * x + q.z * w + q.w * z,
		-q.x * x - q.y * y - q.z * z + q.w * w );
}

decQuaternion decQuaternion::operator/( float k ) const{
	if( k == 0.0 ){
		DETHROW( deeDivisionByZero );
	}
	return decQuaternion( x / k, y / k, z / k, w / k );
}

decVector decQuaternion::operator*( const decVector &v ) const{
	const float tx = v.x * w - v.y * z + v.z * y;
	const float ty = v.x * z + v.y * w - v.z * x;
	const float tz = v.y * x + v.z * w - v.x * y;
	const float tw = v.x * x + v.y * y + v.z * z;
	
	return decVector( x * tw + y * tz - z * ty + w * tx,
		y * tw + z * tx + w * ty - x * tz,
		x * ty - y * tx + z * tw + w * tz );
}

decDVector decQuaternion::operator*( const decDVector &v ) const{
	const double tx = v.x * ( double )w - v.y * ( double )z + v.z * ( double )y;
	const double ty = v.x * ( double )z + v.y * ( double )w - v.z * ( double )x;
	const double tz = v.y * ( double )x + v.z * ( double )w - v.x * ( double )y;
	const double tw = v.x * ( double )x + v.y * ( double )y + v.z * ( double )z;
	
	return decVector( ( double )x * tw + ( double )y * tz - ( double )z * ty + ( double )w * tx,
		( double )y * tw + ( double )z * tx + ( double )w * ty - ( double )x * tz,
		( double )x * ty - ( double )y * tx + ( double )z * tw + ( double )w * tz );
}

bool decQuaternion::operator<( const decQuaternion &q ) const{
	return x < q.x && y < q.y && z < q.z && w < q.w;
}

bool decQuaternion::operator>( const decQuaternion &q ) const{
	return x > q.x && y > q.y && z > q.z && w > q.w;
}

bool decQuaternion::operator<=( const decQuaternion &q ) const{
	return x <= q.x && y <= q.y && z <= q.z && w <= q.w;
}

bool decQuaternion::operator>=( const decQuaternion &q ) const{
	return x >= q.x && y >= q.y && z >= q.z && w >= q.w;
}
