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

#include "igdeNativeFoxContainerBox.h"
#include "../../../layout/igdeContainerBox.h"

#include <dragengine/common/exceptions.h>


namespace{

class igdeNativeFoxContainerBoxY : public FXVerticalFrame{
	FXDECLARE(igdeNativeFoxContainerBoxY)
	
protected:
	igdeNativeFoxContainerBoxY();
	
private:
	igdeContainerBox *pOwner;
	
public:
	igdeNativeFoxContainerBoxY(igdeContainerBox &powner, FXComposite *pparent, int layoutFlags);
	virtual ~igdeNativeFoxContainerBoxY();
	
	long onResize(FXObject*, FXSelector, void*);
	long onChildLayoutFlags(FXObject*, FXSelector, void*);
};


FXDEFMAP(igdeNativeFoxContainerBoxY) igdeNativeFoxContainerBoxYMap[] = {
	FXMAPFUNC(SEL_CONFIGURE, 0, igdeNativeFoxContainerBoxY::onResize),
	FXMAPFUNC(SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerBoxY::onChildLayoutFlags)
};

FXIMPLEMENT(igdeNativeFoxContainerBoxY, FXVerticalFrame,
	igdeNativeFoxContainerBoxYMap, ARRAYNUMBER(igdeNativeFoxContainerBoxYMap))

igdeNativeFoxContainerBoxY::igdeNativeFoxContainerBoxY(){}

igdeNativeFoxContainerBoxY::igdeNativeFoxContainerBoxY(igdeContainerBox &powner, FXComposite *pparent, int layoutFlags) :
FXVerticalFrame(pparent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0,
	powner.GetSpacing(), powner.GetSpacing()),
pOwner(&powner){
}

igdeNativeFoxContainerBoxY::~igdeNativeFoxContainerBoxY(){
}

long igdeNativeFoxContainerBoxY::onResize(FXObject*, FXSelector, void*){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerBoxY::onChildLayoutFlags(FXObject*, FXSelector, void *pdata){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *((igdeUIFoxHelper::sChildLayoutFlags*)pdata);
	clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}



class igdeNativeFoxContainerBoxX : public FXHorizontalFrame{
	FXDECLARE(igdeNativeFoxContainerBoxX)
	
protected:
	igdeNativeFoxContainerBoxX();
	
private:
	igdeContainerBox *pOwner;
	
public:
	igdeNativeFoxContainerBoxX(igdeContainerBox &powner, FXComposite *pparent, int layoutFlags);
	virtual ~igdeNativeFoxContainerBoxX();
	
	long onResize(FXObject*, FXSelector, void*);
	long onChildLayoutFlags(FXObject*, FXSelector, void*);
};


FXDEFMAP(igdeNativeFoxContainerBoxX) igdeNativeFoxContainerBoxXMap[] = {
	FXMAPFUNC(SEL_CONFIGURE, 0, igdeNativeFoxContainerBoxX::onResize),
	FXMAPFUNC(SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerBoxX::onChildLayoutFlags),
};

FXIMPLEMENT(igdeNativeFoxContainerBoxX, FXHorizontalFrame,
	igdeNativeFoxContainerBoxXMap, ARRAYNUMBER(igdeNativeFoxContainerBoxXMap))

igdeNativeFoxContainerBoxX::igdeNativeFoxContainerBoxX(){}

igdeNativeFoxContainerBoxX::igdeNativeFoxContainerBoxX(igdeContainerBox &powner,
	FXComposite *pparent, int layoutFlags) :
FXHorizontalFrame(pparent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0,
	powner.GetSpacing(), powner.GetSpacing()),
pOwner(&powner){
}

igdeNativeFoxContainerBoxX::~igdeNativeFoxContainerBoxX(){
}

long igdeNativeFoxContainerBoxX::onResize(FXObject*, FXSelector, void*){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerBoxX::onChildLayoutFlags(FXObject*, FXSelector, void *pdata){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *((igdeUIFoxHelper::sChildLayoutFlags*)pdata);
	clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}

}



// Class igdeNativeFoxContainerBox
////////////////////////////////////

void *igdeNativeFoxContainerBox::CreateNativeWidget(igdeContainerBox &powner){
	if(! powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(! pparent){
		DETHROW(deeInvalidParam);
	}
	
	const int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags(&powner);
	
	if(powner.GetAxis() == igdeContainerBox::eaX){
		return new igdeNativeFoxContainerBoxX(powner, pparent, layoutFlags);
		
	}else{
		return new igdeNativeFoxContainerBoxY(powner, pparent, layoutFlags);
	}
}

void igdeNativeFoxContainerBox::PostCreateNativeWidget(igdeContainerBox &powner, void *native){
	FXComposite &pparent = *((FXComposite*) powner.GetParent()->GetNativeContainer());
	if(pparent.id()){
		((FXPacker*)native)->create();
	}
}

void igdeNativeFoxContainerBox::DestroyNativeWidget(igdeContainerBox &powner, void *native){
	delete (FXPacker*)native;
}

#endif
