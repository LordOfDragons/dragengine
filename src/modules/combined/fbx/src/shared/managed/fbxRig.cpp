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
	const int childCount = scene.GetNodeObjects()->GetNodeCount();
	int i;
	
	for(i=0; i<childCount; i++){
		fbxNode &nodePoseBone = *scene.GetNodeObjects()->GetNodeAt(i);
		if(nodePoseBone.GetName() != "Model" || nodePoseBone.GetPropertyAt(2)->CastString().GetValue() != "LimbNode"){
			continue;
		}
		
		const fbxRigBone::Ref bone(fbxRigBone::Ref::NewWith(*this, nodePoseBone, nodePoseBone));
		bone->SetIndex(pBones.GetCount());
		
		if(!GetBoneNamed(bone->GetName())){
			pBones.Add(bone);
		}
	}
	*/
	
	int i;
	
	if(nodePose){
		const int childCount = nodePose->GetNodeCount();
		
		for(i=0; i<childCount; i++){
			fbxNode &nodePoseBone = *nodePose->GetNodeAt(i);
			if(nodePoseBone.GetName() != "PoseNode"){
				continue;
			}
			
			fbxNode &nodeModel = *scene.NodeWithID(nodePoseBone.FirstNodeNamed("Node")->GetID());
			if(nodeModel.GetPropertyAt(2)->CastString().GetValue() == "LimbNode"){
				const fbxRigBone::Ref bone(fbxRigBone::Ref::NewWith(*this, nodePoseBone, nodeModel));
				bone->SetIndex(pBones.GetCount());
				
				if(!GetBoneNamed(bone->GetName())){
					pBones.Add(bone);
				}
				
			}else if(nodeModel.GetPropertyAt(2)->CastString().GetValue() == "Root"){
				const fbxRigBone::Ref bone(fbxRigBone::Ref::NewWith(*this, nodeModel, nodeModel));
				bone->SetIndex(pBones.GetCount());
				
				if(!GetBoneNamed(bone->GetName())){
					pBones.Add(bone);
				}
				
				pAddRootBone(scene, nodeModel);
			}
		}
		
	}else{
		const int childCount = scene.GetNodeObjects()->GetNodeCount();
		
		for(i=0; i<childCount; i++){
			fbxNode &nodeModel = *scene.GetNodeObjects()->GetNodeAt(i);
			if(nodeModel.GetName() != "Model"){
				continue;
			}
			
			if(nodeModel.GetPropertyAt(2)->CastString().GetValue() != "Root"){
				continue;
			}
			
			const fbxRigBone::Ref bone(fbxRigBone::Ref::NewWith(*this, nodeModel, nodeModel));
			bone->SetIndex(pBones.GetCount());
			
			if(!GetBoneNamed(bone->GetName())){
				pBones.Add(bone);
			}
			
			pAddRootBone(scene, nodeModel);
		}
	}
	
	
	const int boneCount = pBones.GetCount();
	for(i=0; i<boneCount; i++){
		GetBoneAt(i)->Prepare();
	}
}

fbxRig::~fbxRig(){
}



// Management
///////////////

int fbxRig::GetBoneCount() const{
	return pBones.GetCount();
}

fbxRigBone *fbxRig::GetBoneAt(int index) const{
	return (fbxRigBone*)pBones.GetAt(index);
}

fbxRigBone *fbxRig::GetBoneNamed(const char *name) const{
	const int count = pBones.GetCount();
	int i;
	for(i=0; i<count; i++){
		fbxRigBone * const bone = (fbxRigBone*)pBones.GetAt(i);
		if(bone->GetName() == name){
			return bone;
		}
	}
	return NULL;
}

fbxRigBone *fbxRig::GetBoneWithModelID(int64_t id) const{
	const int count = pBones.GetCount();
	int i;
	for(i=0; i<count; i++){
		fbxRigBone * const bone = (fbxRigBone*)pBones.GetAt(i);
		if(bone->GetNodeModelID() == id){
			return bone;
		}
	}
	return NULL;
}



void fbxRig::DebugPrintStructure(deBaseModule &module, const decString &prefix, bool verbose) const{
	const int count = pBones.GetCount();
	int i;
	
	module.LogInfoFormat("%sRig:", prefix.GetString());
	
	const decString childPrefix(prefix + "  ");
	for(i=0; i<count; i++){
		GetBoneAt(i)->DebugPrintStructure(module, childPrefix, verbose);
	}
}



// Private Functions
//////////////////////

void fbxRig::pAddRootBone(fbxScene &scene, fbxNode &nodeRoot){
	decPointerList cons;
	scene.FindConnections(nodeRoot.GetID(), cons);
	
	const int childCount = cons.GetCount();
	int i;
	
	for(i=0; i<childCount; i++){
		const fbxConnection &con = *(fbxConnection*)cons.GetAt(i);
		if(con.GetTarget() != nodeRoot.GetID()){
			continue;
		}
		
		fbxNode &nodeModel = *scene.NodeWithID(con.GetSource());
		if(nodeModel.GetName() != "Model"){
			continue;
		}
		
		if(nodeModel.GetPropertyAt(2)->CastString().GetValue() != "LimbNode"
		&& nodeModel.GetPropertyAt(2)->CastString().GetValue() != "Root"){
			continue;
		}
		
		const fbxRigBone::Ref bone(fbxRigBone::Ref::NewWith(*this, nodeModel, nodeModel));
		bone->SetIndex(pBones.GetCount());
		
		if(!GetBoneNamed(bone->GetName())){
			pBones.Add(bone);
		}
		
		pAddRootBone(scene, nodeModel);
	}
}
