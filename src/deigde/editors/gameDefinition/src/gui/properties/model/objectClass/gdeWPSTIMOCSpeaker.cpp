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

#include "gdeWPSTIMOCSpeaker.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/speaker/gdeOCSpeaker.h"
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

gdeWPSTIMOCSpeaker::gdeWPSTIMOCSpeaker(gdeWPSTreeModel &tree, gdeObjectClass *objectClass,
	gdeOCSpeaker::Ref speaker, int index) :
gdeWPSTIMOCSubObject(tree, etObjectClassSpeaker, objectClass, index)
{
	if(!speaker){
		DETHROW(deeInvalidParam);
	}
	
	decString text;
	text.Format("Speaker #%d", index + 1);
	SetText(text);
	
	SetIcon(GetWindowMain().GetEnvironment().GetStockIcon(igdeEnvironment::esiNew));
	
	pSpeaker = speaker;
}

gdeWPSTIMOCSpeaker::~gdeWPSTIMOCSpeaker(){
}



// Management
///////////////

void gdeWPSTIMOCSpeaker::Validate(){
	SetIcon(GetWindowMain().GetEnvironment().GetStockIcon(
		IsValid() ? igdeEnvironment::esiNew : igdeEnvironment::esiWarning));
	ItemChanged();
}

bool gdeWPSTIMOCSpeaker::IsValid() const{
	deVirtualFileSystem &vfs = *GetWindowMain().GetEnvironment().GetFileSystemGame();
	
	if(!pSpeaker->GetPathSound().IsEmpty()
	&& !vfs.ExistsFile(decPath::CreatePathUnix(pSpeaker->GetPathSound()))){
		return false;
	}
	
	return true;
}



void gdeWPSTIMOCSpeaker::OnAddedToTree(){
	Validate();
}

void gdeWPSTIMOCSpeaker::OnSelected(){
	GetGameDefinition().SetActiveObjectClass(GetObjectClass());
	GetGameDefinition().SetActiveOCSpeaker(pSpeaker);
	GetGameDefinition().SetSelectedObjectType(gdeGameDefinition::eotOCSpeaker);
}

void gdeWPSTIMOCSpeaker::OnContextMenu(igdeMenuCascade &contextMenu){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(contextMenu, windowMain.GetActionOCSpeakerAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCSpeakerRemove());
	helper.MenuSeparator(contextMenu);
	
	helper.MenuCommand(contextMenu, windowMain.GetActionOCSpeakerCopy());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCSpeakerCut());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCSpeakerPaste());
}
