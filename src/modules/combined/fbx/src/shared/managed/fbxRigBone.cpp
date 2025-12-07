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


// Class fbxRigBone
/////////////////////

// Constructor, destructor
////////////////////////////

fbxRigBone::fbxRigBone(fbxRig &rig, fbxNode &nodePoseBone, fbxNode &nodeModel) :
pRig(rig),
pNodePoseBone(nodePoseBone),
pNodeModel(nodeModel),
pNodeArmature(NULL),
pNodeModelID(nodeModel.GetID()),
pIndex(0),
pName(nodeModel.GetPropertyAt(1)->CastString().GetValue()),
pParent(NULL),
pDirty(true),
pRotationOrder(fbxScene::ConvRotationOrder(nodeModel.GetPropertyInt("RotationOrder", 0))){
}

fbxRigBone::~fbxRigBone(){
}



// Management
///////////////

void fbxRigBone::SetIndex(int index){
	pIndex = index;
}

void fbxRigBone::SetName(const char *name){
	pName = name;
}



void fbxRigBone::Prepare(){
	if(! pDirty){
		return;
	}
	
	decPointerList connections;
	pRig.GetScene().FindConnections(pNodeModelID, connections);
	const int conCount = connections.GetCount();
	int i;
	
	for(i=0; i<conCount; i++){
		const fbxConnection &connection = *((fbxConnection*)connections.GetAt(i));
		if(connection.GetSource() != pNodeModelID || connection.GetTarget() == 0){
			continue;
		}
		
		fbxRigBone * const bone = pRig.GetBoneWithModelID(connection.GetTarget());
		if(bone){
			pParent = bone;
			break;
		}
		
		// not a PoseBone which means it has to be the armature node
		pNodeArmature = pRig.GetScene().NodeWithID(connection.GetTarget());
		break;
	}
	
	const decMatrix &modelMatrix = pNodeModel.GetTransformation();
	pFbxMatrix = modelMatrix;
	
	if(pParent){
		pParent->Prepare();
		pFbxMatrix = pFbxMatrix.QuickMultiply(pParent->pFbxMatrix);
		pMatrix = pFbxMatrix.QuickMultiply(pRig.GetScene().GetTransformation());
		pMatrixInverse = pMatrix.Normalized().QuickInvert();
		pBoneMatrix = pMatrix.QuickMultiply(pParent->pMatrixInverse);
		pAnimMatrix = pParent->pMatrix.QuickMultiply(pMatrixInverse);
		
	}else if(pNodeArmature){
		const decMatrix &armMatrix = pNodeArmature->GetTransformation();
		
		pFbxMatrix = pFbxMatrix.QuickMultiply(armMatrix);
		pMatrix = pFbxMatrix.QuickMultiply(pRig.GetScene().GetTransformation());
		pMatrixInverse = pMatrix.Normalized().QuickInvert();
		//pBoneMatrix = pMatrix.QuickMultiply( invArmMatrix );
		pBoneMatrix = pMatrix;
		pAnimMatrix = armMatrix
			.QuickMultiply(pRig.GetScene().GetTransformation())
			.QuickMultiply(pMatrixInverse);
		
	}else{
		pMatrix = pFbxMatrix.QuickMultiply(pRig.GetScene().GetTransformation());
		pMatrixInverse = pMatrix.Normalized().QuickInvert();
		pBoneMatrix = pMatrix;
		pAnimMatrix = pMatrixInverse;
	}
	
	const decMatrix normalized(pBoneMatrix.Normalized());
	pPosition = normalized.GetPosition();
	pOrientation = normalized.ToQuaternion();
	
	pDirty = false;
}



void fbxRigBone::DebugPrintStructure(deBaseModule &module, const decString &prefix, bool verbose) const{
	const decVector rotation(pOrientation.GetEulerAngles() * RAD2DEG);
	module.LogInfoFormat("%sBone %d '%s': pos=(%g,%g,%g) rot=(%g,%g,%g) parent=(%s)",
		prefix.GetString(), pIndex, pName.GetString(), pPosition.x, pPosition.y, pPosition.z,
		rotation.x, rotation.y, rotation.z, pParent ? pParent->GetName().GetString() : "<null>");
}
