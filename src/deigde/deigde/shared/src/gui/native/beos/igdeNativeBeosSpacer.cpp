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

#include "igdeNativeBeosSpacer.h"
#include "../../igdeSpacer.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/exceptions.h>


igdeNativeBeosSpacer::igdeNativeBeosSpacer(igdeSpacer &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, owner.GetSize().x, owner.GetSize().y), "spacer", B_FOLLOW_NONE, 0),
pOwner(&owner){
}

igdeNativeBeosSpacer::~igdeNativeBeosSpacer() = default;


igdeNativeBeosSpacer* igdeNativeBeosSpacer::CreateNativeWidget(igdeSpacer &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosSpacer(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosSpacer::PostCreateNativeWidget(){
}

void igdeNativeBeosSpacer::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}

void igdeNativeBeosSpacer::Focus(){
	MakeFocus(false);
}

void igdeNativeBeosSpacer::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosSpacer::SetSize(int width, int height){
	ResizeTo(width, height);
}

#endif
