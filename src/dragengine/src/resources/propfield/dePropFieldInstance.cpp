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

#include <stdio.h>
#include <stdlib.h>

#include "dePropFieldInstance.h"

#include "../../common/exceptions.h"



// Class dePropFieldInstance
//////////////////////////////

// Constructor, destructor
////////////////////////////

dePropFieldInstance::dePropFieldInstance(){
	pScaling = 1.0f;
	pBendState = -1;
}

dePropFieldInstance::~dePropFieldInstance(){
}



// Management
///////////////

void dePropFieldInstance::SetPosition( const decVector &position ){
	pPosition = position;
}

void dePropFieldInstance::SetRotation( const decVector &rotation ){
	pRotation = rotation;
}

void dePropFieldInstance::SetScaling( float scaling ){
	pScaling = scaling;
}

void dePropFieldInstance::SetBendState( int state ){
	pBendState = state;
}
