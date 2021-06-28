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

#include "deoglRComponent.h"
#include "deoglComponentTestForTouch.h"

#include "../light/deoglRLight.h"
#include "../utils/collision/deoglDCollisionDetection.h"
#include "../world/deoglWorldOctree.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>



// Class deoglComponentTestForTouch
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglComponentTestForTouch::deoglComponentTestForTouch( deoglRComponent *component ) :
pComponent( component ){
}

deoglComponentTestForTouch::~deoglComponentTestForTouch(){
}



// Visiting
/////////////

// extern int hackCSSpecialCount;
// extern float hackCSSpecialTime;

void deoglComponentTestForTouch::VisitNode( deoglDOctree *node, int intersection ){
	const deoglWorldOctree &soNode = *( ( deoglWorldOctree* )node );
	const decDVector &compMinExt = pComponent->GetMinimumExtend();
	const decDVector &compMaxExt = pComponent->GetMaximumExtend();
	const int count = soNode.GetLightCount();
	int i;
	
	for( i=0; i<count; i++ ){
// 			decTimer timer;
		deoglRLight &light = *soNode.GetLightAt( i );
		
		if( light.GetFullMinExtend() < compMaxExt && light.GetFullMaxExtend() > compMinExt ){
			light.TestComponent( pComponent );
		}
// 			hackCSSpecialCount++;
// 			hackCSSpecialTime += timer.GetElapsedTime();
	}
}
