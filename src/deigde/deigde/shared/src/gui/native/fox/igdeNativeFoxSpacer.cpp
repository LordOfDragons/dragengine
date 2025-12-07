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

#include "igdeNativeFoxSpacer.h"
#include "../../igdeContainer.h"
#include "../../igdeSpacer.h"

#include <dragengine/common/exceptions.h>



// Event map
//////////////

FXIMPLEMENT(igdeNativeFoxSpacer, FXFrame, nullptr, 0)



// Class igdeNativeFoxSpacer
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxSpacer::igdeNativeFoxSpacer(){
}

igdeNativeFoxSpacer::igdeNativeFoxSpacer(igdeSpacer &powner, FXComposite *pparent, int childFlags) :
FXFrame(pparent, LayoutFlags(childFlags), 0, 0, powner.GetSize().x, powner.GetSize().y),
pOwner(&powner),
pWidth(width),
pHeight(height){
}

igdeNativeFoxSpacer::~igdeNativeFoxSpacer(){
	pOwner = NULL;
}

igdeNativeFoxSpacer *igdeNativeFoxSpacer::CreateNativeWidget(igdeSpacer &powner){
	if(! powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(! pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxSpacer(powner, pparent, igdeUIFoxHelper::GetChildLayoutFlags(&powner));
}

void igdeNativeFoxSpacer::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxSpacer::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxSpacer::SetSize(int wwidth, int hheight){
	pWidth = wwidth;
	pHeight = hheight;
	resize(wwidth, hheight);
}

int igdeNativeFoxSpacer::getDefaultWidth(){
	return pWidth;
}

int igdeNativeFoxSpacer::getDefaultHeight(){
	return pHeight;
}

int igdeNativeFoxSpacer::LayoutFlags(int childFlags){
	childFlags &= ~(LAYOUT_FILL_COLUMN | LAYOUT_FILL_ROW);
	childFlags &= ~LAYOUT_FILL;
	childFlags |= LAYOUT_FIX_WIDTH | LAYOUT_FIX_HEIGHT;
	return childFlags;
}

#endif
