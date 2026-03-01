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

#include "deglbIconTabView.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>


// Class deglbIconTabView
///////////////////////////

// Constructor, destructor
////////////////////////////

deglbIconTabView::deglbIconTabView(const char *label, BView *contentView) :
pLabel(label),
pPadding(10),
pIconPadding(4)
{
	pInitLabel(label, contentView);
}

deglbIconTabView::deglbIconTabView(BBitmap *bitmap, const char *label, BView *contentView) :
pBitmap(bitmap),
pLabel(label),
pPadding(10),
pIconPadding(4)
{
	pInitLabel(label, contentView);
}

deglbIconTabView::~deglbIconTabView() = default;



// Management
///////////////

void deglbIconTabView::SetIcon(BBitmap *bitmap){
	pBitmap = bitmap;
}


// BView
//////////

void deglbIconTabView::DrawLabel(BView *owner, BRect frame){
	float x = frame.left + pPadding;
	float top = frame.top + pPadding;
	float frameHeight = frame.Height() - pPadding * 2;
	
	if(pBitmap){
		const float hi = pBitmap->Bounds().Height();
		
		owner->SetDrawingMode(B_OP_ALPHA);
		owner->SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
		owner->DrawBitmap(pBitmap, BPoint(x, top + (frameHeight - hi) / 2));
		x += pBitmap->Bounds().Width() + pIconPadding;
	}
	
	font_height fh;
	owner->GetFontHeight(&fh);
	float textHeight = fh.ascent + fh.descent;
	
	float baseline = top + (frameHeight + textHeight) / 2 - fh.descent;
	
	owner->SetHighColor(ui_color(B_CONTROL_TEXT_COLOR));
	owner->DrawString(pLabel, BPoint(x, baseline));
}


// Private Functions
//////////////////////

void deglbIconTabView::pInitLabel(const char *label, BView *contentView){
	BFont font(be_plain_font);
	
	float totalWidth = 0; //pPadding * 2;
	
	if(pBitmap){
		totalWidth += pBitmap->Bounds().Width() + pIconPadding;
	}
	
	float spaceWidth = font.StringWidth(" ");
	int spaceCount = (int)ceilf(totalWidth / spaceWidth);
	
	BString l;
	l.SetTo(' ', spaceCount);
	l.Append(label);
	SetLabel(l);
	contentView->SetName(l);
}
