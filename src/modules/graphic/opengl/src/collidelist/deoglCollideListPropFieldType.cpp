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
#include "../propfield/deoglPropFieldCluster.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideListPropFieldType
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListPropFieldType::deoglCollideListPropFieldType(){
	pClusters = NULL;
	pClusterCount = 0;
	pClusterSize = 0;
}

deoglCollideListPropFieldType::~deoglCollideListPropFieldType(){
	if( pClusters ) delete [] pClusters;
}



// Management
///////////////

void deoglCollideListPropFieldType::AddCluster( deoglPropFieldCluster *cluster ){
	if( ! cluster ) DETHROW( deeInvalidParam );
	
	if( pClusterCount == pClusterSize ){
		int newSize = pClusterSize * 3 / 2 + 1;
		deoglPropFieldCluster **newArray = new deoglPropFieldCluster*[ newSize ];
		if( pClusters ){
			memcpy( newArray, pClusters, sizeof( deoglPropFieldCluster* ) * pClusterSize );
			delete [] pClusters;
		}
		pClusters = newArray;
		pClusterSize = newSize;
	}
	
	pClusters[ pClusterCount++ ] = cluster;
}

void deoglCollideListPropFieldType::RemoveAllClusters(){
	pClusterCount = 0;
}
