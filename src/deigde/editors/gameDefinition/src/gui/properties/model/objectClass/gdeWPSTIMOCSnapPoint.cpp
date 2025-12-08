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

#include "gdeWPSTIMOCSnapPoint.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/snappoint/gdeOCSnapPoint.h"
#include "../../../../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItem.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTIMOCSnapPoint::gdeWPSTIMOCSnapPoint(gdeWPSTreeModel &tree, gdeObjectClass *objectClass,
	gdeOCSnapPoint::Ref snapPoint, int index) :
gdeWPSTIMOCSubObject(tree, etObjectClassSnapPoint, objectClass, index),
pIndex(index)
{
	if(!snapPoint){
		DETHROW(deeInvalidParam);
	}
	
	decString text;
	text.Format("Snap Point #%d: %s", index + 1, snapPoint->GetName().GetString());
	SetText(text);
	
	SetIcon(GetWindowMain().GetEnvironment().GetStockIcon(igdeEnvironment::esiNew));
	
	pSnapPoint = snapPoint;
}

gdeWPSTIMOCSnapPoint::~gdeWPSTIMOCSnapPoint(){
}



// Management
///////////////

void gdeWPSTIMOCSnapPoint::Validate(){
	SetIcon(GetWindowMain().GetEnvironment().GetStockIcon(
		IsValid() ? igdeEnvironment::esiNew : igdeEnvironment::esiWarning));
	ItemChanged();
}

bool gdeWPSTIMOCSnapPoint::IsValid() const{
	return true;
}



void gdeWPSTIMOCSnapPoint::OnAddedToTree(){
	Validate();
}

void gdeWPSTIMOCSnapPoint::OnSelected(){
	GetGameDefinition().SetActiveObjectClass(GetObjectClass());
	GetGameDefinition().SetActiveOCSnapPoint(pSnapPoint);
	GetGameDefinition().SetSelectedObjectType(gdeGameDefinition::eotOCSnapPoint);
}

void gdeWPSTIMOCSnapPoint::OnContextMenu(igdeMenuCascade &contextMenu){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(contextMenu, windowMain.GetActionOCSnapPointAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCSnapPointRemove());
	helper.MenuSeparator(contextMenu);
	
	helper.MenuCommand(contextMenu, windowMain.GetActionOCSnapPointCopy());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCSnapPointCut());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCSnapPointPaste());
}

void gdeWPSTIMOCSnapPoint::UpdateName(){
	decString text;
	text.Format("Snap Point #%d: %s", pIndex + 1, pSnapPoint->GetName().GetString());
	SetText(text);
	ItemChanged();
}
