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

#include "igdeWPUndoHistory.h"
#include "../igdeUIHelper.h"
#include "../igdeButton.h"
#include "../igdeListBox.h"
#include "../igdeGroupBox.h"
#include "../igdeContainer.h"
#include "../event/igdeActionUndo.h"
#include "../event/igdeActionRedo.h"
#include "../event/igdeActionClearUndo.h"
#include "../layout/igdeContainerBorder.h"
#include "../../environment/igdeEnvironment.h"
#include "../../undo/igdeUndo.h"
#include "../../undo/igdeUndoSystem.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class igdeWPUndoHistory
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWPUndoHistory::igdeWPUndoHistory(igdeEnvironment &environment) :
igdeContainerBox(environment, igdeContainerBox::eaY),
pUndoSystem(nullptr)
{
	igdeUIHelper &helper = environment.GetUIHelperProperties();
	
	// group box
	igdeGroupBox::Ref groupBoxFrame(igdeGroupBox::Ref::New(environment, "Undo History:"));
	groupBoxFrame->SetStretchLast(true);
	AddChild(groupBoxFrame);
	
	igdeContainerBorder::Ref groupBox(igdeContainerBorder::Ref::New(environment));
	groupBoxFrame->AddChild(groupBox);
	
	// buttons
	igdeContainerBox::Ref buttons(igdeContainerBox::Ref::New(environment, igdeContainerBox::eaY));
	groupBox->AddChild(buttons, igdeContainerBorder::eaTop);
	
	// button row 1
	igdeContainerBox::Ref buttonLine(igdeContainerBox::Ref::New(
		environment, igdeContainerBox::eaX));
	buttons->AddChild(buttonLine);
	
	pActionUndo = igdeActionUndo::Ref::New(environment);
	helper.Button(buttonLine, pActionUndo);
	
	pActionRedo = igdeActionRedo::Ref::New(environment);
	helper.Button(buttonLine, pActionRedo);
	
	// button row 2
	buttonLine = igdeContainerBox::Ref::New(environment, igdeContainerBox::eaX);
	buttons->AddChild(buttonLine);
	
	pActionClear = igdeActionClearUndo::Ref::New(environment);
	helper.Button(buttonLine, pActionClear);
	
	// undo action list
	pListUndo = igdeListBox::Ref::New(environment, 10);
	groupBox->AddChild(pListUndo, igdeContainerBorder::eaCenter);
}

igdeWPUndoHistory::~igdeWPUndoHistory(){
}



// Management
///////////////

void igdeWPUndoHistory::SetUndoSystem(igdeUndoSystem *undoSystem){
	if(undoSystem == pUndoSystem){
		return;
	}
	
	pUndoSystem = undoSystem;
	
	pActionUndo->SetUndoSystem(undoSystem);
	pActionRedo->SetUndoSystem(undoSystem);
	pActionClear->SetUndoSystem(undoSystem);
	
	UpdateUndo();
}



void igdeWPUndoHistory::UpdateUndo(){
	pListUndo->RemoveAllItems();
	
	if(!pUndoSystem){
		return;
	}
	
	igdeIcon * const iconUndo = pActionUndo->GetIcon();
	igdeIcon * const iconRedo = pActionRedo->GetIcon();
	const int redoCount = pUndoSystem->GetRedoableCount();
	
	pUndoSystem->GetUndos().VisitIndexed([&](int i, const igdeUndo &undo){
		//undo.GetLongInfo(), undo.GetMemoryConsumption()
		pListUndo->AddItem(undo.GetShortInfo(), i < redoCount ? iconRedo : iconUndo);
	});
	
	if(pUndoSystem->GetUndos().IsNotEmpty()){
		pListUndo->SetSelection(decMath::min(redoCount, pUndoSystem->GetUndos().GetCount() - 1));
	}
}
