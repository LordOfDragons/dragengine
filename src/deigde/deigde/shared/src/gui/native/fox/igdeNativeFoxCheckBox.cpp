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

#include "igdeNativeFoxCheckBox.h"
#include "foxIcons.h"
#include "../../igdeCheckBox.h"
#include "../../igdeContainer.h"
#include "../../igdeCommonDialogs.h"
#include "../../event/igdeAction.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../resources/igdeIcon.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Events
///////////

FXDEFMAP(igdeNativeFoxCheckBox) igdeNativeFoxCheckBoxMap[] = {
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxCheckBox::ID_SELF, igdeNativeFoxCheckBox::onCommand),
	FXMAPFUNC(SEL_UPDATE, igdeNativeFoxCheckBox::ID_SELF, igdeNativeFoxCheckBox::onUpdate)
};



// Class igdeNativeFoxCheckBox
////////////////////////////////

FXIMPLEMENT(igdeNativeFoxCheckBox, FXCheckButton, igdeNativeFoxCheckBoxMap, ARRAYNUMBER(igdeNativeFoxCheckBoxMap))

// Constructor, destructor
////////////////////////////

igdeNativeFoxCheckBox::igdeNativeFoxCheckBox(){}

igdeNativeFoxCheckBox::igdeNativeFoxCheckBox(igdeCheckBox &powner, FXComposite *pparent,
int layoutFlags, const igdeGuiTheme &guitheme) :
FXCheckButton(pparent, CheckBoxText(powner), this, ID_SELF, layoutFlags | CheckBoxFlags(powner),
	0, 0, 0, 0,
	CheckBoxPadLeft(guitheme), CheckBoxPadRight(guitheme),
	CheckBoxPadTop(guitheme), CheckBoxPadBottom(guitheme)),
pOwner(&powner),
pFont(CheckBoxFont(powner, guitheme))
{
	setFont((FXFont*)pFont->GetNativeFont());
	
	if(!powner.GetEnabled()){
		disable();
	}
	setCheck(powner.GetChecked());
	setIcon(CheckBoxIcon(powner));
	setTipText(powner.GetDescription().GetString());
	setHelpText(powner.GetDescription().GetString());
}

igdeNativeFoxCheckBox::~igdeNativeFoxCheckBox(){
}

igdeNativeFoxCheckBox *igdeNativeFoxCheckBox::CreateNativeWidget(igdeCheckBox &powner){
	if(!powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxCheckBox(powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlags(&powner), *powner.GetGuiTheme());
}

void igdeNativeFoxCheckBox::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxCheckBox::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

FXbool igdeNativeFoxCheckBox::canFocus() const{
	if(pOwner->GetStyle() == igdeCheckBox::ebsToolBar){
		return false;
		
	}else{
		return FXCheckButton::canFocus();
	}
}

void igdeNativeFoxCheckBox::UpdateChecked(){
	if(pOwner->GetChecked() != (bool)getCheck()){
		setCheck(pOwner->GetChecked());
	}
}

void igdeNativeFoxCheckBox::UpdateStyle(){
	setText(igdeNativeFoxCheckBox::CheckBoxText(*pOwner));
	setIcon(igdeNativeFoxCheckBox::CheckBoxIcon(*pOwner));
	setCheckButtonStyle(igdeNativeFoxCheckBox::CheckBoxFlags(*pOwner));
}

void igdeNativeFoxCheckBox::UpdateText(){
	setText(igdeNativeFoxCheckBox::CheckBoxText(*pOwner));
}

void igdeNativeFoxCheckBox::UpdateDescription(){
	const char * const description = pOwner->GetDescription();
	setTipText(description);
	setHelpText(description);
}

void igdeNativeFoxCheckBox::UpdateIcon(){
	setIcon(igdeNativeFoxCheckBox::CheckBoxIcon(*pOwner));
}

void igdeNativeFoxCheckBox::UpdateEnabled(){
	if(pOwner->GetEnabled()){
		enable();
		
	}else{
		disable();
	}
}



const char *igdeNativeFoxCheckBox::CheckBoxText(const igdeCheckBox &powner){
	return powner.GetText();
}

FXIcon *igdeNativeFoxCheckBox::CheckBoxIcon(const igdeCheckBox &powner){
	if(powner.GetIcon()){
		return (FXIcon*) powner.GetIcon()->GetNativeIcon();
		
	}else{
		return nullptr;
	}
}

int igdeNativeFoxCheckBox::CheckBoxFlags(const igdeCheckBox &powner){
	if(powner.GetStyle() == igdeCheckBox::ebsToolBar){
		return JUSTIFY_LEFT | ICON_BEFORE_TEXT | FRAME_RAISED;
		
	}else{
		return JUSTIFY_LEFT | ICON_BEFORE_TEXT;
	}
}

igdeFont *igdeNativeFoxCheckBox::CheckBoxFont(const igdeCheckBox &powner, const igdeGuiTheme &guitheme){
	igdeFont::sConfiguration configuration;
	powner.GetEnvironment().GetApplicationFont(configuration);
	
	if(guitheme.GetProperties().Has(igdeGuiThemePropertyNames::checkBoxFontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::checkBoxFontSizeAbsolute, 0);
		
	}else if(guitheme.GetProperties().Has(igdeGuiThemePropertyNames::checkBoxFontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::checkBoxFontSize, 1.0f);
		
	}else if(guitheme.GetProperties().Has(igdeGuiThemePropertyNames::fontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0);
		
	}else if(guitheme.GetProperties().Has(igdeGuiThemePropertyNames::fontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f);
	}
	
	return powner.GetEnvironment().GetSharedFont(configuration);
}

int igdeNativeFoxCheckBox::CheckBoxPadLeft(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::checkBoxPaddingLeft, DEFAULT_PAD);
}

int igdeNativeFoxCheckBox::CheckBoxPadRight(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::checkBoxPaddingRight, DEFAULT_PAD);
}

int igdeNativeFoxCheckBox::CheckBoxPadTop(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::checkBoxPaddingTop, DEFAULT_PAD);
}

int igdeNativeFoxCheckBox::CheckBoxPadBottom(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::checkBoxPaddingBottom, DEFAULT_PAD);
}



// Events
///////////

long igdeNativeFoxCheckBox::onCommand(FXObject*, FXSelector, void*){
	if(!pOwner->GetEnabled()){
		return 0;
	}
	
	pOwner->SetChecked(getCheck());
	
	try{
		pOwner->OnAction();
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(pOwner, e);
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxCheckBox::onUpdate(FXObject*, FXSelector, void*){
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
