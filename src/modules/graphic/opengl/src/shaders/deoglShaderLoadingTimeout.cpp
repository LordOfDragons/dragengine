/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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
#include <ctype.h>

#include "deoglShaderLoadingTimeout.h"

#include <dragengine/common/exceptions.h>



// Class deoglShaderLoadingTimeout
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderLoadingTimeout::deoglShaderLoadingTimeout( float timeout ) :
pInitialTimeout( timeout ),
pTimeout( timeout ),
pFirst( true ),
pTimedOut( false ){
}

deoglShaderLoadingTimeout::~deoglShaderLoadingTimeout(){
}




// Management
///////////////
bool deoglShaderLoadingTimeout::CanLoad(){
	if( pFirst ){
		return true;
	}
	if( pTimedOut ){
		return false;
	}
	
	pTimeout -= pTimer.GetElapsedTime();
	pTimedOut = pTimeout <= 0.0f;
	return ! pTimedOut;
}

void deoglShaderLoadingTimeout::Loaded(){
	pFirst = false;
}
