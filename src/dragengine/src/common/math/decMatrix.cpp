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



// Class decMatrix
///////////////////

// Constructor
///////////////

decMatrix::decMatrix() :
a11( 1.0f ), a12( 0.0f ), a13( 0.0f ), a14( 0.0f ),
a21( 0.0f ), a22( 1.0f ), a23( 0.0f ), a24( 0.0f ),
a31( 0.0f ), a32( 0.0f ), a33( 1.0f ), a34( 0.0f ),
a41( 0.0f ), a42( 0.0f ), a43( 0.0f ), a44( 1.0f ){
}

decMatrix::decMatrix( const decMatrix &m ) :
a11( m.a11 ), a12( m.a12 ), a13( m.a13 ), a14( m.a14 ),
a21( m.a21 ), a22( m.a22 ), a23( m.a23 ), a24( m.a24 ),
a31( m.a31 ), a32( m.a32 ), a33( m.a33 ), a34( m.a34 ),
a41( m.a41 ), a42( m.a42 ), a43( m.a43 ), a44( m.a44 ){
}

decMatrix::decMatrix( const decDMatrix &m ) :
a11( ( float )m.a11 ), a12( ( float )m.a12 ), a13( ( float )m.a13 ), a14( ( float )m.a14 ),
a21( ( float )m.a21 ), a22( ( float )m.a22 ), a23( ( float )m.a23 ), a24( ( float )m.a24 ),
a31( ( float )m.a31 ), a32( ( float )m.a32 ), a33( ( float )m.a33 ), a34( ( float )m.a34 ),
a41( ( float )m.a41 ), a42( ( float )m.a42 ), a43( ( float )m.a43 ), a44( ( float )m.a44 ){
}



// Creation
/////////////

decMatrix decMatrix::CreateIdentity(){
	return decMatrix();
}

decMatrix decMatrix::CreateTranslation( float x, float y, float z ){
	decMatrix m;
	m.SetTranslation( x, y, z );
	return m;
}

decMatrix decMatrix::CreateTranslation( const decVector &t ){
	decMatrix m;
	m.SetTranslation( t );
	return m;
}

decMatrix decMatrix::CreateScale( float sx, float sy, float sz ){
	decMatrix m;
	m.SetScale( sx, sy, sz );
	return m;
}

decMatrix decMatrix::CreateScale( const decVector &s ){
	decMatrix m;
	m.SetScale( s );
	return m;
}

decMatrix decMatrix::CreateRotationX( float arc ){
	decMatrix m;
	m.SetRotationX( arc );
	return m;
}

decMatrix decMatrix::CreateRotationY( float arc ){
	decMatrix m;
	m.SetRotationY( arc );
	return m;
}

decMatrix decMatrix::CreateRotationZ( float arc ){
	decMatrix m;
	m.SetRotationZ( arc );
	return m;
}

decMatrix decMatrix::CreateRotation( float rx, float ry, float rz ){
	decMatrix m;
	m.SetRotation( rx, ry, rz );
	return m;
}

decMatrix decMatrix::CreateRotation( const decVector &r ){
	decMatrix m;
	m.SetRotation( r );
	return m;
}

decMatrix decMatrix::CreateRotationAxis( const decVector &axis, float angle ){
	decMatrix m;
	m.SetRotationAxis( axis, angle );
	return m;
}

decMatrix decMatrix::CreateCamera( const decVector &pos, const decVector &view, const decVector &up ){
	decMatrix m;
	m.SetCamera( pos, view, up );
	return m;
}

decMatrix decMatrix::CreateWorld( const decVector &pos, const decVector &view, const decVector &up ){
	decMatrix m;
	m.SetWorld( pos, view, up );
	return m;
}

decMatrix decMatrix::CreateWorld( const decVector &pos, const decQuaternion &orientation ){
	decMatrix m;
	m.SetWorld( pos, orientation );
	return m;
}

decMatrix decMatrix::CreateWorld( const decVector &pos, const decQuaternion &orientation, const decVector &scale ){
	decMatrix m;
	m.SetWorld( pos, orientation, scale );
	return m;
}

decMatrix decMatrix::CreateProjection( int width, int height, float fov, float fovRatio, float znear, float zfar ){
	decMatrix m;
	m.SetProjection( width, height, fov, fovRatio, znear, zfar );
	return m;
}

decMatrix decMatrix::CreateProjectionInfinite( int width, int height, float fov, float fovRatio, float znear ){
	decMatrix m;
	m.SetProjectionInfinite( width, height, fov, fovRatio, znear );
	return m;
}

decMatrix decMatrix::CreateOrtho( int left, int right, int top, int bottom, int znear, int zfar ){
	decMatrix m;
	m.SetOrtho( left, right, top, bottom, znear, zfar );
	return m;
}

decMatrix decMatrix::CreateClipSpace(){
	decMatrix m;
	m.SetClipSpace();
	return m;
}

decMatrix decMatrix::CreateBiasMatrix(){
	decMatrix m;
	m.SetBiasMatrix();
	return m;
}

decMatrix decMatrix::CreateRT( const decVector &rot, const decVector &pos ){
	decMatrix m;
	m.SetRT( rot, pos );
	return m;
}

decMatrix decMatrix::CreateSRT( const decVector &scale, const decVector &rot, const decVector &pos ){
	decMatrix m;
	m.SetSRT( scale, rot, pos );
	return m;
}

decMatrix decMatrix::CreateSVUT( const decVector &scale, const decVector &view, const decVector &up, const decVector &pos ){
	decMatrix m;
	m.SetSVUT( scale, view, up, pos );
	return m;
}

decMatrix decMatrix::CreateVU( const decVector &view, const decVector &up ){
	decMatrix m;
	m.SetVU( view, up );
	return m;
}

decMatrix decMatrix::CreateFromQuaternion( const decQuaternion &q ){
	decMatrix m;
	m.SetFromQuaternion( q );
	return m;
}

decMatrix decMatrix::CreateFromQuaternion( const decQuaternion &q, const decVector &pivot ){
	decMatrix m;
	m.SetFromQuaternion( q, pivot );
	return m;
}



// Management
///////////////

void decMatrix::SetIdentity(){
	a11 = 1.0f; a12 = 0.0f; a13 = 0.0f; a14 = 0.0f;
	a21 = 0.0f; a22 = 1.0f; a23 = 0.0f; a24 = 0.0f;
	a31 = 0.0f; a32 = 0.0f; a33 = 1.0f; a34 = 0.0f;
	a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f;
}

void decMatrix::SetTranslation( float x, float y, float z ){
	a11 = 1.0f; a12 = 0.0f; a13 = 0.0f; a14 = x;
	a21 = 0.0f; a22 = 1.0f; a23 = 0.0f; a24 = y;
	a31 = 0.0f; a32 = 0.0f; a33 = 1.0f; a34 = z;
	a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f;
}

void decMatrix::SetTranslation( const decVector &t ){
	a11 = 1.0f; a12 = 0.0f; a13 = 0.0f; a14 = t.x;
	a21 = 0.0f; a22 = 1.0f; a23 = 0.0f; a24 = t.y;
	a31 = 0.0f; a32 = 0.0f; a33 = 1.0f; a34 = t.z;
	a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f;
}

void decMatrix::SetScale( float sx, float sy, float sz ){
	a11 = sx;   a12 = 0.0f; a13 = 0.0f; a14 = 0.0f;
	a21 = 0.0f; a22 = sy;   a23 = 0.0f; a24 = 0.0f;
	a31 = 0.0f; a32 = 0.0f; a33 = sz;   a34 = 0.0f;
	a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f;
}

void decMatrix::SetScale( const decVector &s ){
	a11 = s.x;  a12 = 0.0f; a13 = 0.0f; a14 = 0.0f;
	a21 = 0.0f; a22 = s.y;  a23 = 0.0f; a24 = 0.0f;
	a31 = 0.0f; a32 = 0.0f; a33 = s.z;  a34 = 0.0f;
	a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f;
}

void decMatrix::SetRotationX( float arc ){
	const float s = sinf( arc );
	const float c = cosf( arc );
	
	a11 = 1.0f; a12 = 0.0f; a13 = 0.0f; a14 = 0.0f;
	a21 = 0.0f; a22 = c;    a23 = s;    a24 = 0.0f;
	a31 = 0.0f; a32 = -s;   a33 = c;    a34 = 0.0f;
	a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f;
}

void decMatrix::SetRotationY( float arc ){
	const float s = sinf( arc );
	const float c = cosf( arc );
	
	a11 = c;    a12 = 0.0f; a13 = -s;   a14 = 0.0f;
	a21 = 0.0f; a22 = 1.0f; a23 = 0.0f; a24 = 0.0f;
	a31 = s;    a32 = 0.0f; a33 = c;    a34 = 0.0f;
	a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f;
}

void decMatrix::SetRotationZ( float arc ){
	const float s = sinf( arc );
	const float c = cosf( arc );
	
	a11 = c;    a12 = s;    a13 = 0.0f; a14 = 0.0f;
	a21 = -s;   a22 = c;    a23 = 0.0f; a24 = 0.0f;
	a31 = 0.0f; a32 = 0.0f; a33 = 1.0f; a34 = 0.0f;
	a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f;
}

void decMatrix::SetRotation( float rx, float ry, float rz ){
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
	
	a11 = g - a * i; a12 = a * h + j;  a13 = -c * b; a14 = 0.0f;
	a21 = -b * e;    a22 = b * f;      a23 = a;      a24 = 0.0f;
	a31 = h + a * j; a32 = -a * g + i; a33 = b * d;  a34 = 0.0f;
	a41 = 0.0f;      a42 = 0.0f;       a43 = 0.0f;   a44 = 1.0f;
}

void decMatrix::SetRotation( const decVector &r ){
	const float a = sinf( r.x );
	const float b = cosf( r.x );
	const float c = sinf( r.y );
	const float d = cosf( r.y );
	const float e = sinf( r.z );
	const float f = cosf( r.z );
	const float g = d * f;
	const float h = c * f;
	const float i = c * e;
	const float j = d * e;
	
	a11 = g - a * i; a12 = a * h + j;  a13 = -c * b; a14 = 0.0f;
	a21 = -b * e;    a22 = b * f;      a23 = a;      a24 = 0.0f;
	a31 = h + a * j; a32 = -a * g + i; a33 = b * d;  a34 = 0.0f;
	a41 = 0.0f;      a42 = 0.0f;       a43 = 0.0f;   a44 = 1.0f;
}

void decMatrix::SetRotationAxis( const decVector &axis, float angle ){
	const decVector v = axis.Normalized();
	const float s = sinf( angle );
	const float c = cosf( angle );
	const float c1 = 1.0f - c;
	
	a11 = v.x * v.x * c1 + c;
	a12 = v.x * v.y * c1 + v.z * s;
	a13 = v.x * v.z * c1 - v.y * s;
	a14 = 0.0f;
	
	a21 = v.x * v.y * c1 - v.z * s;
	a22 = v.y * v.y * c1 + c;
	a23 = v.y * v.z * c1 + v.x * s;
	a24 = 0.0f;
	
	a31 = v.x * v.z * c1 + v.y * s;
	a32 = v.y * v.z * c1 - v.x * s;
	a33 = v.z * v.z * c1 + c;
	a34 = 0.0f;
	
	a41 = 0.0f;
	a42 = 0.0f;
	a43 = 0.0f;
	a44 = 1.0f;
}

void decMatrix::SetCamera( const decVector &pos, const decVector &view, const decVector &up ){
	const decVector dir = view.Normalized();
	const float dot = up * dir;
	const decVector nup = ( up - dir * dot ).Normalized();
	const decVector right = nup % dir;
	
	a11 = right.x; a12 = right.y; a13 = right.z; a14 = -( pos * right );
	a21 = nup.x;   a22 = nup.y;   a23 = nup.z;   a24 = -( pos * nup );
	a31 = dir.x;   a32 = dir.y;   a33 = dir.z;   a34 = -( pos * dir );
	a41 = 0.0f;    a42 = 0.0f;    a43 = 0.0f;    a44 = 1.0f;
}

void decMatrix::SetWorld( const decVector &pos, const decVector &view, const decVector &up ){
	const decVector dir = view.Normalized();
	const decVector nup = up.Normalized();
	const decVector right = ( nup % dir ).Normalized();
	const decVector nup2 = dir % right;
	
	a11 = right.x; a12 = nup2.x; a13 = dir.x; a14 = pos.x;
	a21 = right.y; a22 = nup2.y; a23 = dir.y; a24 = pos.y;
	a31 = right.z; a32 = nup2.z; a33 = dir.z; a34 = pos.z;
	a41 = 0.0f;    a42 = 0.0f;   a43 = 0.0f;  a44 = 1.0f;
}

void decMatrix::SetWorld( const decVector &pos, const decQuaternion &orientation ){
	const float sqw = orientation.w * orientation.w;
	const float sqx = orientation.x * orientation.x;
	const float sqy = orientation.y * orientation.y;
	const float sqz = orientation.z * orientation.z;
	
	const float inv = 1.0f / ( sqx + sqy + sqz + sqw );
	a11 = (  sqx - sqy - sqz + sqw ) * inv;
	a22 = ( -sqx + sqy - sqz + sqw ) * inv;
	a33 = ( -sqx - sqy + sqz + sqw ) * inv;
	
	const float t1 = orientation.x * orientation.y;
	const float t2 = orientation.z * orientation.w;
	a21 = 2.0f * ( t1 + t2 ) * inv;
	a12 = 2.0f * ( t1 - t2 ) * inv;
	
	const float t3 = orientation.x * orientation.z;
	const float t4 = orientation.y * orientation.w;
	a31 = 2.0f * ( t3 - t4 ) * inv;
	a13 = 2.0f * ( t3 + t4 ) * inv;
	
	const float t5 = orientation.y * orientation.z;
	const float t6 = orientation.x * orientation.w;
	a32 = 2.0f * ( t5 + t6 ) * inv;
	a23 = 2.0f * ( t5 - t6 ) * inv;
	
	a14 = pos.x;
	a24 = pos.y;
	a34 = pos.z;
	a41 = 0.0f;
	a42 = 0.0f;
	a43 = 0.0f;
	a44 = 1.0f;
}

void decMatrix::SetWorld( const decVector &pos, const decQuaternion &orientation, const decVector &scale ){
	const float sqw = orientation.w * orientation.w;
	const float sqx = orientation.x * orientation.x;
	const float sqy = orientation.y * orientation.y;
	const float sqz = orientation.z * orientation.z;
	
	const float inv = 1.0f / ( sqx + sqy + sqz + sqw );
	const float invX = inv * scale.x;
	const float invY = inv * scale.y;
	const float invZ = inv * scale.z;
	a11 = (  sqx - sqy - sqz + sqw ) * invX;
	a22 = ( -sqx + sqy - sqz + sqw ) * invY;
	a33 = ( -sqx - sqy + sqz + sqw ) * invZ;
	
	const float t1 = orientation.x * orientation.y;
	const float t2 = orientation.z * orientation.w;
	const float invX2 = invX * 2.0f;
	const float invY2 = invY * 2.0f;
	a21 = ( t1 + t2 ) * invX2;
	a12 = ( t1 - t2 ) * invY2;
	
	const float t3 = orientation.x * orientation.z;
	const float t4 = orientation.y * orientation.w;
	const float invZ2 = invZ * 2.0f;
	a31 = ( t3 - t4 ) * invX2;
	a13 = ( t3 + t4 ) * invZ2;
	
	const float t5 = orientation.y * orientation.z;
	const float t6 = orientation.x * orientation.w;
	a32 = ( t5 + t6 ) * invY2;
	a23 = ( t5 - t6 ) * invZ2;
	
	a14 = pos.x;
	a24 = pos.y;
	a34 = pos.z;
	a41 = 0.0f;
	a42 = 0.0f;
	a43 = 0.0f;
	a44 = 1.0f;
}

void decMatrix::SetProjection( int width, int height, float fov, float fovRatio, float znear, float zfar ){
	if( width < 1 || height < 1 || fov <= 0.0f || fov >= PI || fovRatio == 0.0f || znear >= zfar ){
		DETHROW( deeInvalidParam );
	}
	
	const float a = ( float )width / ( float )height;
	const float fx = 1.0f / tanf( fov * 0.5f );
	const float fy = a / tanf( fov * fovRatio * 0.5f );
	const float q = zfar / ( zfar - znear );
	
	a11 = fx;   a12 = 0.0f; a13 = 0.0f; a14 = 0.0f;
	a21 = 0.0f; a22 = fy;   a23 = 0.0f; a24 = 0.0f;
	a31 = 0.0f; a32 = 0.0f; a33 = q;    a34 = -q * znear;
	a41 = 0.0f; a42 = 0.0f; a43 = 1.0f; a44 = 0.0f;
}

void decMatrix::SetProjectionInfinite( int width, int height, float fov, float fovRatio, float znear ){
	if( width < 1 || height < 1 || fov <= 0.0f || fov >= PI || fovRatio == 0.0f ){
		DETHROW( deeInvalidParam );
	}
	
	const float a = ( float )width / ( float )height;
	const float fx = 1.0f / tanf( fov * 0.5f );
	const float fy = a / tanf( fov * fovRatio * 0.5f );
	const float e = 0.001f;
	
	a11 = fx;   a12 = 0.0f; a13 = 0.0f;     a14 = 0.0f;
	a21 = 0.0f; a22 = fy;   a23 = 0.0f;     a24 = 0.0f;
	a31 = 0.0f; a32 = 0.0f; a33 = 1.0f - e; a34 = znear * ( e - 2.0f );
	a41 = 0.0f; a42 = 0.0f; a43 = 1.0f;     a44 = 0.0f;
}

void decMatrix::SetOrtho( int left, int right, int top, int bottom, int znear, int zfar ){
	const float a = ( float )( right - left );
	const float b = ( float )( bottom - top );
	const float c = ( float )( zfar - znear );
	
	a11 = 2.0f / a; a12 = 0.0f;     a13 = 0.0f;     a14 = -( float )( right + left ) / a;
	a21 = 0.0f;     a22 = 2.0f / b; a23 = 0.0f;     a24 = -( float )( bottom + top ) / b;
	a31 = 0.0f;     a32 = 0.0f;     a33 = 2.0f / c; a34 = -( float )( zfar + znear ) / c;
	a41 = 0.0f;     a42 = 0.0f;     a43 = 0.0f;     a44 = 1.0f;
}

void decMatrix::SetClipSpace(){
	a11 = 0.5f; a12 = 0.0f; a13 = 0.0f; a14 = 0.0f;
	a21 = 0.0f; a22 = 0.5f; a23 = 0.0f; a24 = 0.0f;
	a31 = 0.0f; a32 = 0.0f; a33 = 1.0f; a34 = 0.0f;
	a41 = 0.5f; a42 = 0.5f; a43 = 0.0f; a44 = 1.0f;
}

void decMatrix::SetBiasMatrix(){
	a11 = 0.5f; a12 = 0.0f; a13 = 0.0f; a14 = 0.5f;
	a21 = 0.0f; a22 = 0.5f; a23 = 0.0f; a24 = 0.5f;
	a31 = 0.0f; a32 = 0.0f; a33 = 0.5f; a34 = 0.5f;
	a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f;
}

void decMatrix::SetRT( const decVector &rot, const decVector &pos ){
	const float a = sinf( rot.x );
	const float b = cosf( rot.x );
	const float c = sinf( rot.y );
	const float d = cosf( rot.y );
	const float e = sinf( rot.z );
	const float f = cosf( rot.z );
	const float g = d * f;
	const float h = c * f;
	const float i = c * e;
	const float j = d * e;
	
	a11 = g - a * i; a12 = a * h + j;  a13 = -c * b; a14 = pos.x;
	a21 = -b * e;    a22 = b * f;      a23 = a;      a24 = pos.y;
	a31 = h + a * j; a32 = -a * g + i; a33 = b * d;  a34 = pos.z;
	a41 = 0.0f;      a42 = 0.0f;       a43 = 0.0f;   a44 = 1.0f;
}

void decMatrix::SetSRT( const decVector &scale, const decVector &rot, const decVector &pos ){
	const float a = sinf( rot.x );
	const float b = cosf( rot.x );
	const float c = sinf( rot.y );
	const float d = cosf( rot.y );
	const float e = sinf( rot.z );
	const float f = cosf( rot.z );
	const float g = d * f;
	const float h = c * f;
	const float i = c * e;
	const float j = d * e;
	
	a11 = scale.x * ( g - a * i ); a12 = scale.y * ( a * h + j );  a13 = scale.z * ( -c * b ); a14 = pos.x;
	a21 = scale.x * ( -b * e );    a22 = scale.y * ( b * f );      a23 = scale.z * a;          a24 = pos.y;
	a31 = scale.x * ( h + a * j ); a32 = scale.y * ( -a * g + i ); a33 = scale.z * ( b * d );  a34 = pos.z;
	a41 = 0.0f;                    a42 = 0.0f;                     a43 = 0.0f;                 a44 = 1.0f;
}

void decMatrix::SetSVUT( const decVector &scale, const decVector &view, const decVector &up, const decVector &pos ){
	const decVector dir = view.Normalized();
	const decVector nup = up.Normalized();
	const decVector right = ( nup % dir ).Normalized();
	const decVector nup2 = dir % right;
	
	a11 = scale.x * right.x; a12 = scale.y * nup2.x; a13 = scale.z * dir.x; a14 = pos.x;
	a21 = scale.x * right.y; a22 = scale.y * nup2.y; a23 = scale.z * dir.y; a24 = pos.y;
	a31 = scale.x * right.z; a32 = scale.y * nup2.z; a33 = scale.z * dir.z; a34 = pos.z;
	a41 = 0.0f;              a42 = 0.0f;             a43 = 0.0f;            a44 = 1.0f;
}

void decMatrix::SetVU( const decVector &view, const decVector &up ){
	const decVector dir = view.Normalized();
	const decVector nup = up.Normalized();
	const decVector right = ( nup % dir ).Normalized();
	const decVector nup2 = dir % right;
	
	a11 = right.x; a12 = nup2.x; a13 = dir.x; a14 = 0.0f;
	a21 = right.y; a22 = nup2.y; a23 = dir.y; a24 = 0.0f;
	a31 = right.z; a32 = nup2.z; a33 = dir.z; a34 = 0.0f;
	a41 = 0.0f;    a42 = 0.0f;   a43 = 0.0f;  a44 = 1.0f;
}

void decMatrix::SetFromQuaternion( const decQuaternion &q ){
	const decQuaternion &qn = q.Normalized();
	
	const float sqnx = qn.x * qn.x;
	const float sqny = qn.y * qn.y;
	const float sqnz = qn.z * qn.z;
	const float sqnw = qn.w * qn.w;
	
	a11 =  sqnx - sqny - sqnz + sqnw;
	a22 = -sqnx + sqny - sqnz + sqnw;
	a33 = -sqnx - sqny + sqnz + sqnw;
	
	const float t1 = qn.x * qn.y;
	const float t2 = qn.z * qn.w;
	a21 = 2.0f * ( t1 + t2 );
	a12 = 2.0f * ( t1 - t2 );
	
	const float t3 = qn.x * qn.z;
	const float t4 = qn.y * qn.w;
	a31 = 2.0f * ( t3 - t4 );
	a13 = 2.0f * ( t3 + t4 );
	
	const float t5 = qn.y * qn.z;
	const float t6 = qn.x * qn.w;
	a32 = 2.0f * ( t5 + t6 );
	a23 = 2.0f * ( t5 - t6 );
	
	a14 = 0.0f;
	a24 = 0.0f;
	a34 = 0.0f;
	a41 = 0.0f;
	a42 = 0.0f;
	a43 = 0.0f;
	a44 = 1.0f;
}

void decMatrix::SetFromQuaternion( const decQuaternion &q, const decVector &pivot ){
	SetFromQuaternion( q );
	
	a14 = pivot.x - pivot.x * a11 - pivot.y * a12 - pivot.z * a13;
	a24 = pivot.y - pivot.x * a21 - pivot.y * a22 - pivot.z * a23;
	a34 = pivot.z - pivot.x * a31 - pivot.y * a32 - pivot.z * a33;
}

float decMatrix::Determinant() const{
	return
		a14*a23*a32*a41 - a13*a24*a32*a41 - a14*a22*a33*a41 + a12*a24*a33*a41 +
		a13*a22*a34*a41 - a12*a23*a34*a41 - a14*a23*a31*a42 + a13*a24*a31*a42 +
		a14*a21*a33*a42 - a11*a24*a33*a42 - a13*a21*a34*a42 + a11*a23*a34*a42 +
		a14*a22*a31*a43 - a12*a24*a31*a43 - a14*a21*a32*a43 + a11*a24*a32*a43 +
		a12*a21*a34*a43 - a11*a22*a34*a43 - a13*a22*a31*a44 + a12*a23*a31*a44 +
		a13*a21*a32*a44 - a11*a23*a32*a44 - a12*a21*a33*a44 + a11*a22*a33*a44;
}
decMatrix decMatrix::Invert() const{
	decMatrix m;
	
	m.a11 = a23*a34*a42 - a24*a33*a42 + a24*a32*a43 - a22*a34*a43 - a23*a32*a44 + a22*a33*a44;
	m.a12 = a14*a33*a42 - a13*a34*a42 - a14*a32*a43 + a12*a34*a43 + a13*a32*a44 - a12*a33*a44;
	m.a13 = a13*a24*a42 - a14*a23*a42 + a14*a22*a43 - a12*a24*a43 - a13*a22*a44 + a12*a23*a44;
	m.a14 = a14*a23*a32 - a13*a24*a32 - a14*a22*a33 + a12*a24*a33 + a13*a22*a34 - a12*a23*a34;
	m.a21 = a24*a33*a41 - a23*a34*a41 - a24*a31*a43 + a21*a34*a43 + a23*a31*a44 - a21*a33*a44;
	m.a22 = a13*a34*a41 - a14*a33*a41 + a14*a31*a43 - a11*a34*a43 - a13*a31*a44 + a11*a33*a44;
	m.a23 = a14*a23*a41 - a13*a24*a41 - a14*a21*a43 + a11*a24*a43 + a13*a21*a44 - a11*a23*a44;
	m.a24 = a13*a24*a31 - a14*a23*a31 + a14*a21*a33 - a11*a24*a33 - a13*a21*a34 + a11*a23*a34;
	m.a31 = a22*a34*a41 - a24*a32*a41 + a24*a31*a42 - a21*a34*a42 - a22*a31*a44 + a21*a32*a44;
	m.a32 = a14*a32*a41 - a12*a34*a41 - a14*a31*a42 + a11*a34*a42 + a12*a31*a44 - a11*a32*a44;
	m.a33 = a12*a24*a41 - a14*a22*a41 + a14*a21*a42 - a11*a24*a42 - a12*a21*a44 + a11*a22*a44;
	m.a34 = a14*a22*a31 - a12*a24*a31 - a14*a21*a32 + a11*a24*a32 + a12*a21*a34 - a11*a22*a34;
	m.a41 = a23*a32*a41 - a22*a33*a41 - a23*a31*a42 + a21*a33*a42 + a22*a31*a43 - a21*a32*a43;
	m.a42 = a12*a33*a41 - a13*a32*a41 + a13*a31*a42 - a11*a33*a42 - a12*a31*a43 + a11*a32*a43;
	m.a43 = a13*a22*a41 - a12*a23*a41 - a13*a21*a42 + a11*a23*a42 + a12*a21*a43 - a11*a22*a43;
	m.a44 = a12*a23*a31 - a13*a22*a31 + a13*a21*a32 - a11*a23*a32 - a12*a21*a33 + a11*a22*a33;
	
	return m / Determinant();
}

decVector decMatrix::GetEulerAngles() const{
	// the typical test uses angles.x=asinf(a23) and then tests for <PI/2 and >-PI/2 . unfortunately
	// the asinf implementation is quite picky about values outside the [-1..1] range. it is though
	// possible that a23 contains values which are slightly larger than 1 or slightly smaller than -1
	// due to rounding errors. this results in asinf returning NAN which breaks a lot of things. to
	// counter this problem the test is replaced with a24<1 and a24>-1 . this is the same as the asinf
	// based test but does not suffer from the problems due to rounding errors
	
	/*
	const float halfPi = PI / 2.0f;
	decVector angles;
	
	angles.x = asinf( a23 );
	
	if( angles.x < halfPi ){
		if( angles.x > -halfPi ){
			angles.z = atan2f( -a21, a22 );
			angles.y = atan2f( -a13, a33 );
		}else{
			// undefined
			angles.z = -atan2f( a31, a11 );
			angles.y = 0;
		}
	}else{
		// undefined
		angles.z = atan2f( a31, a11 );
		angles.y = 0;
	}
	*/
	
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

decVector decMatrix::GetScale() const{
	// the scaling is the length of the coordinate axes. with only translation and rotation
	// the length of the coordinate axes would be 1. thus their length is the scaling
	return decVector(
		sqrtf( a11 * a11 + a21 * a21 + a31 * a31 ),
		sqrtf( a12 * a12 + a22 * a22 + a32 * a32 ),
		sqrtf( a13 * a13 + a23 * a23 + a33 * a33 ) );
}

decQuaternion decMatrix::ToQuaternion() const{
	// WARNING
	// version 1 works with general cases but fails at some cases with track-to
	// version 2 works with the track-to cases but fails someitmes at general cases
	/*
	const float trace = a11 + a22 + a33 + 1.0f;
	
	if( trace > 0.0001f ) { // do not choose this too small or the numerical errors explode the result
		const float s = 0.5f / sqrtf( trace );
		return decQuaternion( ( a32 - a23 ) * s, ( a13 - a31 ) * s, ( a21 - a12 ) * s, 0.25f / s );
		
	}else if( a11 > a22 && a11 > a33 ){
		const float s = 2.0f * sqrtf( 1.0f + a11 - a22 - a33 );
		return decQuaternion( 0.25f * s, ( a12 + a21 ) / s, ( a13 + a31 ) / s, ( a23 - a32 ) / s );
		
	}else if( a22 > a33 ){
		const float s = 2.0f * sqrtf( 1.0f + a22 - a11 - a33 );
		return decQuaternion( ( a12 + a21 ) / s, 0.25f * s, ( a23 + a32 ) / s, ( a13 - a31 ) / s );
		
	}else{
		const float s = 2.0f * sqrtf( 1.0f + a33 - a11 - a22 );
		return decQuaternion( ( a13 + a31 ) / s, ( a23 + a32 ) / s, 0.25f * s, ( a12 - a21 ) / s );
	}
	*/
	
	const float trace = a11 + a22 + a33;
	
	if( trace > 0.0f ){
		const float s = 0.5f / sqrtf( trace + 1.0f );
		return decQuaternion( ( a32 - a23 ) * s, ( a13 - a31 ) * s, ( a21 - a12 ) * s, 0.25f / s );
		
	}else if( a11 > a22 && a11 > a33 ){
		const float s = 2.0f * sqrtf( 1.0f + a11 - a22 - a33 );
		return decQuaternion( 0.25f * s, ( a12 + a21 ) / s, ( a13 + a31 ) / s, ( a32 - a23 ) / s );
		
	}else if( a22 > a33 ){
		const float s = 2.0f * sqrtf( 1.0f + a22 - a11 - a33 );
		return decQuaternion( ( a12 + a21 ) / s, 0.25f * s, ( a23 + a32 ) / s, ( a13 - a31 ) / s );
		
	}else{
		const float s = 2.0f * sqrtf( 1.0f + a33 - a11 - a22 );
		return decQuaternion( ( a13 + a31 ) / s, ( a23 + a32 ) / s, 0.25f * s, ( a21 - a12 ) / s );
	}
	
}

decVector decMatrix::GetPosition() const{
	return decVector( a14, a24, a34 );
}

decVector decMatrix::TransformView() const{
	return decVector( a13, a23, a33 );
}

decVector decMatrix::TransformRight() const{
	return decVector( a11, a21, a31 );
}

decVector decMatrix::TransformUp() const{
	return decVector( a12, a22, a32 );
}

decVector decMatrix::TransformNormal( const decVector &v ) const{
	return decVector(
		a11 * v.x + a12 * v.y + a13 * v.z,
		a21 * v.x + a22 * v.y + a23 * v.z,
		a31 * v.x + a32 * v.y + a33 * v.z );
}

decVector decMatrix::Transform( float x, float y, float z ) const{
	return decVector(
		a11 * x + a12 * y + a13 * z + a14,
		a21 * x + a22 * y + a23 * z + a24,
		a31 * x + a32 * y + a33 * z + a34 );
}

decVector4 decMatrix::Transform( float x, float y, float z, float w ) const{
	return decVector4(
		a11 * x + a12 * y + a13 * z + a14 * w,
		a21 * x + a22 * y + a23 * z + a24 * w,
		a31 * x + a32 * y + a33 * z + a34 * w,
		a41 * x + a42 * y + a43 * z + a44 * w );
}

void decMatrix::Transform( decVector &result, float x, float y, float z ) const{
	result.x = a11 * x + a12 * y + a13 * z + a14;
	result.y = a21 * x + a22 * y + a23 * z + a24;
	result.z = a31 * x + a32 * y + a33 * z + a34;
}

void decMatrix::Transform( decVector4 &result, float x, float y, float z, float w ) const{
	result.x = a11 * x + a12 * y + a13 * z + a14 * w;
	result.y = a21 * x + a22 * y + a23 * z + a24 * w;
	result.z = a31 * x + a32 * y + a33 * z + a34 * w;
	result.w = a41 * x + a42 * y + a43 * z + a44 * w;
}

decMatrix decMatrix::GetRotationMatrix() const{
	decMatrix m;
	
	m.a11 = a11;
	m.a12 = a12;
	m.a13 = a13;
	m.a21 = a21;
	m.a22 = a22;
	m.a23 = a23;
	m.a31 = a31;
	m.a32 = a32;
	m.a33 = a33;
	
	return m;
}

void decMatrix::Normalize(){
	decVector view( TransformView() );
	if( view.IsZero() ){
		view.Set( 0.0f, 0.0f, 1.0f );
	}
	
	decVector up( TransformUp() );
	if( up.IsZero() ){
		up.Set( 0.0f, 1.0f, 0.0f );
	}
	
	SetWorld( GetPosition(), view, up );
}

decMatrix decMatrix::Normalized() const{
	decVector view( TransformView() );
	if( view.IsZero() ){
		view.Set( 0.0f, 0.0f, 1.0f );
	}
	
	decVector up( TransformUp() );
	if( up.IsZero() ){
		up.Set( 0.0f, 1.0f, 0.0f );
	}
	
	return CreateWorld( GetPosition(), view, up );
}

bool decMatrix::IsEqualTo( const decMatrix &matrix, float threshold ) const{
	return fabs( a11 - matrix.a11 ) < threshold
		&& fabs( a12 - matrix.a12 ) < threshold
		&& fabs( a13 - matrix.a13 ) < threshold
		&& fabs( a14 - matrix.a14 ) < threshold
		&& fabs( a21 - matrix.a21 ) < threshold
		&& fabs( a22 - matrix.a22 ) < threshold
		&& fabs( a23 - matrix.a23 ) < threshold
		&& fabs( a24 - matrix.a24 ) < threshold
		&& fabs( a31 - matrix.a31 ) < threshold
		&& fabs( a32 - matrix.a32 ) < threshold
		&& fabs( a33 - matrix.a33 ) < threshold
		&& fabs( a34 - matrix.a34 ) < threshold
		&& fabs( a41 - matrix.a41 ) < threshold
		&& fabs( a42 - matrix.a42 ) < threshold
		&& fabs( a43 - matrix.a43 ) < threshold
		&& fabs( a44 - matrix.a44 ) < threshold;
}

decMatrix decMatrix::QuickMultiply( const decMatrix &m ) const{
	decMatrix n;
	
	n.a11 = a11 * m.a11 + a21 * m.a12 + a31 * m.a13;
	n.a12 = a12 * m.a11 + a22 * m.a12 + a32 * m.a13;
	n.a13 = a13 * m.a11 + a23 * m.a12 + a33 * m.a13;
	n.a14 = a14 * m.a11 + a24 * m.a12 + a34 * m.a13 + m.a14;
	n.a21 = a11 * m.a21 + a21 * m.a22 + a31 * m.a23;
	n.a22 = a12 * m.a21 + a22 * m.a22 + a32 * m.a23;
	n.a23 = a13 * m.a21 + a23 * m.a22 + a33 * m.a23;
	n.a24 = a14 * m.a21 + a24 * m.a22 + a34 * m.a23 + m.a24;
	n.a31 = a11 * m.a31 + a21 * m.a32 + a31 * m.a33;
	n.a32 = a12 * m.a31 + a22 * m.a32 + a32 * m.a33;
	n.a33 = a13 * m.a31 + a23 * m.a32 + a33 * m.a33;
	n.a34 = a14 * m.a31 + a24 * m.a32 + a34 * m.a33 + m.a34;
	n.a41 = 0.0f;
	n.a42 = 0.0f;
	n.a43 = 0.0f;
	n.a44 = 1.0f;
	
	return n;
}

decMatrix decMatrix::QuickMultiply( float k ) const{
	decMatrix n;
	
	n.a11 = a11 * k;
	n.a12 = a12 * k;
	n.a13 = a13 * k;
	n.a14 = a14 * k;
	n.a21 = a21 * k;
	n.a22 = a22 * k;
	n.a23 = a23 * k;
	n.a24 = a24 * k;
	n.a31 = a31 * k;
	n.a32 = a32 * k;
	n.a33 = a33 * k;
	n.a34 = a34 * k;
	n.a41 = 0.0f;
	n.a42 = 0.0f;
	n.a43 = 0.0f;
	n.a44 = 1.0f;
	
	return n;
}

float decMatrix::QuickDeterminant() const{
	return a12 * a23 * a31 + a13 * a21 * a32 + a11 * a22 * a33
		- a13 * a22 * a31 - a11 * a23 * a32 - a12 * a21 * a33;
}

decMatrix decMatrix::QuickInvert() const{
	const float t01 = a11 * a22;
	const float t02 = a11 * a23;
	const float t03 = a11 * a32;
	const float t04 = a11 * a33;
	
	const float t05 = a12 * a21;
	const float t06 = a12 * a23;
	const float t07 = a12 * a31;
	const float t08 = a12 * a33;
	
	const float t09 = a13 * a21;
	const float t10 = a13 * a22;
	const float t11 = a13 * a31;
	const float t12 = a13 * a32;
	
	const float t13 = a21 * a32;
	const float t14 = a21 * a33;
	
	const float t15 = a22 * a31;
	const float t16 = a22 * a33;
	
	const float t17 = a23 * a32;
	const float t18 = a23 * a31;
	
	const float det = t06 * a31 + t09 * a32 + t01 * a33 - t10 * a31 - t02 * a32 - t05 * a33;
	if( det == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	const float factor = 1.0f / det;
	
	decMatrix m;
	
	m.a11 = ( t16 - t17 ) * factor;
	m.a12 = ( t12 - t08 ) * factor;
	m.a13 = ( t06 - t10 ) * factor;
	m.a14 = ( t17 * a14 - t12 * a24 - t16 * a14 + t08 * a24 + t10 * a34 - t06 * a34 ) * factor;
	m.a21 = ( t18 - t14 ) * factor;
	m.a22 = ( t04 - t11 ) * factor;
	m.a23 = ( t09 - t02 ) * factor;
	m.a24 = ( t11 * a24 - t18 * a14 + t14 * a14 - t04 * a24 - t09 * a34 + t02 * a34 ) * factor;
	m.a31 = ( t13 - t15 ) * factor;
	m.a32 = ( t07 - t03 ) * factor;
	m.a33 = ( t01 - t05 ) * factor;
	m.a34 = ( t15 * a14 - t07 * a24 - t13 * a14 + t03 * a24 + t05 * a34 - t01 * a34 ) * factor;
	m.a41 = 0.0f;
	m.a42 = 0.0f;
	m.a43 = 0.0f;
	m.a44 = 1.0f;
	
	return m;
}

decMatrix &decMatrix::QuickAddTo( const decMatrix &m ){
	a11 += m.a11; a12 += m.a12; a13 += m.a13; a14 += m.a14;
	a21 += m.a21; a22 += m.a22; a23 += m.a23; a24 += m.a24;
	a31 += m.a31; a32 += m.a32; a33 += m.a33; a34 += m.a34;
	return *this;
}



// Operators
//////////////

decMatrix &decMatrix::operator=( const decMatrix &m ){
	a11 = m.a11; a12 = m.a12; a13 = m.a13; a14 = m.a14;
	a21 = m.a21; a22 = m.a22; a23 = m.a23; a24 = m.a24;
	a31 = m.a31; a32 = m.a32; a33 = m.a33; a34 = m.a34;
	a41 = m.a41; a42 = m.a42; a43 = m.a43; a44 = m.a44;
	return *this;
}

decMatrix &decMatrix::operator+=( const decMatrix &m ){
	a11 += m.a11; a12 += m.a12; a13 += m.a13; a14 += m.a14;
	a21 += m.a21; a22 += m.a22; a23 += m.a23; a24 += m.a24;
	a31 += m.a31; a32 += m.a32; a33 += m.a33; a34 += m.a34;
	a41 += m.a41; a42 += m.a42; a43 += m.a43; a44 += m.a44;
	return *this;
}

decMatrix &decMatrix::operator-=( const decMatrix &m ){
	a11 -= m.a11; a12 -= m.a12; a13 -= m.a13; a14 -= m.a14;
	a21 -= m.a21; a22 -= m.a22; a23 -= m.a23; a24 -= m.a24;
	a31 -= m.a31; a32 -= m.a32; a33 -= m.a33; a34 -= m.a34;
	a41 -= m.a41; a42 -= m.a42; a43 -= m.a43; a44 -= m.a44;
	return *this;
}

decMatrix &decMatrix::operator*=( float k ){
	a11 *= k; a12 *= k; a13 *= k; a14 *= k;
	a21 *= k; a22 *= k; a23 *= k; a24 *= k;
	a31 *= k; a32 *= k; a33 *= k; a34 *= k;
	a41 *= k; a42 *= k; a43 *= k; a44 *= k;
	return *this;
}

decMatrix &decMatrix::operator/=( float k ){
	if( k == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	a11 /= k; a12 /= k; a13 /= k; a14 /= k;
	a21 /= k; a22 /= k; a23 /= k; a24 /= k;
	a31 /= k; a32 /= k; a33 /= k; a34 /= k;
	a41 /= k; a42 /= k; a43 /= k; a44 /= k;
	
	return *this;
}

decMatrix &decMatrix::operator*=( const decMatrix &m ){
	const float t11 = a11 * m.a11 + a21 * m.a12 + a31 * m.a13 + a41 * m.a14;
	const float t12 = a12 * m.a11 + a22 * m.a12 + a32 * m.a13 + a42 * m.a14;
	const float t13 = a13 * m.a11 + a23 * m.a12 + a33 * m.a13 + a43 * m.a14;
	const float t14 = a14 * m.a11 + a24 * m.a12 + a34 * m.a13 + a44 * m.a14;
	const float t21 = a11 * m.a21 + a21 * m.a22 + a31 * m.a23 + a41 * m.a24;
	const float t22 = a12 * m.a21 + a22 * m.a22 + a32 * m.a23 + a42 * m.a24;
	const float t23 = a13 * m.a21 + a23 * m.a22 + a33 * m.a23 + a43 * m.a24;
	const float t24 = a14 * m.a21 + a24 * m.a22 + a34 * m.a23 + a44 * m.a24;
	const float t31 = a11 * m.a31 + a21 * m.a32 + a31 * m.a33 + a41 * m.a34;
	const float t32 = a12 * m.a31 + a22 * m.a32 + a32 * m.a33 + a42 * m.a34;
	const float t33 = a13 * m.a31 + a23 * m.a32 + a33 * m.a33 + a43 * m.a34;
	const float t34 = a14 * m.a31 + a24 * m.a32 + a34 * m.a33 + a44 * m.a34;
	const float t41 = a11 * m.a41 + a21 * m.a42 + a31 * m.a43 + a41 * m.a44;
	const float t42 = a12 * m.a41 + a22 * m.a42 + a32 * m.a43 + a42 * m.a44;
	const float t43 = a13 * m.a41 + a23 * m.a42 + a33 * m.a43 + a43 * m.a44;
	const float t44 = a14 * m.a41 + a24 * m.a42 + a34 * m.a43 + a44 * m.a44;
	
	a11 = t11; a12 = t12; a13 = t13; a14 = t14;
	a21 = t21; a22 = t22; a23 = t23; a24 = t24;
	a31 = t31; a32 = t32; a33 = t33; a34 = t34;
	a41 = t41; a42 = t42; a43 = t43; a44 = t44;
	
	return *this;
}

decMatrix decMatrix::operator+( const decMatrix &m ) const{
	decMatrix n;
	
	n.a11 = a11 + m.a11; n.a12 = a12 + m.a12; n.a13 = a13 + m.a13; n.a14 = a14 + m.a14;
	n.a21 = a21 + m.a21; n.a22 = a22 + m.a22; n.a23 = a23 + m.a23; n.a24 = a24 + m.a24;
	n.a31 = a31 + m.a31; n.a32 = a32 + m.a32; n.a33 = a33 + m.a33; n.a34 = a34 + m.a34;
	n.a41 = a41 + m.a41; n.a42 = a42 + m.a42; n.a43 = a43 + m.a43; n.a44 = a44 + m.a44;
	
	return n;
}

decMatrix decMatrix::operator-( const decMatrix &m ) const{
	decMatrix n;
	
	n.a11 = a11 - m.a11; n.a12 = a12 - m.a12; n.a13 = a13 - m.a13; n.a14 = a14 - m.a14;
	n.a21 = a21 - m.a21; n.a22 = a22 - m.a22; n.a23 = a23 - m.a23; n.a24 = a24 - m.a24;
	n.a31 = a31 - m.a31; n.a32 = a32 - m.a32; n.a33 = a33 - m.a33; n.a34 = a34 - m.a34;
	n.a41 = a41 - m.a41; n.a42 = a42 - m.a42; n.a43 = a43 - m.a43; n.a44 = a44 - m.a44;
	
	return n;
}

decMatrix decMatrix::operator*( float k ) const{
	decMatrix m;
	
	m.a11 = a11 * k; m.a12 = a12 * k; m.a13 = a13 * k; m.a14 = a14 * k;
	m.a21 = a21 * k; m.a22 = a22 * k; m.a23 = a23 * k; m.a24 = a24 * k;
	m.a31 = a31 * k; m.a32 = a32 * k; m.a33 = a33 * k; m.a34 = a34 * k;
	m.a41 = a41 * k; m.a42 = a42 * k; m.a43 = a43 * k; m.a44 = a44 * k;
	
	return m;
}

decMatrix decMatrix::operator/( float k ) const{
	if( k == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	decMatrix m;
	
	m.a11 = a11 / k; m.a12 = a12 / k; m.a13 = a13 / k; m.a14 = a14 / k;
	m.a21 = a21 / k; m.a22 = a22 / k; m.a23 = a23 / k; m.a24 = a24 / k;
	m.a31 = a31 / k; m.a32 = a32 / k; m.a33 = a33 / k; m.a34 = a34 / k;
	m.a41 = a41 / k; m.a42 = a42 / k; m.a43 = a43 / k; m.a44 = a44 / k;
	
	return m;
}

decMatrix decMatrix::operator*( const decMatrix &m ) const{
	decMatrix n;
	
	n.a11 = a11 * m.a11 + a21 * m.a12 + a31 * m.a13 + a41 * m.a14;
	n.a12 = a12 * m.a11 + a22 * m.a12 + a32 * m.a13 + a42 * m.a14;
	n.a13 = a13 * m.a11 + a23 * m.a12 + a33 * m.a13 + a43 * m.a14;
	n.a14 = a14 * m.a11 + a24 * m.a12 + a34 * m.a13 + a44 * m.a14;
	n.a21 = a11 * m.a21 + a21 * m.a22 + a31 * m.a23 + a41 * m.a24;
	n.a22 = a12 * m.a21 + a22 * m.a22 + a32 * m.a23 + a42 * m.a24;
	n.a23 = a13 * m.a21 + a23 * m.a22 + a33 * m.a23 + a43 * m.a24;
	n.a24 = a14 * m.a21 + a24 * m.a22 + a34 * m.a23 + a44 * m.a24;
	n.a31 = a11 * m.a31 + a21 * m.a32 + a31 * m.a33 + a41 * m.a34;
	n.a32 = a12 * m.a31 + a22 * m.a32 + a32 * m.a33 + a42 * m.a34;
	n.a33 = a13 * m.a31 + a23 * m.a32 + a33 * m.a33 + a43 * m.a34;
	n.a34 = a14 * m.a31 + a24 * m.a32 + a34 * m.a33 + a44 * m.a34;
	n.a41 = a11 * m.a41 + a21 * m.a42 + a31 * m.a43 + a41 * m.a44;
	n.a42 = a12 * m.a41 + a22 * m.a42 + a32 * m.a43 + a42 * m.a44;
	n.a43 = a13 * m.a41 + a23 * m.a42 + a33 * m.a43 + a43 * m.a44;
	n.a44 = a14 * m.a41 + a24 * m.a42 + a34 * m.a43 + a44 * m.a44;
	
	return n;
}

decVector decMatrix::operator*( const decVector &v ) const{
	return decVector(
		a11 * v.x + a12 * v.y + a13 * v.z + a14,
		a21 * v.x + a22 * v.y + a23 * v.z + a24,
		a31 * v.x + a32 * v.y + a33 * v.z + a34 );
}

decVector4 decMatrix::operator*( const decVector4 &v ) const{
	return decVector4(
		a11 * v.x + a12 * v.y + a13 * v.z + a14 * v.w,
		a21 * v.x + a22 * v.y + a23 * v.z + a24 * v.w,
		a31 * v.x + a32 * v.y + a33 * v.z + a34 * v.w,
		a41 * v.x + a42 * v.y + a43 * v.z + a44 * v.w );
}
