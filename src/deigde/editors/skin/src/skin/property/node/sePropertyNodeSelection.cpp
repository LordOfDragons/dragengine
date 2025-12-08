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

#include "sePropertyNode.h"
#include "sePropertyNodeSelection.h"
#include "../seProperty.h"
#include "../../texture/seTexture.h"
#include "../../seSkin.h"

#include <dragengine/common/exceptions.h>



// Class sePropertyNodeSelection
//////////////////////////////////

// Constructor, destructor
////////////////////////////

sePropertyNodeSelection::sePropertyNodeSelection(seProperty &property) :
pProperty(property){
}

sePropertyNodeSelection::~sePropertyNodeSelection(){
}



// Management
///////////////

void sePropertyNodeSelection::Add(sePropertyNode *node){
	if(!node){
		DETHROW(deeInvalidParam);
	}
	
	if(pSelection.Has(node)){
		return;
	}
	
	node->SetSelected(true);
	pSelection.Add(node);
	NotifyNodeSelectionChanged();
	
	if(!pActive){
		SetActive(node);
	}
}

void sePropertyNodeSelection::Remove(sePropertyNode *node){
	if(!node){
		DETHROW(deeInvalidParam);
	}
	
	if(!pSelection.Has(node)){
		return;
	}
	
	if(pActive == node){
		if(pSelection.GetCount() > 1){
			const int index = pSelection.IndexOf(node);
			if(index == pSelection.GetCount() - 1){
				SetActive(pSelection.GetAt(index - 1));
				
			}else{
				SetActive(pSelection.GetAt(index + 1));
			}
			
		}else{
			SetActive(NULL);
		}
	}
	
	node->SetSelected(false);
	pSelection.Remove(node);
	NotifyNodeSelectionChanged();
}

void sePropertyNodeSelection::RemoveAll(){
	if(pSelection.GetCount() == 0){
		return;
	}
	
	SetActive(NULL);
	
	const int count = pSelection.GetCount();
	int i;
	for(i=0; i<count; i++){
		pSelection.GetAt(i)->SetSelected(false);
	}
	pSelection.RemoveAll();
	NotifyNodeSelectionChanged();
}

void sePropertyNodeSelection::SetSelected(const sePropertyNodeList &list){
	const int count = list.GetCount();
	if(count == pSelection.GetCount()){
		int i;
		for(i=0; i<count; i++){
			if(!pSelection.Has(list.GetAt(i))){
				break;
			}
		}
		
		if(i == count){
			return; // same nodes no matter the order
		}
	}
	
	if(!list.Has(pActive)){
		SetActive(NULL);
	}
	
	const int clearCount = pSelection.GetCount();
	int i;
	for(i=0; i<clearCount; i++){
		pSelection.GetAt(i)->SetSelected(false);
	}
	
	pSelection = list;
	
	for(i=0; i<count; i++){
		pSelection.GetAt(i)->SetSelected(true);
	}
	
	NotifyNodeSelectionChanged();
	
	if(pSelection.GetCount() > 0){
		SetActive(pSelection.GetAt(0));
	}
}



bool sePropertyNodeSelection::HasActive() const{
	return pActive != NULL;
}

void sePropertyNodeSelection::SetActive(sePropertyNode *node){
	if(node == pActive){
		return;
	}
	
	if(pActive){
		pActive->SetActive(false);
	}
	
	pActive = node;
	
	if(node){
		node->SetActive(true);
	}
	
	NotifyActiveNodeChanged();
}



void sePropertyNodeSelection::NotifyNodeSelectionChanged() const{
	if(!pProperty.GetTexture()){
		return;
	}
	
	seTexture * const texture = pProperty.GetTexture();
	if(!texture->GetSkin()){
		return;
	}
	
	texture->GetSkin()->NotifyPropertyNodeSelectionChanged(texture, &pProperty);
}

void sePropertyNodeSelection::NotifyActiveNodeChanged() const{
	if(!pProperty.GetTexture()){
		return;
	}
	
	seTexture * const texture = pProperty.GetTexture();
	if(!texture->GetSkin()){
		return;
	}
	
	texture->GetSkin()->NotifyPropertyActiveNodeChanged(texture, &pProperty);
}
