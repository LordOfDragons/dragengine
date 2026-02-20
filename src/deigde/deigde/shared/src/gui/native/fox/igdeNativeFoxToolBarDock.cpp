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

#include "igdeNativeFoxToolBarDock.h"
#include "foxIcons.h"
#include "../../igdeToolBarDock.h"
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

FXDEFMAP(igdeNativeFoxToolBarDock) igdeNativeFoxToolBarDockMap[] = {
	FXMAPFUNC(SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxToolBarDock::onChildLayoutFlags)
};



// Class igdeNativeFoxToolBarDock
//////////////////////////////

FXIMPLEMENT(igdeNativeFoxToolBarDock, FXDockSite,
	igdeNativeFoxToolBarDockMap, ARRAYNUMBER(igdeNativeFoxToolBarDockMap))

// Constructor, destructor
////////////////////////////

igdeNativeFoxToolBarDock::igdeNativeFoxToolBarDock(){}

igdeNativeFoxToolBarDock::igdeNativeFoxToolBarDock(igdeToolBarDock &powner, FXComposite *pparent) :
FXDockSite(pparent, LayoutFlags(powner)),
pOwner(&powner){
}

igdeNativeFoxToolBarDock::~igdeNativeFoxToolBarDock(){
}

igdeNativeFoxToolBarDock *igdeNativeFoxToolBarDock::CreateNativeWidget(igdeToolBarDock &powner){
	if(!powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxToolBarDock(powner, pparent);
}

void igdeNativeFoxToolBarDock::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxToolBarDock::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

int igdeNativeFoxToolBarDock::LayoutFlags(const igdeToolBarDock &powner){
	switch(powner.GetSide()){
	case igdeToolBarDock::esTop:
		return LAYOUT_SIDE_TOP | LAYOUT_FILL_X;
		
	case igdeToolBarDock::esLeft:
		return LAYOUT_SIDE_LEFT | LAYOUT_FILL_Y;
		
	case igdeToolBarDock::esRight:
		return LAYOUT_SIDE_RIGHT | LAYOUT_FILL_Y;
		
	case igdeToolBarDock::esBottom:
		return LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X;
		
	default:
		return 0;
	}
}



// Events
///////////

long igdeNativeFoxToolBarDock::onChildLayoutFlags(FXObject*, FXSelector, void *pdata){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *((igdeUIFoxHelper::sChildLayoutFlags*)pdata);
	
	switch(pOwner->GetSide()){
	case igdeToolBarDock::esTop:
		clflags.flags = LAYOUT_SIDE_TOP; // | LAYOUT_FILL_X;
		break;
		
	case igdeToolBarDock::esLeft:
		clflags.flags = LAYOUT_SIDE_LEFT; // | LAYOUT_FILL_Y;
		break;
		
	case igdeToolBarDock::esRight:
		clflags.flags = LAYOUT_SIDE_RIGHT; // | LAYOUT_FILL_Y;
		break;
		
	case igdeToolBarDock::esBottom:
		clflags.flags = LAYOUT_SIDE_BOTTOM; // | LAYOUT_FILL_X;
		break;
		
	default:
		clflags.flags = LAYOUT_SIDE_TOP; // | LAYOUT_FILL_X;
		break;
	}
	
// 	clflags.flags = LAYOUT_TOP | LAYOUT_LEFT;
	return 1;
}

#endif
