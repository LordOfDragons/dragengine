/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeosSeparator.h"
#include "../../igdeSeparator.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/exceptions.h>


igdeNativeBeosSeparator::igdeNativeBeosSeparator(igdeSeparator &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, 200, 2), "separator", B_FOLLOW_ALL, B_WILL_DRAW),
pOwner(&owner)
{
	if(owner.GetOrientation() == igdeSeparator::eoVertical){
		ResizeTo(2, 200);
	}
}

igdeNativeBeosSeparator::~igdeNativeBeosSeparator() = default;


igdeNativeBeosSeparator* igdeNativeBeosSeparator::CreateNativeWidget(igdeSeparator &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosSeparator(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosSeparator::PostCreateNativeWidget(){
}

void igdeNativeBeosSeparator::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}

void igdeNativeBeosSeparator::Draw(BRect updateRect){
	BView::Draw(updateRect);
	
	try{
		BRect rect = Bounds();
		
		// Draw a light gray line
		SetHighColor(180, 180, 180);
		
		if(pOwner->GetOrientation() == igdeSeparator::eoVertical){
			// Vertical separator
			StrokeLine(BPoint(rect.left + 1, rect.top), BPoint(rect.left + 1, rect.bottom));
			
			// Highlight on right side
			SetHighColor(255, 255, 255);
			StrokeLine(BPoint(rect.right, rect.top), BPoint(rect.right, rect.bottom));
			
		}else{
			// Horizontal separator
			StrokeLine(BPoint(rect.left, rect.top + 1), BPoint(rect.right, rect.top + 1));
			
			// Highlight on bottom
			SetHighColor(255, 255, 255);
			StrokeLine(BPoint(rect.left, rect.bottom), BPoint(rect.right, rect.bottom));
		}
		
	}catch(const deException &e){
		if(pOwner){
			pOwner->GetLogger()->LogException("IGDE Separator", e);
		}
	}
}

#endif
