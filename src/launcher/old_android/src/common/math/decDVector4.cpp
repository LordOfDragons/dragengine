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



// class decDVector4
//////////////////////

// Constructor
////////////////

decDVector4::decDVector4(){
	x = 0.0;
	y = 0.0;
	z = 0.0;
	w = 1.0;
}

decDVector4::decDVector4(double nx, double ny, double nz){
	x = nx;
	y = ny;
	z = nz;
	w = 1.0;
}

decDVector4::decDVector4(double nx, double ny, double nz, double nw){
	x = nx;
	y = ny;
	z = nz;
	w = nw;
}

decDVector4::decDVector4(const decDVector4 &v){
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

decDVector4::decDVector4(const decVector4 &v){
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}



// Management
///////////////

double decDVector4::Length(){
	return sqrt(x * x + y * y + z * z);
}

void decDVector4::Normalize(){
	const double len = sqrt(x * x + y * y + z * z);
	
	if(len == 0.0){
		DETHROW(deeDivisionByZero);
	}
	
	x /= len;
	y /= len;
	z /= len;
}

const decDVector4 decDVector4::Normalized() const{
	const double len = sqrt(x * x + y * y + z * z);
	
	if(len == 0.0){
		DETHROW(deeDivisionByZero);
	}
	
	return decDVector4(x / len, y / len, z / len);
}

const decDVector4 decDVector4::Absolute() const{
	return decDVector4(fabs(x), fabs(y), fabs(z), fabs(w));
}

void decDVector4::SetZero(){
	x = 0.0;
	y = 0.0;
	z = 0.0;
	w = 1.0;
}

void decDVector4::Set(double nx, double ny, double nz, double nw){
	x = nx;
	y = ny;
	z = nz;
	w = nw;
}

decVector4 decDVector4::ToVector4() const{
	return decVector4((double)x, (double)y, (double)z, (double)w);
}



// Operators
//////////////

decDVector4 decDVector4::operator-(){
	return decDVector4(-x, -y, -z);
}

decDVector4 &decDVector4::operator=(const decDVector4 &v){
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

decDVector4 &decDVector4::operator+=(const decDVector4 &v){
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

decDVector4 &decDVector4::operator-=(const decDVector4 &v){
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

decDVector4 &decDVector4::operator*=(double k){
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

decDVector4 &decDVector4::operator/=(double k){
	if(k == 0.0){
		DETHROW(deeDivisionByZero);
	}
	
	x /= k;
	y /= k;
	z /= k;
	
	return *this;
}

decDVector4 decDVector4::operator+(const decDVector4 &v) const{
	return decDVector4(x + v.x, y + v.y, z + v.z);
}

decDVector4 decDVector4::operator-(const decDVector4 &v) const{
	return decDVector4(x - v.x, y - v.y, z - v.z);
}

decDVector4 decDVector4::operator*(double k) const{
	return decDVector4(x * k, y * k, z * k);
}

double decDVector4::operator*(const decDVector4 &v) const{
	return x * v.x + y * v.y + z * v.z;
}

decDVector4 decDVector4::operator/(double k) const{
	if(k == 0.0){
		DETHROW(deeDivisionByZero);
	}
	return decDVector4(x / k, y / k, z / k);
}

decDVector4 decDVector4::operator%(const decDVector4 &v) const{
	return decDVector4(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

bool decDVector4::operator<(const decDVector4 &v) const{
	return x < v.x && y < v.y && z < v.z;
}

bool decDVector4::operator>(const decDVector4 &v) const{
	return x > v.x && y > v.y && z > v.z;
}

bool decDVector4::operator<=(const decDVector4 &v) const{
	return x <= v.x && y <= v.y && z <= v.z;
}

bool decDVector4::operator>=(const decDVector4 &v) const{
	return x >= v.x && y >= v.y && z >= v.z;
}
