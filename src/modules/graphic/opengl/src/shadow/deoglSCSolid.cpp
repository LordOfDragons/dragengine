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

#include "deoglSCSolid.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTTexture.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/arraytexture/deoglRenderableDepthArrayTexture.h"
#include "../texture/arraytexture/deoglRenderableDepthArrayTextureManager.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMapManager.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../texture/texture2d/deoglRenderableDepthTexture.h"
#include "../texture/texture2d/deoglRenderableDepthTextureManager.h"

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

pTemporaryMap( nullptr ),
pTemporaryCubeMap( nullptr ),
pTemporaryArrayMap( nullptr ),

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

deoglCubeMap *deoglSCSolid::ObtainStaticCubeMapWithSize( int size, bool useFloat ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pStaticCubeMap ){
		if( pStaticCubeMap->GetSize() == size
		&& pStaticCubeMap->GetFormat()->GetIsDepthFloat() == useFloat ){
			return pStaticCubeMap;
		}
		
		if( pStaticCubeMap ){
			delete pStaticCubeMap;
			pStaticCubeMap = NULL;
		}
	}
	
	pStaticCubeMap = new deoglCubeMap( pRenderThread );
	pStaticCubeMap->SetSize( size );
	pStaticCubeMap->SetDepthFormat( useFloat );
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

deoglCubeMap *deoglSCSolid::ObtainDynamicCubeMapWithSize( int size, bool useFloat ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDynamicCubeMap ){
		if( pDynamicCubeMap->GetSize() == size
		&& pDynamicCubeMap->GetFormat()->GetIsDepthFloat() == useFloat ){
			return pDynamicCubeMap;
		}
		
		if( pDynamicCubeMap ){
			delete pDynamicCubeMap;
			pDynamicCubeMap = NULL;
		}
	}
	
	pDynamicCubeMap = new deoglCubeMap( pRenderThread );
	pDynamicCubeMap->SetSize( size );
	pDynamicCubeMap->SetDepthFormat( useFloat );
	pDynamicCubeMap->CreateCubeMap();
	pMemUseDynCube = pDynamicCubeMap->GetMemoryConsumption().Total();
	pHasDynamic = true;
	pDirtyDynamic = true;
	pLastUseDynamic = 0;
	pLastSizeDynamic = pNextSizeDynamic;
	
	return pDynamicCubeMap;
}

deoglArrayTexture *deoglSCSolid::ObtainDynamicArrayMapWithSize( int size, int layers, bool useFloat ){
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



deoglRenderableDepthTexture *deoglSCSolid::ObtainTemporaryMapWithSize( int size, bool withStencil, bool useFloat ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTemporaryMap ){
		if( pTemporaryMap->GetWidth() == size
		&& pTemporaryMap->GetWithStencil() == withStencil
		&& pTemporaryMap->GetUseFloat() == useFloat ){
			return pTemporaryMap;
		}
		DropTemporary();
	}
	
	pTemporaryMap = pRenderThread.GetTexture().GetRenderableDepthTexture()
		.GetTextureWith( size, size, withStencil, useFloat );
	
	return pTemporaryMap;
}

deoglRenderableDepthCubeMap *deoglSCSolid::ObtainTemporaryCubeMapWithSize( int size, bool useFloat ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTemporaryCubeMap ){
		if( pTemporaryCubeMap->GetSize() == size
		&& pTemporaryCubeMap->GetUseFloat() == useFloat ){
			return pTemporaryCubeMap;
		}
		DropTemporary();
	}
	
	pTemporaryCubeMap = pRenderThread.GetTexture().GetRenderableDepthCubeMap().GetCubeMapWith( size, useFloat );
	
	return pTemporaryCubeMap;
}

deoglRenderableDepthArrayTexture *deoglSCSolid::ObtainTemporaryArrayMapWithSize( int size, int layers, bool useFloat ){
	if( size < 1 || layers < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTemporaryArrayMap ){
		if( pTemporaryArrayMap->GetWidth() == size && pTemporaryArrayMap->GetLayerCount() == layers
		&& pTemporaryArrayMap->GetUseFloat() == useFloat ){
			return pTemporaryArrayMap;
		}
		DropTemporary();
	}
	
	pTemporaryArrayMap = pRenderThread.GetTexture().GetRenderableDepthArrayTexture()
		.GetWith( size, size, layers, false, useFloat );
	
	return pTemporaryArrayMap;
}

void deoglSCSolid::DropTemporary(){
	if( pTemporaryMap ){
		pTemporaryMap->SetInUse( false );
		pTemporaryMap = NULL;
	}
	if( pTemporaryCubeMap ){
		pTemporaryCubeMap->SetInUse( false );
		pTemporaryCubeMap = NULL;
	}
	if( pTemporaryArrayMap ){
		pTemporaryArrayMap->SetInUse( false );
		pTemporaryArrayMap = NULL;
	}
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
	DropTemporary();
}
