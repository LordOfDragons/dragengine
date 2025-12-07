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

#include "deoglCollideListPropFieldType.h"
#include "deoglCollideListPropFieldCluster.h"
#include "../propfield/deoglPropFieldCluster.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideListPropFieldType
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListPropFieldType::deoglCollideListPropFieldType(deoglCollideListPropField &propField) :
pPropField(propField),
pType(NULL),
pClusterCount(0)
{
	(void)pPropField; // silence compiler warning
}

deoglCollideListPropFieldType::~deoglCollideListPropFieldType(){
	const int count = pClusters.GetCount();
	int i;
	for(i=0; i<count; i++){
		delete (deoglCollideListPropFieldCluster*)pClusters.GetAt(i);
	}
}



// Management
///////////////

void deoglCollideListPropFieldType::Clear(){
	RemoveAllClusters();
	pType = NULL;
}

void deoglCollideListPropFieldType::SetType(deoglRPropFieldType *type){
	pType = type;
	RemoveAllClusters();
}

void deoglCollideListPropFieldType::StartOcclusionTest(deoglOcclusionTest &occlusionTest,
const decVector &offset){
	int i;
	for(i=0; i<pClusterCount; i++){
		((deoglCollideListPropFieldCluster*)pClusters.GetAt(i))->
			StartOcclusionTest(occlusionTest, offset);
	}
}



deoglCollideListPropFieldCluster &deoglCollideListPropFieldType::GetClusterAt(int index) const{
	return *((deoglCollideListPropFieldCluster*)pClusters.GetAt(index));
}

deoglCollideListPropFieldCluster *deoglCollideListPropFieldType::AddCluster(deoglPropFieldCluster *cluster){
	deoglCollideListPropFieldCluster *clcluster = NULL;
	
	if(pClusterCount < pClusters.GetCount()){
		clcluster = (deoglCollideListPropFieldCluster*)pClusters.GetAt(pClusterCount);
		
	}else{
		clcluster = new deoglCollideListPropFieldCluster;
		clcluster->SetCluster(cluster);
		pClusters.Add(clcluster);
	}
	
	clcluster->SetCluster(cluster);
	pClusterCount++;
	return clcluster;
}

void deoglCollideListPropFieldType::RemoveAllClusters(){
	while(pClusterCount > 0){
		((deoglCollideListPropFieldCluster*)pClusters.GetAt(--pClusterCount))->Clear();
	}
}

void deoglCollideListPropFieldType::RemoveCulledClusters(){
	int i, last = 0;
	for(i=0; i<pClusterCount; i++){
		deoglCollideListPropFieldCluster &cluster = *((deoglCollideListPropFieldCluster*)pClusters.GetAt(i));
		if(cluster.GetCulled()){
			cluster.Clear();
			continue;
		}
		
		if(i != last){
			void * const exchange = pClusters.GetAt(last);
			pClusters.SetAt(last, pClusters.GetAt(i));
			pClusters.SetAt(i, exchange);
		}
		last++;
	}
	pClusterCount = last;
}
