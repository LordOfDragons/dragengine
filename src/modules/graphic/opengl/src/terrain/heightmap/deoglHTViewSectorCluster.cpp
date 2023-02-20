/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
