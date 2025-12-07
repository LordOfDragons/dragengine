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

#include "gdeWPSTIMSkies.h"
#include "gdeWPSTIMSky.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/sky/gdeSky.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItem.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMSkies::gdeWPSTIMSkies(gdeWPSTreeModel &tree) :
gdeWPSTreeItemModel(tree, etSkies)
{
	SetText("Skies");
	SetIcon(GetWindowMain().GetEnvironment().GetStockIcon(igdeEnvironment::esiNew));
}

gdeWPSTIMSkies::~gdeWPSTIMSkies(){
}



// Management
///////////////

gdeWPSTIMSky *gdeWPSTIMSkies::GetChildWith(gdeSky *sky) const{
	gdeWPSTIMSky *child = (gdeWPSTIMSky*)GetFirstChild();
	
	while(child){
		if(child->GetSky() == sky){
			return child;
		}
		child = (gdeWPSTIMSky*)child->GetNext();
	}
	
	return NULL;
}

void gdeWPSTIMSkies::StructureChanged(){
	const gdeSkyList &list = GetGameDefinition().GetSkies();
	const int count = list.GetCount();
	igdeTreeItem::Ref item;
	int i;
	
	// update existing and add new categories
	for(i=0; i<count; i++){
		gdeSky * const sky = list.GetAt(i);
		gdeWPSTIMSky * const modelSky = GetChildWith(sky);
		
		if(!modelSky){
			item.TakeOver(new gdeWPSTIMSky(GetTree(), list.GetAt(i)));
			AppendModel(item);
		}
	}
	
	// remove no more existing categories
	igdeTreeItem *child = GetFirstChild();
	while(child){
		gdeWPSTIMSky * const modelSky = (gdeWPSTIMSky*)child;
		child = child->GetNext();
		
		if(!list.Has(modelSky->GetSky())){
			RemoveModel(modelSky);
		}
	}
	
	// sort
	SortChildren();
}



void gdeWPSTIMSkies::OnAddedToTree(){
	const gdeSkyList &list = GetGameDefinition().GetSkies();
	const int count = list.GetCount();
	igdeTreeItem::Ref item;
	int i;
	
	for(i=0; i<count; i++){
		item.TakeOver(new gdeWPSTIMSky(GetTree(), list.GetAt(i)));
		AppendModel(item);
	}
	
	SortChildren();
}

void gdeWPSTIMSkies::OnContextMenu(igdeMenuCascade &contextMenu){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(contextMenu, windowMain.GetActionSkyAdd());
}

void gdeWPSTIMSkies::SelectBestMatching(const char *string){
	if(!string){
		return;
	}
	
	const decString searchString(decString(string).GetLower());
	igdeTreeItem *child = GetFirstChild();
	
	while(child){
		gdeSky * const sky = ((gdeWPSTIMSky*)child)->GetSky();
		child = child->GetNext();
		
		if(sky->GetName().GetLower().FindString(searchString) == -1){
			continue;
		}
		
		GetGameDefinition().SetActiveSky(sky);
		GetGameDefinition().SetSelectedObjectType(gdeGameDefinition::eotSky);
		return;
	}
}
