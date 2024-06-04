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
pFrameCounterTracker( renderThread ),
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
	pFrameCounterTracker.Update();
	if( ! pFrameCounterTracker.HasElapsedFrames() ){
		return;
	}
	
	// call Update() only if render frame changed
	
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

void deoglShadowCaster::DropTemporary(){
	pSolid.DropTemporary();
	pTransparent.DropTemporary();
	pAmbient.DropTemporary();
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
