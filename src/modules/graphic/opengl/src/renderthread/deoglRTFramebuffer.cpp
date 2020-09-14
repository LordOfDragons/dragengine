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
#include <stdlib.h>
#include <string.h>

#include "deoglRTFramebuffer.h"
#include "deoglRenderThread.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"

#include <dragengine/common/exceptions.h>



// Class deoglRTFramebuffer
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTFramebuffer::deoglRTFramebuffer( deoglRenderThread &renderThread ) :
pManager( NULL ),
pPrimary( NULL ),
pActive( NULL ),
pEnvMap( NULL )
{
	try{
		pManager = new deoglFramebufferManager( renderThread );
		
		pPrimary = new deoglFramebuffer( renderThread, true );
		pActive = pPrimary;
		
		pEnvMap = new deoglFramebuffer( renderThread, false );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRTFramebuffer::~deoglRTFramebuffer(){
	pCleanUp();
}



// Management
///////////////

void deoglRTFramebuffer::Activate( deoglFramebuffer *framebuffer ){
	if( ! framebuffer ){
		framebuffer = pPrimary;
	}
	
	if( pActive == framebuffer ){
		return;
	}
	
	framebuffer->SetAsCurrent();
	pActive = framebuffer;
}



// Private Functions
//////////////////////

void deoglRTFramebuffer::pCleanUp(){
	Activate( pPrimary );
	
	if( pManager ){
		delete pManager;
	}
	
	if( pEnvMap ){
		delete pEnvMap;
	}
	if( pPrimary ){
		delete pPrimary;
	}
}
