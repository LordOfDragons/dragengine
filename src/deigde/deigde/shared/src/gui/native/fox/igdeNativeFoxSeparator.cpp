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

#include "igdeNativeFoxSeparator.h"
#include "foxIcons.h"
#include "../../igdeSeparator.h"
#include "../../igdeContainer.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeNativeFoxSeparator
/////////////////////////////////

void *igdeNativeFoxSeparator::CreateNativeWidget(igdeSeparator &powner){
	if(!powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	const int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags(&powner) | SEPARATOR_GROOVE;
	
	switch(powner.GetOrientation()){
	case igdeSeparator::eoHorizontal:
		return new FXHorizontalSeparator(pparent, layoutFlags);
		
	case igdeSeparator::eoVertical:
		return new FXVerticalSeparator(pparent, layoutFlags);
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void igdeNativeFoxSeparator::PostCreateNativeWidget(igdeSeparator &powner, void *native){
	FXComposite &pparent = *((FXComposite*) powner.GetParent()->GetNativeContainer());
	if(pparent.id()){
		((FXSeparator*)native)->create();
	}
}

void igdeNativeFoxSeparator::DestroyNativeWidget(igdeSeparator&, void *native){
	delete (FXSeparator*)native;
}

#endif
