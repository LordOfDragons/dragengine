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

#include "gdeWPSTIMOCParticleEmitter.h"
#include "../../../gdeWindowMain.h"
#include "../../../../gamedef/gdeGameDefinition.h"
#include "../../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../../gamedef/objectClass/particleemitter/gdeOCParticleEmitter.h"
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

gdeWPSTIMOCParticleEmitter::gdeWPSTIMOCParticleEmitter(gdeWPSTreeModel &tree,
	gdeObjectClass *objectClass, gdeOCParticleEmitter *particleEmitter, int index) :
gdeWPSTIMOCSubObject(tree, etObjectClassParticleEmitter, objectClass, index),
pParticleEmitter(NULL)
{
	if(! particleEmitter){
		DETHROW(deeInvalidParam);
	}
	
	decString text;
	text.Format("Particle Emitter #%d", index + 1);
	SetText(text);
	
	SetIcon(GetWindowMain().GetEnvironment().GetStockIcon(igdeEnvironment::esiNew));
	
	pParticleEmitter = particleEmitter;
	particleEmitter->AddReference();
}

gdeWPSTIMOCParticleEmitter::~gdeWPSTIMOCParticleEmitter(){
	if(pParticleEmitter){
		pParticleEmitter->FreeReference();
	}
}



// Management
///////////////

void gdeWPSTIMOCParticleEmitter::Validate(){
	SetIcon(GetWindowMain().GetEnvironment().GetStockIcon(
		IsValid() ? igdeEnvironment::esiNew : igdeEnvironment::esiWarning));
	ItemChanged();
}

bool gdeWPSTIMOCParticleEmitter::IsValid() const{
	deVirtualFileSystem &vfs = *GetWindowMain().GetEnvironment().GetFileSystemGame();
	
	if(! pParticleEmitter->GetPath().IsEmpty()
	&& ! vfs.ExistsFile(decPath::CreatePathUnix(pParticleEmitter->GetPath()))){
		return false;
	}
	
	return true;
}



void gdeWPSTIMOCParticleEmitter::OnAddedToTree(){
	Validate();
}

void gdeWPSTIMOCParticleEmitter::OnSelected(){
	GetGameDefinition().SetActiveObjectClass(GetObjectClass());
	GetGameDefinition().SetActiveOCParticleEmitter(pParticleEmitter);
	GetGameDefinition().SetSelectedObjectType(gdeGameDefinition::eotOCParticleEmitter);
}

void gdeWPSTIMOCParticleEmitter::OnContextMenu(igdeMenuCascade &contextMenu){
	const gdeWindowMain &windowMain = GetWindowMain();
	igdeUIHelper &helper = windowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand(contextMenu, windowMain.GetActionOCParticleEmitterAdd());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCParticleEmitterRemove());
	helper.MenuSeparator(contextMenu);
	
	helper.MenuCommand(contextMenu, windowMain.GetActionOCParticleEmitterCopy());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCParticleEmitterCut());
	helper.MenuCommand(contextMenu, windowMain.GetActionOCParticleEmitterPaste());
}
