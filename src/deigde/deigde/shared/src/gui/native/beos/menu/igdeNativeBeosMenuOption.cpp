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

#include "igdeNativeBeosMenuOption.h"
#include "../../../menu/igdeMenuOption.h"
#include <interface/Menu.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>

static const uint32 kMsgOption = 'mOpt';


// Class igdeNativeBeosMenuOption
///////////////////////////////////

igdeNativeBeosMenuOption::igdeNativeBeosMenuOption(igdeMenuOption &owner, BMenu *parent) :
BMenuItem(owner.GetText().GetString(), new BMessage(kMsgOption)),
pOwner(&owner)
{
	SetMarked(owner.GetSelected());
	if(parent){
		parent->AddItem(this);
	}
	UpdateEnabled();
}

igdeNativeBeosMenuOption::~igdeNativeBeosMenuOption() = default;



igdeNativeBeosMenuOption *igdeNativeBeosMenuOption::CreateNativeWidget(igdeMenuOption &owner){
	if(!owner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	BMenu * const parent = (BMenu*)owner.GetParent()->GetNativeContainer();
	if(!parent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeBeosMenuOption(owner, parent);
}

void igdeNativeBeosMenuOption::PostCreateNativeWidget(){
}

void igdeNativeBeosMenuOption::DestroyNativeWidget(){
	if(Menu()){
		Menu()->RemoveItem(this);
	}
	delete this;
}



// cNativeMenuOption interface
///////////////////////////////

void igdeNativeBeosMenuOption::UpdateText(){
	SetLabel(pOwner->GetText().GetString());
}

void igdeNativeBeosMenuOption::UpdateDescription(){
}

void igdeNativeBeosMenuOption::UpdateHotKey(){
}

void igdeNativeBeosMenuOption::UpdateIcon(){
}

void igdeNativeBeosMenuOption::UpdateEnabled(){
	SetEnabled(pOwner->GetEnabled());
}

void igdeNativeBeosMenuOption::UpdateSelected(){
	SetMarked(pOwner->GetSelected());
}



// BMenuItem overrides
///////////////////////

void igdeNativeBeosMenuOption::Invoked(){
	BMenuItem::Invoked();
	try{
		if(pOwner && pOwner->GetEnabled()){
			pOwner->OnAction();
		}
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE MenuOption", e);
	}
}

#endif
