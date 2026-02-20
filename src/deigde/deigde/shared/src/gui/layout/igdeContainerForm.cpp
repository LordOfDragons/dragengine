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

#include "igdeContainerForm.h"
#include "../native/toolkit.h"
#include "../igdeWidget.h"

#include <dragengine/common/exceptions.h>



// Class igdeContainerForm
////////////////////////////

// Constructor, destructor
////////////////////////////

void igdeContainerForm::RemoveChild(igdeWidget *child){
	igdeContainer::RemoveChild(child);
	
	if(!GetNativeContainer()){
		return;
	}
	
	if(GetChildren().GetCount() < 1){
		return;
	}
	
	((igdeNativeContainerForm*)GetNativeContainer())->ChildRemoved();
}



igdeContainerForm::igdeContainerForm(igdeEnvironment &environment, eStretching stretching,
	int columnSpacing, int rowSpacing) :
igdeContainer(environment),
pStretching(stretching),
pColumnSpacing(columnSpacing),
pRowSpacing(rowSpacing)
{
	if(columnSpacing < 0 || rowSpacing < 0){
		DETHROW(deeInvalidParam);
	}
}

igdeContainerForm::~igdeContainerForm(){
}



// Management
///////////////

void igdeContainerForm::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeContainerForm * const native = igdeNativeContainerForm::CreateNativeWidget(*this);
	SetNativeWidget(native);
	native->PostCreateNativeWidget();
	
	CreateChildWidgetNativeWidgets();
}

void igdeContainerForm::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	igdeNativeContainerForm * const native = (igdeNativeContainerForm*)GetNativeWidget();
	DropNativeWidget();
	native->DestroyNativeWidget();
}
