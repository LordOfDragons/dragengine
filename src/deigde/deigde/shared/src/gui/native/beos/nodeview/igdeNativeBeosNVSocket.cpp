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

#include "igdeNativeBeosNVSocket.h"
#include "../../../theme/igdeGuiTheme.h"

#include <dragengine/common/exceptions.h>
#include <cmath>


// Class igdeNativeBeosNVSocket
/////////////////////////////////

igdeNativeBeosNVSocket::igdeNativeBeosNVSocket(BView *parent, const igdeGuiTheme &) :
BView(BRect(0, 0, 13, 13), "nvSocket", B_FOLLOW_NONE, B_WILL_DRAW),
pChecked(false),
pRadius(6),
pColorFrame({0, 0, 0, 255}),
pColorFillChecked({0, 0, 255, 255}),
pColorFillUnchecked({160, 160, 160, 255})
{
	SetViewColor(B_TRANSPARENT_COLOR);
}

igdeNativeBeosNVSocket::~igdeNativeBeosNVSocket() = default;



// Management
///////////////

void igdeNativeBeosNVSocket::SetChecked(bool checked){
	if(checked == pChecked){
		return;
	}
	pChecked = checked;
	Invalidate();
}

void igdeNativeBeosNVSocket::SetColorFrame(rgb_color color){
	pColorFrame = color;
	Invalidate();
}

void igdeNativeBeosNVSocket::SetColorFillChecked(rgb_color color){
	pColorFillChecked = color;
	Invalidate();
}

void igdeNativeBeosNVSocket::SetColorFillUnchecked(rgb_color color){
	pColorFillUnchecked = color;
	Invalidate();
}



decPoint igdeNativeBeosNVSocket::GetCenter() const{
	const BRect bounds = Bounds();
	return decPoint((int)(bounds.Width() / 2.0f), (int)(bounds.Height() / 2.0f));
}

decPoint igdeNativeBeosNVSocket::GetBorderLeft() const{
	const BRect bounds = Bounds();
	return decPoint((int)(bounds.Width() / 2.0f) - pRadius, (int)(bounds.Height() / 2.0f));
}

decPoint igdeNativeBeosNVSocket::GetBorderRight() const{
	const BRect bounds = Bounds();
	return decPoint((int)(bounds.Width() / 2.0f) + pRadius, (int)(bounds.Height() / 2.0f));
}

bool igdeNativeBeosNVSocket::IsInsideSocket(const decPoint &point) const{
	return (point - GetCenter()).Length() < (float)pRadius + 0.5f;
}



// BView overrides
///////////////////

void igdeNativeBeosNVSocket::Draw(BRect){
	const BRect bounds = Bounds();
	const BPoint center(bounds.Width() / 2.0f, bounds.Height() / 2.0f);
	const float r = (float)pRadius;
	
	SetHighColor(pChecked ? pColorFillChecked : pColorFillUnchecked);
	FillEllipse(center, r - 1.0f, r - 1.0f);
	
	if(pChecked){
		SetHighColor(pColorFillChecked);
		FillEllipse(center, r - 3.0f, r - 3.0f);
	}
	
	SetHighColor(pColorFrame);
	StrokeEllipse(center, r - 1.0f, r - 1.0f);
}

BSize igdeNativeBeosNVSocket::PreferredSize(){
	return BSize((float)(pRadius * 2), (float)(pRadius * 2));
}

#endif
