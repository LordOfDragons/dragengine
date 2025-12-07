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

#include "deoglCollideListPropFieldCluster.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../propfield/deoglPropFieldCluster.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideListPropFieldCluster
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListPropFieldCluster::deoglCollideListPropFieldCluster() :
pCluster(NULL),
pCulled(false),
pCascadeMask(0){
}

deoglCollideListPropFieldCluster::~deoglCollideListPropFieldCluster(){
}



// Management
///////////////

void deoglCollideListPropFieldCluster::Clear(){
	pCluster = nullptr;
	pCulled = false;
	pCascadeMask = 0;
}

void deoglCollideListPropFieldCluster::SetCluster(deoglPropFieldCluster *cluster){
	pCluster = cluster;
}

void deoglCollideListPropFieldCluster::SetCulled(bool culled){
	pCulled = culled;
}

void deoglCollideListPropFieldCluster::SetCascadeMask(int mask){
	pCascadeMask = mask;
}

void deoglCollideListPropFieldCluster::StartOcclusionTest(deoglOcclusionTest &occlusionTest, const decVector &offset){
	DEASSERT_NOTNULL(pCluster)
	
	pCulled = false;
	occlusionTest.AddInputData(offset - pCluster->GetMinimumExtend(), offset + pCluster->GetMaximumExtend(), this);
	pCulled = false;
}

void deoglCollideListPropFieldCluster::OcclusionTestInvisible(){
	pCulled = true;
}
