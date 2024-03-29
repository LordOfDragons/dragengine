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

#include "deoglCollideListPropFieldType.h"
#include "deoglCollideListPropFieldCluster.h"
#include "../propfield/deoglPropFieldCluster.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideListPropFieldType
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListPropFieldType::deoglCollideListPropFieldType( deoglCollideListPropField &propField ) :
pPropField( propField ),
pType( NULL ),
pClusterCount( 0 )
{
	(void)pPropField; // silence compiler warning
}

deoglCollideListPropFieldType::~deoglCollideListPropFieldType(){
	const int count = pClusters.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		delete ( deoglCollideListPropFieldCluster* )pClusters.GetAt( i );
	}
}



// Management
///////////////

void deoglCollideListPropFieldType::Clear(){
	RemoveAllClusters();
	pType = NULL;
}

void deoglCollideListPropFieldType::SetType( deoglRPropFieldType *type ){
	pType = type;
	RemoveAllClusters();
}

void deoglCollideListPropFieldType::StartOcclusionTest( deoglOcclusionTest &occlusionTest,
const decVector &offset ){
	int i;
	for( i=0; i<pClusterCount; i++ ){
		( ( deoglCollideListPropFieldCluster* )pClusters.GetAt( i ) )->
			StartOcclusionTest( occlusionTest, offset );
	}
}



deoglCollideListPropFieldCluster &deoglCollideListPropFieldType::GetClusterAt( int index ) const{
	return *( ( deoglCollideListPropFieldCluster* )pClusters.GetAt( index ) );
}

deoglCollideListPropFieldCluster *deoglCollideListPropFieldType::AddCluster( deoglPropFieldCluster *cluster ){
	deoglCollideListPropFieldCluster *clcluster = NULL;
	
	if( pClusterCount < pClusters.GetCount() ){
		clcluster = ( deoglCollideListPropFieldCluster* )pClusters.GetAt( pClusterCount );
		
	}else{
		clcluster = new deoglCollideListPropFieldCluster;
		clcluster->SetCluster( cluster );
		pClusters.Add( clcluster );
	}
	
	clcluster->SetCluster( cluster );
	pClusterCount++;
	return clcluster;
}

void deoglCollideListPropFieldType::RemoveAllClusters(){
	while( pClusterCount > 0 ){
		( ( deoglCollideListPropFieldCluster* )pClusters.GetAt( --pClusterCount ) )->Clear();
	}
}

void deoglCollideListPropFieldType::RemoveCulledClusters(){
	int i, last = 0;
	for( i=0; i<pClusterCount; i++ ){
		deoglCollideListPropFieldCluster &cluster = *( ( deoglCollideListPropFieldCluster* )pClusters.GetAt( i ) );
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
