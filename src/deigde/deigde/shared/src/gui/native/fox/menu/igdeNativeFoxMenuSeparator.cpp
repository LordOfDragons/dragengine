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

#include "igdeNativeFoxMenuSeparator.h"
#include "../../../igdeContainer.h"
#include "../../../menu/igdeMenuSeparator.h"

#include <dragengine/common/exceptions.h>



// Class igdeNativeFoxMenuSeparator
/////////////////////////////////////

FXIMPLEMENT(igdeNativeFoxMenuSeparator, FXMenuSeparator, nullptr, 0)

// Constructor, destructor
////////////////////////////

igdeNativeFoxMenuSeparator::igdeNativeFoxMenuSeparator(){}

igdeNativeFoxMenuSeparator::igdeNativeFoxMenuSeparator(igdeMenuSeparator &powner, FXComposite *pparent) :
FXMenuSeparator(pparent),
pOwner(&powner){
}

igdeNativeFoxMenuSeparator::~igdeNativeFoxMenuSeparator(){
}

igdeNativeFoxMenuSeparator *igdeNativeFoxMenuSeparator::CreateNativeWidget(igdeMenuSeparator &powner){
	if(!powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxMenuSeparator(powner, pparent);
}

void igdeNativeFoxMenuSeparator::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxMenuSeparator::DestroyNativeWidget(){
	delete this;
}

#endif
