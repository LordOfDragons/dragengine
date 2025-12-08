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

#include "gdeWPSTIMSkin.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/skin/gdeSkin.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItem.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMSkin::gdeWPSTIMSkin(gdeWPSTreeModel &tree, gdeSkin *skin) :
gdeWPSTreeItemModel(tree, etSkin)
{
	if(!skin){
		DETHROW(deeInvalidParam);
	}
	
	SetText(skin->GetName());
	SetIcon(GetWindowMain().GetEnvironment().GetStockIcon(igdeEnvironment::esiNew));
	
	pSkin = skin;
}

gdeWPSTIMSkin::~gdeWPSTIMSkin(){
}



// Management
///////////////

void gdeWPSTIMSkin::NameChanged(){
	SetText(pSkin->GetName());
	ParentSortItems();
}



int gdeWPSTIMSkin::Compare(const gdeWPSTreeItemModel &item) const{
	const gdeWPSTIMSkin &other = (const gdeWPSTIMSkin &)item;
	return pSkin->GetName().Compare(other.GetSkin()->GetName());
}

void gdeWPSTIMSkin::OnSelected(){
	GetGameDefinition().SetActiveSkin(pSkin);
	GetGameDefinition().SetSelectedObjectType(gdeGameDefinition::eotSkin);
}

void gdeWPSTIMSkin::OnContextMenu(igdeMenuCascade &contextMenu){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(contextMenu, windowMain.GetActionSkinAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionSkinRemove());
}

void gdeWPSTIMSkin::SelectBestMatching(const char *string){
	if(GetParent()){
		((gdeWPSTreeItemModel*)GetParent())->SelectBestMatching(string);
	}
}
