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
#include "fbxAnimationMove.h"
#include "fbxAnimationMoveCurves.h"
#include "fbxRig.h"
#include "../fbxConnection.h"
#include "../fbxNode.h"
#include "../fbxScene.h"
#include "../fbxProperty.h"
#include "../property/fbxPropertyString.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxAnimationMove
// ////////////////////////

// Constructor, destructor
////////////////////////////

fbxAnimationMove::fbxAnimationMove(fbxAnimation &animation, fbxNode &nodeStack) :
pAnimation(animation),
pNodeStack(nodeStack),
pNodeStackID(nodeStack.GetID()),
pNodeLayer(NULL),
pNodeLayerID(0),
pName(nodeStack.GetPropertyAt(1)->CastString().GetValue()),
pFrameRate(25)
{
	// find layer
	decPointerList connections;
	animation.GetScene().FindConnections(pNodeStackID, connections);
	int i, conCount = connections.GetCount();
	
	for(i=0; i<conCount; i++){
		const fbxConnection &connection = *((fbxConnection*)connections.GetAt(i));
		if(connection.GetTarget() != pNodeStackID){
			continue;
		}
		
		pNodeLayer = animation.GetScene().NodeWithID(connection.GetSource());
		pNodeLayerID = pNodeLayer->GetID();
		break;
	}
	
	if(!pNodeLayer){
		DETHROW_INFO(deeInvalidParam, "AnimationLayer missing for AnimationStack");
	}
	
	// find curve nodes
	connections.RemoveAll();
	animation.GetScene().FindConnections(pNodeLayerID, connections);
	conCount = connections.GetCount();
	
	for(i=0; i<conCount; i++){
		const fbxConnection &connection = *((fbxConnection*)connections.GetAt(i));
		if(connection.GetTarget() != pNodeLayerID){
			continue;
		}
		
		fbxNode &node = *animation.GetScene().NodeWithID(connection.GetSource());
		if(node.GetName() == "AnimationCurveNode"){
			pCurveNodes.Add(fbxAnimationMoveCurves::Ref::NewWith(*this, node));
		}
	}
}

fbxAnimationMove::~fbxAnimationMove(){
}



// Management
///////////////

void fbxAnimationMove::SetName(const char *name){
	pName = name;
}

int fbxAnimationMove::GetCurvesCount() const{
	return pCurveNodes.GetCount();
}

fbxAnimationMoveCurves *fbxAnimationMove::GetCurvesAt(int index) const{
	return (fbxAnimationMoveCurves*)pCurveNodes.GetAt(index);
}

void fbxAnimationMove::MatchRig(const fbxRig &rig){
	const int count = pCurveNodes.GetCount();
	int i;
	for(i=0; i<count; i++){
		fbxAnimationMoveCurves &curves = *((fbxAnimationMoveCurves*)pCurveNodes.GetAt(i));
		curves.SetRigBone(rig.GetBoneNamed(curves.GetBoneName()));
	}
}

void fbxAnimationMove::SetFrameRate(int frameRate){
	pFrameRate = frameRate;
}

int fbxAnimationMove::TimeToFrame(float time) const{
	return (int)((time * pFrameRate) + 0.5f);
}

float fbxAnimationMove::FrameToTime(int frame) const{
	return (float)frame / (float)pFrameRate;
}

float fbxAnimationMove::QuantizeTime(float time) const{
	return floorf((time * pFrameRate) + 0.5f) / (float)pFrameRate;
}



void fbxAnimationMove::Prepare(){
	const int count = pCurveNodes.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		GetCurvesAt(i)->Prepare();
	}
}



void fbxAnimationMove::DebugPrintStructure(deBaseModule &module, const decString &prefix, bool verbose) const{
	module.LogInfoFormat("%sMove '%s': %d curve nodes", prefix.GetString(), pName.GetString(), pCurveNodes.GetCount());
	
	if(!verbose){
		return;
	}
	
	const int curvesCount = pCurveNodes.GetCount();
	const decString childPrefix(prefix + "  ");
	int i;
	for(i=0; i<curvesCount; i++){
		GetCurvesAt(i)->DebugPrintStructure(module, childPrefix, true);
	}
}
