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

#include "igdeNativeFoxToolBar.h"
#include "foxIcons.h"
#include "../../igdeToolBar.h"
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



// Class igdeNativeFoxToolBar
//////////////////////////////

FXIMPLEMENT(igdeNativeFoxToolBar, FXToolBar, nullptr, 0)

// Constructor, destructor
////////////////////////////

igdeNativeFoxToolBar::igdeNativeFoxToolBar(){}

igdeNativeFoxToolBar::igdeNativeFoxToolBar(igdeToolBar &powner, FXComposite *pparent,
	FXComposite *windowParent, int layoutFlags) :
FXToolBar(pparent, new FXToolBarShell(windowParent, FRAME_RAISED),
	FRAME_RAISED | /* LAYOUT_DOCK_NEXT | */ layoutFlags),
pOwner(&powner),
pWetDock((FXToolBarShell*)getWetDock())
{
	new FXToolBarGrip(this, this, FXToolBar::ID_TOOLBARGRIP, TOOLBARGRIP_DOUBLE);
}

igdeNativeFoxToolBar::~igdeNativeFoxToolBar(){
}

igdeNativeFoxToolBar *igdeNativeFoxToolBar::CreateNativeWidget(igdeToolBar &powner){
	if(! powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(! pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxToolBar(powner, pparent, pparent,
		igdeUIFoxHelper::GetChildLayoutFlags(&powner));
}

void igdeNativeFoxToolBar::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(! pparent.id()){
		return;
	}
	
	const bool createOurself = getParent() != pWetDock;
	
	pWetDock->create();
	
	if(createOurself){
		create();
	}
}

void igdeNativeFoxToolBar::DestroyNativeWidget(){
	const bool deleteOurself = getParent() != pWetDock;
	
	delete pWetDock;
	
	if(deleteOurself){
		delete this;
	}
}

#endif
