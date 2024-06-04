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

#include "igdeNativeFoxContainerScroll.h"
#include "../../../layout/igdeContainerScroll.h"

#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP( igdeNativeFoxContainerScroll ) igdeNativeFoxContainerScrollMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxContainerScroll::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerScroll::onChildLayoutFlags )
};



// Class igdeNativeFoxContainerScroll
///////////////////////////////////////

FXIMPLEMENT( igdeNativeFoxContainerScroll, FXScrollWindow,
	igdeNativeFoxContainerScrollMap, ARRAYNUMBER( igdeNativeFoxContainerScrollMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxContainerScroll::igdeNativeFoxContainerScroll(){ }

igdeNativeFoxContainerScroll::igdeNativeFoxContainerScroll( igdeContainerScroll &powner,
	FXComposite *pparent, int layoutFlags ) :
FXScrollWindow( pparent, layoutFlags, 0, 0, 0, 0 ),
pOwner( &powner ){
}

igdeNativeFoxContainerScroll::~igdeNativeFoxContainerScroll(){
}

igdeNativeFoxContainerScroll *igdeNativeFoxContainerScroll::CreateNativeWidget( igdeContainerScroll &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( &powner ) | SCROLLERS_NORMAL | SCROLLERS_TRACK;
	if( ! powner.GetCanScrollX() ){
		layoutFlags |= HSCROLLING_OFF;
	}
	if( ! powner.GetCanScrollY() ){
		layoutFlags |= VSCROLLING_OFF;
	}
	
	return new igdeNativeFoxContainerScroll( powner, pparent, layoutFlags );
}

void igdeNativeFoxContainerScroll::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxContainerScroll::DestroyNativeWidget(){
	delete this;
}



// Events
///////////

long igdeNativeFoxContainerScroll::onResize( FXObject*, FXSelector, void* ){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerScroll::onChildLayoutFlags( FXObject*, FXSelector, void *pdata ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )pdata );
	clflags.flags = LAYOUT_SIDE_TOP;
	
	//if( ! pOwner->GetCanScrollX() ){
		clflags.flags |= LAYOUT_FILL_X;
	//}
	//if( ! pOwner->GetCanScrollY() ){
		clflags.flags |= LAYOUT_FILL_Y;
	//}
	
	return 1;
}

#endif
