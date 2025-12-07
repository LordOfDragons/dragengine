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

#include "igdeNativeFoxContainerFixed.h"
#include "../../../layout/igdeContainerFixed.h"

#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP(igdeNativeFoxContainerFixed) igdeNativeFoxContainerFixedMap[] = {
	FXMAPFUNC(SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerFixed::onChildLayoutFlags)
};



// Class igdeNativeFoxContainerFixed
//////////////////////////////////////

FXIMPLEMENT(igdeNativeFoxContainerFixed, FXPacker,
	igdeNativeFoxContainerFixedMap, ARRAYNUMBER(igdeNativeFoxContainerFixedMap))

// Constructor, destructor
////////////////////////////

igdeNativeFoxContainerFixed::igdeNativeFoxContainerFixed(){}

igdeNativeFoxContainerFixed::igdeNativeFoxContainerFixed(igdeContainerFixed &powner,
	FXComposite *pparent, int layoutFlags) :
FXPacker(pparent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0),
pOwner(&powner){
}

igdeNativeFoxContainerFixed::~igdeNativeFoxContainerFixed(){
}

igdeNativeFoxContainerFixed *igdeNativeFoxContainerFixed::CreateNativeWidget(igdeContainerFixed &powner){
	if(!powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxContainerFixed(powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlags(&powner));
}

void igdeNativeFoxContainerFixed::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxContainerFixed::DestroyNativeWidget(){
	delete this;
}



// Events
///////////

long igdeNativeFoxContainerFixed::onChildLayoutFlags(FXObject*, FXSelector, void *pdata){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *((igdeUIFoxHelper::sChildLayoutFlags*)pdata);
	clflags.flags = 0;
	clflags.canResizeHorizontal = true;
	clflags.canResizeVertical = true;
	return 1;
}

#endif
