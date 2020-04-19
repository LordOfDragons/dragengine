/* 
 * Drag[en]gine Bullet Physics Module
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debpDecalFace.h"
#include "dragengine/common/exceptions.h"



// Class debpDecalFace
////////////////////////

// Constructor, destructor
////////////////////////////

debpDecalFace::debpDecalFace(){
}

debpDecalFace::~debpDecalFace(){
}



// Management
///////////////

void debpDecalFace::SetNormal( const decVector &normal ){
	pNormal = normal;
}

void debpDecalFace::SetPoint1( const decVector &point ){
	pPoints[ 0 ] = point;
}

void debpDecalFace::SetPoint2( const decVector &point ){
	pPoints[ 1 ] = point;
}

void debpDecalFace::SetPoint3( const decVector &point ){
	pPoints[ 2 ] = point;
}
