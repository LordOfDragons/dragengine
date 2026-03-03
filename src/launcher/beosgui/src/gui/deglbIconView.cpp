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

#include <Bitmap.h>

#include "deglbIconView.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>


// Class deglbIconView
////////////////////////

// Constructor, destructor
////////////////////////////

deglbIconView::deglbIconView(BBitmap *bitmap, int minSize) :
BView("icon", B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
pBitmap(bitmap),
pMinSize(minSize)
{
	SetExplicitAlignment(BAlignment(B_ALIGN_LEFT, B_ALIGN_MIDDLE));
}

deglbIconView::~deglbIconView() = default;



// Management
///////////////

void deglbIconView::SetIcon(BBitmap *bitmap){
	if(bitmap == pBitmap){
		return;
	}
	
	pBitmap = bitmap;
	
	InvalidateLayout();
	Invalidate();
}


// BView
//////////

BSize deglbIconView::MinSize(){
	BSize size(pMinSize, pMinSize);
	if(pBitmap){
		size.width = decMath::max(size.width, pBitmap->Bounds().Width());
		size.height = decMath::max(size.height, pBitmap->Bounds().Height());
	}
	return size;
}

BSize deglbIconView::PreferredSize(){
	return MinSize();
}

void deglbIconView::Draw(BRect updateRect){
	if(!pBitmap){
		return;
	}
	
	const BSize sv(Bounds().Size());
	const BSize si(pBitmap->Bounds().Size());
	
	SetDrawingMode(B_OP_ALPHA);
	SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
	DrawBitmap(pBitmap, BPoint((sv.width - si.width) / 2, (sv.height - si.height) / 2));
}
