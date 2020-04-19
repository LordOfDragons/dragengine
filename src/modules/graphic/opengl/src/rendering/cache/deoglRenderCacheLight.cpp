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

#include "deoglRenderCacheLight.h"
#include "deoglRenderCacheLightShadow.h"
#include "../../light/deoglRLight.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderCacheLight
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderCacheLight::deoglRenderCacheLight( deoglRenderThread &renderThread ){
	pLight = NULL;
	
	pPrepared = false;
	pUseCubeMaps = false;
	pUseEncodedDepth = false;
	pUseTransparency = false;
	pMemoryConsumption = 0;
	
	pShadowSolid = NULL;
	pShadowTransparent = NULL;
	
	try{
		pShadowSolid = new deoglRenderCacheLightShadow( renderThread );
		if( ! pShadowSolid ) DETHROW( deeOutOfMemory );
		
		pShadowTransparent = new deoglRenderCacheLightShadow( renderThread );
		if( ! pShadowTransparent ) DETHROW( deeOutOfMemory );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderCacheLight::~deoglRenderCacheLight(){
	pCleanUp();
}



// Management
///////////////

void deoglRenderCacheLight::SetLight( deoglRLight *light ){
	if( light != pLight ){
		pLight = light;
		
		pPrepared = false;
		pUseCubeMaps = false;
		pUseEncodedDepth = false;
		pUseTransparency = false;
		pMemoryConsumption = 0;
		
		pShadowSolid->SetSize( 0 );
		pShadowSolid->SetMemoryConsumption( 0 );
		
		pShadowTransparent->SetSize( 0 );
		pShadowTransparent->SetMemoryConsumption( 0 );
	}
}



void deoglRenderCacheLight::SetPrepared( bool prepared ){
	pPrepared = prepared;
}

void deoglRenderCacheLight::SetUseCubeMaps( bool useCubeMaps ){
	pUseCubeMaps = useCubeMaps;
}

void deoglRenderCacheLight::SetUseEncodedDepth( bool useEncodedDepth ){
	pUseEncodedDepth = useEncodedDepth;
}

void deoglRenderCacheLight::SetUseTransparency( bool useTransparency ){
	pUseTransparency = useTransparency;
}



void deoglRenderCacheLight::SetMemoryConsumption( int consumption ){
	if( consumption < 0 ) DETHROW( deeInvalidParam );
	
	pMemoryConsumption = consumption;
}



// Private Functions
//////////////////////

void deoglRenderCacheLight::pCleanUp(){
	if( pShadowTransparent ) delete pShadowTransparent;
	if( pShadowSolid ) delete pShadowSolid;
}
