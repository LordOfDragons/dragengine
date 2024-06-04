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

deoglCollideListHTSCluster *deoglCollideListHTSector::AddCluster( const decPoint &coordinates ){
	deoglCollideListHTSCluster *cluster = NULL;
	
	if( pClusterCount < pClusters.GetCount() ){
		cluster = ( deoglCollideListHTSCluster* )pClusters.GetAt( pClusterCount );
		
	}else{
		cluster = new deoglCollideListHTSCluster;
		pClusters.Add( cluster );
	}
	
	cluster->SetCluster( &pSector->GetClusterAt( coordinates ) );
	pClusterCount++;
	return cluster;
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
