/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "deoglSharedSPBElementMapBuffer.h"
#include "deoglSharedSPBElement.h"
#include "../deoglShaderParameterBlock.h"


// Class deoglSharedSPBElementMapBuffer
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////


deoglSharedSPBElementMapBuffer::deoglSharedSPBElementMapBuffer( deoglSharedSPBElement &element ) :
pElement( element ),
pBlock( nullptr )
{
	Map();
}

deoglSharedSPBElementMapBuffer::~deoglSharedSPBElementMapBuffer(){
	Unmap();
}



// Management
///////////////

void deoglSharedSPBElementMapBuffer::Map(){
	if( pBlock ){
		return;
	}
	
	pBlock = &pElement.MapBuffer();
}

deoglShaderParameterBlock &deoglSharedSPBElementMapBuffer::GetBlockRef() const{
	DEASSERT_NOTNULL( pBlock );
	return *pBlock;
}

void deoglSharedSPBElementMapBuffer::Unmap(){
	if( ! pBlock ){
		return;
	}
	
	pBlock->UnmapBuffer();
	pBlock = nullptr;
}
