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



// Class decColorMatrix
/////////////////////////

// Constructors
/////////////////

decColorMatrix::decColorMatrix(){
	a11 = 1.0f; a12 = 0.0f; a13 = 0.0f; a14 = 0.0f; a15 = 0.0f; // red
	a21 = 0.0f; a22 = 1.0f; a23 = 0.0f; a24 = 0.0f; a25 = 0.0f; // green
	a31 = 0.0f; a32 = 0.0f; a33 = 1.0f; a34 = 0.0f; a35 = 0.0f; // blue
	a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f; a45 = 0.0f; // alpha
}

decColorMatrix::decColorMatrix( const decColorMatrix &m ){
	a11 = m.a11; a12 = m.a12; a13 = m.a13; a14 = m.a14; a15 = m.a15;
	a21 = m.a21; a22 = m.a22; a23 = m.a23; a24 = m.a24; a25 = m.a25;
	a31 = m.a31; a32 = m.a32; a33 = m.a33; a34 = m.a34; a35 = m.a35;
	a41 = m.a41; a42 = m.a42; a43 = m.a43; a44 = m.a44; a45 = m.a45;
}



// Creation
/////////////

decColorMatrix decColorMatrix::CreateIdentity(){
	return decColorMatrix();
}

decColorMatrix decColorMatrix::CreateTranslation( float red, float green, float blue, float alpha ){
	decColorMatrix m;
	m.SetTranslation( red, green, blue, alpha );
	return m;
}

decColorMatrix decColorMatrix::CreateTranslation( const decColor &color ){
	decColorMatrix m;
	m.SetTranslation( color );
	return m;
}

decColorMatrix decColorMatrix::CreateScaling( float sred, float sgreen, float sblue, float salpha ){
	decColorMatrix m;
	m.SetScaling( sred, sgreen, sblue, salpha );
	return m;
}

decColorMatrix decColorMatrix::CreateScaling( const decColor &color ){
	decColorMatrix m;
	m.SetScaling( color );
	return m;
}

decColorMatrix decColorMatrix::CreateST( const decColor &scale, const decColor &translate ){
	decColorMatrix m;
	m.SetST( scale, translate );
	return m;
}

decColorMatrix decColorMatrix::CreateFrom( const decColor &red, const decColor &green,
const decColor &blue, const decColor &alpha, const decColor &white ){
	decColorMatrix m;
	m.SetFrom( red, green, blue, alpha, white );
	return m;
}

decColorMatrix decColorMatrix::CreateBrightness( float brightness ){
	decColorMatrix m;
	m.SetBrightness( brightness );
	return m;
}

decColorMatrix decColorMatrix::CreateBrightness( float red, float green, float blue ){
	decColorMatrix m;
	m.SetBrightness( red, green, blue );
	return m;
}

decColorMatrix decColorMatrix::CreateBrightness( const decColor &brightness ){
	decColorMatrix m;
	m.SetBrightness( brightness );
	return m;
}

decColorMatrix decColorMatrix::CreateContrast( float contrast ){
	decColorMatrix m;
	m.SetContrast( contrast );
	return m;
}

decColorMatrix decColorMatrix::CreateContrast( float red, float green, float blue ){
	decColorMatrix m;
	m.SetContrast( red, green, blue );
	return m;
}

decColorMatrix decColorMatrix::CreateContrast( const decColor &contrast ){
	decColorMatrix m;
	m.SetContrast( contrast );
	return m;
}

decColorMatrix decColorMatrix::CreateSaturation( float saturation ){
	decColorMatrix m;
	m.SetSaturation( saturation );
	return m;
}

decColorMatrix decColorMatrix::CreateSaturation( float red, float green, float blue ){
	decColorMatrix m;
	m.SetSaturation( red, green, blue );
	return m;
}

decColorMatrix decColorMatrix::CreateSaturation( const decColor &saturation ){
	decColorMatrix m;
	m.SetSaturation( saturation );
	return m;
}

decColorMatrix decColorMatrix::CreateColorInversion(){
	decColorMatrix m;
	m.SetColorInversion();
	return m;
}



// Management
///////////////

void decColorMatrix::SetIdentity(){
	a11 = 1.0f;
	a12 = 0.0f;
	a13 = 0.0f;
	a14 = 0.0f;
	a15 = 0.0f;
	
	a21 = 0.0f;
	a22 = 1.0f;
	a23 = 0.0f;
	a24 = 0.0f;
	a25 = 0.0f;
	
	a31 = 0.0f;
	a32 = 0.0f;
	a33 = 1.0f;
	a34 = 0.0f;
	a35 = 0.0f;
	
	a41 = 0.0f;
	a42 = 0.0f;
	a43 = 0.0f;
	a44 = 1.0f;
	a45 = 0.0f;
}

void decColorMatrix::SetTranslation( float red, float green, float blue, float alpha ){
	a11 = 1.0f;
	a12 = 0.0f;
	a13 = 0.0f;
	a14 = 0.0f;
	a15 = red;
	
	a21 = 0.0f;
	a22 = 1.0f;
	a23 = 0.0f;
	a24 = 0.0f;
	a25 = green;
	
	a31 = 0.0f;
	a32 = 0.0f;
	a33 = 1.0f;
	a34 = 0.0f;
	a35 = blue;
	
	a41 = 0.0f;
	a42 = 0.0f;
	a43 = 0.0f;
	a44 = 1.0f;
	a45 = alpha;
}

void decColorMatrix::SetTranslation( const decColor &color ){
	SetTranslation( color.r, color.g, color.b, color.a );
}

void decColorMatrix::SetScaling( float sred, float sgreen, float sblue, float salpha ){
	a11 = sred;
	a12 = 0.0f;
	a13 = 0.0f;
	a14 = 0.0f;
	a15 = 0.0f;
	
	a21 = 0.0f;
	a22 = sgreen;
	a23 = 0.0f;
	a24 = 0.0f;
	a25 = 0.0f;
	
	a31 = 0.0f;
	a32 = 0.0f;
	a33 = sblue;
	a34 = 0.0f;
	a35 = 0.0f;
	
	a41 = 0.0f;
	a42 = 0.0f;
	a43 = 0.0f;
	a44 = salpha;
	a45 = 0.0f;
}

void decColorMatrix::SetScaling( const decColor &color ){
	SetScaling( color.r, color.g, color.b, color.a );
}

void decColorMatrix::SetST( const decColor &scale, const decColor &translate ){
	a11 = scale.r;
	a12 = 0.0f;
	a13 = 0.0f;
	a14 = 0.0f;
	a15 = translate.r;
	
	a21 = 0.0f;
	a22 = scale.g;
	a23 = 0.0f;
	a24 = 0.0f;
	a25 = translate.g;
	
	a31 = 0.0f;
	a32 = 0.0f;
	a33 = scale.b;
	a34 = 0.0f;
	a35 = translate.b;
	
	a41 = 0.0f;
	a42 = 0.0f;
	a43 = 0.0f;
	a44 = scale.a;
	a45 = translate.a;
}

void decColorMatrix::SetFrom( const decColor &red, const decColor &green,
const decColor &blue, const decColor &alpha, const decColor &white ){
	a11 = red.r;
	a12 = green.r;
	a13 = blue.r;
	a14 = alpha.r;
	a15 = white.r;
	
	a21 = red.g;
	a22 = green.g;
	a23 = blue.g;
	a24 = alpha.g;
	a25 = white.g;
	
	a31 = red.b;
	a32 = green.b;
	a33 = blue.b;
	a34 = alpha.b;
	a35 = white.b;
	
	a41 = red.a;
	a42 = green.a;
	a43 = blue.a;
	a44 = alpha.a;
	a45 = white.a;
}

void decColorMatrix::SetBrightness( float brightness ){
	SetBrightness( brightness, brightness, brightness );
}

void decColorMatrix::SetBrightness( float red, float green, float blue ){
	SetTranslation( red, green, blue, 0.0f );
}

void decColorMatrix::SetBrightness( const decColor &brightness ){
	SetBrightness( brightness.r, brightness.g, brightness.b );
}

void decColorMatrix::SetContrast( float contrast ){
	SetContrast( contrast, contrast, contrast );
}

void decColorMatrix::SetContrast( float red, float green, float blue ){
	const decColor scale( red, green, blue, 1.0f );
	const decColor translate( ( 1.0f - red ) * 0.5f, ( 1.0f - green ) * 0.5f, ( 1.0f - blue ) * 0.5f, 0.0f );
	SetST( scale, translate );
}

void decColorMatrix::SetContrast( const decColor &contrast ){
	SetContrast( contrast.r, contrast.g, contrast.b );
}

void decColorMatrix::SetSaturation( float saturation ){
	SetSaturation( saturation, saturation, saturation );
}

void decColorMatrix::SetSaturation( float red, float green, float blue ){
	const float lumRed = 0.3086f;
	const float lumGreen = 0.6094f;
	const float lumBlue = 0.0820f;
	
	const float sred = ( 1.0f - red ) * lumRed;
	const float sgreen = ( 1.0f - green ) * lumGreen;
	const float sblue = ( 1.0f - blue ) * lumBlue;
	
	a11 = sred + red;
	a12 = sgreen;
	a13 = sblue;
	a14 = 0.0f;
	a15 = 0.0f;
	
	a21 = sred;
	a22 = sgreen + green;
	a23 = sblue;
	a24 = 0.0f;
	a25 = 0.0f;
	
	a31 = sred;
	a32 = sgreen;
	a33 = sblue + blue;
	a34 = 0.0f;
	a35 = 0.0f;
	
	a41 = 0.0f;
	a42 = 0.0f;
	a43 = 0.0f;
	a44 = 1.0f;
	a45 = 0.0f;
}

void decColorMatrix::SetSaturation( const decColor &saturation ){
	SetSaturation( saturation.r, saturation.g, saturation.b );
}

void decColorMatrix::SetColorInversion(){
	a11 = -1.0f; a12 =  0.0f; a13 =  0.0f; a14 = 0.0f; a15 = 1.0f;
	a21 =  0.0f; a22 = -1.0f; a23 =  0.0f; a24 = 0.0f; a25 = 1.0f;
	a31 =  0.0f; a32 =  0.0f; a33 = -1.0f; a34 = 0.0f; a35 = 1.0f;
	a41 =  0.0f; a42 =  0.0f; a43 =  0.0f; a44 = 1.0f; a45 = 0.0f;
}



float decColorMatrix::Determinant() const{
	// TODO implement
	DETHROW( deeInvalidAction );
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

decColorMatrix decColorMatrix::Invert() const{
	// TODO implement
	DETHROW( deeInvalidAction );
	return *this;
	/*
	decColorMatrix m;
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

decColor decColorMatrix::GetRedBase() const{
	return decColor( a11, a21, a31, a41 );
}

decColor decColorMatrix::GetGreenBase() const{
	return decColor( a12, a22, a32, a42 );
}

decColor decColorMatrix::GetBlueBase() const{
	return decColor( a13, a23, a33, a43 );
}

decColor decColorMatrix::GetAlphaBase() const{
	return decColor( a14, a24, a34, a44 );
}

decColor decColorMatrix::GetWhiteBase() const{
	return decColor( a15, a25, a35, a45 );
}

void decColorMatrix::SetRedBase( const decColor &color ){
	a11 = color.r;
	a21 = color.g;
	a31 = color.b;
	a41 = color.a;
}

void decColorMatrix::SetGreenBase( const decColor &color ){
	a12 = color.r;
	a22 = color.g;
	a32 = color.b;
	a42 = color.a;
}

void decColorMatrix::SetBlueBase( const decColor &color ){
	a13 = color.r;
	a23 = color.g;
	a33 = color.b;
	a43 = color.a;
}

void decColorMatrix::SetAlphaBase( const decColor &color ){
	a14 = color.r;
	a24 = color.g;
	a34 = color.b;
	a44 = color.a;
}

void decColorMatrix::SetWhiteBase( const decColor &color ){
	a15 = color.r;
	a25 = color.g;
	a35 = color.b;
	a45 = color.a;
}

bool decColorMatrix::IsEqualTo( const decColorMatrix &matrix, float threshold ) const{
	return fabs( a11 - matrix.a11 ) < threshold
		&& fabs( a12 - matrix.a12 ) < threshold
		&& fabs( a13 - matrix.a13 ) < threshold
		&& fabs( a14 - matrix.a14 ) < threshold
		&& fabs( a15 - matrix.a15 ) < threshold
		&& fabs( a21 - matrix.a21 ) < threshold
		&& fabs( a22 - matrix.a22 ) < threshold
		&& fabs( a23 - matrix.a23 ) < threshold
		&& fabs( a24 - matrix.a24 ) < threshold
		&& fabs( a25 - matrix.a25 ) < threshold
		&& fabs( a31 - matrix.a31 ) < threshold
		&& fabs( a32 - matrix.a32 ) < threshold
		&& fabs( a33 - matrix.a33 ) < threshold
		&& fabs( a34 - matrix.a34 ) < threshold
		&& fabs( a35 - matrix.a35 ) < threshold
		&& fabs( a41 - matrix.a41 ) < threshold
		&& fabs( a42 - matrix.a42 ) < threshold
		&& fabs( a43 - matrix.a43 ) < threshold
		&& fabs( a44 - matrix.a44 ) < threshold
		&& fabs( a45 - matrix.a45 ) < threshold ;
}

decColor decColorMatrix::Transform( const decColor &color ) const{
	return decColor(
		a11 * color.r + a12 * color.g + a13 * color.b + a14 * color.a + a15,
		a21 * color.r + a22 * color.g + a23 * color.b + a24 * color.a + a25,
		a31 * color.r + a32 * color.g + a33 * color.b + a34 * color.a + a35,
		a41 * color.r + a42 * color.g + a43 * color.b + a44 * color.a + a45
	);
}

decColor decColorMatrix::Transform( float red, float green, float blue ) const{
	return decColor(
		a11 * red + a12 * green + a13 * blue + a14 + a15,
		a21 * red + a22 * green + a23 * blue + a24 + a25,
		a31 * red + a32 * green + a33 * blue + a34 + a35,
		a41 * red + a42 * green + a43 * blue + a44 + a45
	);
}

decColor decColorMatrix::Transform( float red, float green, float blue, float alpha ) const{
	return decColor(
		a11 * red + a12 * green + a13 * blue + a14 * alpha + a15,
		a21 * red + a22 * green + a23 * blue + a24 * alpha + a25,
		a31 * red + a32 * green + a33 * blue + a34 * alpha + a35,
		a41 * red + a42 * green + a43 * blue + a44 * alpha + a45
	);
}

decColor decColorMatrix::TransformColor( const decColor &color ) const{
	return decColor(
		a11 * color.r + a12 * color.g + a13 * color.b + a14 + a15,
		a21 * color.r + a22 * color.g + a23 * color.b + a24 + a25,
		a31 * color.r + a32 * color.g + a33 * color.b + a34 + a35
	);
}



// Operators
//////////////

decColorMatrix &decColorMatrix::operator=( const decColorMatrix &matrix ){
	a11 = matrix.a11;
	a12 = matrix.a12;
	a13 = matrix.a13;
	a14 = matrix.a14;
	a15 = matrix.a15;
	
	a21 = matrix.a21;
	a22 = matrix.a22;
	a23 = matrix.a23;
	a24 = matrix.a24;
	a25 = matrix.a25;
	
	a31 = matrix.a31;
	a32 = matrix.a32;
	a33 = matrix.a33;
	a34 = matrix.a34;
	a35 = matrix.a35;
	
	a41 = matrix.a41;
	a42 = matrix.a42;
	a43 = matrix.a43;
	a44 = matrix.a44;
	a45 = matrix.a45;
	
	return *this;
}

decColorMatrix &decColorMatrix::operator+=( const decColorMatrix &matrix ){
	a11 += matrix.a11;
	a12 += matrix.a12;
	a13 += matrix.a13;
	a14 += matrix.a14;
	a15 += matrix.a15;
	
	a21 += matrix.a21;
	a22 += matrix.a22;
	a23 += matrix.a23;
	a24 += matrix.a24;
	a25 += matrix.a25;
	
	a31 += matrix.a31;
	a32 += matrix.a32;
	a33 += matrix.a33;
	a34 += matrix.a34;
	a35 += matrix.a35;
	
	a41 += matrix.a41;
	a42 += matrix.a42;
	a43 += matrix.a43;
	a44 += matrix.a44;
	a45 += matrix.a45;
	
	return *this;
}

decColorMatrix &decColorMatrix::operator-=( const decColorMatrix &matrix ){
	a11 -= matrix.a11;
	a12 -= matrix.a12;
	a13 -= matrix.a13;
	a14 -= matrix.a14;
	a15 -= matrix.a15;
	
	a21 -= matrix.a21;
	a22 -= matrix.a22;
	a23 -= matrix.a23;
	a24 -= matrix.a24;
	a25 -= matrix.a25;
	
	a31 -= matrix.a31;
	a32 -= matrix.a32;
	a33 -= matrix.a33;
	a34 -= matrix.a34;
	a35 -= matrix.a35;
	
	a41 -= matrix.a41;
	a42 -= matrix.a42;
	a43 -= matrix.a43;
	a44 -= matrix.a44;
	a45 -= matrix.a45;
	
	return *this;
}

decColorMatrix &decColorMatrix::operator*=( float scalar ){
	// red
	a11 *= scalar;
	a12 *= scalar;
	a13 *= scalar;
	a14 *= scalar;
	a15 *= scalar;
	
	// green
	a21 *= scalar;
	a22 *= scalar;
	a23 *= scalar;
	a24 *= scalar;
	a25 *= scalar;
	
	// blue
	a31 *= scalar;
	a32 *= scalar;
	a33 *= scalar;
	a34 *= scalar;
	a35 *= scalar;
	
	// alpha
	a41 *= scalar;
	a42 *= scalar;
	a43 *= scalar;
	a44 *= scalar;
	a45 *= scalar;
	
	// return ourself
	return *this;
}

decColorMatrix &decColorMatrix::operator/=( float scalar ){
	if( scalar == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	return *this *= ( 1.0f / scalar );
}

decColorMatrix &decColorMatrix::operator*=( const decColorMatrix &matrix ){
	const float t11 = a11 * matrix.a11 + a21 * matrix.a12 + a31 * matrix.a13 + a41 * matrix.a14;
	const float t12 = a12 * matrix.a11 + a22 * matrix.a12 + a32 * matrix.a13 + a42 * matrix.a14;
	const float t13 = a13 * matrix.a11 + a23 * matrix.a12 + a33 * matrix.a13 + a43 * matrix.a14;
	const float t14 = a14 * matrix.a11 + a24 * matrix.a12 + a34 * matrix.a13 + a44 * matrix.a14;
	const float t15 = a15 * matrix.a11 + a25 * matrix.a12 + a35 * matrix.a13 + a45 * matrix.a14 + matrix.a15;
	
	const float t21 = a11 * matrix.a21 + a21 * matrix.a22 + a31 * matrix.a23 + a41 * matrix.a24;
	const float t22 = a12 * matrix.a21 + a22 * matrix.a22 + a32 * matrix.a23 + a42 * matrix.a24;
	const float t23 = a13 * matrix.a21 + a23 * matrix.a22 + a33 * matrix.a23 + a43 * matrix.a24;
	const float t24 = a14 * matrix.a21 + a24 * matrix.a22 + a34 * matrix.a23 + a44 * matrix.a24;
	const float t25 = a15 * matrix.a21 + a25 * matrix.a22 + a35 * matrix.a23 + a45 * matrix.a24 + matrix.a25;
	
	const float t31 = a11 * matrix.a31 + a21 * matrix.a32 + a31 * matrix.a33 + a41 * matrix.a34;
	const float t32 = a12 * matrix.a31 + a22 * matrix.a32 + a32 * matrix.a33 + a42 * matrix.a34;
	const float t33 = a13 * matrix.a31 + a23 * matrix.a32 + a33 * matrix.a33 + a43 * matrix.a34;
	const float t34 = a14 * matrix.a31 + a24 * matrix.a32 + a34 * matrix.a33 + a44 * matrix.a34;
	const float t35 = a15 * matrix.a31 + a25 * matrix.a32 + a35 * matrix.a33 + a45 * matrix.a34 + matrix.a35;
	
	const float t41 = a11 * matrix.a41 + a21 * matrix.a42 + a31 * matrix.a43 + a41 * matrix.a44;
	const float t42 = a12 * matrix.a41 + a22 * matrix.a42 + a32 * matrix.a43 + a42 * matrix.a44;
	const float t43 = a13 * matrix.a41 + a23 * matrix.a42 + a33 * matrix.a43 + a43 * matrix.a44;
	const float t44 = a14 * matrix.a41 + a24 * matrix.a42 + a34 * matrix.a43 + a44 * matrix.a44;
	const float t45 = a15 * matrix.a41 + a25 * matrix.a42 + a35 * matrix.a43 + a45 * matrix.a44 + matrix.a45;
	
	a11 = t11;
	a12 = t12;
	a13 = t13;
	a14 = t14;
	a15 = t15;
	
	a21 = t21;
	a22 = t22;
	a23 = t23;
	a24 = t24;
	a25 = t25;
	
	a31 = t31;
	a32 = t32;
	a33 = t33;
	a34 = t34;
	a35 = t35;
	
	a41 = t41;
	a42 = t42;
	a43 = t43;
	a44 = t44;
	a45 = t45;
	
	// return ourself
	return *this;
}

decColorMatrix decColorMatrix::operator+( const decColorMatrix &matrix ) const{
	decColorMatrix result;
	
	result.a11 = a11 + matrix.a11;
	result.a12 = a12 + matrix.a12;
	result.a13 = a13 + matrix.a13;
	result.a14 = a14 + matrix.a14;
	result.a15 = a15 + matrix.a15;
	
	result.a21 = a21 + matrix.a21;
	result.a22 = a22 + matrix.a22;
	result.a23 = a23 + matrix.a23;
	result.a24 = a24 + matrix.a24;
	result.a25 = a25 + matrix.a25;
	
	result.a31 = a31 + matrix.a31;
	result.a32 = a32 + matrix.a32;
	result.a33 = a33 + matrix.a33;
	result.a34 = a34 + matrix.a34;
	result.a35 = a35 + matrix.a35;
	
	result.a41 = a41 + matrix.a41;
	result.a42 = a42 + matrix.a42;
	result.a43 = a43 + matrix.a43;
	result.a44 = a44 + matrix.a44;
	result.a45 = a45 + matrix.a45;
	
	return result;
}

decColorMatrix decColorMatrix::operator-( const decColorMatrix &matrix ) const{
	decColorMatrix result;
	
	result.a11 = a11 - matrix.a11;
	result.a12 = a12 - matrix.a12;
	result.a13 = a13 - matrix.a13;
	result.a14 = a14 - matrix.a14;
	result.a15 = a15 - matrix.a15;
	
	result.a21 = a21 - matrix.a21;
	result.a22 = a22 - matrix.a22;
	result.a23 = a23 - matrix.a23;
	result.a24 = a24 - matrix.a24;
	result.a25 = a25 - matrix.a25;
	
	result.a31 = a31 - matrix.a31;
	result.a32 = a32 - matrix.a32;
	result.a33 = a33 - matrix.a33;
	result.a34 = a34 - matrix.a34;
	result.a35 = a35 - matrix.a35;
	
	result.a41 = a41 - matrix.a41;
	result.a42 = a42 - matrix.a42;
	result.a43 = a43 - matrix.a43;
	result.a44 = a44 - matrix.a44;
	result.a45 = a45 - matrix.a45;
	
	return result;
}

decColorMatrix decColorMatrix::operator*( float scalar ) const{
	decColorMatrix result;
	
	result.a11 = a11 * scalar;
	result.a12 = a12 * scalar;
	result.a13 = a13 * scalar;
	result.a14 = a14 * scalar;
	result.a15 = a15 * scalar;
	
	result.a21 = a21 * scalar;
	result.a22 = a22 * scalar;
	result.a23 = a23 * scalar;
	result.a24 = a24 * scalar;
	result.a25 = a25 * scalar;
	
	result.a31 = a31 * scalar;
	result.a32 = a32 * scalar;
	result.a33 = a33 * scalar;
	result.a34 = a34 * scalar;
	result.a35 = a35 * scalar;
	
	result.a41 = a41 * scalar;
	result.a42 = a42 * scalar;
	result.a43 = a43 * scalar;
	result.a44 = a44 * scalar;
	result.a45 = a45 * scalar;
	
	return result;
}

decColorMatrix decColorMatrix::operator/( float scalar ) const{
	if( scalar == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	return *this * ( 1.0f / scalar );
}

decColorMatrix decColorMatrix::operator*( const decColorMatrix &matrix ) const{
	decColorMatrix result;
	
	result.a11 = a11 * matrix.a11 + a21 * matrix.a12 + a31 * matrix.a13 + a41 * matrix.a14;
	result.a12 = a12 * matrix.a11 + a22 * matrix.a12 + a32 * matrix.a13 + a42 * matrix.a14;
	result.a13 = a13 * matrix.a11 + a23 * matrix.a12 + a33 * matrix.a13 + a43 * matrix.a14;
	result.a14 = a14 * matrix.a11 + a24 * matrix.a12 + a34 * matrix.a13 + a44 * matrix.a14;
	result.a15 = a15 * matrix.a11 + a25 * matrix.a12 + a35 * matrix.a13 + a45 * matrix.a14 + matrix.a15;
	
	result.a21 = a11 * matrix.a21 + a21 * matrix.a22 + a31 * matrix.a23 + a41 * matrix.a24;
	result.a22 = a12 * matrix.a21 + a22 * matrix.a22 + a32 * matrix.a23 + a42 * matrix.a24;
	result.a23 = a13 * matrix.a21 + a23 * matrix.a22 + a33 * matrix.a23 + a43 * matrix.a24;
	result.a24 = a14 * matrix.a21 + a24 * matrix.a22 + a34 * matrix.a23 + a44 * matrix.a24;
	result.a25 = a15 * matrix.a21 + a25 * matrix.a22 + a35 * matrix.a23 + a45 * matrix.a24 + matrix.a25;
	
	result.a31 = a11 * matrix.a31 + a21 * matrix.a32 + a31 * matrix.a33 + a41 * matrix.a34;
	result.a32 = a12 * matrix.a31 + a22 * matrix.a32 + a32 * matrix.a33 + a42 * matrix.a34;
	result.a33 = a13 * matrix.a31 + a23 * matrix.a32 + a33 * matrix.a33 + a43 * matrix.a34;
	result.a34 = a14 * matrix.a31 + a24 * matrix.a32 + a34 * matrix.a33 + a44 * matrix.a34;
	result.a35 = a15 * matrix.a31 + a25 * matrix.a32 + a35 * matrix.a33 + a45 * matrix.a34 + matrix.a35;
	
	result.a41 = a11 * matrix.a41 + a21 * matrix.a42 + a31 * matrix.a43 + a41 * matrix.a44;
	result.a42 = a12 * matrix.a41 + a22 * matrix.a42 + a32 * matrix.a43 + a42 * matrix.a44;
	result.a43 = a13 * matrix.a41 + a23 * matrix.a42 + a33 * matrix.a43 + a43 * matrix.a44;
	result.a44 = a14 * matrix.a41 + a24 * matrix.a42 + a34 * matrix.a43 + a44 * matrix.a44;
	result.a45 = a15 * matrix.a41 + a25 * matrix.a42 + a35 * matrix.a43 + a45 * matrix.a44 + matrix.a45;
	
	return result;
}

decColor decColorMatrix::operator*( const decColor &color ) const{
	return decColor(
		a11 * color.r + a12 * color.g + a13 * color.b + a14 * color.a + a15,
		a21 * color.r + a22 * color.g + a23 * color.b + a24 * color.a + a25,
		a31 * color.r + a32 * color.g + a33 * color.b + a34 * color.a + a35,
		a41 * color.r + a42 * color.g + a43 * color.b + a44 * color.a + a45
	);
}
