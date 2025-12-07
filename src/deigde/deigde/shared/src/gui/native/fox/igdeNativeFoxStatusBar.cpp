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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "igdeNativeFoxStatusBar.h"
#include "../../igdeContainer.h"
#include "../../igdeStatusBar.h"
#include "../../resources/igdeFont.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Event map
//////////////

FXDEFMAP(igdeNativeFoxStatusBar) igdeNativeFoxStatusBarMap[] = {
	FXMAPFUNC(SEL_CONFIGURE, 0, igdeNativeFoxStatusBar::onResize),
	FXMAPFUNC(SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxStatusBar::onChildLayoutFlags)
};

FXIMPLEMENT(igdeNativeFoxStatusBar, FXStatusBar,
	igdeNativeFoxStatusBarMap, ARRAYNUMBER(igdeNativeFoxStatusBarMap))



// Class igdeNativeFoxStatusBar
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxStatusBar::igdeNativeFoxStatusBar(){}

igdeNativeFoxStatusBar::igdeNativeFoxStatusBar(igdeStatusBar &powner, FXComposite *pparent,
	const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme) :
FXStatusBar(pparent, layoutFlags.flags | StatusBarFlags(powner)),
pOwner(&powner)
{
	if(!pOwner->GetVisible()){
		hide();
	}
	
	getStatusLine()->setFont((FXFont*)StatusBarFont(powner, guitheme)->GetNativeFont());
	
	UpdateText();
}

igdeNativeFoxStatusBar::~igdeNativeFoxStatusBar(){
}

igdeNativeFoxStatusBar *igdeNativeFoxStatusBar::CreateNativeWidget(igdeStatusBar &powner){
	if(!powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxStatusBar(powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlagsAll(&powner), *powner.GetGuiTheme());
}

void igdeNativeFoxStatusBar::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxStatusBar::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxStatusBar::UpdateText(){
	getStatusLine()->setNormalText(pOwner->GetText().GetString());
	getStatusLine()->setText(pOwner->GetText().GetString());
}



int igdeNativeFoxStatusBar::StatusBarFlags(const igdeStatusBar &){
	return 0;
}

igdeFont *igdeNativeFoxStatusBar::StatusBarFont(const igdeStatusBar &powner, const igdeGuiTheme &guitheme){
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



// Events
///////////

long igdeNativeFoxStatusBar::onResize(FXObject*, FXSelector, void*){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxStatusBar::onChildLayoutFlags(FXObject*, FXSelector, void *pdata){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *((igdeUIFoxHelper::sChildLayoutFlags*)pdata);
	clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}

#endif
