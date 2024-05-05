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

#include "igdeNativeFoxToolBarSeparator.h"
#include "foxIcons.h"
#include "../../igdeToolBarSeparator.h"
#include "../../igdeContainer.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class igdeNativeFoxToolBarSeparator
////////////////////////////////////////

FXIMPLEMENT( igdeNativeFoxToolBarSeparator, FXSeparator, nullptr, 0 )

// Constructor, destructor
////////////////////////////

igdeNativeFoxToolBarSeparator::igdeNativeFoxToolBarSeparator(){ }

igdeNativeFoxToolBarSeparator::igdeNativeFoxToolBarSeparator(
	igdeToolBarSeparator &powner, FXComposite *pparent, int layoutFlags ) :
FXSeparator( pparent, layoutFlags | SEPARATOR_GROOVE ),
pOwner( &powner ){
}

igdeNativeFoxToolBarSeparator::~igdeNativeFoxToolBarSeparator(){
}

igdeNativeFoxToolBarSeparator *igdeNativeFoxToolBarSeparator::CreateNativeWidget( igdeToolBarSeparator &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxToolBarSeparator( powner, pparent, 0 );
}

void igdeNativeFoxToolBarSeparator::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxToolBarSeparator::DestroyNativeWidget(){
	delete this;
}

#endif
