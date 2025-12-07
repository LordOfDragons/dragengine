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

#include "igdeNativeFoxContainerForm.h"
#include "../../layout/igdeContainerForm.h"

#include <dragengine/common/exceptions.h>



// Event map
//////////////

FXDEFMAP(igdeNativeFoxContainerForm) igdeNativeFoxContainerFormMap[] = {
	FXMAPFUNC(SEL_CONFIGURE, 0, igdeNativeFoxContainerForm::onResize),
	FXMAPFUNC(SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerForm::onChildLayoutFlags)
};


FXIMPLEMENT(igdeNativeFoxContainerForm, FXMatrix,
	igdeNativeFoxContainerFormMap, ARRAYNUMBER(igdeNativeFoxContainerFormMap))



// Class igdeNativeFoxContainerForm
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxContainerForm::igdeNativeFoxContainerForm(){
}

igdeNativeFoxContainerForm::igdeNativeFoxContainerForm(
	igdeContainerForm &powner, FXComposite *pparent, int layoutFlags) :
FXMatrix(pparent, 2, MATRIX_BY_COLUMNS | layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0,
	powner.GetColumnSpacing(), powner.GetRowSpacing()),
pOwner(&powner)
{
	if(!pOwner->GetVisible()){
		hide();
	}
}

igdeNativeFoxContainerForm::~igdeNativeFoxContainerForm(){
}

igdeNativeFoxContainerForm *igdeNativeFoxContainerForm::CreateNativeWidget(igdeContainerForm &powner){
	if(!powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxContainerForm(powner, pparent, igdeUIFoxHelper::GetChildLayoutFlags(&powner));
}

void igdeNativeFoxContainerForm::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxContainerForm::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxContainerForm::ChildRemoved(){
	if(pOwner->GetStretching() != igdeContainerForm::esLast){
		return;
	}
	
	const int count = pOwner->GetChildCount();
	const int index = count - (count % 2);
	igdeUIFoxHelper::UpdateLayoutFlags(pOwner->GetChildAt(index));
	if(index + 1 < count){
		igdeUIFoxHelper::UpdateLayoutFlags(pOwner->GetChildAt(index + 1));
	}
	
	recalc();
}



// Events
///////////

long igdeNativeFoxContainerForm::onResize(FXObject*, FXSelector, void*){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerForm::onChildLayoutFlags(FXObject*, FXSelector, void *pdata){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *((igdeUIFoxHelper::sChildLayoutFlags*)pdata);
	clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	
	const int index = pOwner->IndexOfChild(clflags.widget);
	if(index == -1){
		return 1;
	}
	
	// right side widgets fill column but not left side widgets. this makes the right
	// column to stretch but keeps the left column at minimal size
	clflags.canResizeHorizontal = true;
	
	if(index % 2 == 1){
		clflags.flags |= LAYOUT_FILL_COLUMN;
		clflags.canResizeHorizontal = false;
	}
	
	// stretch rows
	clflags.canResizeVertical = true;
	
	switch(pOwner->GetStretching()){
	case igdeContainerForm::esFirst:
		if(index / 2 == 0){
			clflags.flags |= LAYOUT_FILL_ROW;
			clflags.canResizeVertical = false;
		}
		break;
		
	case igdeContainerForm::esLast:
		if(index / 2 == pOwner->GetChildCount() / 2 - 1){
			clflags.flags |= LAYOUT_FILL_ROW;
			clflags.canResizeVertical = false;
		}
		break;
		
	case igdeContainerForm::esNone:
	default:
		break;
	}
	
	return 1;
}

#endif
