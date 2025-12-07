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

#include "gdeWPSTIMParticleEmitter.h"
#include "gdeWPSTIMParticleEmitters.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/particleemitter/gdeParticleEmitter.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItem.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMParticleEmitters::gdeWPSTIMParticleEmitters(gdeWPSTreeModel &tree) :
gdeWPSTreeItemModel(tree, etParticleEmitters)
{
	SetText("Particle Emitters");
	SetIcon(GetWindowMain().GetEnvironment().GetStockIcon(igdeEnvironment::esiNew));
}

gdeWPSTIMParticleEmitters::~gdeWPSTIMParticleEmitters(){
}



// Management
///////////////

gdeWPSTIMParticleEmitter *gdeWPSTIMParticleEmitters::GetChildWith(
gdeParticleEmitter *particleEmitter) const{
	gdeWPSTIMParticleEmitter *child = (gdeWPSTIMParticleEmitter*)GetFirstChild();
	
	while(child){
		if(child->GetParticleEmitter() == particleEmitter){
			return child;
		}
		child = (gdeWPSTIMParticleEmitter*)child->GetNext();
	}
	
	return NULL;
}

void gdeWPSTIMParticleEmitters::StructureChanged(){
	const gdeParticleEmitterList &list = GetGameDefinition().GetParticleEmitters();
	const int count = list.GetCount();
	igdeTreeItem::Ref item;
	int i;
	
	// update existing and add new categories
	for(i=0; i<count; i++){
		gdeParticleEmitter * const particleEmitter = list.GetAt(i);
		gdeWPSTIMParticleEmitter * const modelParticleEmitter = GetChildWith(particleEmitter);
		
		if(! modelParticleEmitter){
			item.TakeOver(new gdeWPSTIMParticleEmitter(GetTree(), list.GetAt(i)));
			AppendModel(item);
		}
	}
	
	// remove no more existing categories
	igdeTreeItem *child = GetFirstChild();
	while(child){
		gdeWPSTIMParticleEmitter * const modelParticleEmitter = (gdeWPSTIMParticleEmitter*)child;
		child = child->GetNext();
		
		if(! list.Has(modelParticleEmitter->GetParticleEmitter())){
			RemoveModel(modelParticleEmitter);
		}
	}
	
	// sort
	SortChildren();
}



void gdeWPSTIMParticleEmitters::OnAddedToTree(){
	const gdeParticleEmitterList &list = GetGameDefinition().GetParticleEmitters();
	const int count = list.GetCount();
	igdeTreeItem::Ref item;
	int i;
	
	for(i=0; i<count; i++){
		item.TakeOver(new gdeWPSTIMParticleEmitter(GetTree(), list.GetAt(i)));
		AppendModel(item);
	}
	
	SortChildren();
}

void gdeWPSTIMParticleEmitters::OnContextMenu(igdeMenuCascade &contextMenu){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(contextMenu, windowMain.GetActionParticleEmitterAdd());
}

void gdeWPSTIMParticleEmitters::SelectBestMatching(const char *string){
	if(! string){
		return;
	}
	
	const decString searchString(decString(string).GetLower());
	igdeTreeItem *child = GetFirstChild();
	
	while(child){
		gdeParticleEmitter * const particleEmitter = ((gdeWPSTIMParticleEmitter*)child)->GetParticleEmitter();
		child = child->GetNext();
		
		if(particleEmitter->GetName().GetLower().FindString(searchString) == -1){
			continue;
		}
		
		GetGameDefinition().SetActiveParticleEmitter(particleEmitter);
		GetGameDefinition().SetSelectedObjectType(gdeGameDefinition::eotParticleEmitter);
		return;
	}
}
