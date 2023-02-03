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

#include "deoglSPBMapBuffer.h"
#include "deoglShaderParameterBlock.h"


// Class deoglSPBMapBuffer
////////////////////////////

// Constructor, destructor
////////////////////////////


deoglSPBMapBuffer::deoglSPBMapBuffer( deoglShaderParameterBlock &block ) :
pBlock( block ),
pElement( -1 ),
pMapped( false )
{
	Map();
}

deoglSPBMapBuffer::deoglSPBMapBuffer( deoglShaderParameterBlock &block, int element ) :
pBlock( block ),
pElement( element ),
pMapped( false )
{
	Map();
}

deoglSPBMapBuffer::~deoglSPBMapBuffer(){
	Unmap();
}



// Management
///////////////

void deoglSPBMapBuffer::Map(){
	if( pMapped ){
		return;
	}
	
	if( pElement > -1 ){
		pBlock.MapBuffer( pElement );
		
	}else{
		pBlock.MapBuffer();
	}
	
	pMapped = true;
}

void deoglSPBMapBuffer::Unmap(){
	if( ! pMapped ){
		return;
	}
	
	pBlock.UnmapBuffer();
	pMapped = false;
}
