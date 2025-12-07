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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "native/toolkit.h"
#include "igdeContainer.h"
#include "igdeWidget.h"

#include <dragengine/common/exceptions.h>



// Native Widget
//////////////////



// Class igdeContainer
////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainer::igdeContainer(igdeEnvironment &environment) :
igdeWidget(environment){
}

igdeContainer::~igdeContainer(){
	RemoveAllChildren();
}



// Management
///////////////

int igdeContainer::GetChildCount() const{
	return pChildren.GetCount();
}

igdeWidget *igdeContainer::GetChildAt(int index) const{
	return (igdeWidget*)pChildren.GetAt(index);
}

int igdeContainer::IndexOfChild(igdeWidget *widget) const{
	const int count = pChildren.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if((igdeWidget*)pChildren.GetAt(i) == widget){
			return i;
		}
	}
	
	return -1;
}

void igdeContainer::AddChild(igdeWidget *child){
	if(!child || child->GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	pChildren.Add(child);
	child->SetParent(this);
	if(GetNativeContainer()){
		child->CreateNativeWidget();
	}
}

void igdeContainer::RemoveChild(igdeWidget *child){
	if(!child || child->GetParent() != this || !pChildren.Has(child)){
		DETHROW(deeInvalidParam);
	}
	
	child->DestroyNativeWidget();
	child->SetParent(NULL);
	pChildren.Remove(child);
}

void igdeContainer::RemoveAllChildren(){
	while(pChildren.GetCount() > 0){
		const int index = pChildren.GetCount() - 1;
		igdeWidget &child = *((igdeWidget*)pChildren.GetAt(index));
		
		child.DestroyNativeWidget();
		child.SetParent(NULL);
		
		pChildren.RemoveFrom(index);
	}
}



void igdeContainer::DropNativeWidget(){
	const int count = pChildren.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeWidget*)pChildren.GetAt(i))->DropNativeWidget();
	}
	
	igdeWidget::DropNativeWidget();
}

void igdeContainer::CreateChildWidgetNativeWidgets(){
	const int count = pChildren.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeWidget*)pChildren.GetAt(i))->CreateNativeWidget();
	}
}

void igdeContainer::OnGuiThemeChanged(){
	igdeWidget::OnGuiThemeChanged();
	NotifyChildrenGuiThemeChanged();
}

void igdeContainer::NotifyChildrenGuiThemeChanged(){
	igdeGuiTheme * const guitheme = GetGuiTheme();
	const int count = pChildren.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		igdeWidget * const widget = (igdeWidget*)pChildren.GetAt(i);
		if(widget->GetGuiTheme() != guitheme){
			widget->OnGuiThemeChanged();
		}
	}
}

void *igdeContainer::GetNativeContainer() const{
	return GetNativeWidget();
}
