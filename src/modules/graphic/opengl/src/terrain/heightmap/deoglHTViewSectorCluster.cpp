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

#include "deoglHTSCluster.h"
#include "deoglHTSTexture.h"
#include "deoglHTView.h"
#include "deoglHTViewSector.h"
#include "deoglHTViewSectorCluster.h"
#include "deoglRHTSector.h"
#include "deoglRHeightTerrain.h"
#include "../../rendering/task/shared/deoglRenderTaskSharedInstance.h"
#include "../../rendering/task/shared/deoglRenderTaskSharedPool.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"

#include <dragengine/common/exceptions.h>



// Class deoglHTViewSectorCluster
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglHTViewSectorCluster::deoglHTViewSectorCluster( deoglHTViewSector &sector, const decPoint &coordinate ) :
pSector( sector ),
pCoordinate( coordinate ),
pLodLevel( 0 )
{
	pBorderTargets[ ebLeft ] = ebtLeft;
	pBorderTargets[ ebTop ] = ebtTop;
	pBorderTargets[ ebRight ] = ebtRight;
	pBorderTargets[ ebBottom ] = ebtBottom;
}

deoglHTViewSectorCluster::~deoglHTViewSectorCluster(){
	pRemoveAllRTSInstances();
}



// Management
///////////////

void deoglHTViewSectorCluster::SetLodLevel( int lodLevel ){
	pLodLevel = lodLevel;
}

deoglHTViewSectorCluster::eBorderTargets deoglHTViewSectorCluster::GetBorderTarget( eBorders border ) const{
	return pBorderTargets[ border ];
}

void deoglHTViewSectorCluster::SetBorderTarget( eBorders border,  eBorderTargets target ){
	pBorderTargets[ border ] = target;
}

void deoglHTViewSectorCluster::Reset(){
	pLodLevel = 0;
	pBorderTargets[ ebLeft ] = ebtLeft;
	pBorderTargets[ ebTop ] = ebtTop;
	pBorderTargets[ ebRight ] = ebtRight;
	pBorderTargets[ ebBottom ] = ebtBottom;
}



deoglRenderTaskSharedInstance *deoglHTViewSectorCluster::GetRTSInstanceAt( int texture, int index ) const{
	const int rtsIndex = texture * 5 + index;
	if( rtsIndex >= pRTSInstances.GetCount() ){
		return NULL;
	}
	return ( deoglRenderTaskSharedInstance* )pRTSInstances.GetAt( rtsIndex );
}

void deoglHTViewSectorCluster::UpdateRTSInstances(){
	pRemoveAllRTSInstances();
	
	if( pLodLevel < 0 ){
		return;
	}
	
	const deoglRHTSector &sector = pSector.GetSector();
	if( ! sector.GetValid() || ! sector.GetValidTextures() ){
		return;
	}
	
	deoglRenderTaskSharedPool &pool = sector.GetHeightTerrain().GetRenderThread().GetRenderTaskSharedPool();
	const deoglHTSCluster &htscluster = sector.GetClusterAt( pCoordinate );
	const deoglHTSClusterLOD &clod = htscluster.GetLODAt( pLodLevel );
	const int vboOffsetFaces = htscluster.GetOffsetVBODataFaces();
	const int textureCount = sector.GetTextureCount();
	deoglRenderTaskSharedInstance *rtinstance;
	int i, j;
	
	for( i=0; i<textureCount; i++ ){
		const deoglHTSTexture &httexture = sector.GetTextureAt( i );
		const deoglSkinTexture * const skinTexture = httexture.GetUseSkinTexture();
		if( ! skinTexture ){
			for( j=0; j<5; j++ ){
				pRTSInstances.Add( NULL );
			}
			continue;
		}
		
		deoglSPBlockUBO &spbInstance = httexture.GetParamBlock();
		
		// add an instance for this cluster
		rtinstance = pool.GetInstance();
		rtinstance->SetParameterBlock( &spbInstance );
		rtinstance->SetFirstPoint( 0 );
		rtinstance->SetFirstIndex( vboOffsetFaces + clod.firstBasePoint );
		rtinstance->SetIndexCount( clod.basePointCount );
		rtinstance->SetPrimitiveType( GL_TRIANGLES );
		pRTSInstances.Add( rtinstance );
		
		// add instances for borders if required
		if( pLodLevel < 1 ){
			for( j=0; j<4; j++ ){
				pRTSInstances.Add( NULL );
			}
			continue;
		}
		
		for( j=0; j<4; j++ ){
			const eBorderTargets target = pBorderTargets[ j ];
			
			rtinstance = pool.GetInstance();
			rtinstance->SetParameterBlock( &spbInstance );
			rtinstance->SetFirstPoint( 0 );
			rtinstance->SetFirstIndex( vboOffsetFaces + clod.firstBorderPoint[ target ] );
			rtinstance->SetIndexCount( clod.borderPointCount[ target ] );
			rtinstance->SetPrimitiveType( GL_TRIANGLES );
			pRTSInstances.Add( rtinstance );
		}
	}
}



// Private Functions
//////////////////////

void deoglHTViewSectorCluster::pRemoveAllRTSInstances(){
	const int count = pRTSInstances.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRenderTaskSharedInstance * const rtinstance =
			( deoglRenderTaskSharedInstance* )pRTSInstances.GetAt( i );
		if( rtinstance ){
			rtinstance->ReturnToPool();
		}
	}
	
	pRTSInstances.RemoveAll();
}
