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

#include "deoglSCSolid.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTTexture.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class deoglSCSolid
///////////////////////

// Constructor, destructor
////////////////////////////

deoglSCSolid::deoglSCSolid( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pStaticMap( NULL ),
pStaticCubeMap( NULL ),
pStaticArrayMap( NULL ),
pLastUseStatic( 0 ),
pHasStatic( false ),

pDynamicMap( NULL ),
pDynamicCubeMap( NULL ),
pDynamicArrayMap( NULL ),
pLastUseDynamic( 0 ),
pHasDynamic( false ),
pDirtyDynamic( true ),

pLastSizeStatic( 0 ),
pNextSizeStatic( 0 ),
pLastSizeDynamic( 0 ),
pNextSizeDynamic( 0 ),

pMemUseStaMap( renderThread.GetMemoryManager().GetConsumption().shadow.solidStaticMap ),
pMemUseStaCube( renderThread.GetMemoryManager().GetConsumption().shadow.solidStaticCube ),
pMemUseStaArray( renderThread.GetMemoryManager().GetConsumption().shadow.solidStaticArray ),
pMemUseDynMap( renderThread.GetMemoryManager().GetConsumption().shadow.solidDynamicMap ),
pMemUseDynCube( renderThread.GetMemoryManager().GetConsumption().shadow.solidDynamicCube ),
pMemUseDynArray( renderThread.GetMemoryManager().GetConsumption().shadow.solidDynamicArray ){
}

deoglSCSolid::~deoglSCSolid(){
	Clear();
}



// Management
///////////////

deoglTexture *deoglSCSolid::ObtainStaticMapWithSize( int size, bool withStencil, bool useFloat ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pStaticMap ){
		if( pStaticMap->GetWidth() == size
		&& pStaticMap->GetFormat()->GetIsStencil() == withStencil
		&& pStaticMap->GetFormat()->GetIsDepthFloat() == useFloat ){
			return pStaticMap;
		}
		
		if( pStaticMap ){
			delete pStaticMap;
			pStaticMap = NULL;
		}
	}
	
	pStaticMap = new deoglTexture( pRenderThread );
	pStaticMap->SetDepthFormat( withStencil, useFloat );
	pStaticMap->SetSize( size, size );
	pStaticMap->CreateTexture();
	pMemUseStaMap = pStaticMap->GetMemoryConsumption().Total();
	pHasStatic = true;
	pLastUseStatic = 0;
	pLastSizeStatic = pNextSizeStatic;
	
	return pStaticMap;
}

deoglCubeMap *deoglSCSolid::ObtainStaticCubeMapWithSize( int size ){
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

deoglArrayTexture *deoglSCSolid::ObtainStaticArrayMapWithSize( int size, int layers, bool useFloat ){
	if( size < 1 || layers < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pStaticArrayMap ){
		if( pStaticArrayMap->GetWidth() == size && pStaticArrayMap->GetLayerCount() == layers
		&& pStaticArrayMap->GetFormat()->GetIsDepthFloat() == useFloat ){
			return pStaticArrayMap;
		}
		
		if( pStaticArrayMap ){
			delete pStaticArrayMap;
			pStaticArrayMap = NULL;
		}
	}
	
	pStaticArrayMap = new deoglArrayTexture( pRenderThread );
	pStaticArrayMap->SetDepthFormat( false, useFloat );
	pStaticArrayMap->SetSize( size, size, layers );
	pStaticArrayMap->CreateTexture();
	pMemUseStaArray = pStaticArrayMap->GetMemoryConsumption().Total();
	pHasStatic = true;
	pLastUseStatic = 0;
	pLastSizeStatic = pNextSizeStatic;
	
	return pStaticArrayMap;
}

void deoglSCSolid::DropStatic(){
	pMemUseStaMap = 0;
	pMemUseStaCube = 0;
	pMemUseStaArray = 0;
	
	if( pStaticMap ){
		delete pStaticMap;
		pStaticMap = NULL;
	}
	if( pStaticCubeMap ){
		delete pStaticCubeMap;
		pStaticCubeMap = NULL;
	}
	if( pStaticArrayMap ){
		delete pStaticArrayMap;
		pStaticArrayMap = NULL;
	}
	
	pLastSizeStatic = 0;
	pHasStatic = false;
}

void deoglSCSolid::IncrementLastUseStatic(){
	pLastUseStatic++;
}

void deoglSCSolid::ResetLastUseStatic(){
	pLastUseStatic = 0;
}



deoglTexture *deoglSCSolid::ObtainDynamicMapWithSize( int size, bool withStencil, bool useFloat ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDynamicMap ){
		if( pDynamicMap->GetWidth() == size
		&& pDynamicMap->GetFormat()->GetIsStencil() == withStencil
		&& pDynamicMap->GetFormat()->GetIsDepthFloat() == useFloat ){
			return pDynamicMap;
		}
		
		if( pDynamicMap ){
			delete pDynamicMap;
			pDynamicMap = NULL;
		}
	}
	
	pDynamicMap = new deoglTexture( pRenderThread );
	pDynamicMap->SetDepthFormat( withStencil, useFloat );
	pDynamicMap->SetSize( size, size );
	pDynamicMap->CreateTexture();
	pMemUseDynMap = pDynamicMap->GetMemoryConsumption().Total();
	pHasDynamic = true;
	pDirtyDynamic = true;
	pLastUseDynamic = 0;
	pLastSizeDynamic = pNextSizeDynamic;
	
	return pDynamicMap;
}

deoglCubeMap *deoglSCSolid::ObtainDynamicCubeMapWithSize( int size ){
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

deoglArrayTexture *deoglSCSolid::ObtainDynamicArrayMapWithSize(
int size, int layers, bool useFloat ){
	if( size < 1 || layers < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDynamicArrayMap ){
		if( pDynamicArrayMap->GetWidth() == size && pDynamicArrayMap->GetLayerCount() == layers
		&& pDynamicArrayMap->GetFormat()->GetIsDepthFloat() == useFloat ){
			return pDynamicArrayMap;
		}
		
		if( pDynamicArrayMap ){
			delete pDynamicArrayMap;
			pDynamicArrayMap = NULL;
		}
	}
	
	pDynamicArrayMap = new deoglArrayTexture( pRenderThread );
	pDynamicArrayMap->SetDepthFormat( false, useFloat );
	pDynamicArrayMap->SetSize( size, size, layers );
	pDynamicArrayMap->CreateTexture();
	pMemUseDynArray = pDynamicArrayMap->GetMemoryConsumption().Total();
	pHasDynamic = true;
	pDirtyDynamic = true;
	pLastUseDynamic = 0;
	pLastSizeDynamic = pNextSizeDynamic;
	
	return pDynamicArrayMap;
}

void deoglSCSolid::DropDynamic(){
	pMemUseDynMap = 0;
	pMemUseDynCube = 0;
	pMemUseDynArray = 0;
	
	if( pDynamicMap ){
		delete pDynamicMap;
		pDynamicMap = NULL;
	}
	if( pDynamicCubeMap ){
		delete pDynamicCubeMap;
		pDynamicCubeMap = NULL;
	}
	if( pDynamicArrayMap ){
		delete pDynamicArrayMap;
		pDynamicArrayMap = NULL;
	}
	
	pLastSizeDynamic = 0;
	pHasDynamic = false;
	pDirtyDynamic = true;
}

void deoglSCSolid::IncrementLastUseDynamic(){
	pLastUseDynamic++;
}

void deoglSCSolid::ResetLastUseDynamic(){
	pLastUseDynamic = 0;
}

void deoglSCSolid::SetDirtyDynamic( bool dirty ){
	pDirtyDynamic = dirty;
}



void deoglSCSolid::SetLargestNextSizeStatic( int size ){
	if( size > pNextSizeStatic ){
		pNextSizeStatic = size;
	}
}

void deoglSCSolid::SetLargestNextSizeDynamic( int size ){
	if( size > pNextSizeDynamic ){
		pNextSizeDynamic = size;
	}
}



void deoglSCSolid::Update(){
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

bool deoglSCSolid::RequiresUpdate() const{
	return pHasStatic || pHasDynamic;
}

void deoglSCSolid::Clear(){
	DropStatic();
	DropDynamic();
}
