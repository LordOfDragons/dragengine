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

#include "deoglSCTransparent.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTTexture.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/cubemap/deoglRenderableColorCubeMap.h"
#include "../texture/cubemap/deoglRenderableColorCubeMapManager.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMapManager.h"
#include "../texture/texture2d/deoglRenderableColorTexture.h"
#include "../texture/texture2d/deoglRenderableColorTextureManager.h"
#include "../texture/texture2d/deoglRenderableDepthTexture.h"
#include "../texture/texture2d/deoglRenderableDepthTextureManager.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class deoglSCTransparent
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSCTransparent::deoglSCTransparent( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pStaticShadowMap( NULL ),
pStaticColorMap( NULL ),
pStaticShadowCubeMap( NULL ),
pStaticColorCubeMap( NULL ),
pLastUseStatic( 0 ),
pHasStatic( false ),

pDynamicShadowMap( NULL ),
pDynamicColorMap( NULL ),
pDynamicShadowCubeMap( NULL ),
pDynamicColorCubeMap( NULL ),

pLastUseDynamic( 0 ),
pHasDynamic( false ),
pDirtyDynamic( true ),

pLastSizeStatic( 0 ),
pNextSizeStatic( 0 ),
pLastSizeDynamic( 0 ),
pNextSizeDynamic( 0 ),

pMemUseStaMapDepth( renderThread.GetMemoryManager().GetConsumption().shadow.transparentStaticMapDepth ),
pMemUseStaMapColor( renderThread.GetMemoryManager().GetConsumption().shadow.transparentStaticMapColor ),
pMemUseStaCubeDepth( renderThread.GetMemoryManager().GetConsumption().shadow.transparentStaticCubeDepth ),
pMemUseStaCubeColor( renderThread.GetMemoryManager().GetConsumption().shadow.transparentStaticCubeColor ),
pMemUseDynMapDepth( renderThread.GetMemoryManager().GetConsumption().shadow.transparentDynamicMapDepth ),
pMemUseDynMapColor( renderThread.GetMemoryManager().GetConsumption().shadow.transparentDynamicMapColor ),
pMemUseDynCubeDepth( renderThread.GetMemoryManager().GetConsumption().shadow.transparentDynamicCubeDepth ),
pMemUseDynCubeColor( renderThread.GetMemoryManager().GetConsumption().shadow.transparentDynamicCubeColor ){
}

deoglSCTransparent::~deoglSCTransparent(){
	Clear();
}



// Management
///////////////

deoglTexture *deoglSCTransparent::ObtainStaticShadowMapWithSize( int size, bool useFloat ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pStaticShadowMap ){
		if( pStaticShadowMap->GetWidth() == size
		&& pStaticShadowMap->GetFormat()->GetIsDepthFloat() == useFloat ){
			return pStaticShadowMap;
		}
		
		if( pStaticShadowMap ){
			delete pStaticShadowMap;
			pStaticShadowMap = NULL;
		}
	}
	
	pStaticShadowMap = new deoglTexture( pRenderThread );
	pStaticShadowMap->SetDepthFormat( false, useFloat );
	pStaticShadowMap->SetSize( size, size );
	pStaticShadowMap->CreateTexture();
	pMemUseStaMapDepth = pStaticShadowMap->GetMemoryConsumption().Total();
	pHasStatic = true;
	pLastUseStatic = 0;
	pLastSizeStatic = pNextSizeStatic;
	
	return pStaticShadowMap;
}

deoglTexture *deoglSCTransparent::ObtainStaticColorMapWithSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pStaticColorMap ){
		if( pStaticColorMap->GetWidth() == size ){
			return pStaticColorMap;
		}
		
		if( pStaticColorMap ){
			delete pStaticColorMap;
			pStaticColorMap = NULL;
		}
	}
	
	pStaticColorMap = new deoglTexture( pRenderThread );
	pStaticColorMap->SetFBOFormat( 4, false );
	pStaticColorMap->SetSize( size, size );
	pStaticColorMap->CreateTexture();
	pMemUseStaMapColor = pStaticColorMap->GetMemoryConsumption().Total();
	pHasStatic = true;
	pLastUseStatic = 0;
	pLastSizeStatic = pNextSizeStatic;
	
	return pStaticColorMap;
}

deoglCubeMap *deoglSCTransparent::ObtainStaticShadowCubeMapWithSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pStaticShadowCubeMap ){
		if( pStaticShadowCubeMap->GetSize() == size ){
			return pStaticShadowCubeMap;
		}
		
		if( pStaticShadowCubeMap ){
			delete pStaticShadowCubeMap;
			pStaticShadowCubeMap = NULL;
		}
	}
	
	pStaticShadowCubeMap = new deoglCubeMap( pRenderThread );
	pStaticShadowCubeMap->SetSize( size );
	pStaticShadowCubeMap->SetDepthFormat();
	pStaticShadowCubeMap->CreateCubeMap();
	pMemUseStaCubeDepth = pStaticShadowCubeMap->GetMemoryConsumption().Total();
	pHasStatic = true;
	pLastUseStatic = 0;
	pLastSizeStatic = pNextSizeStatic;
	
	return pStaticShadowCubeMap;
}

deoglCubeMap *deoglSCTransparent::ObtainStaticColorCubeMapWithSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pStaticColorCubeMap ){
		if( pStaticColorCubeMap->GetSize() == size ){
			return pStaticColorCubeMap;
		}
		
		if( pStaticColorCubeMap ){
			delete pStaticColorCubeMap;
			pStaticColorCubeMap = NULL;
		}
	}
	
	pStaticColorCubeMap = new deoglCubeMap( pRenderThread );
	pStaticColorCubeMap->SetSize( size );
	pStaticColorCubeMap->SetFBOFormat( 4, false );
	pStaticColorCubeMap->CreateCubeMap();
	pMemUseStaCubeColor = pStaticColorCubeMap->GetMemoryConsumption().Total();
	pHasStatic = true;
	pLastUseStatic = 0;
	pLastSizeStatic = pNextSizeStatic;
	
	return pStaticColorCubeMap;
}

void deoglSCTransparent::DropStatic(){
	pMemUseStaMapDepth = 0;
	pMemUseStaMapColor = 0;
	pMemUseStaCubeDepth = 0;
	pMemUseStaCubeColor = 0;
	
	if( pStaticShadowMap ){
		delete pStaticShadowMap;
		pStaticShadowMap = NULL;
	}
	if( pStaticColorMap ){
		delete pStaticColorMap;
		pStaticColorMap = NULL;
	}
	if( pStaticShadowCubeMap ){
		delete pStaticShadowCubeMap;
		pStaticShadowCubeMap = NULL;
	}
	if( pStaticColorCubeMap ){
		delete pStaticColorCubeMap;
		pStaticColorCubeMap = NULL;
	}
	
	pLastSizeStatic = 0;
	pHasStatic = false;
}

void deoglSCTransparent::IncrementLastUseStatic(){
	pLastUseStatic++;
}

void deoglSCTransparent::ResetLastUseStatic(){
	pLastUseStatic = 0;
}



deoglRenderableDepthTexture *deoglSCTransparent::ObtainDynamicShadowMapWithSize( int size, bool useFloat ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDynamicShadowMap ){
		if( pDynamicShadowMap->GetWidth() == size && pDynamicShadowMap->GetUseFloat() == useFloat ){
			return pDynamicShadowMap;
		}
		DropDynamic();
	}
	
	pDynamicShadowMap = pRenderThread.GetTexture().GetRenderableDepthTexture()
		.GetTextureWith( size, size, false, false );
	pMemUseDynMapDepth = pDynamicShadowMap->GetTexture()->GetMemoryConsumption().Total();
	pHasDynamic = true;
	pDirtyDynamic = true;
	pLastUseDynamic = 0;
	pLastSizeDynamic = pNextSizeDynamic;
	
	return pDynamicShadowMap;
}

deoglRenderableColorTexture *deoglSCTransparent::ObtainDynamicColorMapWithSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDynamicColorMap ){
		if( pDynamicColorMap->GetWidth() == size ){
			return pDynamicColorMap;
		}
		DropDynamic();
	}
	
	pDynamicColorMap = pRenderThread.GetTexture().GetRenderableColorTexture()
		.GetTextureWith( size, size, 4, false );
	pMemUseDynMapColor = pDynamicColorMap->GetTexture()->GetMemoryConsumption().Total();
	pHasDynamic = true;
	pDirtyDynamic = true;
	pLastUseDynamic = 0;
	pLastSizeDynamic = pNextSizeDynamic;
	
	return pDynamicColorMap;
}

deoglRenderableDepthCubeMap *deoglSCTransparent::ObtainDynamicShadowCubeMapWithSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDynamicShadowCubeMap ){
		if( pDynamicShadowCubeMap->GetSize() == size ){
			return pDynamicShadowCubeMap;
		}
		DropDynamic();
	}
	
	pDynamicShadowCubeMap = pRenderThread.GetTexture().GetRenderableDepthCubeMap().GetCubeMapWith( size );
	pMemUseDynCubeDepth = pDynamicShadowCubeMap->GetCubeMap()->GetMemoryConsumption().Total();
	pHasDynamic = true;
	pDirtyDynamic = true;
	pLastUseDynamic = 0;
	pLastSizeDynamic = pNextSizeDynamic;
	
	return pDynamicShadowCubeMap;
}

deoglRenderableColorCubeMap *deoglSCTransparent::ObtainDynamicColorCubeMapWithSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDynamicColorCubeMap ){
		if( pDynamicColorCubeMap->GetSize() == size ){
			return pDynamicColorCubeMap;
		}
		DropDynamic();
	}
	
	pDynamicColorCubeMap = pRenderThread.GetTexture().GetRenderableColorCubeMap().
		GetCubeMapWith( size, 4, false );
	pMemUseDynCubeColor = pDynamicColorCubeMap->GetCubeMap()->GetMemoryConsumption().Total();
	pHasDynamic = true;
	pDirtyDynamic = true;
	pLastUseDynamic = 0;
	pLastSizeDynamic = pNextSizeDynamic;
	
	return pDynamicColorCubeMap;
}

void deoglSCTransparent::DropDynamic(){
	pMemUseDynMapDepth = 0;
	pMemUseDynMapColor = 0;
	pMemUseDynCubeDepth = 0;
	pMemUseDynCubeColor = 0;
	
	if( pDynamicShadowMap ){
		pDynamicShadowMap->SetInUse( false );
		pDynamicShadowMap = NULL;
	}
	if( pDynamicColorMap ){
		pDynamicColorMap->SetInUse( false );
		pDynamicColorMap = NULL;
	}
	if( pDynamicShadowCubeMap ){
		pDynamicShadowCubeMap->SetInUse( false );
		pDynamicShadowCubeMap = NULL;
	}
	if( pDynamicColorCubeMap ){
		pDynamicColorCubeMap->SetInUse( false );
		pDynamicColorCubeMap = NULL;
	}
	
	pLastSizeDynamic = 0;
	pHasDynamic = false;
	pDirtyDynamic = true;
}

void deoglSCTransparent::IncrementLastUseDynamic(){
	pLastUseDynamic++;
}

void deoglSCTransparent::ResetLastUseDynamic(){
	pLastUseDynamic = 0;
}

void deoglSCTransparent::SetDirtyDynamic( bool dirty ){
	pDirtyDynamic = dirty;
}



void deoglSCTransparent::SetLargestNextSizeStatic( int size ){
	if( size > pNextSizeStatic ){
		pNextSizeStatic = size;
	}
}

void deoglSCTransparent::SetLargestNextSizeDynamic( int size ){
	if( size > pNextSizeDynamic ){
		pNextSizeDynamic = size;
	}
}



void deoglSCTransparent::Update(){
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

bool deoglSCTransparent::RequiresUpdate() const{
	return pHasStatic || pHasDynamic;
}

void deoglSCTransparent::Clear(){
	DropStatic();
	DropDynamic();
}
