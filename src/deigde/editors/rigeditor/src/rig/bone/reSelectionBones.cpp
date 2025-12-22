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

#include "reRigBone.h"
#include "reSelectionBones.h"
#include "../reRig.h"

#include <dragengine/common/exceptions.h>



// Class reSelectionBones
///////////////////////////

// Constructor, destructor
////////////////////////////

reSelectionBones::reSelectionBones(reRig *rig){
	DEASSERT_NOTNULL(rig)
	pRig = rig;
}

reSelectionBones::~reSelectionBones(){
	Reset();
}



// Management
///////////////
int reSelectionBones::IndexOfBoneWith(deColliderVolume *collider) const{
	DEASSERT_NOTNULL(collider)
	return pBones.IndexOfMatching([&](const reRigBone &b){
		return b.GetCollider() == collider;
	});
}

void reSelectionBones::AddBone(reRigBone *bone){
	DEASSERT_FALSE(pBones.Has(bone))
	
	pBones.Add(bone);
	bone->SetSelected(true);
	
	pRig->NotifyBoneSelectedChanged(bone);
	
	if(!pActiveBone){
		SetActiveBone(bone);
	}
}

void reSelectionBones::RemoveBone(reRigBone *bone){
	const reRigBone::Ref guard(bone);
	pBones.Remove(bone);
	bone->SetSelected(false);
	
	if(bone == pActiveBone){
		if(pBones.IsNotEmpty()){
			SetActiveBone(pBones.First());
			
		}else{
			SetActiveBone(nullptr);
		}
	}
	
	pRig->NotifyBoneSelectedChanged(bone);
}

void reSelectionBones::RemoveAllBones(){
	SetActiveBone(nullptr);
	
	pRig->NotifyAllBonesDeselected();
	
	pBones.Visit([](reRigBone &b){
		b.SetSelected(false);
	});
	pBones.RemoveAll();
}



bool reSelectionBones::HasActiveBone() const{
	return pActiveBone.IsNotNull();
}

void reSelectionBones::SetActiveBone(reRigBone *bone){
	if(bone != pActiveBone){
		if(bone){
			DEASSERT_TRUE(pBones.Has(bone))
		}
		
		if(pActiveBone){
			pActiveBone->SetActive(false);
		}
		
		pActiveBone = bone;
		
		if(bone){
			bone->SetActive(true);
		}
		
		pRig->NotifyActiveBoneChanged();
	}
}

void reSelectionBones::Reset(){
	RemoveAllBones();
}
