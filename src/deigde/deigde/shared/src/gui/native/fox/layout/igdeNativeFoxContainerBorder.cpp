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

#include "igdeNativeFoxContainerBorder.h"
#include "../../../layout/igdeContainerBorder.h"

#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP(igdeNativeFoxContainerBorder) igdeNativeFoxContainerBorderMap[] = {
	FXMAPFUNC(SEL_CONFIGURE, 0, igdeNativeFoxContainerBorder::onResize),
	FXMAPFUNC(SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerBorder::onChildLayoutFlags)
};



// Class igdeNativeFoxContainerBorder
//////////////////////////////

FXIMPLEMENT(igdeNativeFoxContainerBorder, FXPacker,
	igdeNativeFoxContainerBorderMap, ARRAYNUMBER(igdeNativeFoxContainerBorderMap))

// Constructor, destructor
////////////////////////////

igdeNativeFoxContainerBorder::igdeNativeFoxContainerBorder(){}

igdeNativeFoxContainerBorder::igdeNativeFoxContainerBorder(
igdeContainerBorder &powner, FXComposite *pparent, int layoutFlags) :
FXPacker(pparent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0, powner.GetSpacing(), powner.GetSpacing()),
pOwner(&powner){
}

igdeNativeFoxContainerBorder::~igdeNativeFoxContainerBorder(){
}

igdeNativeFoxContainerBorder *igdeNativeFoxContainerBorder::CreateNativeWidget(igdeContainerBorder &powner){
	if(! powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(! pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxContainerBorder(powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlags(&powner));
}

void igdeNativeFoxContainerBorder::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxContainerBorder::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxContainerBorder::layout(){
	// unfortunately wee need to re-implement the layout code from FXPacker::layout().
	// this is required since for border layout to work properly the widgets have to be
	// processed in the correct order or the result is incorrect.
	// 
	// so why not just add them in the right order? the problem is that it is allowed to
	// add widgets while the native widget is already visible. since FOX has no way to
	// move windows index wise inside inside an already created parent window we would
	// have to delete all native child widgets and adding them again in the right order.
	// the other solution is to overwrite FXPacker::layout() to ensure the right order
	// is used. this is not so bad as it sounds since we know what layout flags we allow
	// and thus not required code can be dropped. the rest is copy-pasted and adjusted
	// to the coding style used.
	// 
	// as a goody this allows to re-implement the layout if it shows the existing packer
	// code is not fully suitable. so far it is.
	enum eSides{esTop, esBottom, esLeft, esRight, esCenter};
	
	igdeWidget * const children[5] = {
		pOwner->GetWidgetIn(igdeContainerBorder::eaTop),
		pOwner->GetWidgetIn(igdeContainerBorder::eaBottom),
		pOwner->GetWidgetIn(igdeContainerBorder::eaLeft),
		pOwner->GetWidgetIn(igdeContainerBorder::eaRight),
		pOwner->GetWidgetIn(igdeContainerBorder::eaCenter)
	};
	
	int left = border + padleft;
	int right = width - border - padright;
	int top = border + padtop;
	int bottom = height - border - padbottom;
	int i;
	
	for(i=0; i<5; i++){
		if(! children[i]){
			continue;
		}
		
		FXWindow * const child = (FXWindow*)children[i]->GetNativeWidget();
		if(! child){
			continue;
		}
		
		if(! child->shown()){
			continue;
		}
		
		const FXuint hints = child->getLayoutHints();
		int x = child->getX();
		int y = child->getY();
		int w, h;
		
		// height and width
		if(hints & LAYOUT_FIX_HEIGHT){
			h = child->getHeight();
			
		}else if(hints & LAYOUT_FILL_Y){
			h = bottom - top;
			
		}else{
			h = child->getDefaultHeight();
		}
		
		if(hints & LAYOUT_FIX_WIDTH){
			w = child->getWidth();
			
		}else if(hints & LAYOUT_FILL_X){
			w = right - left;
			
		}else{
			w = child->getDefaultWidth();
		}
		
		// position. original fox code uses LAYOUT_SIDE_* but this is error prone since things
		// like LAYOUT_SIDE_RIGHT is actually an or combination of LAYOUT_SIDE_LEFT and
		// LAYOUT_SIDE_BOTTOM which messes up the if conditions. changed to use the actual
		// widget area since we can do this in this situation here
		switch(i){
		case esTop:
			x = left;
			y = top;
			top += h + vspacing;
			break;
			
		case esBottom:
			x = left;
			y = bottom - h;
			bottom -= h + vspacing;
			break;
			
		case esLeft:
			x = left;
			left += w + hspacing;
			y = top;
			break;
			
		case esRight:
			x = right - w;
			right -= w + hspacing;
			y = top;
			break;
			
		default: //esCenter
			x = left;
			y = top;
			break;
		}
		
		child->position(x, y, w, h);
	}
	
	flags &= ~FLAG_DIRTY;
}



// Events
///////////

long igdeNativeFoxContainerBorder::onResize(FXObject*, FXSelector, void*){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerBorder::onChildLayoutFlags(FXObject*, FXSelector, void *pdata){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *((igdeUIFoxHelper::sChildLayoutFlags*)pdata);
	
	if(pOwner->GetWidgetIn(igdeContainerBorder::eaTop) == clflags.widget){
		clflags.flags = LAYOUT_SIDE_TOP | LAYOUT_FILL_X;
		clflags.canResizeVertical = true;
		
	}else if(pOwner->GetWidgetIn(igdeContainerBorder::eaBottom) == clflags.widget){
		clflags.flags = LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X;
		clflags.canResizeVertical = true;
		
	}else if(pOwner->GetWidgetIn(igdeContainerBorder::eaLeft) == clflags.widget){
		clflags.flags = LAYOUT_SIDE_LEFT | LAYOUT_FILL_Y;
		clflags.canResizeHorizontal = true;
		
	}else if(pOwner->GetWidgetIn(igdeContainerBorder::eaRight) == clflags.widget){
		clflags.flags = LAYOUT_SIDE_RIGHT | LAYOUT_FILL_Y;
		clflags.canResizeHorizontal = true;
		
	}else if(pOwner->GetWidgetIn(igdeContainerBorder::eaCenter) == clflags.widget){
		clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
		
	}else{
		clflags.flags = 0; // error
	}
	return 1;
}

#endif
