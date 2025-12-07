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

#include "igdeNativeFoxScrollBar.h"
#include "foxIcons.h"
#include "../../igdeScrollBar.h"
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

FXDEFMAP(igdeNativeFoxScrollBar) igdeNativeFoxScrollBarMap[] = {
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxScrollBar::ID_SELF, igdeNativeFoxScrollBar::onCommand),
	FXMAPFUNC(SEL_CHANGED, igdeNativeFoxScrollBar::ID_SELF, igdeNativeFoxScrollBar::onCommand)
};



// Class igdeNativeFoxScrollBar
/////////////////////////////////

FXIMPLEMENT(igdeNativeFoxScrollBar, FXScrollBar, igdeNativeFoxScrollBarMap, ARRAYNUMBER(igdeNativeFoxScrollBarMap))

// Constructor, destructor
////////////////////////////

igdeNativeFoxScrollBar::igdeNativeFoxScrollBar(){}

igdeNativeFoxScrollBar::igdeNativeFoxScrollBar(igdeScrollBar &powner, FXComposite *pparent, int layoutFlags) :
FXScrollBar(pparent, this, ID_SELF, layoutFlags | ScrollBarFlags(powner)),
pOwner(&powner)
{
	UpdateRange();
	UpdateValue();
	if(! powner.GetEnabled()){
		disable();
	}
}

igdeNativeFoxScrollBar::~igdeNativeFoxScrollBar(){
}

igdeNativeFoxScrollBar *igdeNativeFoxScrollBar::CreateNativeWidget(igdeScrollBar &powner){
	if(! powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(! pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxScrollBar(powner, pparent, igdeUIFoxHelper::GetChildLayoutFlags(&powner));
}

void igdeNativeFoxScrollBar::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxScrollBar::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

FXbool igdeNativeFoxScrollBar::canFocus() const{
	return false;
	//return FXScrollBar::canFocus();
}

void igdeNativeFoxScrollBar::UpdateRange(){
	setRange(decMath::max(pOwner->GetUpper() - pOwner->GetLower(), 0));
	setPage(pOwner->GetPageSize());
	UpdateValue();
}

void igdeNativeFoxScrollBar::UpdateValue(){
	const int position = decMath::clamp(pOwner->GetValue() - pOwner->GetLower(), 0, getRange());
	if(position != getPosition()){
		setPosition(position, true);
	}
}

void igdeNativeFoxScrollBar::UpdateEnabled(){
	if(pOwner->GetEnabled()){
		enable();
		
	}else{
		disable();
	}
}



int igdeNativeFoxScrollBar::ScrollBarFlags(const igdeScrollBar &powner){
	switch(powner.GetOrientation()){
	case igdeScrollBar::eoHorizontal:
		return SCROLLBAR_HORIZONTAL | SCROLLBAR_WHEELJUMP;
		
	case igdeScrollBar::eoVertical:
		return SCROLLBAR_VERTICAL | SCROLLBAR_WHEELJUMP;
		
	default:
		return 0;
	}
}



// Events
///////////

long igdeNativeFoxScrollBar::onCommand(FXObject*, FXSelector, void*){
	if(! pOwner->GetEnabled()){
		return 0;
	}
	
	try{
		pOwner->SetValue(pOwner->GetLower() + getPosition());
		pOwner->NotifyValueChanged();
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(pOwner, e);
		return 0;
	}
	
	return 1;
}

#endif
