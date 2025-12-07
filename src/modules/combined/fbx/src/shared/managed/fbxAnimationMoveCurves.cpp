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

#include "fbxAnimation.h"
#include "fbxAnimationCurve.h"
#include "fbxAnimationMove.h"
#include "fbxAnimationMoveCurves.h"
#include "../fbxConnection.h"
#include "../fbxNode.h"
#include "../fbxScene.h"
#include "../fbxProperty.h"
#include "../property/fbxPropertyString.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxAnimationMoveCurves
// ////////////////////////

// Constructor, destructor
////////////////////////////

fbxAnimationMoveCurves::fbxAnimationMoveCurves(fbxAnimationMove &move, fbxNode &nodeCurves) :
pMove(move),
pNodeCurves(nodeCurves),
pNodeCurvesID(nodeCurves.GetID()),
pNodeModel(NULL),
pNodeModelID(0),
pTargetProperty(etpUnsupported),
pRigBone(NULL)
{
	// find curves and model
	decPointerList connections;
	move.GetAnimation().GetScene().FindConnections(pNodeCurvesID, connections);
	int i, conCount = connections.GetCount();
	
	for(i=0; i<conCount; i++){
		const fbxConnection &connection = *((fbxConnection*)connections.GetAt(i));
		if(connection.GetTarget() == pNodeCurvesID){
			fbxNode &node = *move.GetAnimation().GetScene().NodeWithID(connection.GetSource());
			if(node.GetName() == "AnimationCurve"){
				if(connection.GetProperty() == "d|X"){
					pCurveX.TakeOver(new fbxAnimationCurve(node));
					
				}else if(connection.GetProperty() == "d|Y"){
					pCurveY.TakeOver(new fbxAnimationCurve(node));
					
				}else if (connection.GetProperty() == "d|Z"){
					pCurveZ.TakeOver(new fbxAnimationCurve(node));
				}
			}
			
		}else{
			fbxNode &node = *move.GetAnimation().GetScene().NodeWithID(connection.GetTarget());
			if(node.GetName() == "Model"){
				pNodeModel = &node;
				pNodeModelID = node.GetID();
				pBoneName = node.GetPropertyAt(1)->CastString().GetValue();
				
				if(connection.GetProperty() == "Lcl Translation"){
					pTargetProperty = etpPosition;
					pDefaultValue = node.GetLocalTranslation();
					
				}else if(connection.GetProperty() == "Lcl Rotation"){
					pTargetProperty = etpRotation;
					pDefaultValue = node.GetLocalRotation();
					
				}else if(connection.GetProperty() == "Lcl Scaling"){
					pTargetProperty = etpScale;
					pDefaultValue = node.GetLocalScaling();
				}
			}
		}
	}
	
	pDefaultValue.x = nodeCurves.GetPropertyFloat("d|X", (float)pDefaultValue.x);
	pDefaultValue.y = nodeCurves.GetPropertyFloat("d|Y", (float)pDefaultValue.y);
	pDefaultValue.z = nodeCurves.GetPropertyFloat("d|Z", (float)pDefaultValue.z);
}

fbxAnimationMoveCurves::~fbxAnimationMoveCurves(){
}



// Management
///////////////

void fbxAnimationMoveCurves::SetTargetProperty(eTargetProperty targetProperty){
	pTargetProperty = targetProperty;
}

void fbxAnimationMoveCurves::SetBoneName(const char *name){
	pBoneName = name;
}

void fbxAnimationMoveCurves::SetRigBone(fbxRigBone *bone){
	pRigBone = bone;
}

void fbxAnimationMoveCurves::SetDefaultValue(const decVector &defaultValue){
	pDefaultValue = defaultValue;
}



void fbxAnimationMoveCurves::Prepare(){
}



void fbxAnimationMoveCurves::DebugPrintStructure(deBaseModule &module, const decString &prefix, bool verbose) const{
	static const char * const tpnames[4] = {"position", "rotation", "scale", "unsupported"};
	module.LogInfoFormat("%sCurves '%s': bone='%s'(%p) targetProperty=%s, curves(%p %p %p)",
		prefix.GetString(), pNodeModel->GetPropertyAt(1)->CastString().GetValue().GetString(),
		pBoneName.GetString(), pRigBone, tpnames[pTargetProperty], (deObject*)pCurveX,
		(deObject*)pCurveY, (deObject*)pCurveZ);
}
