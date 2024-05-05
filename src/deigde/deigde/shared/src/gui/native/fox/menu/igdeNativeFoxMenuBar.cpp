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

#include "igdeNativeFoxMenuBar.h"
#include "../../../igdeContainer.h"
#include "../../../menu/igdeMenuBar.h"

#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP( igdeNativeFoxMenuBar ) igdeNativeFoxMenuBarMap[] = {
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxMenuBar::onChildLayoutFlags )
};



// Class igdeNativeFoxMenuBar
//////////////////////////////

FXIMPLEMENT( igdeNativeFoxMenuBar, FXMenuBar,
	igdeNativeFoxMenuBarMap, ARRAYNUMBER( igdeNativeFoxMenuBarMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxMenuBar::igdeNativeFoxMenuBar(){ }

igdeNativeFoxMenuBar::igdeNativeFoxMenuBar( igdeMenuBar &powner, FXComposite *pparent, int layoutFlags ) :
FXMenuBar( pparent, layoutFlags ),
pOwner( &powner ){
}

igdeNativeFoxMenuBar::~igdeNativeFoxMenuBar(){
}

igdeNativeFoxMenuBar *igdeNativeFoxMenuBar::CreateNativeWidget( igdeMenuBar &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxMenuBar( powner, pparent, igdeUIFoxHelper::GetChildLayoutFlags( &powner ) );
}

void igdeNativeFoxMenuBar::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxMenuBar::DestroyNativeWidget(){
	delete this;
}



// Events
///////////

long igdeNativeFoxMenuBar::onChildLayoutFlags( FXObject*, FXSelector, void *pdata ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )pdata );
	clflags.flags = LAYOUT_TOP | LAYOUT_LEFT;
	return 1;
}

#endif
