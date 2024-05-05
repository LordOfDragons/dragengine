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




// Class decTexMatrix
//////////////////////

// Constructor
////////////////

decTexMatrix::decTexMatrix(){
	a11 = 1.0f; a12 = 0.0f; a13 = 0.0f;
	a21 = 0.0f; a22 = 1.0f; a23 = 0.0f;
	a31 = 0.0f; a32 = 0.0f; a33 = 1.0f;
}



// Creation
/////////////

decTexMatrix decTexMatrix::CreateIdentity(){
	decTexMatrix m;
	
	m.a11 = 1.0f; m.a12 = 0.0f; m.a13 = 0.0f;
	m.a21 = 0.0f; m.a22 = 1.0f; m.a23 = 0.0f;
	m.a31 = 0.0f; m.a32 = 0.0f; m.a33 = 1.0f;
	
	return m;
}

decTexMatrix decTexMatrix::CreateTranslation( float u, float v ){
	decTexMatrix m;
	
	m.a11 = 1.0f; m.a12 = 0.0f; m.a13 = u;
	m.a21 = 0.0f; m.a22 = 1.0f; m.a23 = v;
	m.a31 = 0.0f; m.a32 = 0.0f; m.a33 = 1.0f;
	
	return m;
}

decTexMatrix decTexMatrix::CreateTranslation( const decVector2 &t ){
	decTexMatrix m;
	
	m.a11 = 1.0f; m.a12 = 0.0f; m.a13 = t.x;
	m.a21 = 0.0f; m.a22 = 1.0f; m.a23 = t.y;
	m.a31 = 0.0f; m.a32 = 0.0f; m.a33 = 1.0f;
	
	return m;
}

decTexMatrix decTexMatrix::CreateScale( float su, float sv ){
	decTexMatrix m;
	
	m.a11 = su;   m.a12 = 0.0f; m.a13 = 0.0f;
	m.a21 = 0.0f; m.a22 = sv;   m.a23 = 0.0f;
	m.a31 = 0.0f; m.a32 = 0.0f; m.a33 = 1.0f;
	
	return m;
}

decTexMatrix decTexMatrix::CreateScale( const decVector2 &s ){
	decTexMatrix m;
	
	m.a11 = s.x;  m.a12 = 0.0f; m.a13 = 0.0f;
	m.a21 = 0.0f; m.a22 = s.y;  m.a23 = 0.0f;
	m.a31 = 0.0f; m.a32 = 0.0f; m.a33 = 1.0f;
	
	return m;
}

decTexMatrix decTexMatrix::CreateRotation( float arc ){
	const float s = sinf( arc );
	const float c = cosf( arc );
	decTexMatrix m;
	
	m.a11 = c;    m.a12 = -s;   m.a13 = 0.0f;
	m.a21 = s;    m.a22 = c;    m.a23 = 0.0f;
	m.a31 = 0.0f; m.a32 = 0.0f; m.a33 = 1.0f;
	
	return m;
}

decTexMatrix decTexMatrix::CreateShear( float shearU, float shearV ){
	decTexMatrix m;
	
	m.a11 =   1.0f; m.a12 = shearU; m.a13 = 0.0f;
	m.a21 = shearV; m.a22 =   1.0f; m.a23 = 0.0f;
	m.a31 =   0.0f; m.a32 =   0.0f; m.a33 = 1.0f;
	
	return m;
}

decTexMatrix decTexMatrix::CreateShear( const decVector2 &shear ){
	return CreateShear( shear.x, shear.y );
}



// Management
///////////////

void decTexMatrix::SetIdentity(){
	a11 = 1.0f; a12 = 0.0f; a13 = 0.0f;
	a21 = 0.0f; a22 = 1.0f; a23 = 0.0f;
	a31 = 0.0f; a32 = 0.0f; a33 = 1.0f;
}

bool decTexMatrix::IsEqualTo( const decTexMatrix &matrix, float threshold ) const{
	return fabs( a11 - matrix.a11 ) < threshold
		&& fabs( a12 - matrix.a12 ) < threshold
		&& fabs( a13 - matrix.a13 ) < threshold
		&& fabs( a21 - matrix.a21 ) < threshold
		&& fabs( a22 - matrix.a22 ) < threshold
		&& fabs( a23 - matrix.a23 ) < threshold
		&& fabs( a31 - matrix.a31 ) < threshold
		&& fabs( a32 - matrix.a32 ) < threshold
		&& fabs( a33 - matrix.a33 ) < threshold;
}



float decTexMatrix::Determinant() const{
	return -a13*a22*a31 + a12*a23*a31 + a13*a21*a32 - a11*a23*a32 - a12*a21*a33 + a11*a22*a33;
}

decTexMatrix decTexMatrix::Invert() const{
	decTexMatrix m;
	
	m.a11 = a22*a33 - a23*a32;
	m.a12 = a13*a32 - a12*a33;
	m.a13 = a12*a23 - a13*a22;
	m.a21 = a23*a31 - a21*a33;
	m.a22 = a11*a33 - a13*a31;
	m.a23 = a13*a21 - a11*a23;
	m.a31 = a21*a32 - a22*a31;
	m.a32 = a12*a31 - a11*a32;
	m.a33 = a11*a22 - a12*a21;
	
	return m / Determinant();
}



decTexMatrix2 decTexMatrix::ToTexMatrix2() const{
	decTexMatrix2 m;
	
	m.a11 = a11; m.a12 = a12; m.a13 = a13;
	m.a21 = a21; m.a22 = a22; m.a23 = a23;
	
	return m;
}



// Operators
//////////////

decTexMatrix &decTexMatrix::operator=( const decTexMatrix &m ){
	a11 = m.a11; a12 = m.a12; a13 = m.a13;
	a21 = m.a21; a22 = m.a22; a23 = m.a23;
	a31 = m.a31; a32 = m.a32; a33 = m.a33;
	return *this;
}

decTexMatrix &decTexMatrix::operator+=( const decTexMatrix &m ){
	a11 += m.a11; a12 += m.a12; a13 += m.a13;
	a21 += m.a21; a22 += m.a22; a23 += m.a23;
	a31 += m.a31; a32 += m.a32; a33 += m.a33;
	return *this;
}

decTexMatrix &decTexMatrix::operator-=( const decTexMatrix &m ){
	a11 -= m.a11; a12 -= m.a12; a13 -= m.a13;
	a21 -= m.a21; a22 -= m.a22; a23 -= m.a23;
	a31 -= m.a31; a32 -= m.a32; a33 -= m.a33;
	return *this;
}

decTexMatrix &decTexMatrix::operator*=( float k ){
	a11 *= k; a12 *= k; a13 *= k;
	a21 *= k; a22 *= k; a23 *= k;
	a31 *= k; a32 *= k; a33 *= k;
	return *this;
}

decTexMatrix &decTexMatrix::operator/=( float k ){
	if( k == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	a11 /= k; a12 /= k; a13 /= k;
	a21 /= k; a22 /= k; a23 /= k;
	a31 /= k; a32 /= k; a33 /= k;
	
	return *this;
}

decTexMatrix &decTexMatrix::operator*=( const decTexMatrix &m ){
	const float t11 = a11 * m.a11 + a21 * m.a12 + a31 * m.a13;
	const float t12 = a12 * m.a11 + a22 * m.a12 + a32 * m.a13;
	const float t13 = a13 * m.a11 + a23 * m.a12 + a33 * m.a13;
	const float t21 = a11 * m.a21 + a21 * m.a22 + a31 * m.a23;
	const float t22 = a12 * m.a21 + a22 * m.a22 + a32 * m.a23;
	const float t23 = a13 * m.a21 + a23 * m.a22 + a33 * m.a23;
	const float t31 = a11 * m.a31 + a21 * m.a32 + a31 * m.a33;
	const float t32 = a12 * m.a31 + a22 * m.a32 + a32 * m.a33;
	const float t33 = a13 * m.a31 + a23 * m.a32 + a33 * m.a33;
	
	a11 = t11; a12 = t12; a13 = t13;
	a21 = t21; a22 = t22; a23 = t23;
	a31 = t31; a32 = t32; a33 = t33;
	
	return *this;
}

decTexMatrix decTexMatrix::operator+( const decTexMatrix &m ) const{
	decTexMatrix n;
	
	n.a11 = a11 + m.a11; n.a12 = a12 + m.a12; n.a13 = a13 + m.a13;
	n.a21 = a21 + m.a21; n.a22 = a22 + m.a22; n.a23 = a23 + m.a23;
	n.a31 = a31 + m.a31; n.a32 = a32 + m.a32; n.a33 = a33 + m.a33;
	
	return n;
}

decTexMatrix decTexMatrix::operator-( const decTexMatrix &m ) const{
	decTexMatrix n;
	
	n.a11 = a11 - m.a11; n.a12 = a12 - m.a12; n.a13 = a13 - m.a13;
	n.a21 = a21 - m.a21; n.a22 = a22 - m.a22; n.a23 = a23 - m.a23;
	n.a31 = a31 - m.a31; n.a32 = a32 - m.a32; n.a33 = a33 - m.a33;
	
	return n;
}

decTexMatrix decTexMatrix::operator*( float k ) const{
	decTexMatrix n;
	
	n.a11 = a11 * k; n.a12 = a12 * k; n.a13 = a13 * k;
	n.a21 = a21 * k; n.a22 = a22 * k; n.a23 = a23 * k;
	n.a31 = a31 * k; n.a32 = a32 * k; n.a33 = a33 * k;
	
	return n;
}

decTexMatrix decTexMatrix::operator/( float k ) const{
	if( k == 0.0f ){
		DETHROW( deeDivisionByZero );
	}
	
	decTexMatrix n;
	
	n.a11 = a11 / k; n.a12 = a12 / k; n.a13 = a13 / k;
	n.a21 = a21 / k; n.a22 = a22 / k; n.a23 = a23 / k;
	n.a31 = a31 / k; n.a32 = a32 / k; n.a33 = a33 / k;
	
	return n;
}

decTexMatrix decTexMatrix::operator*( const decTexMatrix &m ) const{
	decTexMatrix n;
	
	n.a11 = a11 * m.a11 + a21 * m.a12 + a31 * m.a13;
	n.a12 = a12 * m.a11 + a22 * m.a12 + a32 * m.a13;
	n.a13 = a13 * m.a11 + a23 * m.a12 + a33 * m.a13;
	n.a21 = a11 * m.a21 + a21 * m.a22 + a31 * m.a23;
	n.a22 = a12 * m.a21 + a22 * m.a22 + a32 * m.a23;
	n.a23 = a13 * m.a21 + a23 * m.a22 + a33 * m.a23;
	n.a31 = a11 * m.a31 + a21 * m.a32 + a31 * m.a33;
	n.a32 = a12 * m.a31 + a22 * m.a32 + a32 * m.a33;
	n.a33 = a13 * m.a31 + a23 * m.a32 + a33 * m.a33;
	n.a31 = 0.0f;
	n.a32 = 0.0f;
	n.a33 = 1.0f;
	
	return n;
}

decVector2 decTexMatrix::operator*( const decVector2 &v ) const{
	return decVector2(
		a11 * v.x + a12 * v.y + a13,
		a21 * v.x + a22 * v.y + a23 );
}
