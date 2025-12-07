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

#include "gdeWPSTIMOCComponent.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/component/gdeOCComponent.h"
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

gdeWPSTIMOCComponent::gdeWPSTIMOCComponent(gdeWPSTreeModel &tree, gdeObjectClass *objectClass,
	gdeOCComponent *component, int index) :
gdeWPSTIMOCSubObject(tree, etObjectClassComponent, objectClass, index),
pComponent(NULL)
{
	if(! component){
		DETHROW(deeInvalidParam);
	}
	
	decString text;
	text.Format("Component #%d", index + 1);
	SetText(text);
	
	pComponent = component;
	component->AddReference();
}

gdeWPSTIMOCComponent::~gdeWPSTIMOCComponent(){
	if(pComponent){
		pComponent->FreeReference();
	}
}



// Management
///////////////

void gdeWPSTIMOCComponent::Validate(){
	SetIcon(GetWindowMain().GetEnvironment().GetStockIcon(
		IsValid() ? igdeEnvironment::esiNew : igdeEnvironment::esiWarning));
	ItemChanged();
}

bool gdeWPSTIMOCComponent::IsValid() const{
	deVirtualFileSystem &vfs = *GetWindowMain().GetEnvironment().GetFileSystemGame();
	
	if(! pComponent->GetModelPath().IsEmpty()
	&& ! vfs.ExistsFile(decPath::CreatePathUnix(pComponent->GetModelPath()))){
		return false;
	}
	if(! pComponent->GetSkinPath().IsEmpty()
	&& ! vfs.ExistsFile(decPath::CreatePathUnix(pComponent->GetSkinPath()))){
		return false;
	}
	if(! pComponent->GetRigPath().IsEmpty()
	&& ! vfs.ExistsFile(decPath::CreatePathUnix(pComponent->GetRigPath()))){
		return false;
	}
	if(! pComponent->GetAnimatorPath().IsEmpty()
	&& ! vfs.ExistsFile(decPath::CreatePathUnix(pComponent->GetAnimatorPath()))){
		return false;
	}
	if(! pComponent->GetAnimationPath().IsEmpty()
	&& ! vfs.ExistsFile(decPath::CreatePathUnix(pComponent->GetAnimationPath()))){
		return false;
	}
	if(! pComponent->GetOcclusionMeshPath().IsEmpty()
	&& ! vfs.ExistsFile(decPath::CreatePathUnix(pComponent->GetOcclusionMeshPath()))){
		return false;
	}
	if(! pComponent->GetAudioModelPath().IsEmpty()
	&& ! vfs.ExistsFile(decPath::CreatePathUnix(pComponent->GetAudioModelPath()))){
		return false;
	}
	
	return true;
}

void gdeWPSTIMOCComponent::ValidateObjectClassName(){
	Validate();
}



void gdeWPSTIMOCComponent::OnAddedToTree(){
	Validate();
}

void gdeWPSTIMOCComponent::OnSelected(){
	GetGameDefinition().SetActiveObjectClass(GetObjectClass());
	GetGameDefinition().SetActiveOCComponent(pComponent);
	GetGameDefinition().SetSelectedObjectType(gdeGameDefinition::eotOCComponent);
}

void gdeWPSTIMOCComponent::OnContextMenu(igdeMenuCascade &contextMenu){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(contextMenu, windowMain.GetActionOCComponentAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCComponentRemove());
	helper.MenuSeparator(contextMenu);
	
	helper.MenuCommand(contextMenu, windowMain.GetActionOCComponentCopy());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCComponentCut());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCComponentPaste());
}
