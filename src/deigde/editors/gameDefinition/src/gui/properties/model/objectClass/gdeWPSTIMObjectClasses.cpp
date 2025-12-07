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

#include "gdeWPSTIMObjectClass.h"
#include "gdeWPSTIMObjectClasses.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItem.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMObjectClasses::gdeWPSTIMObjectClasses(gdeWPSTreeModel &tree) :
gdeWPSTreeItemModel(tree, etObjectClasses)
{
	SetText("Object Classes");
	SetIcon(GetWindowMain().GetEnvironment().GetStockIcon(igdeEnvironment::esiNew));
}

gdeWPSTIMObjectClasses::~gdeWPSTIMObjectClasses(){
}



// Management
///////////////

gdeWPSTIMObjectClass *gdeWPSTIMObjectClasses::GetChildWith(gdeObjectClass *objectClass) const{
	gdeWPSTIMObjectClass *child = (gdeWPSTIMObjectClass*)GetFirstChild();
	
	while(child){
		if(child->GetObjectClass() == objectClass){
			return child;
		}
		child = (gdeWPSTIMObjectClass*)child->GetNext();
	}
	
	return NULL;
}

void gdeWPSTIMObjectClasses::StructureChanged(){
	const gdeObjectClassList &list = GetGameDefinition().GetObjectClasses();
	const int count = list.GetCount();
	igdeTreeItem::Ref item;
	int i;
	
	// update existing and add new categories
	for(i=0; i<count; i++){
		gdeObjectClass * const objectClass = list.GetAt(i);
		gdeWPSTIMObjectClass * const modelObjectClass = GetChildWith(objectClass);
		
		if(!modelObjectClass){
			item.TakeOver(new gdeWPSTIMObjectClass(GetTree(), list.GetAt(i)));
			AppendModel(item);
		}
	}
	
	// remove no more existing categories
	igdeTreeItem *child = GetFirstChild();
	while(child){
		gdeWPSTIMObjectClass * const modelObjectClass = (gdeWPSTIMObjectClass*)child;
		child = child->GetNext();
		
		if(!list.Has(modelObjectClass->GetObjectClass())){
			RemoveModel(modelObjectClass);
		}
	}
	
	// sort
	SortChildren();
	
	// validate
	ValidateObjectClassName();
}

void gdeWPSTIMObjectClasses::ValidateObjectClassName(){
	gdeWPSTIMObjectClass *child = (gdeWPSTIMObjectClass*)GetFirstChild();
	while(child){
		child->ValidateObjectClassName();
		child = (gdeWPSTIMObjectClass*)child->GetNext();
	}
}

void gdeWPSTIMObjectClasses::ValidateCategoryName(){
	gdeWPSTIMObjectClass *child = (gdeWPSTIMObjectClass*)GetFirstChild();
	while(child){
		child->ValidateCategoryName();
		child = (gdeWPSTIMObjectClass*)child->GetNext();
	}
}



void gdeWPSTIMObjectClasses::OnAddedToTree(){
	const gdeObjectClassList &list = GetGameDefinition().GetObjectClasses();
	const int count = list.GetCount();
	igdeTreeItem::Ref item;
	int i;
	
	for(i=0; i<count; i++){
		item.TakeOver(new gdeWPSTIMObjectClass(GetTree(), list.GetAt(i)));
		AppendModel(item);
	}
	
	SortChildren();
}

void gdeWPSTIMObjectClasses::OnContextMenu(igdeMenuCascade &contextMenu){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(contextMenu, windowMain.GetActionObjectClassAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionObjectClassPaste());
}

void gdeWPSTIMObjectClasses::SelectBestMatching(const char *string){
	if(!string){
		return;
	}
	
	const decString searchString(decString(string).GetLower());
	igdeTreeItem *child = GetFirstChild();
	gdeObjectClass *bestObjectClass = NULL;
	decString bestName;
	
	while(child){
		gdeObjectClass * const objectClass = ((gdeWPSTIMObjectClass*)child)->GetObjectClass();
		child = child->GetNext();
		
		const decString name(objectClass->GetName().GetLower());
		
		if(name == searchString){
			// exact match is always best
			bestObjectClass = objectClass;
			break;
		}
		
		if(name.FindString(searchString) == -1){
			continue;
		}
		
		// partial matching. best match is the one earlier in the alphabet
		if(bestObjectClass && bestName < name){
			continue;
		}
		
		bestObjectClass = objectClass;
		bestName = name;
	}

	if(bestObjectClass){
		GetGameDefinition().SetActiveObjectClass(bestObjectClass);
		GetGameDefinition().SetSelectedObjectType(gdeGameDefinition::eotObjectClass);
	}
}
