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

#include "deoglCollideListHTSector.h"
#include "deoglCollideListHTSCluster.h"
#include "../terrain/heightmap/deoglHTView.h"
#include "../terrain/heightmap/deoglHTViewSector.h"
#include "../terrain/heightmap/deoglRHTSector.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideListHTSector
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListHTSector::deoglCollideListHTSector() :
pSector( NULL ),
pClusterCount( 0 ){
}

deoglCollideListHTSector::~deoglCollideListHTSector(){
	const int count = pClusters.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		delete ( deoglCollideListHTSCluster* )pClusters.GetAt( i );
	}
}



// Management
///////////////

void deoglCollideListHTSector::Clear(){
	RemoveAllClusters();
	pSector = NULL;
}

void deoglCollideListHTSector::SetSector( deoglHTViewSector *sector ){
	pSector = sector;
	RemoveAllClusters();
}

void deoglCollideListHTSector::StartOcclusionTest( deoglOcclusionTest &occlusionTest,
const decDVector &referencePosition ){
	const decVector offset( pSector->GetSector().CalcWorldPosition( referencePosition ) );
	int i;
	
	for( i=0; i<pClusterCount; i++ ){
		( ( deoglCollideListHTSCluster* )pClusters.GetAt( i ) )->
			StartOcclusionTest( occlusionTest, offset );
	}
}



deoglCollideListHTSCluster &deoglCollideListHTSector::GetClusterAt( int index ) const{
	return *( ( deoglCollideListHTSCluster* )pClusters.GetAt( index ) );
}

deoglCollideListHTSCluster &deoglCollideListHTSector::AddCluster( const decPoint &coordinates ){
	deoglCollideListHTSCluster *cluster = NULL;
	
	if( pClusterCount < pClusters.GetCount() ){
		cluster = ( deoglCollideListHTSCluster* )pClusters.GetAt( pClusterCount );
		
	}else{
		cluster = new deoglCollideListHTSCluster( *this );
		pClusters.Add( cluster );
	}
	
	cluster->SetCoordinates( coordinates );
	pClusterCount++;
	return *cluster;
}

void deoglCollideListHTSector::RemoveAllClusters(){
	while( pClusterCount > 0 ){
		( ( deoglCollideListHTSCluster* )pClusters.GetAt( --pClusterCount ) )->Clear();
	}
}

void deoglCollideListHTSector::RemoveCulledClusters(){
	int i, last = 0;
	for( i=0; i<pClusterCount; i++ ){
		deoglCollideListHTSCluster &cluster = *( ( deoglCollideListHTSCluster* )pClusters.GetAt( i ) );
		if( cluster.GetCulled() ){
			cluster.Clear();
			continue;
		}
		
		if( i != last ){
			void * const exchange = pClusters.GetAt( last );
			pClusters.SetAt( last, pClusters.GetAt( i ) );
			pClusters.SetAt( i, exchange );
		}
		last++;
	}
	pClusterCount = last;
}
