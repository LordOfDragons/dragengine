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

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "decMath.h"
#include "../exceptions.h"



// Class decTexMatrix
//////////////////////

// Constructor
////////////////

decTexMatrix2::decTexMatrix2(){
	a11 = 1.0f;
	a12 = 0.0f;
	a13 = 0.0f;
	a21 = 0.0f;
	a22 = 1.0f;
	a23 = 0.0f;
}



// Matrix creation
////////////////////

decTexMatrix2 decTexMatrix2::CreateIdentity(){
	decTexMatrix2 m;
	
	m.a11 = 1.0f;
	m.a12 = 0.0f;
	m.a13 = 0.0f;
	m.a21 = 0.0f;
	m.a22 = 1.0f;
	m.a23 = 0.0f;
	
	return m;
}

decTexMatrix2 decTexMatrix2::CreateTranslation( float u, float v ){
	decTexMatrix2 m;
	
	m.a11 = 1.0f;
	m.a12 = 0.0f;
	m.a13 = u;
	m.a21 = 0.0f;
	m.a22 = 1.0f;
	m.a23 = v;
	
	return m;
}

decTexMatrix2 decTexMatrix2::CreateTranslation( const decVector2 &translation ){
	decTexMatrix2 m;
	
	m.a11 = 1.0f;
	m.a12 = 0.0f;
	m.a13 = translation.x;
	m.a21 = 0.0f;
	m.a22 = 1.0f;
	m.a23 = translation.y;
	
	return m;
}

decTexMatrix2 decTexMatrix2::CreateScale( float su, float sv ){
	decTexMatrix2 m;
	
	m.a11 = su;
	m.a12 = 0.0f;
	m.a13 = 0.0f;
	m.a21 = 0.0f;
	m.a22 = sv;
	m.a23 = 0.0f;
	
	return m;
}

decTexMatrix2 decTexMatrix2::CreateScale( const decVector2 &scaling ){
	decTexMatrix2 m;
	
	m.a11 = scaling.x;
	m.a12 = 0.0f;
	m.a13 = 0.0f;
	m.a21 = 0.0f;
	m.a22 = scaling.y;
	m.a23 = 0.0f;
	
	return m;
}

decTexMatrix2 decTexMatrix2::CreateRotation( float rotation ){
	decTexMatrix2 m;
	const float s = sinf( rotation );
	const float c = cosf( rotation );
	
	m.a11 = c;
	m.a12 = -s;
	m.a13 = 0.0f;
	m.a21 = s;
	m.a22 = c;
	m.a23 = 0.0f;
	
	return m;
}

decTexMatrix2 decTexMatrix2::CreateShear( float shearU, float shearV ){
	decTexMatrix2 m;
	
	m.a11 =   1.0f; m.a12 = shearU; m.a13 = 0.0f;
	m.a21 = shearV; m.a22 =   1.0f; m.a23 = 0.0f;
	
	return m;
}

decTexMatrix2 decTexMatrix2::CreateShear( const decVector2 &shear ){
	return CreateShear( shear.x, shear.y );
}

decTexMatrix2 decTexMatrix2::CreateST( float su, float sv, float tu, float tv ){
	decTexMatrix2 m;
	
	m.a11 = su;
	m.a12 = 0.0f;
	m.a13 = tu;
	m.a21 = 0.0f;
	m.a22 = sv;
	m.a23 = tv;
	
	return m;
}

decTexMatrix2 decTexMatrix2::CreateST( const decVector2 &scaling, const decVector2 &translation ){
	decTexMatrix2 m;
	
	m.a11 = scaling.x;
	m.a12 = 0.0f;
	m.a13 = translation.x;
	m.a21 = 0.0f;
	m.a22 = scaling.y;
	m.a23 = translation.y;
	
	return m;
}

decTexMatrix2 decTexMatrix2::CreateSRT( float su, float sv, float rotation, float tu, float tv ){
	decTexMatrix2 m;
	const float rsin = sinf( rotation );
	const float rcos = cosf( rotation );
	
	m.a11 = su * rcos;
	m.a12 = -sv * rsin;
	m.a13 = tu;
	m.a21 = su * rsin;
	m.a22 = sv * rcos;
	m.a23 = tv;
	
	return m;
}

decTexMatrix2 decTexMatrix2::CreateSRT( const decVector2 &scaling, float rotation, const decVector2 &translation ){
	decTexMatrix2 m;
	const float rsin = sinf( rotation );
	const float rcos = cosf( rotation );
	
	m.a11 = scaling.x * rcos;
	m.a12 = -scaling.y * rsin;
	m.a13 = translation.x;
	m.a21 = scaling.x * rsin;
	m.a22 = scaling.y * rcos;
	m.a23 = translation.y;
	
	return m;
}

decTexMatrix2 decTexMatrix2::CreateCenterSRT( float su, float sv, float rotation, float tu, float tv ){
	decTexMatrix2 m;
	const float rsin = sinf( rotation );
	const float rcos = cosf( rotation );
	const float su_rcos = su * rcos;
	const float su_rsin = su * rsin;
	const float sv_rcos = sv * rcos;
	const float sv_rsin = sv * rsin;
	
	m.a11 = su_rcos;
	m.a12 = -sv_rsin;
	m.a13 = 0.5f * ( sv_rsin - su_rcos ) + tu + 0.5f;
	m.a21 = su_rsin;
	m.a22 = sv_rcos;
	m.a23 = tv + 0.5f - 0.5f * ( su_rsin + sv_rcos );
	
	return m;
}

decTexMatrix2 decTexMatrix2::CreateCenterSRT( const decVector2 &scaling, float rotation, const decVector2 &translation ){
	decTexMatrix2 m;
	const float rsin = sinf( rotation );
	const float rcos = cosf( rotation );
	const float su_rcos = scaling.x * rcos;
	const float su_rsin = scaling.x * rsin;
	const float sv_rcos = scaling.y * rcos;
	const float sv_rsin = scaling.y * rsin;
	
	m.a11 = su_rcos;
	m.a12 = -sv_rsin;
	m.a13 = 0.5f * ( sv_rsin - su_rcos ) + translation.x + 0.5f;
	m.a21 = su_rsin;
	m.a22 = sv_rcos;
	m.a23 = translation.y + 0.5f - 0.5f * ( su_rsin + sv_rcos );
	
	return m;
}



// Management
///////////////

void decTexMatrix2::SetIdentity(){
	a11 = 1.0f;
	a12 = 0.0f;
	a13 = 0.0f;
	a21 = 0.0f;
	a22 = 1.0f;
	a23 = 0.0f;
}

void decTexMatrix2::SetTranslation( float u, float v ){
	a11 = 1.0f;
	a12 = 0.0f;
	a13 = u;
	a21 = 0.0f;
	a22 = 1.0f;
	a23 = v;
}

void decTexMatrix2::SetTranslation( const decVector2 &translation ){
	a11 = 1.0f;
	a12 = 0.0f;
	a13 = translation.x;
	a21 = 0.0f;
	a22 = 1.0f;
	a23 = translation.y;
}

void decTexMatrix2::SetRotation( float rotation ){
	const float s = sinf( rotation );
	const float c = cosf( rotation );
	
	a11 = c;
	a12 = -s;
	a13 = 0.0f;
	a21 = s;
	a22 = c;
	a23 = 0.0f;
}

void decTexMatrix2::SetShear( float shearU, float shearV ) {
	a11 =   1.0f; a12 = shearU; a13 = 0.0f;
	a21 = shearV; a22 =   1.0f; a23 = 0.0f;
}

void decTexMatrix2::SetShear( const decVector2 &shear ){
	SetShear( shear.x, shear.y );
}

void decTexMatrix2::SetScale( float su, float sv ){
	a11 = su;
	a12 = 0.0f;
	a13 = 0.0f;
	a21 = 0.0f;
	a22 = sv;
	a23 = 0.0f;
}

void decTexMatrix2::SetScale( const decVector2 &scaling ){
	a11 = scaling.x;
	a12 = 0.0f;
	a13 = 0.0f;
	a21 = 0.0f;
	a22 = scaling.y;
	a23 = 0.0f;
}

void decTexMatrix2::SetST( float su, float sv, float tu, float tv ){
	a11 = su;
	a12 = 0.0f;
	a13 = tu;
	a21 = 0.0f;
	a22 = sv;
	a23 = tv;
}

void decTexMatrix2::SetST( const decVector2 &scaling, const decVector2 &translation ){
	a11 = scaling.x;
	a12 = 0.0f;
	a13 = translation.x;
	a21 = 0.0f;
	a22 = scaling.y;
	a23 = translation.y;
}

void decTexMatrix2::SetSRT( float su, float sv, float rotation, float tu, float tv ){
	const float rsin = sinf( rotation );
	const float rcos = cosf( rotation );
	
	a11 = su * rcos;
	a12 = -sv * rsin;
	a13 = tu;
	a21 = su * rsin;
	a22 = sv * rcos;
	a23 = tv;
}

void decTexMatrix2::SetSRT( const decVector2 &scaling, float rotation, const decVector2 &translation ){
	const float rsin = sinf( rotation );
	const float rcos = cosf( rotation );
	
	a11 = scaling.x * rcos;
	a12 = -scaling.y * rsin;
	a13 = translation.x;
	a21 = scaling.x * rsin;
	a22 = scaling.y * rcos;
	a23 = translation.y;
}

void decTexMatrix2::SetCenterSRT( float su, float sv, float rotation, float tu, float tv ){
	const float rsin = sinf( rotation );
	const float rcos = cosf( rotation );
	const float su_rcos = su * rcos;
	const float su_rsin = su * rsin;
	const float sv_rcos = sv * rcos;
	const float sv_rsin = sv * rsin;
	
	a11 = su_rcos;
	a12 = -sv_rsin;
	a13 = 0.5f * ( sv_rsin - su_rcos ) + tu + 0.5f;
	a21 = su_rsin;
	a22 = sv_rcos;
	a23 = tv + 0.5f - 0.5f * ( su_rsin + sv_rcos );
}

void decTexMatrix2::SetCenterSRT( const decVector2 &scaling, float rotation, const decVector2 &translation ){
	const float rsin = sinf( rotation );
	const float rcos = cosf( rotation );
	const float su_rcos = scaling.x * rcos;
	const float su_rsin = scaling.x * rsin;
	const float sv_rcos = scaling.y * rcos;
	const float sv_rsin = scaling.y * rsin;
	
	a11 = su_rcos;
	a12 = -sv_rsin;
	a13 = 0.5f * ( sv_rsin - su_rcos ) + translation.x + 0.5f;
	a21 = su_rsin;
	a22 = sv_rcos;
	a23 = translation.y + 0.5f - 0.5f * ( su_rsin + sv_rcos );
}

bool decTexMatrix2::IsEqualTo( const decTexMatrix2 &matrix, float threshold ) const{
	return fabs( a11 - matrix.a11 ) < threshold
		&& fabs( a12 - matrix.a12 ) < threshold
		&& fabs( a13 - matrix.a13 ) < threshold
		&& fabs( a21 - matrix.a21 ) < threshold
		&& fabs( a22 - matrix.a22 ) < threshold
		&& fabs( a23 - matrix.a23 ) < threshold ;
}



float decTexMatrix2::Determinant() const{
	return a11*a22 - a12*a21;
}

decTexMatrix decTexMatrix2::Invert() const{
	decTexMatrix m;
	
	m.a11 = a22;
	m.a12 = -a12;
	m.a13 = a12*a23 - a13*a22;
	m.a21 = -a21;
	m.a22 = a11;
	m.a23 = a13*a21 - a11*a23;
	m.a31 = 0.0f;
	m.a32 = 0.0f;
	m.a33 = a11*a22 - a12*a21;
	
	return m / Determinant();
}

decTexMatrix decTexMatrix2::ToTexMatrix() const{
	decTexMatrix m;
	
	m.a11 =  a11; m.a12 =  a12; m.a13 =  a13;
	m.a21 =  a21; m.a22 =  a22; m.a23 =  a23;
	m.a31 = 0.0f; m.a32 = 0.0f; m.a33 = 1.0f;
	
	return m;
}

decVector2 decTexMatrix2::GetPosition() const{
	return decVector2( a13, a23 );
}

decVector2 decTexMatrix2::GetAxisX() const{
	return decVector2( a11, a21 );
}

decVector2 decTexMatrix2::GetAxisY() const{
	return decVector2( a12, a22 );
}

decVector2 decTexMatrix2::GetScaling() const{
	const decVector2 axisX( GetAxisX() );
	return decVector2( axisX.Length(), axisX.RotateLeft().Normalized() * GetAxisY() );
}

float decTexMatrix2::GetRotation() const{
	return atan2f( a21, a11 );
}

float decTexMatrix2::GetShearing() const{
	const decVector2 axisX( GetAxisX().Normalized() );
	const decVector2 axisY( GetAxisY() );
	const float x = axisY * axisX;
	const float y = axisY * axisX.RotateLeft();
	
	if( fabsf( y ) < FLOAT_SAFE_EPSILON ){
		DETHROW( deeDivisionByZero );
	}
	
	return x / y;
}

decTexMatrix2 decTexMatrix2::GetRotationMatrix() const{
	decTexMatrix2 m;
	m.a11 = a11; m.a12 = a12;
	m.a21 = a21; m.a22 = a22;
	return m;
}



// Operators
//////////////

decTexMatrix2 &decTexMatrix2::operator=( const decTexMatrix2 &m ){
	a11 = m.a11;
	a12 = m.a12;
	a13 = m.a13;
	a21 = m.a21;
	a22 = m.a22;
	a23 = m.a23;
	
	return *this;
}

decTexMatrix2 &decTexMatrix2::operator+=( const decTexMatrix2 &m ){
	a11 += m.a11;
	a12 += m.a12;
	a13 += m.a13;
	a21 += m.a21;
	a22 += m.a22;
	a23 += m.a23;
	
	return *this;
}

decTexMatrix2 &decTexMatrix2::operator-=( const decTexMatrix2 &m ){
	a11 -= m.a11;
	a12 -= m.a12;
	a13 -= m.a13;
	a21 -= m.a21;
	a22 -= m.a22;
	a23 -= m.a23;
	
	return *this;
}

decTexMatrix2 &decTexMatrix2::operator*=( float k ){
	a11 *= k;
	a12 *= k;
	a13 *= k;
	a21 *= k;
	a22 *= k;
	a23 *= k;
	
	return *this;
}

decTexMatrix2 &decTexMatrix2::operator/=( float k ){
	if( k == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	const float factor = 1.0f / k;
	
	a11 *= factor;
	a12 *= factor;
	a13 *= factor;
	a21 *= factor;
	a22 *= factor;
	a23 *= factor;
	
	return *this;
}

decTexMatrix2 &decTexMatrix2::operator*=( const decTexMatrix2 &m ){
	float t11, t12, t13;
	float t21, t22, t23;
	
	t11 = a11 * m.a11 + a21 * m.a12;
	t12 = a12 * m.a11 + a22 * m.a12;
	t13 = a13 * m.a11 + a23 * m.a12 + m.a13;
	t21 = a11 * m.a21 + a21 * m.a22;
	t22 = a12 * m.a21 + a22 * m.a22;
	t23 = a13 * m.a21 + a23 * m.a22 + m.a23;
	
	a11 = t11;
	a12 = t12;
	a13 = t13;
	a21 = t21;
	a22 = t22;
	a23 = t23;
	
	return *this;
}

decTexMatrix2 decTexMatrix2::operator+( const decTexMatrix2 &m ) const{
	decTexMatrix2 n;
	
	n.a11 = a11 + m.a11;
	n.a12 = a12 + m.a12;
	n.a13 = a13 + m.a13;
	n.a21 = a21 + m.a21;
	n.a22 = a22 + m.a22;
	n.a23 = a23 + m.a23;
	
	return n;
}

decTexMatrix2 decTexMatrix2::operator-( const decTexMatrix2 &m ) const{
	decTexMatrix2 n;
	
	n.a11 = a11 - m.a11;
	n.a12 = a12 - m.a12;
	n.a13 = a13 - m.a13;
	n.a21 = a21 - m.a21;
	n.a22 = a22 - m.a22;
	n.a23 = a23 - m.a23;
	
	return n;
}

decTexMatrix2 decTexMatrix2::operator*( float k ) const{
	decTexMatrix2 n;
	
	n.a11 = a11 * k;
	n.a12 = a12 * k;
	n.a13 = a13 * k;
	n.a21 = a21 * k;
	n.a22 = a22 * k;
	n.a23 = a23 * k;
	
	return n;
}

decTexMatrix2 decTexMatrix2::operator/( float k ) const{
	if( k == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	decTexMatrix2 n;
	const float factor = 1.0f / k;
	
	n.a11 = a11 * factor;
	n.a12 = a12 * factor;
	n.a13 = a13 * factor;
	n.a21 = a21 * factor;
	n.a22 = a22 * factor;
	n.a23 = a23 * factor;
	
	return n;
}

decTexMatrix2 decTexMatrix2::operator*( const decTexMatrix2 &m ) const{
	decTexMatrix2 n;
	
	n.a11 = a11 * m.a11 + a21 * m.a12;
	n.a12 = a12 * m.a11 + a22 * m.a12;
	n.a13 = a13 * m.a11 + a23 * m.a12 + m.a13;
	n.a21 = a11 * m.a21 + a21 * m.a22;
	n.a22 = a12 * m.a21 + a22 * m.a22;
	n.a23 = a13 * m.a21 + a23 * m.a22 + m.a23;
	
	return n;
}

decVector2 decTexMatrix2::operator*( const decVector2 &v ) const{
	return decVector2( a11 * v.x + a12 * v.y + a13, a21 * v.x + a22 * v.y + a23 );
}
