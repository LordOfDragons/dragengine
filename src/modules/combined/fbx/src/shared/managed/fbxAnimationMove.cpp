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
pName(nodeStack.GetProperties().GetAt(1)->CastString().GetValue()),
pFrameRate(25)
{
	// find layer
	decTList<fbxConnection*> connections;
	animation.GetScene().FindConnections(pNodeStackID, connections);
	
	for(const auto &connection : connections){
		if(connection->GetTarget() != pNodeStackID){
			continue;
		}
		
		pNodeLayer = animation.GetScene().NodeWithID(connection->GetSource());
		pNodeLayerID = pNodeLayer->GetID();
		break;
	}
	
	if(!pNodeLayer){
		DETHROW_INFO(deeInvalidParam, "AnimationLayer missing for AnimationStack");
	}
	
	// find curve nodes
	connections.RemoveAll();
	animation.GetScene().FindConnections(pNodeLayerID, connections);
	
	connections.Visit([&](fbxConnection* connection){
		if(connection->GetTarget() != pNodeLayerID){
			return;
		}
		
		fbxNode &node = *animation.GetScene().NodeWithID(connection->GetSource());
		if(node.GetName() == "AnimationCurveNode"){
			pCurveNodes.Add(fbxAnimationMoveCurves::Ref::New(*this, node));
		}
	});
}

fbxAnimationMove::~fbxAnimationMove(){
}



// Management
///////////////

void fbxAnimationMove::SetName(const char *name){
	pName = name;
}

void fbxAnimationMove::MatchRig(const fbxRig &rig){
	pCurveNodes.Visit([&](fbxAnimationMoveCurves &curves){
		curves.SetRigBone(rig.GetBones().FindNamed(curves.GetBoneName()));
	});
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
	pCurveNodes.Visit([&](fbxAnimationMoveCurves &curves){
		curves.Prepare();
	});
}



void fbxAnimationMove::DebugPrintStructure(deBaseModule &module, const decString &prefix, bool verbose) const{
	module.LogInfoFormat("%sMove '%s': %d curve nodes", prefix.GetString(), pName.GetString(), pCurveNodes.GetCount());
	
	if(!verbose){
		return;
	}
	
	const decString childPrefix(prefix + "  ");
	pCurveNodes.Visit([&](const fbxAnimationMoveCurves &curves){
		curves.DebugPrintStructure(module, childPrefix, true);
	});
}
