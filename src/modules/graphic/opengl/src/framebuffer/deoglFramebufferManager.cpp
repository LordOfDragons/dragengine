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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "deoglFramebuffer.h"
#include "deoglFramebufferManager.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglFramebufferManager
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglFramebufferManager::deoglFramebufferManager( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pFBOs( NULL ),
pFBOCount( 0 ),
pFBOSize( 0 ){
}

deoglFramebufferManager::~deoglFramebufferManager(){
	int i;
	
	if( pFBOs ){
		for( i=0; i<pFBOCount; i++ ){
			delete pFBOs[ i ];
		}
		
		delete [] pFBOs;
	}
}



// Management
///////////////



// Framebuffer Management
///////////////////////////

const deoglFramebuffer *deoglFramebufferManager::GetFBOAt( int index ) const{
	if( index < 0 || index >= pFBOCount ) DETHROW( deeInvalidParam );
	return pFBOs[ index ];
}

deoglFramebuffer *deoglFramebufferManager::GetFBOWithResolution( int width, int height ){
	// if we have to hand out only one fbo use the first one
	if( pRenderThread.GetConfiguration().GetUseOneFBO() ){
		// make sure one fbo exists
		if( pFBOCount == 0 ){
			if( pFBOSize == 0 ){
				pFBOs = new deoglFramebuffer*[ 1 ];
				if( ! pFBOs ) DETHROW( deeOutOfMemory );
				pFBOSize = 1;
			}
			
			pFBOs[ 0 ] = new deoglFramebuffer( pRenderThread, false );
			if( ! pFBOs[ 0 ] ) DETHROW( deeOutOfMemory );
			pFBOCount = 1;
		}
		
		pFBOs[ 0 ]->SetUsageResolution( width, height );
		pFBOs[ 0 ]->IncreaseUsageCount();
		
		return pFBOs[ 0 ];
		
	// otherwise hand out an fbo for each resolution
	}else{
		//deoglFramebuffer *reusableFBO = NULL;
		int i;
		
		// try to find a framebuffer with the same dimensions or one that is reusable
		for( i=0; i<pFBOCount; i++ ){
			if( pFBOs[ i ]->GetUsageWidth() == width && pFBOs[ i ]->GetUsageHeight() == height ){
				pFBOs[ i ]->IncreaseUsageCount();
				return pFBOs[ i ];
			//	
			//}else if( ! reusableFBO && pFBOs[ i ]->GetUsageCount() == 0 ){
			//	reusableFBO = pFBOs[ i ];
			}
		}
		
		// if we have a reusable fbo change the size and hand it out
		//if( reusableFBO ){
		//	pRenderThread.GetLogger().LogInfoFormat( "FBO Manager: Resize FBO ( %i x %i )", width, height );
		//	reusableFBO->SetUsageResolution( width, height );
		//	reusableFBO->IncreaseUsageCount();
		//	return reusableFBO;
		//}
		
		// otherwise create a new fbo with the given size and hand it out
		if( pFBOCount == pFBOSize ){
			int newSize = pFBOSize * 3 / 2 + 1;
			deoglFramebuffer **newArray = new deoglFramebuffer*[ newSize ];
			if( ! newArray ) DETHROW( deeOutOfMemory );
			if( pFBOs ){
				memcpy( newArray, pFBOs, sizeof( deoglFramebuffer* ) * pFBOSize );
				delete [] pFBOs;
			}
			pFBOs = newArray;
			pFBOSize = newSize;
		}
		
		pFBOs[ pFBOCount ] = new deoglFramebuffer( pRenderThread, false );
		if( ! pFBOs[ pFBOCount ] ) DETHROW( deeOutOfMemory );
		pFBOCount++;
		
		pFBOs[ pFBOCount - 1 ]->SetUsageResolution( width, height );
		pFBOs[ pFBOCount - 1 ]->IncreaseUsageCount();
		
		pRenderThread.GetLogger().LogInfoFormat( "FBO Manager: Create FBO ( %i x %i )", width, height );
		return pFBOs[ pFBOCount - 1 ];
	}
}
