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
#include <stdlib.h>
#include <string.h>

#include "deoglComponent.h"
#include "deoglComponentLOD.h"
#include "deoglRComponentLOD.h"

#include <dragengine/common/exceptions.h>



// Class deoglComponentLOD
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglComponentLOD::deoglComponentLOD( deoglComponent &component, int lodIndex ) :
pComponent( component ),
pLODIndex( lodIndex ),
pRLOD( NULL )
{
	try{
		pRLOD = new deoglRComponentLOD( *component.GetRComponent(), lodIndex );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglComponentLOD::~deoglComponentLOD(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void deoglComponentLOD::pCleanUp(){
	if( pRLOD ){
		pRLOD->FreeReference();
	}
}
