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



// Class decBoundary
/////////////////////

void decBoundary::OrientateRect(){
	if( x1 > x2 ){
		const int temp = x2;
		x2 = x1 + 1;
		x1 = temp + 1;
	}
	
	if( y1 > y2 ){
		const int temp = y2;
		y2 = y1 + 1;
		y1 = temp + 1;
	}
}

void decBoundary::OrientateLine(){
	if( x1 == x2 ){
		if( y1 > y2 ){
			const int temp = y2;
			y2 = y1 + 1;
			y1 = temp + 1;
		}
		
	}else if( x1 > x2 ){
		const int temp1 = y2;
		y2 = y1;
		y1 = temp1;
		
		const int temp2 = x2;
		x2 = x1;
		x1 = temp2;
		
		const int diffx = x2 - x1;
		const int diffy = y2 - y1;
		
		if( diffx > diffy * 0.5 ){
			x1++;
			x2++;
		}
		
		if( diffy > diffx * 0.5 ){
			y1++;
			y2++;
			
		}else if( -diffy > diffx * 0.5 ){
			y1--;
			y2--;
		}
	}
}



// Operators
//////////////

decBoundary &decBoundary::operator=( const decBoundary &b ){
	x1 = b.x1;
	y1 = b.y1;
	x2 = b.x2;
	y2 = b.y2;
	return *this;
}
