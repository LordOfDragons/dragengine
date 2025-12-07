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
	if(! powner.GetEnabled()){
		disable();
	}
	
	setCheck(powner.GetSelected());
}

igdeNativeFoxMenuOption::~igdeNativeFoxMenuOption(){
}

igdeNativeFoxMenuOption *igdeNativeFoxMenuOption::CreateNativeWidget(igdeMenuOption &powner){
	if(! powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(! pparent){
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

void igdeNativeFoxMenuOption::UpdateSelected(){
	setCheck(pOwner->GetSelected());
}


FXString igdeNativeFoxMenuOption::BuildConstrText(igdeMenuOption &powner){
	return igdeUIFoxHelper::MnemonizeString(powner.GetText(), powner.GetMnemonic())
		+ "\t" + igdeUIFoxHelper::AccelString(powner.GetHotKey())
		+ "\t" + powner.GetDescription().GetString();
}



// Events
///////////

long igdeNativeFoxMenuOption::onMenuAction(FXObject*, FXSelector, void*){
	if(! pOwner->GetEnabled()){
		return 0;
	}
	
	pOwner->SetSelected(true);
	// TODO set others in group unselected
	
	try{
		pOwner->OnAction();
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(pOwner, e);
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxMenuOption::updateMenuAction(FXObject*, FXSelector, void*){
	igdeAction * const action = pOwner->GetAction();
	if(! action){
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
