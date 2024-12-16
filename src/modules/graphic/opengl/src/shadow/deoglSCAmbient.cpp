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

#include "deoglSCAmbient.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTTexture.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMapManager.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../texture/texture2d/deoglRenderableDepthTexture.h"
#include "../texture/texture2d/deoglRenderableDepthTextureManager.h"

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

pTemporaryMap( NULL ),
pTemporaryCubeMap( NULL ),

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

deoglCubeMap *deoglSCAmbient::ObtainStaticCubeMapWithSize( int size, bool useFloat ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pStaticCubeMap ){
		if( pStaticCubeMap->GetSize() == size && pStaticCubeMap->GetFormat()->GetIsDepthFloat() == useFloat ){
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



deoglTexture *deoglSCAmbient::ObtainDynamicMapWithSize( int size, bool useFloat ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	
	if( pDynamicMap ){
		if( pDynamicMap->GetWidth() == size && pDynamicMap->GetFormat()->GetIsDepthFloat() == useFloat ){
			return pDynamicMap;
		}
		
		if( pDynamicMap ){
			delete pDynamicMap;
			pDynamicMap = NULL;
		}
	}
	
	pDynamicMap = new deoglTexture( pRenderThread );
	pDynamicMap->SetDepthFormat( false, useFloat );
	pDynamicMap->SetSize( size, size );
	pDynamicMap->CreateTexture();
	pMemUseDynMap = pDynamicMap->GetMemoryConsumption().Total();
	pHasDynamic = true;
	pDirtyDynamic = true;
	pLastUseDynamic = 0;
	pLastSizeDynamic = pNextSizeDynamic;
	
	return pDynamicMap;
}

deoglCubeMap *deoglSCAmbient::ObtainDynamicCubeMapWithSize( int size, bool useFloat ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDynamicCubeMap ){
		if( pDynamicCubeMap->GetSize() == size && pDynamicCubeMap->GetFormat()->GetIsDepthFloat() == useFloat ){
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



deoglRenderableDepthTexture *deoglSCAmbient::ObtainTemporaryMapWithSize( int size, bool useFloat ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTemporaryMap ){
		if( pTemporaryMap->GetWidth() == size && pTemporaryMap->GetUseFloat() == useFloat ){
			return pTemporaryMap;
		}
		
		DropTemporary();
	}
	
	pTemporaryMap = pRenderThread.GetTexture().GetRenderableDepthTexture()
		.GetTextureWith( size, size, false, useFloat );
	
	return pTemporaryMap;
}

deoglRenderableDepthCubeMap *deoglSCAmbient::ObtainTemporaryCubeMapWithSize( int size, bool useFloat ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTemporaryCubeMap ){
		if( pTemporaryCubeMap->GetSize() == size && pTemporaryCubeMap->GetUseFloat() == useFloat ){
			return pTemporaryCubeMap;
		}
		DropTemporary();
	}
	
	pTemporaryCubeMap = pRenderThread.GetTexture().GetRenderableDepthCubeMap().GetCubeMapWith( size, useFloat );
	
	return pTemporaryCubeMap;
}

void deoglSCAmbient::DropTemporary(){
	if( pTemporaryMap ){
		pTemporaryMap->SetInUse( false );
		pTemporaryMap = NULL;
	}
	if( pTemporaryCubeMap ){
		pTemporaryCubeMap->SetInUse( false );
		pTemporaryCubeMap = NULL;
	}
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
	DropTemporary();
}
