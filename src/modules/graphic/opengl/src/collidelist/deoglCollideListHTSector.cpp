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
pSector(nullptr){
}

deoglCollideListHTSector::deoglCollideListHTSector(deoglHTViewSector *sector) :
pSector(sector){
}

deoglCollideListHTSector::deoglCollideListHTSector(deoglCollideListHTSector &&other) noexcept :
pSector(other.pSector),
pClusters(std::move(other.pClusters))
{
	other.pSector = nullptr;
}

deoglCollideListHTSector& deoglCollideListHTSector::operator=(deoglCollideListHTSector &&other) noexcept{
	if(this != &other){
		pSector = other.pSector;
		other.pSector = nullptr;
		
		pClusters.RemoveAll();
		if(other.pClusters.IsNotEmpty()){
			pClusters.Swap(other.pClusters);
		}
	}
	return *this;
}


// Management
///////////////

void deoglCollideListHTSector::Clear(){
	RemoveAllClusters();
	pSector = nullptr;
}

void deoglCollideListHTSector::SetSector(deoglHTViewSector *sector){
	pSector = sector;
	RemoveAllClusters();
}

void deoglCollideListHTSector::StartOcclusionTest(deoglOcclusionTest &occlusionTest,
const decDVector &referencePosition){
	const decVector offset(pSector->GetSector().CalcWorldPosition(referencePosition));
	pClusters.Visit([&](deoglCollideListHTSCluster &cluster){
		cluster.StartOcclusionTest(occlusionTest, offset);
	});
}


deoglCollideListHTSCluster &deoglCollideListHTSector::GetClusterAt(int index){
	return pClusters.GetAt(index);
}

const deoglCollideListHTSCluster &deoglCollideListHTSector::GetClusterAt(int index) const{
	return pClusters.GetAt(index);
}

deoglCollideListHTSCluster &deoglCollideListHTSector::AddCluster(const decPoint &coordinates){
	pClusters.Add(deoglCollideListHTSCluster(&pSector->GetClusterAt(coordinates)));
	return pClusters.Last();
}

void deoglCollideListHTSector::RemoveAllClusters(){
	pClusters.RemoveAll();
}

void deoglCollideListHTSector::RemoveCulledClusters(){
	pClusters.RemoveIf([](const deoglCollideListHTSCluster &cluster){
		return cluster.GetCulled();
	});
}
