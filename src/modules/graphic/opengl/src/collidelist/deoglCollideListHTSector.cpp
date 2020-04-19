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
#include "../terrain/heightmap/deoglHTView.h"
#include "../terrain/heightmap/deoglHTViewSector.h"

#include "dragengine/common/exceptions.h"



// Class deoglCollideListHTSector
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListHTSector::deoglCollideListHTSector(){
	pSector = NULL;
	
	pClusters = NULL;
	pClusterCount = 0;
	pClusterSize = 0;
}

deoglCollideListHTSector::~deoglCollideListHTSector(){
	if( pClusters ) delete [] pClusters;
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



int deoglCollideListHTSector::GetClusterAt( int index ) const{
	if( index < 0 || index >= pClusterCount ) DETHROW( deeInvalidParam );
	
	return pClusters[ index ];
}

void deoglCollideListHTSector::AddCluster( int cluster ){
	if( cluster < 0 ) DETHROW( deeInvalidParam );
	
	if( pClusterCount == pClusterSize ){
		int newSize = pClusterSize * 3 / 2 + 1;
		int *newArray = new int[ newSize ];
		if( pClusters ){
			memcpy( newArray, pClusters, sizeof( int ) * pClusterSize );
			delete [] pClusters;
		}
		pClusters = newArray;
		pClusterSize = newSize;
	}
	
	pClusters[ pClusterCount++ ] = cluster;
}

void deoglCollideListHTSector::RemoveAllClusters(){
	pClusterCount = 0;
}
