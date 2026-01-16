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
#include <inttypes.h>

#include "fbxModel.h"
#include "fbxModelCluster.h"
#include "fbxRig.h"
#include "fbxRigBone.h"
#include "../fbxConnection.h"
#include "../fbxNode.h"
#include "../fbxScene.h"
#include "../fbxProperty.h"
#include "../property/fbxPropertyString.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/model/deModelWeight.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxModel
///////////////////

// Constructor, destructor
////////////////////////////

fbxModel::fbxModel(fbxScene &scene, fbxNode &nodeGeometry) :
pScene(scene),
pNodeGeomtry(nodeGeometry),
pNodeModel(nullptr),
pNodeDeformer(nullptr),
pGeometryID(nodeGeometry.GetID()),
pModelID(0),
pDeformerID(0),
pVertexWeights(nullptr),
pVertexWeightCount(0),
pVertexWeightSize(0),
pWeights(nullptr),
pWeightCount(0),
pWeightSize(0),
pWeightMatchThreshold(0.001f),
pVertices(nullptr),
pVertexCount(0),
pCulling(true)
{
	decTList<fbxConnection*> consGeometry;
	scene.FindConnections(pGeometryID, consGeometry);
	
	consGeometry.Visit([&](const fbxConnection *connection){
		fbxNode * const node = scene.NodeWithID(connection->OtherID(pGeometryID));
		
		if(node->GetName() == "Model"){
			if(connection->GetSource() == pGeometryID
			&& node->GetProperties().GetAt(2)->CastString().GetValue() == "Mesh"){
				if(pNodeModel){
					DETHROW_INFO(deeInvalidParam, "duplicate model");
				}
				pNodeModel = node;
				pModelID = connection->GetTarget();
			}
			
		}else if(node->GetName() == "Deformer"){
			if(connection->GetTarget() == pGeometryID
			&& node->GetProperties().GetAt(2)->CastString().GetValue() == "Skin"){
				if(pNodeDeformer){
					DETHROW_INFO(deeInvalidParam, "duplicate deformer");
				}
				pNodeDeformer = node;
				pDeformerID = connection->GetSource();
			}
		}
	});
	
	if(!pNodeModel){
		DETHROW_INFO(deeInvalidParam, "model not found");
	}
	
	pMatrix = pNodeModel->GetTransformation();
	if(pNodeDeformer){
		pMatrix = pMatrix.QuickMultiply(pNodeDeformer->GetTransformation());
	}
	pMatrix = pMatrix.QuickMultiply(scene.GetTransformation());
	
	if(pNodeDeformer){
		decTList<fbxConnection*> consDeformer;
		scene.FindConnections(pDeformerID, consDeformer);
		
		consDeformer.Visit([&](const fbxConnection *connection){
			if(connection->GetTarget() != pDeformerID){
				return;
			}
			
			fbxNode &node = *scene.NodeWithID(connection->GetSource());
			if(node.GetName() == "Deformer"
			&& node.GetProperties().GetAt(2)->CastString().GetValue() == "Cluster"){
				pClusters.Add(fbxModelCluster::Ref::New(*this, node));
			}
		});
	}
	
	pClusters.Visit([&](fbxModelCluster &cluster){
		cluster.Prepare();
	});
	
	// create vertices
	const fbxProperty &propVertices = *nodeGeometry.FirstNodeNamed("Vertices")->GetProperties().GetAt(0);
	const int count = propVertices.GetValueCount() / 3;
	
	if(count > 0){
		pVertices = new sVertex[count];
		
		int i;
		for(i=0; i<count; i++){
			const int base = i * 3;
			pVertices[i].weightSet = -1;
			pVertices[i].position = pMatrix * decVector(
				propVertices.GetValueAtAsFloat(base),
				propVertices.GetValueAtAsFloat(base + 1),
				propVertices.GetValueAtAsFloat(base + 2));
		}
		pVertexCount = count;
	}
	
	// properties
	const fbxNode * const nodeCulling = pNodeModel->FirstNodeNamedOrNull("Culling");
	if(nodeCulling){
		pCulling = nodeCulling->GetProperties().GetAt(0)->CastString().GetValue() != "CullingOff";
	}
}

fbxModel::~fbxModel(){
	if(pVertices){
		delete [] pVertices;
	}
	if(pWeights){
		delete [] pWeights;
	}
	if(pVertexWeights){
		delete [] pVertexWeights;
	}
}



// Management
///////////////

fbxModelCluster *fbxModel::GetClusterNamed(const char *name) const{
	const int count = pClusters.GetCount();
	int i;
	for(i=0; i<count; i++){
		fbxModelCluster * const cluster = pClusters.GetAt(i);
		if(cluster->GetRigBone() && cluster->GetRigBone()->GetName() == name){
			return cluster;
		}
	}
	return nullptr;
}

void fbxModel::MatchClusters(const fbxRig &rig){
	const int count = pClusters.GetCount();
	decTList<fbxConnection*> connections;
	int i;
	
	for(i=0; i<count; i++){
		fbxModelCluster &cluster = pClusters.GetAt(i);
		
		connections.RemoveAll();
		
		pScene.FindConnections(cluster.GetNodeClusterID(), connections);
		
		for(const auto &connection : connections){
			if(connection->GetTarget() != cluster.GetNodeClusterID() || connection->GetSource() == 0){
				continue;
			}
			
			cluster.SetRigBone(rig.GetBones().FindWithModelID(connection->GetSource()));
			if(cluster.GetRigBone()){
				break;
			}
		}
	}
}

void fbxModel::BuildWeights(){
	const int count = pClusters.GetCount();
	int i, j;
	
	for(i=0; i<count; i++){
		fbxModelCluster &cluster = pClusters.GetAt(i);
		if(!cluster.GetRigBone()){
			continue;
		}
		
		//const fbxScene::eWeightMode weightMode = fbxScene::ConvWeightMode( cluster.GetNodeCluster() );
		
		const fbxNode * const nodeIndex = cluster.GetNodeCluster().FirstNodeNamedOrNull("Indexes");
		const fbxNode * const nodeWeight = cluster.GetNodeCluster().FirstNodeNamedOrNull("Weights");
		if(!nodeIndex || !nodeWeight){
			continue; // rig affects nobody
		}
		
		const fbxProperty &propIndex = *nodeIndex->GetProperties().GetAt(0);
		const fbxProperty &propWeight = *nodeWeight->GetProperties().GetAt(0);
		const int entryCount = propIndex.GetValueCount();
		const int bone = cluster.GetRigBone()->GetIndex();
		
		for(j=0; j<entryCount; j++){
			const int index = propIndex.GetValueAtAsInt(j);
			const float weight = propWeight.GetValueAtAsFloat(j);
			pVertices[index].weights.Add(pAddVertexWeight(bone, weight));
		}
	}
	
	for(i=0; i<pVertexCount; i++){
		decTList<int> &weights = pVertices[i].weights;
		const int weightCount = weights.GetCount();
		float weightSum = 0.0f;
		
		for(j=0; j<weightCount; j++){
			weightSum += pVertexWeights[weights.GetAt(j)].GetWeight();
		}
		
		if(weightSum < 0.001f){
			pVertices[i].weightSet = -1;
			continue;
		}
		
		const float factor = 1.0f / weightSum;
		for(j=0; j<weightCount; j++){
			const deModelWeight &weight = pVertexWeights[weights.GetAt(j)];
			pVertices[i].weights.SetAt(j, pAddWeight(weight.GetBone(), weight.GetWeight() * factor));
		}
	}
	
	int maxWGWeightCount = 0;
	for(i=0; i<pVertexCount; i++){
		maxWGWeightCount = decMath::max(maxWGWeightCount, pVertices[i].weights.GetCount());
	}
	
	for(i=0; i<maxWGWeightCount; i++){
		const int firstWeightSet = pWeightSetsFirstWeight.GetCount();
		const int wgWeightCount = i + 1;
		
		for(j=0; j<pVertexCount; j++){
			if(wgWeightCount == pVertices[j].weights.GetCount()){
				pVertices[j].weightSet = pAddWeightSet(pVertices[j].weights);
			}
		}
		
		pWeightGroupsSetCount.Add(pWeightSetsFirstWeight.GetCount() - firstWeightSet);
	}
}

const deModelWeight &fbxModel::GetWeightAt(int index) const{
	if(index < 0 || index >= pWeightCount){
		DETHROW(deeInvalidParam);
	}
	return pWeights[index];
}



void fbxModel::DebugPrintStructure(deBaseModule &module, const decString &prefix, bool verbose) const{
	module.LogInfoFormat("%sModel", prefix.GetString());
	
	const decString childPrefix(prefix + "  ");
	pClusters.Visit([&](fbxModelCluster &cluster){
		cluster.DebugPrintStructure(module, childPrefix, verbose);
	});
}



// Private Functions
//////////////////////

int fbxModel::pAddVertexWeight(int bone, float weight){
	if(pVertexWeightCount == pVertexWeightSize){
		const int newSize = pVertexWeightSize * 3 / 2 + 1;
		deModelWeight * const newArray = new deModelWeight[newSize];
		if(pVertexWeights){
			memcpy(newArray, pVertexWeights, sizeof(deModelWeight) * pVertexWeightSize);
			delete [] pVertexWeights;
		}
		pVertexWeights = newArray;
		pVertexWeightSize = newSize;
	}
	
	const int index = pVertexWeightCount++;
	pVertexWeights[index].SetBone(bone);
	pVertexWeights[index].SetWeight(weight);
	return index;
}

int fbxModel::pAddWeight(int bone, float weight){
	// find matching weight
	int i;
	for(i=0; i<pWeightCount; i++){
		if(pWeights[i].GetBone() == bone
		&& fabsf(pWeights[i].GetWeight() - weight) <= pWeightMatchThreshold){
			return i;
		}
	}
	
	// add weight
	if(pWeightCount == pWeightSize){
		const int newSize = pWeightSize * 3 / 2 + 1;
		deModelWeight * const newArray = new deModelWeight[newSize];
		if(pWeights){
			memcpy(newArray, pWeights, sizeof(deModelWeight) * pWeightSize);
			delete [] pWeights;
		}
		pWeights = newArray;
		pWeightSize = newSize;
	}
	
	const int index = pWeightCount++;
	pWeights[index].SetBone(bone);
	pWeights[index].SetWeight(weight);
	return index;
}

int fbxModel::pAddWeightSet(const decTList<int> &weights){
	// find matching weight set
	const int count = pWeightSetsFirstWeight.GetCount();
	const int weightCount = weights.GetCount();
	int i, j;
	
	for(i=0; i<count; i++){
		if(weightCount != pWeightSetsWeightsCount.GetAt(i)){
			continue;
		}
		
		const int setFirstWeight = pWeightSetsFirstWeight.GetAt(i);
		for(j=0; j<weightCount; j++){
			if(!weights.Has(pWeightSetWeights.GetAt(setFirstWeight + j))){
				break;
			}
		}
		
		if(j == weightCount){
			return i;
		}
	}
	
	// add weight set
	const int firstWeight = pWeightSetWeights.GetCount();
	for(i=0; i<weightCount; i++){
		pWeightSetWeights.Add(weights.GetAt(i));
	}
	pWeightSetsFirstWeight.Add(firstWeight);
	pWeightSetsWeightsCount.Add(weightCount);
	return count;
}
