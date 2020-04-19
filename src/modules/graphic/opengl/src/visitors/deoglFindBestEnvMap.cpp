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

#include "deoglFindBestEnvMap.h"

#include "../envmap/deoglEnvironmentMap.h"
#include "../world/deoglWorldOctree.h"

#include <dragengine/common/exceptions.h>
#include "../utils/collision/deoglDCollisionDetection.h"



// Class deoglFindBestEnvMap
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglFindBestEnvMap::deoglFindBestEnvMap(){
	pEnvMap = NULL;
	pDistance = 0.0;
}

deoglFindBestEnvMap::~deoglFindBestEnvMap(){
}



// Visiting
/////////////

void deoglFindBestEnvMap::SetPosition( const decDVector &position ){
	pPosition = position;
}

void deoglFindBestEnvMap::Reset(){
	pEnvMap = NULL;
	pDistance = 0.0;
}



void deoglFindBestEnvMap::VisitNode( deoglDOctree *node, int intersection ){
	VisitList( ( ( deoglWorldOctree* )node )->GetEnvMapList() );
}

void deoglFindBestEnvMap::VisitList( const deoglEnvironmentMapList &list ){
	const int count = list.GetCount();
	deoglEnvironmentMap *envmap;
	double distance;
	int i;
	
	for( i=0; i<count; i++ ){
		envmap = list.GetAt( i );
		
		if( ! envmap->GetSkyOnly() ){
			distance = ( envmap->GetPosition() - pPosition ).Length();
			
			if( ! pEnvMap || distance < pDistance ){
				pEnvMap = envmap;
				pDistance = distance;
			}
		}
	}
}
