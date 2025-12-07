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

#include "igdeNativeFoxMenuCommand.h"
#include "../../../igdeContainer.h"
#include "../../../igdeCommonDialogs.h"
#include "../../../event/igdeAction.h"
#include "../../../menu/igdeMenuCommand.h"
#include "../../../resources/igdeIcon.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Events
///////////

FXDEFMAP(igdeNativeFoxMenuCommand) igdeNativeFoxMenuCommandMap[] = {
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxMenuCommand::ID_SELF, igdeNativeFoxMenuCommand::onCommand),
	FXMAPFUNC(SEL_UPDATE, igdeNativeFoxMenuCommand::ID_SELF, igdeNativeFoxMenuCommand::onUpdate)
};



// Class igdeNativeFoxMenuCommand
///////////////////////////////////

FXIMPLEMENT(igdeNativeFoxMenuCommand, FXMenuCommand,
	igdeNativeFoxMenuCommandMap, ARRAYNUMBER(igdeNativeFoxMenuCommandMap))

// Constructor, destructor
////////////////////////////

igdeNativeFoxMenuCommand::igdeNativeFoxMenuCommand(){}

igdeNativeFoxMenuCommand::igdeNativeFoxMenuCommand(igdeMenuCommand &powner, FXComposite *pparent) :
FXMenuCommand(pparent, BuildConstrText(powner), powner.GetIcon()
	? (FXIcon*) powner.GetIcon()->GetNativeIcon() : NULL, this, ID_SELF),
pOwner(&powner)
{
	if(!powner.GetEnabled()){
		disable();
	}
}

igdeNativeFoxMenuCommand::~igdeNativeFoxMenuCommand(){
}

igdeNativeFoxMenuCommand *igdeNativeFoxMenuCommand::CreateNativeWidget(igdeMenuCommand &powner){
	if(!powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxMenuCommand(powner, pparent);
}

void igdeNativeFoxMenuCommand::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxMenuCommand::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxMenuCommand::UpdateText(){
	setText(pOwner->GetText().GetString());
}

void igdeNativeFoxMenuCommand::UpdateDescription(){
	const char * const description = pOwner->GetDescription();
	setTipText(description);
	setHelpText(description);
}

void igdeNativeFoxMenuCommand::UpdateHotKey(){
	setAccelText(igdeUIFoxHelper::AccelString(pOwner->GetHotKey()), true);
}

/*
UpdateMnemonic:
	//( ( FXMenuCommand* )GetNativeWidget() )->setKey( igdeUIFoxHelper::MnemonicKey( pMnemonic ) );
*/

void igdeNativeFoxMenuCommand::UpdateIcon(){
	FXIcon *iicon = NULL;
	if(pOwner->GetIcon()){
		iicon = (FXIcon*)pOwner->GetIcon()->GetNativeIcon();
	}
	
	setIcon(iicon);
}

void igdeNativeFoxMenuCommand::UpdateEnabled(){
	if(pOwner->GetEnabled()){
		enable();
		
	}else{
		disable();
	}
}


FXString igdeNativeFoxMenuCommand::BuildConstrText(igdeMenuCommand &powner){
	return igdeUIFoxHelper::MnemonizeString(powner.GetText(), powner.GetMnemonic())
		+ "\t" + igdeUIFoxHelper::AccelString(powner.GetHotKey())
		+ "\t" + powner.GetDescription().GetString();
}



// Events
///////////

long igdeNativeFoxMenuCommand::onCommand(FXObject*, FXSelector, void*){
	if(!pOwner->GetEnabled()){
		return 0;
	}
	
	try{
		pOwner->OnAction();
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(pOwner, e);
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxMenuCommand::onUpdate(FXObject*, FXSelector, void*){
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
