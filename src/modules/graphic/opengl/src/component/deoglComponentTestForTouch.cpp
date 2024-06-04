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
