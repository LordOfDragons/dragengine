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

#include "igdeNativeFoxGroupBox.h"
#include "foxIcons.h"
#include "../../igdeGroupBox.h"
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

FXDEFMAP(igdeNativeFoxGroupBox) igdeNativeFoxGroupBoxMap[] = {
	FXMAPFUNC(SEL_LEFTBUTTONPRESS, 0, igdeNativeFoxGroupBox::onLeftMouseDown),
	FXMAPFUNC(SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxGroupBox::onChildLayoutFlags)
};



// Class igdeNativeFoxGroupBox
////////////////////////////////

FXIMPLEMENT(igdeNativeFoxGroupBox, FXGroupBox, igdeNativeFoxGroupBoxMap, ARRAYNUMBER(igdeNativeFoxGroupBoxMap))

// Constructor, destructor
////////////////////////////

igdeNativeFoxGroupBox::igdeNativeFoxGroupBox(){}

igdeNativeFoxGroupBox::igdeNativeFoxGroupBox(igdeGroupBox &powner, FXComposite *pparent,
int layoutFlags,const igdeGuiTheme &guitheme) :
FXGroupBox(pparent, powner.GetTitle().GetString(), layoutFlags,
	0, 0, 0, 0,
	GroupBoxPadLeft(guitheme), GroupBoxPadRight(guitheme),
	GroupBoxPadTop(guitheme), GroupBoxPadBottom(guitheme)),
pOwner(&powner),
pUncollapsedFlags(layoutFlags),
pCollapsedFlags(layoutFlags & ~LAYOUT_FILL_Y),
pFont(GroupBoxFont(powner, guitheme))
{
	setFont((FXFont*)pFont->GetNativeFont());
	
	if(powner.GetCollapsed()){
		setLayoutHints(pCollapsedFlags);
	}
	
// 	setTipText( owner.GetDescription().GetString() ); // not supported
// 	setHelpText( owner.GetDescription().GetString() );
}

igdeNativeFoxGroupBox::~igdeNativeFoxGroupBox(){
}

igdeNativeFoxGroupBox *igdeNativeFoxGroupBox::CreateNativeWidget(igdeGroupBox &powner){
	if(!powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxGroupBox(powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlags(&powner)
			| igdeNativeFoxGroupBox::GroupBoxFlags(powner),
		*powner.GetGuiTheme() );
}

void igdeNativeFoxGroupBox::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxGroupBox::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

FXint igdeNativeFoxGroupBox::getDefaultHeight(){
	if(pOwner->GetCollapsed()){
		return getFont()->getSize() / 10 + 11;
		
	}else{
		return FXGroupBox::getDefaultHeight();
	}
}


void igdeNativeFoxGroupBox::UpdateCollapsed(){
	setLayoutHints(pOwner->GetCollapsed() ? pCollapsedFlags : pUncollapsedFlags);
	
	FXWindow *pparent = getParent();
	update();
	while(pparent){
		pparent->recalc();
		pparent = pparent->getParent();
	}
}

void igdeNativeFoxGroupBox::UpdateTitle(){
	setText(pOwner->GetTitle().GetString());
}

void igdeNativeFoxGroupBox::UpdateTitleAlignment(){
	setGroupBoxStyle(igdeNativeFoxGroupBox::GroupBoxFlags(*pOwner));
}

void igdeNativeFoxGroupBox::UpdateStretchLast(){
	const int count = pOwner->GetChildCount();
	int i;
	for(i=0; i<count; i++){
		igdeUIFoxHelper::UpdateLayoutFlags(pOwner->GetChildAt(i));
	}
	recalc();
}



int igdeNativeFoxGroupBox::GroupBoxFlags(const igdeGroupBox &powner){
	int fflags = FRAME_RIDGE;
	
	switch(powner.GetTitleAlignment()){
	case igdeGroupBox::etaLeft:
		fflags |= GROUPBOX_TITLE_LEFT;
		break;
		
	case igdeGroupBox::etaRight:
		fflags |= GROUPBOX_TITLE_RIGHT;
		break;
		
	case igdeGroupBox::etaCenter:
		fflags |= GROUPBOX_TITLE_CENTER;
		break;
		
	default:
		fflags |= GROUPBOX_TITLE_LEFT;
	}
	
	return fflags;
}

igdeFont *igdeNativeFoxGroupBox::GroupBoxFont(const igdeGroupBox &powner, const igdeGuiTheme &guitheme){
	igdeFont::sConfiguration configuration;
	powner.GetEnvironment().GetApplicationFont(configuration);
	
	if(guitheme.HasProperty(igdeGuiThemePropertyNames::groupBoxFontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::groupBoxFontSizeAbsolute, 0);
		
	}else if(guitheme.HasProperty(igdeGuiThemePropertyNames::groupBoxFontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::groupBoxFontSize, 1.0f);
		
	}else if(guitheme.HasProperty(igdeGuiThemePropertyNames::fontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0);
		
	}else if(guitheme.HasProperty(igdeGuiThemePropertyNames::fontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f);
	}
	
	return powner.GetEnvironment().GetSharedFont(configuration);
}

int igdeNativeFoxGroupBox::GroupBoxPadLeft(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::groupBoxPaddingLeft, DEFAULT_PAD);
}

int igdeNativeFoxGroupBox::GroupBoxPadRight(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::groupBoxPaddingRight, DEFAULT_PAD);
}

int igdeNativeFoxGroupBox::GroupBoxPadTop(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::groupBoxPaddingTop, DEFAULT_PAD);
}

int igdeNativeFoxGroupBox::GroupBoxPadBottom(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::groupBoxPaddingBottom, DEFAULT_PAD);
}



// Events
///////////

long igdeNativeFoxGroupBox::onLeftMouseDown(FXObject*, FXSelector, void *pdata){
	if(!pOwner->GetCanCollapse()){
		return 0;
	}
	
	const FXEvent &event = *((FXEvent*)pdata);
	const int y = event.win_y;
	
	// change collapsed state only if clicked in the area of the caption
	// TODO figure out the correct height. check out FXPacker source to see where it places
	//      the first child item using LAYOUT_SIDE_TOP. this should give a hint
	if(y >= 0 && y < /*15*/ (int)getFont()->getSize() / 10 + 11){
		pOwner->SetCollapsed(!pOwner->GetCollapsed());
	}
	
	return 1;
}

long igdeNativeFoxGroupBox::onChildLayoutFlags(FXObject*, FXSelector, void *pdata){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *((igdeUIFoxHelper::sChildLayoutFlags*)pdata);
	clflags.flags = LAYOUT_FILL_X;
	
	const int index = pOwner->IndexOfChild(clflags.widget);
	if(index == -1){
		return 1;
	}
	
	if(pOwner->GetStretchLast() && index == pOwner->GetChildCount() - 1){
		clflags.flags |= LAYOUT_FILL_Y;
	}
	
	return 1;
}

#endif
