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



// Class decBoundary
/////////////////////

void decBoundary::OrientateRect(){
	if(x1 > x2){
		const int temp = x2;
		x2 = x1 + 1;
		x1 = temp + 1;
	}
	
	if(y1 > y2){
		const int temp = y2;
		y2 = y1 + 1;
		y1 = temp + 1;
	}
}

void decBoundary::OrientateLine(){
	if(x1 == x2){
		if(y1 > y2){
			const int temp = y2;
			y2 = y1 + 1;
			y1 = temp + 1;
		}
		
	}else if(x1 > x2){
		const int temp1 = y2;
		y2 = y1;
		y1 = temp1;
		
		const int temp2 = x2;
		x2 = x1;
		x1 = temp2;
		
		const int diffx = x2 - x1;
		const int diffy = y2 - y1;
		
		if(diffx > diffy * 0.5){
			x1++;
			x2++;
		}
		
		if(diffy > diffx * 0.5){
			y1++;
			y2++;
			
		}else if(-diffy > diffx * 0.5){
			y1--;
			y2--;
		}
	}
}



// Operators
//////////////

decBoundary &decBoundary::operator=(const decBoundary &b){
	x1 = b.x1;
	y1 = b.y1;
	x2 = b.x2;
	y2 = b.y2;
	return *this;
}
