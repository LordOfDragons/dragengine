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

#include "deoglShadowCaster.h"

#include <dragengine/common/exceptions.h>

#ifdef OS_W32
#undef near
#undef far
#endif



// Class deoglShadowCaster
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShadowCaster::deoglShadowCaster( deoglRenderThread &renderThread ) :
pSolid( renderThread ),
pTransparent( renderThread ),
pAmbient( renderThread ),

pShadowType( estNoShadows ),

pStaticNear( 0.01f ),
pStaticFar( 10.0f ),
pStaticScale( 2.887585e-2f ),
pStaticOffset( 2.887585e-4f ),

pDynamicNear( 0.01f ),
pDynamicFar( 10.0f ),
pDynamicScale( 2.887585e-2f ),
pDynamicOffset( 2.887585e-4f ),

pShadowLayers( NULL ),
pShadowLayerCount( 0 ){
}

deoglShadowCaster::~deoglShadowCaster(){
	if( pShadowLayers ){
		delete [] pShadowLayers;
	}
}



// Management
/////////////

void deoglShadowCaster::Update(){
	pSolid.Update();
	pTransparent.Update();
	pAmbient.Update();
}

bool deoglShadowCaster::RequiresUpdate() const{
	return pSolid.RequiresUpdate() || pTransparent.RequiresUpdate() || pAmbient.RequiresUpdate();
}



void deoglShadowCaster::Clear(){
	pSolid.Clear();
	pTransparent.Clear();
	pAmbient.Clear();
}

void deoglShadowCaster::DropDynamic(){
	pSolid.DropDynamic();
	//pTransparent.DropDynamic();
	pAmbient.DropDynamic();
}



void deoglShadowCaster::SetShadowType( eShadowTypes shadowType ){
	pShadowType = shadowType;
}

void deoglShadowCaster::SetStaticParams( float near, float far ){
	pStaticNear = near;
	pStaticFar = far;
	//pStaticScale = 1.0f / ( sqrtf( pStaticFar * pStaticFar * 3.0f ) - pStaticNear );
	pStaticScale = 1.0f / ( pStaticFar - pStaticNear );
	pStaticOffset = -pStaticNear * pStaticScale;
}

void deoglShadowCaster::SetDynamicParams( float near, float far ){
	pDynamicNear = near;
	pDynamicFar = far;
	//pDynamicScale = 1.0f / ( sqrtf( pDynamicFar * pDynamicFar * 3.0f ) - pDynamicNear );
	pDynamicScale = 1.0f / ( pDynamicFar - pDynamicNear );
	pDynamicOffset = -pDynamicNear * pDynamicScale;
}

void deoglShadowCaster::SetShadowLayerCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pShadowLayerCount ){
		return;
	}
	
	if( pShadowLayers ){
		delete [] pShadowLayers;
		pShadowLayers = NULL;
		pShadowLayerCount = 0;
	}
	
	if( count > 0 ){
		pShadowLayers = new sShadowLayer[ count ];
		pShadowLayerCount = count;
	}
}

const deoglShadowCaster::sShadowLayer &deoglShadowCaster::GetShadowLayerAt( int index ) const{
	if( index < 0 || index >= pShadowLayerCount ){
		DETHROW( deeInvalidParam );
	}
	return pShadowLayers[ index ];
}

deoglShadowCaster::sShadowLayer &deoglShadowCaster::GetShadowLayerAt( int index ){
	if( index < 0 || index >= pShadowLayerCount ){
		DETHROW( deeInvalidParam );
	}
	return pShadowLayers[ index ];
}
