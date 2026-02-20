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

#include "fbxRig.h"
#include "fbxRigBone.h"
#include "../fbxConnection.h"
#include "../fbxNode.h"
#include "../fbxScene.h"
#include "../fbxProperty.h"
#include "../property/fbxPropertyString.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxRig
/////////////////

// Constructor, destructor
////////////////////////////

fbxRig::fbxRig(fbxScene &scene, fbxNode *nodePose) :
pScene(scene),
pNodePose(nodePose)
{
	if(nodePose){
		pMatrix = nodePose->GetTransformation().QuickMultiply(scene.GetTransformation());
		pMatrixInverse = pMatrix.QuickInvert();
	}
	
	/*
	const int childCount = scene.GetNodeObjects()->GetNodes().GetCount();
	int i;
	
	for(i=0; i<childCount; i++){
		fbxNode &nodePoseBone = *scene.GetNodeObjects()->GetNodes().GetAt(i);
		if(nodePoseBone.GetName() != "Model" || nodePoseBone.GetProperties().GetAt(2)->CastString().GetValue() != "LimbNode"){
			continue;
		}
		
		const fbxRigBone::Ref bone(fbxRigBone::Ref::New(*this, nodePoseBone, nodePoseBone));
		bone->SetIndex(pBones.GetCount());
		
		if(!GetBoneNamed(bone->GetName())){
			pBones.Add(bone);
		}
	}
	*/
	
	int i;
	
	if(nodePose){
		const int childCount = nodePose->GetNodes().GetCount();
		
		for(i=0; i<childCount; i++){
			fbxNode &nodePoseBone = *nodePose->GetNodes().GetAt(i);
			if(nodePoseBone.GetName() != "PoseNode"){
				continue;
			}
			
			fbxNode &nodeModel = *scene.NodeWithID(nodePoseBone.FirstNodeNamed("Node")->GetID());
			if(nodeModel.GetProperties().GetAt(2)->CastString().GetValue() == "LimbNode"){
				const fbxRigBone::Ref bone(fbxRigBone::Ref::New(*this, nodePoseBone, nodeModel));
				bone->SetIndex(pBones.GetCount());
				
				if(!pBones.HasNamed(bone->GetName())){
					pBones.Add(bone);
				}
				
			}else if(nodeModel.GetProperties().GetAt(2)->CastString().GetValue() == "Root"){
				const fbxRigBone::Ref bone(fbxRigBone::Ref::New(*this, nodeModel, nodeModel));
				bone->SetIndex(pBones.GetCount());
				
				if(!pBones.HasNamed(bone->GetName())){
					pBones.Add(bone);
				}
				
				pAddRootBone(scene, nodeModel);
			}
		}
		
	}else{
		const int childCount = scene.GetNodeObjects()->GetNodes().GetCount();
		
		for(i=0; i<childCount; i++){
			fbxNode &nodeModel = *scene.GetNodeObjects()->GetNodes().GetAt(i);
			if(nodeModel.GetName() != "Model"){
				continue;
			}
			
			if(nodeModel.GetProperties().GetAt(2)->CastString().GetValue() != "Root"){
				continue;
			}
			
			const fbxRigBone::Ref bone(fbxRigBone::Ref::New(*this, nodeModel, nodeModel));
			bone->SetIndex(pBones.GetCount());
			
			if(!pBones.HasNamed(bone->GetName())){
				pBones.Add(bone);
			}
			
			pAddRootBone(scene, nodeModel);
		}
	}
	
	
	pBones.Visit([&](fbxRigBone &bone){
		bone.Prepare();
	});
}

fbxRig::~fbxRig(){
}



// Management
///////////////

void fbxRig::DebugPrintStructure(deBaseModule &module, const decString &prefix, bool verbose) const{
	module.LogInfoFormat("%sRig:", prefix.GetString());
	
	const decString childPrefix(prefix + "  ");
	pBones.Visit([&](fbxRigBone &bone){
		bone.DebugPrintStructure(module, childPrefix, verbose);
	});
}



// Private Functions
//////////////////////

void fbxRig::pAddRootBone(fbxScene &scene, fbxNode &nodeRoot){
	decTList<fbxConnection*> cons;
	scene.FindConnections(nodeRoot.GetID(), cons);
	
	cons.Visit([&](const fbxConnection *con){
		if(con->GetTarget() != nodeRoot.GetID()){
			return;
		}
		
		fbxNode &nodeModel = *scene.NodeWithID(con->GetSource());
		if(nodeModel.GetName() != "Model"){
			return;
		}
		
		if(nodeModel.GetProperties().GetAt(2)->CastString().GetValue() != "LimbNode"
		&& nodeModel.GetProperties().GetAt(2)->CastString().GetValue() != "Root"){
			return;
		}
		
		const fbxRigBone::Ref bone(fbxRigBone::Ref::New(*this, nodeModel, nodeModel));
		bone->SetIndex(pBones.GetCount());
		
		if(!pBones.HasNamed(bone->GetName())){
			pBones.Add(bone);
		}
		
		pAddRootBone(scene, nodeModel);
	});
}
