/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglRenderTaskSharedPool.h"
#include "deoglRenderTaskSharedSubInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskSharedSubInstance
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskSharedSubInstance::deoglRenderTaskSharedSubInstance( deoglRenderTaskSharedPool &pool, int index ) :
pPool( pool ),
pIndex( index ),
pGroupIndex( 0 ),
pFlags( 0 ){
}

deoglRenderTaskSharedSubInstance::~deoglRenderTaskSharedSubInstance(){
}



// Management
///////////////

void deoglRenderTaskSharedSubInstance::SetGroupIndex( int index ){
	pGroupIndex = index;
}

void deoglRenderTaskSharedSubInstance::SetFlags( int flags ){
	pFlags = flags;
}



void deoglRenderTaskSharedSubInstance::Clear(){
	pGroupIndex = 0;
	pFlags = 0;
}

void deoglRenderTaskSharedSubInstance::ReturnToPool(){
	pPool.ReturnSubInstance( this );
}
