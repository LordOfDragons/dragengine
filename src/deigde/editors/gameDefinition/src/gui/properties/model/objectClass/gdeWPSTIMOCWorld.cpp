/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "gdeWPSTIMOCWorld.h"
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
#include <dragengine/filesystem/deVirtualFileSystem.h>


// Constructor, destructor
////////////////////////////

gdeWPSTIMOCWorld::gdeWPSTIMOCWorld(gdeWPSTreeModel &tree, gdeObjectClass *objectClass,
	gdeOCWorld *world, int index) :
gdeWPSTIMOCSubObject(tree, etObjectClassWorld, objectClass, index),
pWorld(world)
{
	DEASSERT_NOTNULL(world)
	
	decString text;
	text.Format("World #%d", index + 1);
	SetText(text);
	
	SetIcon(GetWindowMain().GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ));
}


// Management
///////////////

void gdeWPSTIMOCWorld::Validate(){
	SetIcon(GetWindowMain().GetEnvironment().GetStockIcon(
		IsValid() ? igdeEnvironment::esiNew : igdeEnvironment::esiWarning));
	ItemChanged();
}

bool gdeWPSTIMOCWorld::IsValid() const{
	deVirtualFileSystem &vfs = *GetWindowMain().GetEnvironment().GetFileSystemGame();
	
	if(!pWorld->GetPath().IsEmpty()
	&& !vfs.ExistsFile(decPath::CreatePathUnix(pWorld->GetPath()))){
		return false;
	}
	
	return true;
}


void gdeWPSTIMOCWorld::OnAddedToTree(){
	Validate();
}

void gdeWPSTIMOCWorld::OnSelected(){
	GetGameDefinition().SetActiveObjectClass(GetObjectClass());
	GetGameDefinition().SetActiveOCWorld(pWorld);
	GetGameDefinition().SetSelectedObjectType(gdeGameDefinition::eotOCWorld);
}

void gdeWPSTIMOCWorld::OnContextMenu(igdeMenuCascade &contextMenu){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(contextMenu, windowMain.GetActionOCWorldAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCWorldRemove());
	helper.MenuSeparator(contextMenu);
	
	helper.MenuCommand(contextMenu, windowMain.GetActionOCWorldCopy());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCWorldCut());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCWorldPaste());
}
