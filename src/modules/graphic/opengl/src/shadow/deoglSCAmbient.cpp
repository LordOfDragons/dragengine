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
#include "../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMapManager.h"
#include "../texture/texture2d/deoglRenderableDepthTexture.h"
#include "../texture/texture2d/deoglRenderableDepthTextureManager.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include "../utils/collision/deoglDCollisionDetection.h"



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

pPlanStaticSize( 16 ),
pPlanDynamicSize( 16 ),
pPlanTransparentSize( 16 ){
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
	pHasStatic = true;
	pLastUseStatic = 0;
	
	return pStaticMap;
}

deoglCubeMap *deoglSCAmbient::ObtainStaticCubeMapWithSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	const deoglConfiguration &config = pRenderThread.GetConfiguration();
	const bool useCubeEncodeDepth = config.GetUseShadowCubeEncodeDepth();
	
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
	
	if( useCubeEncodeDepth ){
		pStaticCubeMap->SetFBOFormat( 4, false );
		
	}else{
		pStaticCubeMap->SetDepthFormat();
	}
	
	pStaticCubeMap->CreateCubeMap();
	pLastUseStatic = 0;
	
	return pStaticCubeMap;
}

void deoglSCAmbient::DropStatic(){
	if( pStaticMap ){
		delete pStaticMap;
		pStaticMap = NULL;
	}
	if( pStaticCubeMap ){
		delete pStaticCubeMap;
		pStaticCubeMap = NULL;
	}
	
	pHasStatic = false;
}

void deoglSCAmbient::IncrementLastUseStatic(){
	pLastUseStatic++;
}

void deoglSCAmbient::ResetLastUseStatic(){
	pLastUseStatic = 0;
}



deoglRenderableDepthTexture *deoglSCAmbient::ObtainDynamicMapWithSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDynamicMap ){
		if( pDynamicMap->GetWidth() == size ){
			return pDynamicMap;
		}
		
		DropDynamic();
	}
	
	pDynamicMap = pRenderThread.GetTexture().GetRenderableDepthTexture()
		.GetTextureWith( size, size, false, false );
	
	return pDynamicMap;
}

deoglRenderableDepthCubeMap *deoglSCAmbient::ObtainDynamicCubeMapWithSize( int size ){
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

void deoglSCAmbient::DropDynamic(){
	if( pDynamicMap ){
		pDynamicMap->SetInUse( false );
		pDynamicMap = NULL;
	}
	if( pDynamicCubeMap ){
		pDynamicCubeMap->SetInUse( false );
		pDynamicCubeMap = NULL;
	}
}



void deoglSCAmbient::Update(){
	if( pHasStatic && pLastUseStatic++ > 5 ){ // for the time being 5 frames
		DropStatic();
	}
}

void deoglSCAmbient::Clear(){
	DropStatic();
	DropDynamic();
}



void deoglSCAmbient::SetPlanStaticSize( int size ){
	if( size < 16 ){
		DETHROW( deeInvalidParam );
	}
	pPlanStaticSize = size;
}

void deoglSCAmbient::SetPlanDynamicSize( int size ){
	if( size < 16 ){
		DETHROW( deeInvalidParam );
	}
	pPlanDynamicSize = size;
}

void deoglSCAmbient::SetPlanTransparentSize( int size ){
	if( size < 16 ){
		DETHROW( deeInvalidParam );
	}
	pPlanTransparentSize = size;
}
