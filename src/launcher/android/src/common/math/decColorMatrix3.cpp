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



// Class decColorMatrix3
//////////////////////////

// Constructors
/////////////////

decColorMatrix3::decColorMatrix3(){
	a11 = 1.0f; a12 = 0.0f; a13 = 0.0f; a14 = 0.0f; // red
	a21 = 0.0f; a22 = 1.0f; a23 = 0.0f; a24 = 0.0f; // green
	a31 = 0.0f; a32 = 0.0f; a33 = 1.0f; a34 = 0.0f; // blue
}

decColorMatrix3::decColorMatrix3( const decColorMatrix3 &m ){
	a11 = m.a11; a12 = m.a12; a13 = m.a13; a14 = m.a14;
	a21 = m.a21; a22 = m.a22; a23 = m.a23; a24 = m.a24;
	a31 = m.a31; a32 = m.a32; a33 = m.a33; a34 = m.a34;
}



// Creation
/////////////

decColorMatrix3 decColorMatrix3::CreateIdentity(){
	return decColorMatrix3();
}

decColorMatrix3 decColorMatrix3::CreateTranslation( float red, float green, float blue ){
	decColorMatrix3 matrix;
	
	matrix.a14 = red;
	matrix.a24 = green;
	matrix.a34 = blue;
	
	return matrix;
}

decColorMatrix3 decColorMatrix3::CreateTranslation( const decColor &color ){
	decColorMatrix3 matrix;
	
	matrix.a14 = color.r;
	matrix.a24 = color.g;
	matrix.a34 = color.b;
	
	return matrix;
}

decColorMatrix3 decColorMatrix3::CreateScaling( float sred, float sgreen, float sblue ){
	decColorMatrix3 matrix;
	
	matrix.a11 = sred;
	matrix.a22 = sgreen;
	matrix.a33 = sblue;
	
	return matrix;
}

decColorMatrix3 decColorMatrix3::CreateScaling( const decColor &color ){
	decColorMatrix3 matrix;
	
	matrix.a11 = color.r;
	matrix.a22 = color.g;
	matrix.a33 = color.b;
	
	return matrix;
}

decColorMatrix3 decColorMatrix3::CreateFrom( const decColor &red, const decColor &green, const decColor &blue, const decColor &translation ){
	decColorMatrix3 matrix;
	
	matrix.SetRedBase( red );
	matrix.SetGreenBase( green );
	matrix.SetBlueBase( blue );
	matrix.SetTranslationBase( translation );
	
	return matrix;
}



// Management
///////////////

void decColorMatrix3::SetIdentity(){
	// red
	a11 = 1.0f;
	a12 = 0.0f;
	a13 = 0.0f;
	a14 = 0.0f;
	
	// green
	a21 = 0.0f;
	a22 = 1.0f;
	a23 = 0.0f;
	a24 = 0.0f;
	
	// blue
	a31 = 0.0f;
	a32 = 0.0f;
	a33 = 1.0f;
	a34 = 0.0f;
}

void decColorMatrix3::SetTranslation( float red, float green, float blue ){
	// red
	a11 = 1.0f;
	a12 = 0.0f;
	a13 = 0.0f;
	a14 = red;
	
	// green
	a21 = 0.0f;
	a22 = 1.0f;
	a23 = 0.0f;
	a24 = green;
	
	// blue
	a31 = 0.0f;
	a32 = 0.0f;
	a33 = 1.0f;
	a34 = blue;
}

void decColorMatrix3::SetTranslation( const decColor &color ){
	// red
	a11 = 1.0f;
	a12 = 0.0f;
	a13 = 0.0f;
	a14 = color.r;
	
	// green
	a21 = 0.0f;
	a22 = 1.0f;
	a23 = 0.0f;
	a24 = color.g;
	
	// blue
	a31 = 0.0f;
	a32 = 0.0f;
	a33 = 1.0f;
	a34 = color.b;
}

void decColorMatrix3::SetScaling( float sred, float sgreen, float sblue ){
	// red
	a11 = sred;
	a12 = 0.0f;
	a13 = 0.0f;
	a14 = 0.0f;
	
	// green
	a21 = 0.0f;
	a22 = sgreen;
	a23 = 0.0f;
	a24 = 0.0f;
	
	// blue
	a31 = 0.0f;
	a32 = 0.0f;
	a33 = sblue;
	a34 = 0.0f;
}

void decColorMatrix3::SetScaling( const decColor &color ){
	// red
	a11 = color.r;
	a12 = 0.0f;
	a13 = 0.0f;
	a14 = 0.0f;
	
	// green
	a21 = 0.0f;
	a22 = color.g;
	a23 = 0.0f;
	a24 = 0.0f;
	
	// blue
	a31 = 0.0f;
	a32 = 0.0f;
	a33 = color.b;
	a34 = 0.0f;
}

void decColorMatrix3::SetFrom( const decColor &red, const decColor &green, const decColor &blue, const decColor &translation ){
	// red
	a11 = red.r;
	a12 = green.r;
	a13 = blue.r;
	a14 = translation.r;
	
	// green
	a21 = red.g;
	a22 = green.g;
	a23 = blue.g;
	a24 = translation.g;
	
	// blue
	a31 = red.b;
	a32 = green.b;
	a33 = blue.b;
	a34 = translation.b;
}

float decColorMatrix3::Determinant() const{
	// TODO
	return 0.0f;
	/*
	return a14*a23*a32*a41 - a13*a24*a32*a41 - a14*a22*a33*a41 + a12*a24*a33*a41 +
		a13*a22*a34*a41 - a12*a23*a34*a41 - a14*a23*a31*a42 + a13*a24*a31*a42 +
		a14*a21*a33*a42 - a11*a24*a33*a42 - a13*a21*a34*a42 + a11*a23*a34*a42 +
		a14*a22*a31*a43 - a12*a24*a31*a43 - a14*a21*a32*a43 + a11*a24*a32*a43 +
		a12*a21*a34*a43 - a11*a22*a34*a43 - a13*a22*a31*a44 + a12*a23*a31*a44 +
		a13*a21*a32*a44 - a11*a23*a32*a44 - a12*a21*a33*a44 + a11*a22*a33*a44;
	*/
}

decColorMatrix3 decColorMatrix3::Invert() const{
	// TODO
	return *this;
	/*
	decColorMatrix3 m;
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
	*/
}


decColor decColorMatrix3::GetRedBase() const{
	return decColor( a11, a21, a31 );
}

decColor decColorMatrix3::GetGreenBase() const{
	return decColor( a12, a22, a32 );
}

decColor decColorMatrix3::GetBlueBase() const{
	return decColor( a13, a23, a33 );
}

decColor decColorMatrix3::GetTranslationBase() const{
	return decColor( a14, a24, a34 );
}

void decColorMatrix3::SetRedBase( const decColor &color ){
	a11 = color.r;
	a21 = color.g;
	a31 = color.b;
}

void decColorMatrix3::SetGreenBase( const decColor &color ){
	a12 = color.r;
	a22 = color.g;
	a32 = color.b;
}

void decColorMatrix3::SetBlueBase( const decColor &color ){
	a13 = color.r;
	a23 = color.g;
	a33 = color.b;
}

void decColorMatrix3::SetTranslationBase( const decColor &color ){
	a14 = color.r;
	a24 = color.g;
	a34 = color.b;
}

bool decColorMatrix3::IsEqualTo( const decColorMatrix3 &matrix, float threshold ) const{
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
		&& fabs( a34 - matrix.a34 ) < threshold;
}

decColor decColorMatrix3::Transform( const decColor &color ) const{
	return decColor(
		a11 * color.r + a12 * color.g + a13 * color.b + a14,
		a21 * color.r + a22 * color.g + a23 * color.b + a24,
		a31 * color.r + a32 * color.g + a33 * color.b + a34 );
}



// Operators
//////////////

decColorMatrix3 &decColorMatrix3::operator=( const decColorMatrix3 &matrix ){
	// red
	a11 = matrix.a11;
	a12 = matrix.a12;
	a13 = matrix.a13;
	a14 = matrix.a14;
	
	// green
	a21 = matrix.a21;
	a22 = matrix.a22;
	a23 = matrix.a23;
	a24 = matrix.a24;
	
	// blue
	a31 = matrix.a31;
	a32 = matrix.a32;
	a33 = matrix.a33;
	a34 = matrix.a34;
	
	// return ourself
	return *this;
}

decColorMatrix3 &decColorMatrix3::operator+=( const decColorMatrix3 &matrix ){
	// red
	a11 += matrix.a11;
	a12 += matrix.a12;
	a13 += matrix.a13;
	a14 += matrix.a14;
	
	// green
	a21 += matrix.a21;
	a22 += matrix.a22;
	a23 += matrix.a23;
	a24 += matrix.a24;
	
	// blue
	a31 += matrix.a31;
	a32 += matrix.a32;
	a33 += matrix.a33;
	a34 += matrix.a34;
	
	// return ourself
	return *this;
}

decColorMatrix3 &decColorMatrix3::operator-=( const decColorMatrix3 &matrix ){
	// red
	a11 -= matrix.a11;
	a12 -= matrix.a12;
	a13 -= matrix.a13;
	a14 -= matrix.a14;
	
	// green
	a21 -= matrix.a21;
	a22 -= matrix.a22;
	a23 -= matrix.a23;
	a24 -= matrix.a24;
	
	// blue
	a31 -= matrix.a31;
	a32 -= matrix.a32;
	a33 -= matrix.a33;
	a34 -= matrix.a34;
	
	// return ourself
	return *this;
}

decColorMatrix3 &decColorMatrix3::operator*=( float scalar ){
	// red
	a11 *= scalar;
	a12 *= scalar;
	a13 *= scalar;
	a14 *= scalar;
	
	// green
	a21 *= scalar;
	a22 *= scalar;
	a23 *= scalar;
	a24 *= scalar;
	
	// blue
	a31 *= scalar;
	a32 *= scalar;
	a33 *= scalar;
	a34 *= scalar;
	
	// return ourself
	return *this;
}

decColorMatrix3 &decColorMatrix3::operator/=( float scalar ){
	if( scalar == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	return *this *= ( 1.0f / scalar );
}

decColorMatrix3 &decColorMatrix3::operator*=( const decColorMatrix3 &matrix ){
	// red
	const float t11 = a11 * matrix.a11 + a21 * matrix.a12 + a31 * matrix.a13;
	const float t12 = a12 * matrix.a11 + a22 * matrix.a12 + a32 * matrix.a13;
	const float t13 = a13 * matrix.a11 + a23 * matrix.a12 + a33 * matrix.a13;
	const float t14 = a14 * matrix.a11 + a24 * matrix.a12 + a34 * matrix.a13 + matrix.a14;
	
	// green
	const float t21 = a11 * matrix.a21 + a21 * matrix.a22 + a31 * matrix.a23;
	const float t22 = a12 * matrix.a21 + a22 * matrix.a22 + a32 * matrix.a23;
	const float t23 = a13 * matrix.a21 + a23 * matrix.a22 + a33 * matrix.a23;
	const float t24 = a14 * matrix.a21 + a24 * matrix.a22 + a34 * matrix.a23 + matrix.a24;
	
	// blue
	const float t31 = a11 * matrix.a31 + a21 * matrix.a32 + a31 * matrix.a33;
	const float t32 = a12 * matrix.a31 + a22 * matrix.a32 + a32 * matrix.a33;
	const float t33 = a13 * matrix.a31 + a23 * matrix.a32 + a33 * matrix.a33;
	const float t34 = a14 * matrix.a31 + a24 * matrix.a32 + a34 * matrix.a33 + matrix.a34;
	
	// assign values
	a11 = t11;
	a12 = t12;
	a13 = t13;
	a14 = t14;
	
	a21 = t21;
	a22 = t22;
	a23 = t23;
	a24 = t24;
	
	a31 = t31;
	a32 = t32;
	a33 = t33;
	a34 = t34;
	
	// return ourself
	return *this;
}

decColorMatrix3 decColorMatrix3::operator+( const decColorMatrix3 &matrix ) const{
	decColorMatrix3 result;
	
	// red
	result.a11 = a11 + matrix.a11;
	result.a12 = a12 + matrix.a12;
	result.a13 = a13 + matrix.a13;
	result.a14 = a14 + matrix.a14;
	
	// green
	result.a21 = a21 + matrix.a21;
	result.a22 = a22 + matrix.a22;
	result.a23 = a23 + matrix.a23;
	result.a24 = a24 + matrix.a24;
	
	// blue
	result.a31 = a31 + matrix.a31;
	result.a32 = a32 + matrix.a32;
	result.a33 = a33 + matrix.a33;
	result.a34 = a34 + matrix.a34;
	
	// return result
	return result;
}

decColorMatrix3 decColorMatrix3::operator-( const decColorMatrix3 &matrix ) const{
	decColorMatrix3 result;
	
	// red
	result.a11 = a11 - matrix.a11;
	result.a12 = a12 - matrix.a12;
	result.a13 = a13 - matrix.a13;
	result.a14 = a14 - matrix.a14;
	
	// green
	result.a21 = a21 - matrix.a21;
	result.a22 = a22 - matrix.a22;
	result.a23 = a23 - matrix.a23;
	result.a24 = a24 - matrix.a24;
	
	// blue
	result.a31 = a31 - matrix.a31;
	result.a32 = a32 - matrix.a32;
	result.a33 = a33 - matrix.a33;
	result.a34 = a34 - matrix.a34;
	
	// return result
	return result;
}

decColorMatrix3 decColorMatrix3::operator*( float scalar ) const{
	decColorMatrix3 result;
	
	// red
	result.a11 = a11 * scalar;
	result.a12 = a12 * scalar;
	result.a13 = a13 * scalar;
	result.a14 = a14 * scalar;
	
	// green
	result.a21 = a21 * scalar;
	result.a22 = a22 * scalar;
	result.a23 = a23 * scalar;
	result.a24 = a24 * scalar;
	
	// blue
	result.a31 = a31 * scalar;
	result.a32 = a32 * scalar;
	result.a33 = a33 * scalar;
	result.a34 = a34 * scalar;
	
	// return result
	return result;
}

decColorMatrix3 decColorMatrix3::operator/( float scalar ) const{
	if( scalar == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	return *this * ( 1.0f / scalar );
}

decColorMatrix3 decColorMatrix3::operator*( const decColorMatrix3 &matrix ) const{
	decColorMatrix3 result;
	
	// red
	result.a11 = a11 * matrix.a11 + a21 * matrix.a12 + a31 * matrix.a13;
	result.a12 = a12 * matrix.a11 + a22 * matrix.a12 + a32 * matrix.a13;
	result.a13 = a13 * matrix.a11 + a23 * matrix.a12 + a33 * matrix.a13;
	result.a14 = a14 * matrix.a11 + a24 * matrix.a12 + a34 * matrix.a13 + matrix.a14;
	
	// green
	result.a21 = a11 * matrix.a21 + a21 * matrix.a22 + a31 * matrix.a23;
	result.a22 = a12 * matrix.a21 + a22 * matrix.a22 + a32 * matrix.a23;
	result.a23 = a13 * matrix.a21 + a23 * matrix.a22 + a33 * matrix.a23;
	result.a24 = a14 * matrix.a21 + a24 * matrix.a22 + a34 * matrix.a23 + matrix.a24;
	
	// blue
	result.a31 = a11 * matrix.a31 + a21 * matrix.a32 + a31 * matrix.a33;
	result.a32 = a12 * matrix.a31 + a22 * matrix.a32 + a32 * matrix.a33;
	result.a33 = a13 * matrix.a31 + a23 * matrix.a32 + a33 * matrix.a33;
	result.a34 = a14 * matrix.a31 + a24 * matrix.a32 + a34 * matrix.a33 + matrix.a34;
	
	// return result
	return result;
}

decColor decColorMatrix3::operator*( const decColor &color ) const{
	return decColor(
		a11 * color.r + a12 * color.g + a13 * color.b + a14,
		a21 * color.r + a22 * color.g + a23 * color.b + a24,
		a31 * color.r + a32 * color.g + a33 * color.b + a34 );
}
