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

#include "igdeNativeFoxContainerBoxAlternate.h"
#include "../../../layout/igdeContainerBoxAlternate.h"

#include <dragengine/common/exceptions.h>


namespace{

class igdeNativeFoxContainerBoxAlternateY : public FXVerticalFrame{
	FXDECLARE(igdeNativeFoxContainerBoxAlternateY)
	
protected:
	igdeNativeFoxContainerBoxAlternateY();
	
private:
	igdeContainerBoxAlternate *pOwner;
	
public:
	igdeNativeFoxContainerBoxAlternateY(igdeContainerBoxAlternate &powner, FXComposite *pparent, int layoutFlags);
	virtual ~igdeNativeFoxContainerBoxAlternateY();
	
	long onResize(FXObject*, FXSelector, void*);
	long onChildLayoutFlags(FXObject*, FXSelector, void*);
};


FXDEFMAP(igdeNativeFoxContainerBoxAlternateY) igdeNativeFoxContainerBoxAlternateYMap[] = {
	FXMAPFUNC(SEL_CONFIGURE, 0, igdeNativeFoxContainerBoxAlternateY::onResize),
	FXMAPFUNC(SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerBoxAlternateY::onChildLayoutFlags)
};

FXIMPLEMENT(igdeNativeFoxContainerBoxAlternateY, FXVerticalFrame,
	igdeNativeFoxContainerBoxAlternateYMap, ARRAYNUMBER(igdeNativeFoxContainerBoxAlternateYMap))

igdeNativeFoxContainerBoxAlternateY::igdeNativeFoxContainerBoxAlternateY(){}

igdeNativeFoxContainerBoxAlternateY::igdeNativeFoxContainerBoxAlternateY(igdeContainerBoxAlternate &powner, FXComposite *pparent, int layoutFlags) :
FXVerticalFrame(pparent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
pOwner(&powner){
}

igdeNativeFoxContainerBoxAlternateY::~igdeNativeFoxContainerBoxAlternateY(){
}

long igdeNativeFoxContainerBoxAlternateY::onResize(FXObject*, FXSelector, void*){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerBoxAlternateY::onChildLayoutFlags(FXObject*, FXSelector, void *pdata){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *((igdeUIFoxHelper::sChildLayoutFlags*)pdata);
	clflags.flags = LAYOUT_FILL_X;
	
	const int index = pOwner->GetChildren().IndexOf(clflags.widget);
	if(index == -1){
		return 1;
	}
	
	const bool oddIndex = index % 2 == 1;
	if(pOwner->GetStretchOdd() == oddIndex){
		clflags.flags |= LAYOUT_FILL_Y;
	}
	
	return 1;
}



class igdeNativeFoxContainerBoxAlternateX : public FXHorizontalFrame{
	FXDECLARE(igdeNativeFoxContainerBoxAlternateX)
	
protected:
	igdeNativeFoxContainerBoxAlternateX();
	
private:
	igdeContainerBoxAlternate *pOwner;
	
public:
	igdeNativeFoxContainerBoxAlternateX(igdeContainerBoxAlternate &powner, FXComposite *pparent, int layoutFlags);
	virtual ~igdeNativeFoxContainerBoxAlternateX();
	
	long onResize(FXObject*, FXSelector, void*);
	long onChildLayoutFlags(FXObject*, FXSelector, void*);
};


FXDEFMAP(igdeNativeFoxContainerBoxAlternateX) igdeNativeFoxContainerBoxAlternateXMap[] = {
	FXMAPFUNC(SEL_CONFIGURE, 0, igdeNativeFoxContainerBoxAlternateX::onResize),
	FXMAPFUNC(SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerBoxAlternateX::onChildLayoutFlags),
};

FXIMPLEMENT(igdeNativeFoxContainerBoxAlternateX, FXHorizontalFrame,
	igdeNativeFoxContainerBoxAlternateXMap, ARRAYNUMBER(igdeNativeFoxContainerBoxAlternateXMap))

igdeNativeFoxContainerBoxAlternateX::igdeNativeFoxContainerBoxAlternateX(){}

igdeNativeFoxContainerBoxAlternateX::igdeNativeFoxContainerBoxAlternateX(igdeContainerBoxAlternate &powner,
	FXComposite *pparent, int layoutFlags) :
FXHorizontalFrame(pparent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0,
	powner.GetSpacing(), powner.GetSpacing()),
pOwner(&powner){
}

igdeNativeFoxContainerBoxAlternateX::~igdeNativeFoxContainerBoxAlternateX(){
}

long igdeNativeFoxContainerBoxAlternateX::onResize(FXObject*, FXSelector, void*){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerBoxAlternateX::onChildLayoutFlags(FXObject*, FXSelector, void *pdata){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *((igdeUIFoxHelper::sChildLayoutFlags*)pdata);
	clflags.flags = LAYOUT_FILL_Y;
	
	const int index = pOwner->GetChildren().IndexOf(clflags.widget);
	if(index == -1){
		return 1;
	}
	
	const bool oddIndex = index % 2 == 1;
	if(pOwner->GetStretchOdd() == oddIndex){
		clflags.flags |= LAYOUT_FILL_X;
	}
	
	return 1;
}

}



// Class igdeNativeFoxContainerBoxAlternate
/////////////////////////////////////////////

void *igdeNativeFoxContainerBoxAlternate::CreateNativeWidget(igdeContainerBoxAlternate &powner){
	if(!powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	const int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags(&powner);
	
	if(powner.GetAxis() == igdeContainerBoxAlternate::eaX){
		return new igdeNativeFoxContainerBoxAlternateX(powner, pparent, layoutFlags);
		
	}else{
		return new igdeNativeFoxContainerBoxAlternateY(powner, pparent, layoutFlags);
	}
}

void igdeNativeFoxContainerBoxAlternate::PostCreateNativeWidget(igdeContainerBoxAlternate &powner, void *native){
	FXComposite &pparent = *((FXComposite*)powner.GetParent()->GetNativeContainer());
	if(pparent.id()){
		((FXPacker*)native)->create();
	}
}

void igdeNativeFoxContainerBoxAlternate::DestroyNativeWidget(igdeContainerBoxAlternate&, void *native){
	delete (FXPacker*)native;
}

#endif
