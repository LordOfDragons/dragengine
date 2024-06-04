/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
