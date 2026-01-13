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

#include "deoglCollideListPropFieldType.h"
#include "deoglCollideListPropFieldCluster.h"
#include "../propfield/deoglPropFieldCluster.h"

#include <dragengine/common/exceptions.h>


// Class deoglCollideListPropFieldType
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListPropFieldType::deoglCollideListPropFieldType() :
pType(nullptr){
}

deoglCollideListPropFieldType::deoglCollideListPropFieldType(deoglRPropFieldType *type) :
pType(type){
}

deoglCollideListPropFieldType::deoglCollideListPropFieldType(deoglCollideListPropFieldType &&other) noexcept :
pType(other.pType),
pClusters(std::move(other.pClusters))
{
	other.pType = nullptr;
}

deoglCollideListPropFieldType &deoglCollideListPropFieldType::operator=(deoglCollideListPropFieldType &&other) noexcept{
	if(this != &other){
		pType = other.pType;
		other.pType = nullptr;
		
		pClusters.RemoveAll();
		if(other.pClusters.IsNotEmpty()){
			pClusters.Swap(other.pClusters);
		}
	}
	return *this;
}


// Management
///////////////

void deoglCollideListPropFieldType::Clear(){
	RemoveAllClusters();
	pType = nullptr;
}

void deoglCollideListPropFieldType::SetType(deoglRPropFieldType *type){
	pType = type;
	RemoveAllClusters();
}
void deoglCollideListPropFieldType::StartOcclusionTest(deoglOcclusionTest &occlusionTest,
const decVector &offset){
	pClusters.Visit([&](deoglCollideListPropFieldCluster &cluster){
		cluster.StartOcclusionTest(occlusionTest, offset);
	});
}


deoglCollideListPropFieldCluster &deoglCollideListPropFieldType::GetClusterAt(int index){
	return pClusters.GetAt(index);
}

const deoglCollideListPropFieldCluster &deoglCollideListPropFieldType::GetClusterAt(int index) const{
	return pClusters.GetAt(index);
}

deoglCollideListPropFieldCluster &deoglCollideListPropFieldType::AddCluster(deoglPropFieldCluster *cluster){
	pClusters.Add(deoglCollideListPropFieldCluster(cluster));
	return pClusters.Last();
}

void deoglCollideListPropFieldType::RemoveAllClusters(){
	pClusters.RemoveAll();
}

void deoglCollideListPropFieldType::RemoveCulledClusters(){
	pClusters.RemoveIf([](deoglCollideListPropFieldCluster &cluster){
		return cluster.GetCulled();
	});
}
