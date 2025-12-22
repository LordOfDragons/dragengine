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

#include "reSelectionPushes.h"
#include "reRigPush.h"
#include "../reRig.h"

#include <dragengine/common/exceptions.h>



// Class reSelectionPushes
////////////////////////////

// Constructor, destructor
////////////////////////////

reSelectionPushes::reSelectionPushes(reRig *rig){
	DEASSERT_NOTNULL(rig)
	pRig = rig;
}

reSelectionPushes::~reSelectionPushes(){
	Reset();
}



// Management
///////////////
int reSelectionPushes::IndexOfPushWith(deColliderVolume *collider) const{
	DEASSERT_NOTNULL(collider)
	
	return pPushes.IndexOfMatching([collider](const reRigPush &p){
		return p.GetCollider() == collider;
	});
}

void reSelectionPushes::AddPush(reRigPush *push){
	DEASSERT_FALSE(pPushes.Has(push))
	
	pPushes.Add(push);
	push->SetSelected(true);
	
	pRig->NotifyPushSelectedChanged(push);
	
	if(!pActivePush){
		SetActivePush(push);
	}
}

void reSelectionPushes::RemovePush(reRigPush *push){
	const reRigPush::Ref guard(push);
	pPushes.Remove(push);
	push->SetSelected(false);
	
	if(push == pActivePush){
		if(pPushes.IsNotEmpty()){
			SetActivePush(pPushes.First());
			
		}else{
			SetActivePush(nullptr);
		}
	}
	
	pRig->NotifyPushSelectedChanged(push);
}

void reSelectionPushes::RemoveAllPushes(){
	SetActivePush(nullptr);
	
	pRig->NotifyAllPushesDeselected();
	
	pPushes.Visit([](reRigPush &p){
		p.SetSelected(false);
	});
	pPushes.RemoveAll();
}



bool reSelectionPushes::HasActivePush() const{
	return pActivePush.IsNotNull();
}

void reSelectionPushes::SetActivePush(reRigPush *push){
	if(push != pActivePush){
		if(push){
			DEASSERT_TRUE(pPushes.Has(push))
		}
		
		if(pActivePush){
			pActivePush->SetActive(false);
		}
		
		pActivePush = push;
		
		if(push){
			push->SetActive(true);
		}
		
		pRig->NotifyActivePushChanged();
	}
}

void reSelectionPushes::Reset(){
	RemoveAllPushes();
}
void reSelectionPushes::AddVisiblePushesTo(reRigPush::List &list) const{
	pPushes.Visit([&list](reRigPush *p){
		if(p->IsVisible()){
			list.Add(p);
		}
	});
}
