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



// Class decPoint
///////////////////

// Constructor
////////////////

decPoint::decPoint() :
x(0),
y(0){
}

decPoint::decPoint(int nx, int ny) :
x(nx),
y(ny){
}

decPoint::decPoint(const decPoint &p) :
x(p.x),
y(p.y){
}

decPoint::decPoint(const decVector2 &v) :
x((int)v.x),
y((int)v.y){
}



// Management
///////////////

float decPoint::Length() const{
	return sqrtf((float)(x * x + y * y));
}

void decPoint::SetZero(){
	x = 0;
	y = 0;
}

void decPoint::Set(int nx, int ny){
	x = nx;
	y = ny;
}

decPoint decPoint::Absolute() const{
	return decPoint(x < 0 ? -x : x, y < 0 ? -y : y);
}

void decPoint::SetSmallest(const decPoint &p){
	if(p.x < x){
		x = p.x;
	}
	if(p.y < y){
		y = p.y;
	}
}

decPoint decPoint::Smallest(const decPoint &p) const{
	return decPoint(
		(x < p.x) ? x : p.x,
		(y < p.y) ? y : p.y
);
}

void decPoint::SetLargest(const decPoint &p){
	if(p.x > x){
		x = p.x;
	}
	if(p.y > y){
		y = p.y;
	}
}

decPoint decPoint::Largest(const decPoint &p) const{
	return decPoint(
		(x > p.x) ? x : p.x,
		(y > p.y) ? y : p.y
);
}

void decPoint::SetClamped(const decPoint &minValue, const decPoint &maxValue){
	if(x < minValue.x){
		x = minValue.x;
		
	}else if(x > maxValue.x){
		x = maxValue.x;
	}
	
	if(y < minValue.y){
		y = minValue.y;
		
	}else if(y > maxValue.y){
		y = maxValue.y;
	}
}

decPoint decPoint::Clamped(const decPoint &minValue, const decPoint &maxValue) const{
	return decPoint(
		(x < minValue.x) ? minValue.x : ((x > maxValue.x) ? maxValue.x : x),
		(y < minValue.y) ? minValue.y : ((y > maxValue.y) ? maxValue.y : y)
);
}



// Operators
//////////////

decPoint decPoint::operator-() const{
	return decPoint(-x, -y);
}

decPoint &decPoint::operator=(const decPoint &p){
	x = p.x;
	y = p.y;
	return *this;
}

decPoint &decPoint::operator+=(const decPoint &p){
	x += p.x;
	y += p.y;
	return *this;
}

decPoint &decPoint::operator-=(const decPoint &p){
	x -= p.x;
	y -= p.y;
	return *this;
}

decPoint &decPoint::operator*=(int k){
	x = x * k;
	y = x * k;
	return *this;
}

decPoint &decPoint::operator/=(int k){
	if(k == 0){
		DETHROW(deeDivisionByZero);
	}
	x /= k;
	y /= k;
	return *this;
}

decPoint decPoint::operator+(const decPoint &p) const{
	return decPoint(x + p.x, y + p.y);
}

decPoint decPoint::operator-(const decPoint &p) const{
	return decPoint(x - p.x, y - p.y);
}

decPoint decPoint::operator*(int k) const{
	return decPoint(x * k, y * k);
}

int decPoint::operator*(const decPoint &p) const{
	return x * p.x + y * p.y;
}

decPoint decPoint::operator/(int k) const{
	if(k == 0){
		DETHROW(deeDivisionByZero);
	}
	return decPoint(x / k, y / k);
}

bool decPoint::operator==(const decPoint &p) const{
	return x == p.x && y == p.y;
}

bool decPoint::operator!=(const decPoint &p) const{
	return x != p.x || y != p.y;
}

bool decPoint::operator<(const decPoint &p) const{
	return x < p.x && y < p.y;
}

bool decPoint::operator>(const decPoint &p) const{
	return x > p.x && y > p.y;
}

bool decPoint::operator<=(const decPoint &p) const{
	return x <= p.x && y <= p.y;
}

bool decPoint::operator>=(const decPoint &p) const{
	return x >= p.x && y >= p.y;
}
