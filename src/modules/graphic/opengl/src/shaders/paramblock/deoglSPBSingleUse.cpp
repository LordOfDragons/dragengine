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

#include "deoglSPBSingleUse.h"
#include "../../renderthread/deoglRenderThread.h"


// Class deoglSPBSingleUse
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSPBSingleUse::deoglSPBSingleUse( const deoglRenderThread& renderThread,
	deoglShaderParameterBlock *block ) :
pRenderThread( renderThread ),
pInitialBlock( block ),
pFrameCounter( ~0 ),
pStartIndex( 0 ),
pNextIndex( 0 )
{
	DEASSERT_NOTNULL( block );
	pBlocks.Add( block );
}

deoglSPBSingleUse::~deoglSPBSingleUse(){
}



// Management
///////////////

deoglShaderParameterBlock *deoglSPBSingleUse::Next(){
	const uint32_t frameCounter = pRenderThread.GetFrameCounter();
	
	if( frameCounter == pFrameCounter ){
		if( pNextIndex == pStartIndex ){
			pBlocks.Insert( deoglShaderParameterBlock::Ref::New( pInitialBlock->Copy() ), pNextIndex );
			pStartIndex++;
		}
		
	}else{
		pFrameCounter = frameCounter;
		pStartIndex = pNextIndex;
	}
	
	deoglShaderParameterBlock * const block = ( deoglShaderParameterBlock* )pBlocks.GetAt( pNextIndex );
	pNextIndex = ( pNextIndex + 1 ) % pBlocks.GetCount();
	return block;
}
