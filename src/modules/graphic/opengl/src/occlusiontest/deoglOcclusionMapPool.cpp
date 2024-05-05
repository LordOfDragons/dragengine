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

#include "deoglOcclusionMap.h"
#include "deoglOcclusionMapPool.h"

#include <dragengine/common/exceptions.h>



// Class deoglOcclusionMapPool
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionMapPool::deoglOcclusionMapPool( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
}

deoglOcclusionMapPool::~deoglOcclusionMapPool(){
	int count = pOcclusionMaps.GetCount();
	while( count > 0 ){
		delete ( deoglOcclusionMap* )pOcclusionMaps.GetAt( --count );
	}
}



// Management
///////////////

deoglOcclusionMap *deoglOcclusionMapPool::Get( int width, int height, int layerCount ){
	if( width < 1 || height < 1 || layerCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pOcclusionMaps.GetCount();
	int i;
	
	for( i=count-1; i>=0; i-- ){
		deoglOcclusionMap * const occlusionMap = ( deoglOcclusionMap* )pOcclusionMaps.GetAt( i );
		if( occlusionMap->GetWidth() == width
		&& occlusionMap->GetHeight() == height
		&& occlusionMap->GetLayerCount() == layerCount ){
			pOcclusionMaps.RemoveFrom( i );
			return occlusionMap;
		}
	}
	
	return new deoglOcclusionMap( pRenderThread, width, height, layerCount );
}

void deoglOcclusionMapPool::Return( deoglOcclusionMap *occlusionMap ){
	if( ! occlusionMap ){
		DETHROW( deeInvalidParam );
	}
	
	pOcclusionMaps.Add( occlusionMap );
}
