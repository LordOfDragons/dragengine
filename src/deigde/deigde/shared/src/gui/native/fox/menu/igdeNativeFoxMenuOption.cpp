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

#ifdef IGDE_TOOLKIT_FOX

#include "igdeNativeFoxMenuOption.h"
#include "../../../igdeContainer.h"
#include "../../../igdeCommonDialogs.h"
#include "../../../event/igdeAction.h"
#include "../../../menu/igdeMenuOption.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Events
///////////

FXDEFMAP(igdeNativeFoxMenuOption) igdeNativeFoxMenuOptionMap[] = {
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxMenuOption::ID_SELF, igdeNativeFoxMenuOption::onMenuAction),
	FXMAPFUNC(SEL_UPDATE, igdeNativeFoxMenuOption::ID_SELF, igdeNativeFoxMenuOption::updateMenuAction)
};



// Class igdeNativeFoxMenuOption
/////////////////////////////////

FXIMPLEMENT(igdeNativeFoxMenuOption, FXMenuRadio,
	igdeNativeFoxMenuOptionMap, ARRAYNUMBER(igdeNativeFoxMenuOptionMap))

// Constructor, destructor
////////////////////////////

igdeNativeFoxMenuOption::igdeNativeFoxMenuOption(){}

igdeNativeFoxMenuOption::igdeNativeFoxMenuOption(igdeMenuOption &powner, FXComposite *pparent) :
FXMenuRadio(pparent, BuildConstrText(powner), this, ID_SELF),
pOwner(&powner)
{
	setCheck(powner.GetSelected());
	UpdateHotKey();
	
	if(!powner.GetEnabled()){
		disable();
	}
}

igdeNativeFoxMenuOption::~igdeNativeFoxMenuOption(){
}

igdeNativeFoxMenuOption *igdeNativeFoxMenuOption::CreateNativeWidget(igdeMenuOption &powner){
	if(!powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxMenuOption(powner, pparent);
}

void igdeNativeFoxMenuOption::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxMenuOption::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxMenuOption::UpdateText(){
	setText(igdeUIFoxHelper::TranslateIf(*pOwner, pOwner->GetText().GetString()));
}

void igdeNativeFoxMenuOption::UpdateDescription(){
	const FXString description(igdeUIFoxHelper::TranslateIf(*pOwner, pOwner->GetDescription().GetString()));
	setTipText(description);
	setHelpText(description);
}

void igdeNativeFoxMenuOption::UpdateHotKey(){
	// this here is annoying. since the accel text also sets the hotkey it has to be in English
	// or it breaks. but for display display we want the user language. to solve this we have
	// to set the accel text twice, once in English to set the hotkey and once in the user
	// language to show the correct text
	setAccelText(igdeUIFoxHelper::AccelStringSystem(pOwner->GetHotKey()), true);
	setAccelText(igdeUIFoxHelper::AccelStringTranslated(*pOwner, pOwner->GetHotKey()), false);
}

void igdeNativeFoxMenuOption::UpdateIcon(){
	FXIcon *iicon = nullptr;
	if(pOwner->GetIcon()){
		iicon = (FXIcon*)pOwner->GetIcon()->GetNativeIcon();
	}
	
	setIcon(iicon);
}

void igdeNativeFoxMenuOption::UpdateEnabled(){
	if(pOwner->GetEnabled()){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxMenuOption::UpdateSelected(){
	setCheck(pOwner->GetSelected());
}


FXString igdeNativeFoxMenuOption::BuildConstrText(igdeMenuOption &powner){
	const FXString text(igdeUIFoxHelper::TranslateIf(powner, powner.GetText().GetString()));
	return igdeUIFoxHelper::MnemonizeString(text.text(), powner.GetMnemonic())
		+ "\t" + igdeUIFoxHelper::AccelStringSystem(powner.GetHotKey())
		+ "\t" + igdeUIFoxHelper::TranslateIf(powner, powner.GetDescription());
}



// Events
///////////

long igdeNativeFoxMenuOption::onMenuAction(FXObject*, FXSelector, void*){
	if(!pOwner->GetEnabled()){
		return 0;
	}
	
	pOwner->SetSelected(true);
	// TODO set others in group unselected
	
	try{
		pOwner->OnAction();
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(*pOwner, e);
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxMenuOption::updateMenuAction(FXObject*, FXSelector, void*){
	igdeAction * const action = pOwner->GetAction();
	if(!action){
		return 0;
	}
	
	try{
		action->Update();
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
	}
	
	return 0;
}

#endif
