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

#include "igdeNativeBeosGroupBox.h"
#include "../../igdeGroupBox.h"
#include "../../igdeWidget.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>


igdeNativeBeosGroupBox::igdeNativeBeosGroupBox(igdeGroupBox &owner, BView *parent,
	const igdeGuiTheme &guitheme) :
BView(BRect(0, 0, 400, 300), "groupbox", B_FOLLOW_ALL, B_WILL_DRAW),
pOwner(&owner),
pGroupView(nullptr),
pCollapsed(false)
{
	pGroupView = new BGroupView(B_VERTICAL, 5);
	pGroupView->SetLabel(owner.GetTitle().GetString());
	
	auto scroller = new BScrollView("groupscroll", pGroupView, B_FOLLOW_ALL, 0, false, true);
	AddChild(scroller);
}

igdeNativeBeosGroupBox::~igdeNativeBeosGroupBox() = default;

igdeNativeBeosGroupBox* igdeNativeBeosGroupBox::CreateNativeWidget(igdeGroupBox &owner){
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosGroupBox(owner, parent, *owner.GetGuiTheme());
	parent->AddChild(widget);
	return widget;
}

void igdeNativeBeosGroupBox::PostCreateNativeWidget(){
}

void igdeNativeBeosGroupBox::DestroyNativeWidget(){
	RemoveSelf();
	delete this;
}

void igdeNativeBeosGroupBox::Focus(){
	MakeFocus(true);
}

void igdeNativeBeosGroupBox::UpdateStyle(){
	Invalidate();
}

void igdeNativeBeosGroupBox::UpdateCollapsed(){
	if(!pGroupView){
		return;
	}
	
	pCollapsed = pOwner->GetCollapsed();
	
	if(pCollapsed){
		pGroupView->Hide();
		
	} else {
		pGroupView->Show();
	}
}

void igdeNativeBeosGroupBox::UpdateTitle(){
	if(pGroupView){
		pGroupView->SetLabel(pOwner->GetTitle().GetString());
	}
}

void igdeNativeBeosGroupBox::UpdateTitleAlignment(){
	// Alignment handling - BeOS BGroupView has limited title alignment
	// Default is left-aligned, which is standard
}

void igdeNativeBeosGroupBox::UpdateStretchLast(){
	// Stretch last child support - would need custom layout
}

void *igdeNativeBeosGroupBox::GetNativeContainer() const{
	return pGroupView;
}

#endif
