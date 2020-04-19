/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglLPTProbe.h"



// Class deoglLPTProbe
////////////////////////

// Constructor, destructor
////////////////////////////

deoglLPTProbe::deoglLPTProbe(){
}

deoglLPTProbe::~deoglLPTProbe(){
}



// Management
///////////////

void deoglLPTProbe::SetPosition( const decVector &position ){
	pPosition = position;
}

void deoglLPTProbe::SetTextureCoordinates( const decPoint &tc ){
	pTexCoords = tc;
}

void deoglLPTProbe::SetTextureCoordinates( int x, int y ){
	pTexCoords.Set( x, y );
}
