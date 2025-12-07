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



// class decVector4
/////////////////////

// Constructor
////////////////

decVector4::decVector4(){
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

decVector4::decVector4(float nx, float ny, float nz){
	x = nx;
	y = ny;
	z = nz;
	w = 1.0f;
}

decVector4::decVector4(float nx, float ny, float nz, float nw){
	x = nx;
	y = ny;
	z = nz;
	w = nw;
}

decVector4::decVector4(const decVector4 &v){
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}



// Management
///////////////

float decVector4::Length(){
	return sqrtf(x * x + y * y + z * z);
}

void decVector4::Normalize(){
	const float len = sqrtf(x * x + y * y + z * z);
	
	if(len == 0.0f){
		DETHROW(deeDivisionByZero);
	}
	
	x /= len;
	y /= len;
	z /= len;
}

const decVector4 decVector4::Normalized() const{
	const float len = sqrtf(x * x + y * y + z * z);
	
	if(len == 0.0f){
		DETHROW(deeDivisionByZero);
	}
	
	return decVector4(x / len, y / len, z / len);
}

const decVector4 decVector4::Absolute() const{
	return decVector4(fabsf(x), fabsf(y), fabsf(z), fabsf(w));
}

void decVector4::SetZero(){
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

void decVector4::Set(float nx, float ny, float nz, float nw){
	x = nx;
	y = ny;
	z = nz;
	w = nw;
}

decVector4 decVector4::Mix (const decVector4 &vector, float blendFactor) const{
	const float factor1 = decMath::clamp(blendFactor, 0.0f, 1.0f);
	const float factor2 = 1.0f - factor1;
	return decVector4(
		x * factor2 + vector.x * factor1,
		y * factor2 + vector.y * factor1,
		z * factor2 + vector.z * factor1,
		w * factor2 + vector.w * factor1);
}

decVector4 decVector4::Multiply(const decVector4 &vector) const{
	return decVector4(x * vector.x, y * vector.y, z * vector.z, w * vector.w);
}



// Operators
//////////////

decVector4 decVector4::operator-(){
	return decVector4(-x, -y, -z);
}

decVector4 &decVector4::operator=(const decVector4 &v){
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

decVector4 &decVector4::operator+=(const decVector4 &v){
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

decVector4 &decVector4::operator-=(const decVector4 &v){
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

decVector4 &decVector4::operator*=(float k){
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

decVector4 &decVector4::operator/=(float k){
	if(k == 0.0f){
		DETHROW(deeDivisionByZero);
	}
	x /= k;
	y /= k;
	z /= k;
	return *this;
}

decVector4 decVector4::operator+(const decVector4 &v) const{
	return decVector4(x + v.x, y + v.y, z + v.z);
}

decVector4 decVector4::operator-(const decVector4 &v) const{
	return decVector4(x - v.x, y - v.y, z - v.z);
}

decVector4 decVector4::operator*(float k) const{
	return decVector4(x * k, y * k, z * k);
}

float decVector4::operator*(const decVector4 &v) const{
	return x * v.x + y * v.y + z * v.z;
}

decVector4 decVector4::operator/(float k) const{
	if(k == 0.0f){
		DETHROW(deeDivisionByZero);
	}
	return decVector4(x / k, y / k, z / k);
}

decVector4 decVector4::operator%(const decVector4 &v) const{
	return decVector4(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

bool decVector4::operator<(const decVector4 &v) const{
	return x < v.x && y < v.y && z < v.z;
}

bool decVector4::operator>(const decVector4 &v) const{
	return x > v.x && y > v.y && z > v.z;
}

bool decVector4::operator<=(const decVector4 &v) const{
	return x <= v.x && y <= v.y && z <= v.z;
}

bool decVector4::operator>=(const decVector4 &v) const{
	return x >= v.x && y >= v.y && z >= v.z;
}
