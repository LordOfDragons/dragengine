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

#include "deoglCubeMap.h"
#include "deoglRenderableDepthCubeMap.h"
#include "../../deoglBasics.h"
#include "../../memory/deoglMemoryManager.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableDepthCubeMap
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableDepthCubeMap::deoglRenderableDepthCubeMap( deoglRenderThread &renderThread, int size ) :
pSize( size ),
pInUse( false ),
pCubeMap( NULL ),
pMemoryUsageGPU( 0 )
{
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		pCubeMap = new deoglCubeMap( renderThread );
		pCubeMap->SetSize( size );
		pCubeMap->SetDepthFormat();
		pCubeMap->CreateCubeMap();
		UpdateMemoryUsage();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderableDepthCubeMap::~deoglRenderableDepthCubeMap(){
	pCleanUp();
	
//	pRenderThread.GetMemoryManager().IncRenderableCubeDepthGPUTotal( -pMemoryUsageGPU );
}



// Management
///////////////

bool deoglRenderableDepthCubeMap::Matches( int size ) const{
	return ( size == pSize );
}

void deoglRenderableDepthCubeMap::SetInUse( bool inUse ){
	pInUse = inUse;
}

void deoglRenderableDepthCubeMap::UpdateMemoryUsage(){
	//const int oldMemoryGPU = pMemoryUsageGPU;
	
	if( pCubeMap ){
		pMemoryUsageGPU = pSize * pSize * 6 * 2;
		
	}else{
		pMemoryUsageGPU = 0;
	}
	
//	pRenderThread.GetMemoryManager().IncRenderableCubeDepthGPUTotal( pMemoryUsageGPU - oldMemoryGPU );
}



// private functions
//////////////////////

void deoglRenderableDepthCubeMap::pCleanUp(){
	if( pCubeMap ){
		delete pCubeMap;
	}
}
