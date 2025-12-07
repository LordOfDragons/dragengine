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

void deoglFindBestEnvMap::SetPosition(const decDVector &position){
	pPosition = position;
}

void deoglFindBestEnvMap::Reset(){
	pEnvMap = NULL;
	pDistance = 0.0;
}



void deoglFindBestEnvMap::VisitNode(deoglDOctree *node, int intersection){
	VisitList(((deoglWorldOctree*)node)->GetEnvMapList());
}

void deoglFindBestEnvMap::VisitList(const deoglEnvironmentMapList &list){
	const int count = list.GetCount();
	deoglEnvironmentMap *envmap;
	double distance;
	int i;
	
	for(i=0; i<count; i++){
		envmap = list.GetAt(i);
		
		if(!envmap->GetSkyOnly()){
			distance = (envmap->GetPosition() - pPosition).Length();
			
			if(!pEnvMap || distance < pDistance){
				pEnvMap = envmap;
				pDistance = distance;
			}
		}
	}
}
