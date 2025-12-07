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

#include "igdeNativeFoxSpinTextField.h"
#include "foxIcons.h"
#include "../../igdeSpinTextField.h"
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

FXDEFMAP(igdeNativeFoxSpinTextField) igdeNativeFoxSpinTextFieldMap[] = {
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxSpinTextField::ID_SELF, igdeNativeFoxSpinTextField::onCommand)
};



// Class igdeNativeFoxSpinTextField
//////////////////////////////

FXIMPLEMENT(igdeNativeFoxSpinTextField, FXSpinner, igdeNativeFoxSpinTextFieldMap, ARRAYNUMBER(igdeNativeFoxSpinTextFieldMap))

// Constructor, destructor
////////////////////////////

igdeNativeFoxSpinTextField::igdeNativeFoxSpinTextField(){}

igdeNativeFoxSpinTextField::igdeNativeFoxSpinTextField(igdeSpinTextField &powner, FXComposite *pparent,
int layoutFlags, const igdeGuiTheme &guitheme) :
FXSpinner(pparent, powner.GetColumns(), this, ID_SELF, layoutFlags | SpinTextFieldFlags(powner),
	0, 0, 0, 0,
	SpinTextFieldPadLeft(guitheme), SpinTextFieldPadRight(guitheme),
	SpinTextFieldPadTop(guitheme), SpinTextFieldPadBottom(guitheme)),
pOwner(&powner),
pFont(SpinTextFieldFont(powner, guitheme))
{
	setFont((FXFont*)pFont->GetNativeFont());
	
	if(! powner.GetEnabled()){
		disable();
	}
	
	setRange(powner.GetLower(), powner.GetUpper());
	setValue(powner.GetValue());
	setTipText(powner.GetDescription().GetString());
	setHelpText(powner.GetDescription().GetString());
}

igdeNativeFoxSpinTextField::~igdeNativeFoxSpinTextField(){
}

igdeNativeFoxSpinTextField *igdeNativeFoxSpinTextField::CreateNativeWidget(igdeSpinTextField &powner){
	if(! powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(! pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxSpinTextField(powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlags(&powner), *powner.GetGuiTheme());
}

void igdeNativeFoxSpinTextField::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxSpinTextField::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxSpinTextField::Focus(){
	setFocus();
}

void igdeNativeFoxSpinTextField::UpdateRange(){
	setRange(pOwner->GetLower(), pOwner->GetUpper());
}

void igdeNativeFoxSpinTextField::UpdateEnabled(){
	if(pOwner->GetEnabled()){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxSpinTextField::UpdateValue(){
	setValue(pOwner->GetValue());
}

void igdeNativeFoxSpinTextField::UpdateDescription(){
	const char * const description = pOwner->GetDescription();
	setTipText(description);
	setHelpText(description);
}




int igdeNativeFoxSpinTextField::SpinTextFieldFlags(const igdeSpinTextField&){
	return FRAME_SUNKEN;
}

igdeFont *igdeNativeFoxSpinTextField::SpinTextFieldFont(const igdeSpinTextField &powner, const igdeGuiTheme &guitheme){
	igdeFont::sConfiguration configuration;
	powner.GetEnvironment().GetApplicationFont(configuration);
	
	if(guitheme.HasProperty(igdeGuiThemePropertyNames::spinTextFieldFontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::spinTextFieldFontSizeAbsolute, 0);
		
	}else if(guitheme.HasProperty(igdeGuiThemePropertyNames::spinTextFieldFontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::spinTextFieldFontSize, 1.0f);
		
	}else if(guitheme.HasProperty(igdeGuiThemePropertyNames::fontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0);
		
	}else if(guitheme.HasProperty(igdeGuiThemePropertyNames::fontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f);
	}
	
	return powner.GetEnvironment().GetSharedFont(configuration);
}

int igdeNativeFoxSpinTextField::SpinTextFieldPadLeft(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::spinTextFieldPaddingLeft, DEFAULT_PAD);
}

int igdeNativeFoxSpinTextField::SpinTextFieldPadRight(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::spinTextFieldPaddingRight, DEFAULT_PAD);
}

int igdeNativeFoxSpinTextField::SpinTextFieldPadTop(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::spinTextFieldPaddingTop, DEFAULT_PAD);
}

int igdeNativeFoxSpinTextField::SpinTextFieldPadBottom(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::spinTextFieldPaddingBottom, DEFAULT_PAD);
}



// Events
///////////

long igdeNativeFoxSpinTextField::onCommand(FXObject*, FXSelector, void*){
	if(! pOwner->GetEnabled()){
		return 0;
	}
	
	try{
		pOwner->SetValue(getValue());
		pOwner->NotifyValueChanged();
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(pOwner, e);
		return 0;
	}
	
	return 1;
}

#endif
