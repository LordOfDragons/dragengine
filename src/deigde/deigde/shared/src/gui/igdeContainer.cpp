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
	child->SetParent(nullptr);
	pChildren.Remove(child);
}

void igdeContainer::RemoveAllChildren(){
	pChildren.Visit([&](igdeWidget &child){
		child.DestroyNativeWidget();
		child.SetParent(nullptr);
	});
	pChildren.RemoveAll();
}



void igdeContainer::DropNativeWidget(){
	pChildren.Visit([&](igdeWidget &child){
		child.DropNativeWidget();
	});
	
	igdeWidget::DropNativeWidget();
}

void igdeContainer::CreateChildWidgetNativeWidgets(){
	pChildren.Visit([&](igdeWidget &child){
		child.CreateNativeWidget();
	});
}

void igdeContainer::OnGuiThemeChanged(){
	igdeWidget::OnGuiThemeChanged();
	NotifyChildrenGuiThemeChanged();
}

void igdeContainer::NotifyChildrenGuiThemeChanged(){
	igdeGuiTheme * const guitheme = GetGuiTheme();
	
	pChildren.Visit([&](igdeWidget &child){
		if(child.GetGuiTheme() != guitheme){
			child.OnGuiThemeChanged();
		}
	});
}

void *igdeContainer::GetNativeContainer() const{
	return GetNativeWidget();
}
