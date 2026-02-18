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

#include "igdeCheckBox.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "event/igdeAction.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeCheckBox
///////////////////////

// Constructor, destructor
////////////////////////////

igdeCheckBox::igdeCheckBox(igdeEnvironment &environment, const char *text,
	igdeIcon *icon, eButtonStyle style) :
igdeButton(environment, text, icon, style),
pChecked(false),
pNativeCheckBox(nullptr){
}

igdeCheckBox::igdeCheckBox(igdeEnvironment &environment, const char *text,
	const char *description, igdeIcon *icon, eButtonStyle style) :
igdeButton(environment, text, description, icon, style),
pChecked(false),
pNativeCheckBox(nullptr){
}

igdeCheckBox::igdeCheckBox(igdeEnvironment &environment, igdeAction *action, eButtonStyle style) :
igdeButton(environment, action, style),
pChecked(false),
pNativeCheckBox(nullptr)
{
	SetAction(action);
}

igdeCheckBox::~igdeCheckBox(){
}



// Management
///////////////

void igdeCheckBox::SetChecked(bool checked){
	if(pChecked == checked){
		return;
	}
	
	pChecked = checked;
	OnCheckedChanged();
}



void igdeCheckBox::OnParameterChanged(igdeAction *action){
	igdeButton::OnParameterChanged(action);
	
	SetChecked(action->GetSelected());
}



void igdeCheckBox::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeCheckBox * const native = igdeNativeCheckBox::CreateNativeWidget(*this);
	SetNativeWidget(native);
	pNativeCheckBox = native;
	pNativeButton = native;
	native->PostCreateNativeWidget();
}

void igdeCheckBox::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeCheckBox*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}

void igdeCheckBox::DropNativeWidget(){
	pNativeCheckBox = nullptr;
	igdeButton::DropNativeWidget();
}


void igdeCheckBox::OnCheckedChanged(){
	if(pNativeCheckBox){
		pNativeCheckBox->UpdateChecked();
	}
}
