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

#include "igdeNativeFoxTextField.h"
#include "foxIcons.h"
#include "../../igdeTextField.h"
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

FXDEFMAP(igdeNativeFoxTextField) igdeNativeFoxTextFieldMap[] = {
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxTextField::ID_SELF, igdeNativeFoxTextField::onCommand),
	FXMAPFUNC(SEL_CHANGED, igdeNativeFoxTextField::ID_SELF, igdeNativeFoxTextField::onChanged),
	FXMAPFUNC(SEL_KEYPRESS, igdeNativeFoxTextField::ID_SELF, igdeNativeFoxTextField::onKeyPress)
};



// Class igdeNativeFoxTextField
//////////////////////////////

FXIMPLEMENT(igdeNativeFoxTextField, FXTextField, igdeNativeFoxTextFieldMap, ARRAYNUMBER(igdeNativeFoxTextFieldMap))

// Constructor, destructor
////////////////////////////

igdeNativeFoxTextField::igdeNativeFoxTextField(){}

igdeNativeFoxTextField::igdeNativeFoxTextField(igdeTextField &powner, FXComposite *pparent,
int layoutFlags, const igdeGuiTheme &guitheme) :
FXTextField(pparent, powner.GetColumns(), this, ID_SELF, layoutFlags | TextFieldFlags(powner),
	0, 0, 0, 0,
	TextFieldPadLeft(guitheme), TextFieldPadRight(guitheme),
	TextFieldPadTop(guitheme), TextFieldPadBottom(guitheme)),
pOwner(&powner),
pFont(TextFieldFont(powner, guitheme)),
pOrgBackColor(getBackColor()),
pInvalidBackColor(igdeUIFoxHelper::BlendColor(pOrgBackColor, FXRGB(255, 0, 0), 0.25f)),
pPreventKeyPressHook(false)
{
	setFont((FXFont*)pFont->GetNativeFont());
	
	UpdateEditable();
	UpdateEnabled();
	UpdateText();
	UpdateDescription();
}

igdeNativeFoxTextField::~igdeNativeFoxTextField(){
}

igdeNativeFoxTextField *igdeNativeFoxTextField::CreateNativeWidget(igdeTextField &powner){
	if(! powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(! pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxTextField(powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlags(&powner), *powner.GetGuiTheme());
}

void igdeNativeFoxTextField::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxTextField::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxTextField::Focus(){
	setFocus();
}

void igdeNativeFoxTextField::OnInvalidValueChanged(){
	setBackColor(pOwner->GetInvalidValue() ? pInvalidBackColor : pOrgBackColor);
}

void igdeNativeFoxTextField::UpdateText(){
	if(pOwner->GetText() != getText().text()){
		setText(pOwner->GetText().GetString());
	}
}

void igdeNativeFoxTextField::UpdateEnabled(){
	if(pOwner->GetEnabled()){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxTextField::UpdateDescription(){
	setTipText(pOwner->GetDescription().GetString());
	setHelpText(pOwner->GetDescription().GetString());
}

void igdeNativeFoxTextField::UpdateEditable(){
	setEditable(pOwner->GetEditable());
}

int igdeNativeFoxTextField::GetCursorPosition() const{
	return getCursorPos();
}

void igdeNativeFoxTextField::SetCursorPosition(int position){
	setCursorPos(position);
}



int igdeNativeFoxTextField::TextFieldFlags(const igdeTextField &){
	return FRAME_SUNKEN;
}

igdeFont *igdeNativeFoxTextField::TextFieldFont(const igdeTextField &powner, const igdeGuiTheme &guitheme){
	igdeFont::sConfiguration configuration;
	powner.GetEnvironment().GetApplicationFont(configuration);
	
	if(guitheme.HasProperty(igdeGuiThemePropertyNames::textFieldFontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::textFieldFontSizeAbsolute, 0);
		
	}else if(guitheme.HasProperty(igdeGuiThemePropertyNames::textFieldFontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::textFieldFontSize, 1.0f);
		
	}else if(guitheme.HasProperty(igdeGuiThemePropertyNames::fontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0);
		
	}else if(guitheme.HasProperty(igdeGuiThemePropertyNames::fontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f);
	}
	
	return powner.GetEnvironment().GetSharedFont(configuration);
}

int igdeNativeFoxTextField::TextFieldPadLeft(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::textFieldPaddingLeft, DEFAULT_PAD);
}

int igdeNativeFoxTextField::TextFieldPadRight(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::textFieldPaddingRight, DEFAULT_PAD);
}

int igdeNativeFoxTextField::TextFieldPadTop(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::textFieldPaddingTop, DEFAULT_PAD);
}

int igdeNativeFoxTextField::TextFieldPadBottom(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::textFieldPaddingBottom, DEFAULT_PAD);
}



// Events
///////////

long igdeNativeFoxTextField::onCommand(FXObject*, FXSelector, void*){
	if(! pOwner->GetEnabled()){
		return 0;
	}
	
	try{
		pOwner->SetText(getText().text(), false, true);
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(pOwner, e);
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxTextField::onChanged(FXObject*, FXSelector, void*){
	if(! pOwner->GetEnabled()){
		return 0;
	}
	
	try{
		pOwner->SetText(getText().text(), true);
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(pOwner, e);
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxTextField::onKeyPress(FXObject *sender, FXSelector selector, void *pdata){
	// returning 1 from this function stops FXTextField from processing
	if(! pOwner->GetEnabled() || pPreventKeyPressHook){
		return 0;
	}
	
	const FXEvent &event = *((FXEvent*)pdata);
	if(event.code != KEY_Return && event.code != KEY_KP_Enter){
		return 0;
	}
	
	// we do it this way instead of a silent hook-method because we want the super-class
	// implementation to call SEL_COMMAND before we send our NotifyEnterKey. do avoid a
	// dead-loop in this case pPreventKeyPressHook is used.
	pPreventKeyPressHook = true;
	const int result = onKeyPress(sender, selector, pdata);
	pPreventKeyPressHook = false;
	
	try{
		pOwner->NotifyEnterKey();
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(pOwner, e);
	}
	
	return result;
}

#endif
