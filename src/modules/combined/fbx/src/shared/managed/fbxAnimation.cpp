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
#include "fbxRig.h"
#include "fbxRigBone.h"
#include "../fbxConnection.h"
#include "../fbxNode.h"
#include "../fbxScene.h"
#include "../fbxProperty.h"
#include "../property/fbxPropertyString.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxAnimation
///////////////////////

// Constructor, destructor
////////////////////////////

fbxAnimation::fbxAnimation(fbxScene &scene) :
pScene(scene)
{
	decPointerList nodeStacks;
	scene.FindNodesNamed(nodeStacks, "AnimationStack");
	
	const int nodeStackCount = nodeStacks.GetCount();
	int i;
	
	for(i=0; i<nodeStackCount; i++){
		pMoves.Add(fbxAnimationMove::Ref::NewWith(*this, *((fbxNode*)nodeStacks.GetAt(i))));
	}
	
	const int moveCount = pMoves.GetCount();
	for(i=0; i<moveCount; i++){
		GetMoveAt(i)->Prepare();
	}
}

fbxAnimation::~fbxAnimation(){
}



// Management
///////////////

int fbxAnimation::GetMoveCount() const{
	return pMoves.GetCount();
}

fbxAnimationMove *fbxAnimation::GetMoveAt(int index) const{
	return (fbxAnimationMove*)pMoves.GetAt(index);
}

fbxAnimationMove *fbxAnimation::GetMoveNamed(const char *name) const{
	const int count = pMoves.GetCount();
	int i;
	for(i=0; i<count; i++){
		fbxAnimationMove * const move = (fbxAnimationMove*)pMoves.GetAt(i);
		if(move->GetName() == name){
			return move;
		}
	}
	return NULL;
}

void fbxAnimation::MatchRig(const fbxRig &rig){
	const int count = pMoves.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((fbxAnimationMove*)pMoves.GetAt(i))->MatchRig(rig);
	}
}

float fbxAnimation::ConvTime(int64_t time){
	// time unit in FBX (FbxTime) is 1/46186158000 of one second
	return (float)((double)time / 46186158000.0);
}



void fbxAnimation::DebugPrintStructure(deBaseModule &module, const decString &prefix, bool verbose) const{
	const int count = pMoves.GetCount();
	int i;
	
	module.LogInfoFormat("%sAnimation", prefix.GetString());
	
	const decString childPrefix(prefix + "  ");
	for(i=0; i<count; i++){
		GetMoveAt(i)->DebugPrintStructure(module, childPrefix, verbose);
	}
}



// Private Functions
//////////////////////
