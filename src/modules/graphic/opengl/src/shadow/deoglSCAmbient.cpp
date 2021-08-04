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

#include "deoglSCAmbient.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTTexture.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class deoglSCAmbient
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglSCAmbient::deoglSCAmbient( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pStaticMap( NULL ),
pStaticCubeMap( NULL ),
pLastUseStatic( 0 ),
pHasStatic( false ),

pDynamicMap( NULL ),
pDynamicCubeMap( NULL ),
pLastUseDynamic( 0 ),
pHasDynamic( false ),
pDirtyDynamic( true ),

pLastSizeStatic( 0 ),
pNextSizeStatic( 0 ),
pLastSizeDynamic( 0 ),
pNextSizeDynamic( 0 ),

pMemUseStaMap( renderThread.GetMemoryManager().GetConsumption().shadow.ambientStaticMap ),
pMemUseStaCube( renderThread.GetMemoryManager().GetConsumption().shadow.ambientStaticCube ),
pMemUseDynMap( renderThread.GetMemoryManager().GetConsumption().shadow.ambientDynamicMap ),
pMemUseDynCube( renderThread.GetMemoryManager().GetConsumption().shadow.ambientDynamicCube ){
}

deoglSCAmbient::~deoglSCAmbient(){
	Clear();
}



// Management
///////////////

deoglTexture *deoglSCAmbient::ObtainStaticMapWithSize( int size, bool useFloat ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pStaticMap ){
		if( pStaticMap->GetWidth() == size && pStaticMap->GetFormat()->GetIsDepthFloat() == useFloat ){
			return pStaticMap;
		}
		
		if( pStaticMap ){
			delete pStaticMap;
			pStaticMap = NULL;
		}
	}
	
	pStaticMap = new deoglTexture( pRenderThread );
	pStaticMap->SetDepthFormat( false, useFloat );
	pStaticMap->SetSize( size, size );
	pStaticMap->CreateTexture();
	pMemUseStaMap = pStaticMap->GetMemoryConsumption().Total();
	pHasStatic = true;
	pLastUseStatic = 0;
	pLastSizeStatic = pNextSizeStatic;
	
	return pStaticMap;
}

deoglCubeMap *deoglSCAmbient::ObtainStaticCubeMapWithSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pStaticCubeMap ){
		if( pStaticCubeMap->GetSize() == size ){
			return pStaticCubeMap;
		}
		
		if( pStaticCubeMap ){
			delete pStaticCubeMap;
			pStaticCubeMap = NULL;
		}
	}
	
	pStaticCubeMap = new deoglCubeMap( pRenderThread );
	pStaticCubeMap->SetSize( size );
	pStaticCubeMap->SetDepthFormat();
	pStaticCubeMap->CreateCubeMap();
	pMemUseStaCube = pStaticCubeMap->GetMemoryConsumption().Total();
	pHasStatic = true;
	pLastUseStatic = 0;
	pLastSizeStatic = pNextSizeStatic;
	
	return pStaticCubeMap;
}

void deoglSCAmbient::DropStatic(){
	pMemUseStaMap = 0;
	pMemUseStaCube = 0;
	
	if( pStaticMap ){
		delete pStaticMap;
		pStaticMap = NULL;
	}
	if( pStaticCubeMap ){
		delete pStaticCubeMap;
		pStaticCubeMap = NULL;
	}
	
	pLastSizeStatic = 0;
	pHasStatic = false;
}

void deoglSCAmbient::IncrementLastUseStatic(){
	pLastUseStatic++;
}

void deoglSCAmbient::ResetLastUseStatic(){
	pLastUseStatic = 0;
}



deoglTexture *deoglSCAmbient::ObtainDynamicMapWithSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	
	if( pDynamicMap ){
		if( pDynamicMap->GetWidth() == size ){
			return pDynamicMap;
		}
		
		if( pDynamicMap ){
			delete pDynamicMap;
			pDynamicMap = NULL;
		}
	}
	
	pDynamicMap = new deoglTexture( pRenderThread );
	pDynamicMap->SetDepthFormat( false, false );
	pDynamicMap->SetSize( size, size );
	pDynamicMap->CreateTexture();
	pMemUseDynMap = pDynamicMap->GetMemoryConsumption().Total();
	pHasDynamic = true;
	pDirtyDynamic = true;
	pLastUseDynamic = 0;
	pLastSizeDynamic = pNextSizeDynamic;
	
	return pDynamicMap;
}

deoglCubeMap *deoglSCAmbient::ObtainDynamicCubeMapWithSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDynamicCubeMap ){
		if( pDynamicCubeMap->GetSize() == size ){
			return pDynamicCubeMap;
		}
		
		if( pDynamicCubeMap ){
			delete pDynamicCubeMap;
			pDynamicCubeMap = NULL;
		}
	}
	
	pDynamicCubeMap = new deoglCubeMap( pRenderThread );
	pDynamicCubeMap->SetSize( size );
	pDynamicCubeMap->SetDepthFormat();
	pDynamicCubeMap->CreateCubeMap();
	pMemUseDynCube = pDynamicCubeMap->GetMemoryConsumption().Total();
	pHasDynamic = true;
	pDirtyDynamic = true;
	pLastUseDynamic = 0;
	pLastSizeDynamic = pNextSizeDynamic;
	
	return pDynamicCubeMap;
}

void deoglSCAmbient::DropDynamic(){
	pMemUseDynMap = 0;
	pMemUseDynCube = 0;
	
	if( pDynamicMap ){
		delete pDynamicMap;
		pDynamicMap = NULL;
	}
	if( pDynamicCubeMap ){
		delete pDynamicCubeMap;
		pDynamicCubeMap = NULL;
	}
	
	pLastSizeDynamic = 0;
	pHasDynamic = false;
	pDirtyDynamic = true;
}

void deoglSCAmbient::IncrementLastUseDynamic(){
	pLastUseDynamic++;
}

void deoglSCAmbient::ResetLastUseDynamic(){
	pLastUseDynamic = 0;
}

void deoglSCAmbient::SetDirtyDynamic( bool dirty ){
	pDirtyDynamic = dirty;
}



void deoglSCAmbient::SetLargestNextSizeStatic( int size ){
	if( size > pNextSizeStatic ){
		pNextSizeStatic = size;
	}
}

void deoglSCAmbient::SetLargestNextSizeDynamic( int size ){
	if( size > pNextSizeDynamic ){
		pNextSizeDynamic = size;
	}
}



void deoglSCAmbient::Update(){
	if( pHasStatic && pLastUseStatic++ > 5 ){ // for the time being 5 frames
		DropStatic();
	}
	if( pHasDynamic && pLastUseDynamic++ > 5 ){ // for the time being 5 frames
		DropDynamic();
	}
	
	if( pNextSizeStatic > 0 ){
		pLastSizeStatic = pNextSizeStatic;
		pNextSizeStatic = 0;
	}
	
	if( pNextSizeDynamic > 0 ){
		pLastSizeDynamic = pNextSizeDynamic;
		pNextSizeDynamic = 0;
	}
	
	pDirtyDynamic = true;
}

bool deoglSCAmbient::RequiresUpdate() const{
	return pHasStatic || pHasDynamic;
}

void deoglSCAmbient::Clear(){
	DropStatic();
	DropDynamic();
}
