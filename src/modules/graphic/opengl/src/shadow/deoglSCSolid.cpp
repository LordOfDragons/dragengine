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
#include "../texture/arraytexture/deoglRenderableDepthArrayTexture.h"
#include "../texture/arraytexture/deoglRenderableDepthArrayTextureManager.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMapManager.h"
#include "../texture/texture2d/deoglRenderableDepthTexture.h"
#include "../texture/texture2d/deoglRenderableDepthTextureManager.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include "../utils/collision/deoglDCollisionDetection.h"



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

pPlanStaticSize( 16 ),
pPlanDynamicSize( 16 ),
pPlanTransparentSize( 16 ){
}

deoglSCSolid::~deoglSCSolid(){
	Clear();
}



// Management
///////////////

deoglTexture *deoglSCSolid::ObtainStaticMapWithSize( int size, bool useFloat ){
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
	pHasStatic = true;
	pLastUseStatic = 0;
	
	return pStaticMap;
}

deoglCubeMap *deoglSCSolid::ObtainStaticCubeMapWithSize( int size ){
	const deoglConfiguration &config = pRenderThread.GetConfiguration();
	const bool useShadowCubeEncodeDepth = config.GetUseShadowCubeEncodeDepth();
	
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
	
	if( useShadowCubeEncodeDepth ){
		pStaticCubeMap->SetFBOFormat( 4, false );
		
	}else{
		pStaticCubeMap->SetDepthFormat();
	}
	
	pStaticCubeMap->CreateCubeMap();
	pHasStatic = true;
	pLastUseStatic = 0;
	
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
	pHasStatic = true;
	pLastUseStatic = 0;
	
	return pStaticArrayMap;
}

void deoglSCSolid::DropStatic(){
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
	
	pHasStatic = false;
}

void deoglSCSolid::IncrementLastUseStatic(){
	pLastUseStatic++;
}

void deoglSCSolid::ResetLastUseStatic(){
	pLastUseStatic = 0;
}



deoglRenderableDepthTexture *deoglSCSolid::ObtainDynamicMapWithSize( int size, bool useFloat ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDynamicMap ){
		if( pDynamicMap->GetWidth() == size && pDynamicMap->GetUseFloat() == useFloat ){
			return pDynamicMap;
		}
		DropDynamic();
	}
	
	pDynamicMap = pRenderThread.GetTexture().GetRenderableDepthTexture()
		.GetTextureWith( size, size, false, useFloat );
	
	return pDynamicMap;
}

deoglRenderableDepthCubeMap *deoglSCSolid::ObtainDynamicCubeMapWithSize( int size ){
	if( pRenderThread.GetConfiguration().GetUseShadowCubeEncodeDepth() ){
		DETHROW( deeInvalidAction );
	}
	
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDynamicCubeMap ){
		if( pDynamicCubeMap->GetSize() == size ){
			return pDynamicCubeMap;
		}
		DropDynamic();
	}
	
	pDynamicCubeMap = pRenderThread.GetTexture().GetRenderableDepthCubeMap().GetCubeMapWith( size );
	
	return pDynamicCubeMap;
}

deoglRenderableDepthArrayTexture *deoglSCSolid::ObtainDynamicArrayMapWithSize(
int size, int layers, bool useFloat ){
	if( size < 1 || layers < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDynamicArrayMap ){
		if( pDynamicArrayMap->GetWidth() == size && pDynamicArrayMap->GetLayerCount() == layers
		&& pDynamicArrayMap->GetUseFloat() == useFloat ){
			return pDynamicArrayMap;
		}
		DropDynamic();
	}
	
	pDynamicArrayMap = pRenderThread.GetTexture().GetRenderableDepthArrayTexture()
		.GetWith( size, size, layers, false, useFloat );
	
	return pDynamicArrayMap;
}

void deoglSCSolid::DropDynamic(){
	if( pDynamicMap ){
		pDynamicMap->SetInUse( false );
		pDynamicMap = NULL;
	}
	if( pDynamicCubeMap ){
		pDynamicCubeMap->SetInUse( false );
		pDynamicCubeMap = NULL;
	}
	if( pDynamicArrayMap ){
		pDynamicArrayMap->SetInUse( false );
		pDynamicArrayMap = NULL;
	}
}



void deoglSCSolid::Update(){
	if( pHasStatic && pLastUseStatic++ > 5 ){ // for the time being 5 frames
		DropStatic();
	}
}

bool deoglSCSolid::RequiresUpdate() const{
	return pHasStatic;
}

void deoglSCSolid::Clear(){
	DropStatic();
	DropDynamic();
}



void deoglSCSolid::SetPlanStaticSize( int size ){
	if( size < 16 ){
		DETHROW( deeInvalidParam );
	}
	pPlanStaticSize = size;
}

void deoglSCSolid::SetPlanDynamicSize( int size ){
	if( size < 16 ){
		DETHROW( deeInvalidParam );
	}
	pPlanDynamicSize = size;
}

void deoglSCSolid::SetPlanTransparentSize( int size ){
	if( size < 16 ){
		DETHROW( deeInvalidParam );
	}
	pPlanTransparentSize = size;
}
