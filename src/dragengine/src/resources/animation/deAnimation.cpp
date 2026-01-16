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

#include "deAnimation.h"
#include "deAnimationBone.h"
#include "deAnimationMove.h"
#include "deAnimationManager.h"
#include "../model/deModel.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decPath.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/animator/deBaseAnimatorAnimation.h"



// Class deAnimation
//////////////////////

// Constructor, destructor
////////////////////////////

deAnimation::deAnimation(deAnimationManager *manager, deVirtualFileSystem *vfs,
	const char *filename, TIME_SYSTEM modificationTime) :
deFileResource(manager, vfs, filename, modificationTime),
pPeerAnimator(nullptr){
}

deAnimation::~deAnimation(){
	pCleanUp();
}



// Management
///////////////

bool deAnimation::MatchesModel(deModel *model) const{
	DEASSERT_NOTNULL(model)
	
	return pBones.AllMatching([&](const deAnimationBone &bone){
		return model->HasBoneNamed(bone.GetName());
	})
	&& pVertexPositionSets.AllMatching([&](const decString &vpsName){
		return model->HasVertexPositionSetNamed(vpsName);
	});
}



// Bones
//////////

const deAnimationBone::Ref &deAnimation::GetBone(int index) const{
	return pBones.GetAt(index);
}

int deAnimation::FindBone(const char *name) const{
	return pBones.IndexOfMatching([&](const deAnimationBone &bone){
		return bone.GetName() == name;
	});
}

void deAnimation::AddBone(deTUniqueReference<deAnimationBone> &&bone){
	if(!bone || FindBone(bone->GetName()) != -1){
		DETHROW(deeInvalidParam);
	}
	
	pBones.Add(std::move(bone));
}



// Moves
//////////

const deAnimationMove::Ref &deAnimation::GetMove(int index) const{
	return pMoves.GetAt(index);
}

int deAnimation::FindMove(const char *name) const{
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	return pMoves.IndexOfMatching([&](const deAnimationMove &move){
		return move.GetName() == name;
	});
}

void deAnimation::AddMove(deTUniqueReference<deAnimationMove> &&move){
	if(!move || FindMove(move->GetName()) != -1){
		DETHROW(deeInvalidParam);
	}
	
	pMoves.Add(std::move(move));
}



// System Peers
/////////////////

void deAnimation::SetPeerAnimator(deBaseAnimatorAnimation *peer){
	if(peer == pPeerAnimator){
		return;
	}
	
	if(pPeerAnimator){
		delete pPeerAnimator;
	}
	pPeerAnimator = peer;
}



// Private Functions
//////////////////////

void deAnimation::pCleanUp(){
	if(pPeerAnimator){
		delete pPeerAnimator;
		pPeerAnimator = nullptr;
	}
}
