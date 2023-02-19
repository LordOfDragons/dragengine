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

#include "deoglCaches.h"
#include "deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deCacheHelper.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/deEngine.h>



// Class deoglCaches
//////////////////////

// Constructor, Destructor
////////////////////////////

deoglCaches::deoglCaches( deGraphicOpenGl &ogl ) :
pOgl( ogl ),
pSkinTextures( nullptr ),
pModels( nullptr ),
pShaders( nullptr )
{
	(void)pOgl;
	
	try{
		pSkinTextures = new deCacheHelper( &ogl.GetVFS(),
			decPath::CreatePathUnix( "/cache/local/skintextures" ) );
		
		pModels = new deCacheHelper( &ogl.GetVFS(),
			decPath::CreatePathUnix( "/cache/local/models" ) );
		
		pShaders = new deCacheHelper( &ogl.GetVFS(),
			decPath::CreatePathUnix( "/cache/global/shaders" ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglCaches::~deoglCaches(){
	pCleanUp();
}



// Management
///////////////

void deoglCaches::Lock(){
	pMutex.Lock();
}

void deoglCaches::Unlock(){
	pMutex.Unlock();
}



// Private Functions
//////////////////////

void deoglCaches::pCleanUp(){
	if( pModels ){
		delete pModels;
	}
	if( pSkinTextures ){
		delete pSkinTextures;
	}
	if( pShaders ){
		delete pShaders;
	}
}
