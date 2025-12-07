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

#include "igdeMenuOption.h"
#include "igdeMenuCascade.h"
#include "../igdeCommonDialogs.h"
#include "../event/igdeAction.h"
#include "../native/toolkit.h"
#include "../resources/igdeIcon.h"
#include "../../engine/igdeEngineController.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gameproject/igdeGameProject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeMenuOption
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeMenuOption::igdeMenuOption(igdeEnvironment &environment) :
igdeMenuCommand(environment),
pSelected(false){
}

igdeMenuOption::igdeMenuOption(igdeEnvironment &environment, igdeAction *action) :
igdeMenuCommand(environment),
pSelected(false)
{
	// WARNING we have to use SetAction not the base class constructor otherwise
	//         OnParameterChanged is called before pSelected is constructed
	//         and initialized causing wrong state to be stored
	SetAction(action);
}

igdeMenuOption::~igdeMenuOption(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeMenuOption::SetSelected(bool selected){
	if(pSelected == selected){
		return;
	}
	
	pSelected = selected;
	OnSelectedChanged();
}



void igdeMenuOption::OnParameterChanged(igdeAction *action){
	igdeMenuCommand::OnParameterChanged(action);
	
	SetSelected(action->GetSelected());
}



void igdeMenuOption::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeMenuOption * const native = igdeNativeMenuOption::CreateNativeWidget(*this);
	SetNativeWidget(native);
	native->PostCreateNativeWidget();
}

void igdeMenuOption::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeMenuOption*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeMenuOption::OnSelectedChanged(){
	if(GetNativeWidget()){
		((igdeNativeMenuOption*)GetNativeWidget())->UpdateSelected();
	}
}
