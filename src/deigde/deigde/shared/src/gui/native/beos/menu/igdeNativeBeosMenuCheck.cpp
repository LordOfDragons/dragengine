/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeosMenuCheck.h"
#include "../../../menu/igdeMenuCheck.h"
#include <interface/Menu.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>

static const uint32 kMsgCheck = 'mChk';


// Class igdeNativeBeosMenuCheck
/////////////////////////////////

igdeNativeBeosMenuCheck::igdeNativeBeosMenuCheck(igdeMenuCheck &owner, BMenu *parent) :
BMenuItem(owner.GetText().GetString(), new BMessage(kMsgCheck)),
pOwner(&owner)
{
	SetMarked(owner.GetChecked());
	if(parent){
		parent->AddItem(this);
	}
	UpdateEnabled();
}

igdeNativeBeosMenuCheck::~igdeNativeBeosMenuCheck() = default;



igdeNativeBeosMenuCheck *igdeNativeBeosMenuCheck::CreateNativeWidget(igdeMenuCheck &owner){
	if(!owner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	BMenu * const parent = (BMenu*)owner.GetParent()->GetNativeContainer();
	if(!parent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeBeosMenuCheck(owner, parent);
}

void igdeNativeBeosMenuCheck::PostCreateNativeWidget(){
}

void igdeNativeBeosMenuCheck::DestroyNativeWidget(){
	if(Menu()){
		Menu()->RemoveItem(this);
	}
	delete this;
}



// cNativeMenuCheck interface
//////////////////////////////

void igdeNativeBeosMenuCheck::UpdateText(){
	SetLabel(pOwner->GetText().GetString());
}

void igdeNativeBeosMenuCheck::UpdateDescription(){
}

void igdeNativeBeosMenuCheck::UpdateHotKey(){
}

void igdeNativeBeosMenuCheck::UpdateIcon(){
}

void igdeNativeBeosMenuCheck::UpdateEnabled(){
	SetEnabled(pOwner->GetEnabled());
}

void igdeNativeBeosMenuCheck::UpdateChecked(){
	SetMarked(pOwner->GetChecked());
}



// BMenuItem overrides
///////////////////////

void igdeNativeBeosMenuCheck::Invoked(){
	BMenuItem::Invoked();
	try{
		if(pOwner && pOwner->GetEnabled()){
			pOwner->OnAction();
		}
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE MenuCheck", e);
	}
}

#endif
