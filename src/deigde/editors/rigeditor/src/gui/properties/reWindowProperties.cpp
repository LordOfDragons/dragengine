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
#include <string.h>
#include <stdlib.h>

#include "reWindowProperties.h"
#include "reWPBone.h"
#include "reWPConstraint.h"
#include "reWPPush.h"
#include "reWPRig.h"
#include "reWPShape.h"
#include "reWPUndoHistory.h"
#include "reWPView.h"
#include "../reWindowMain.h"

#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class reWindowProperties
/////////////////////////////

// Constructor, destructor
////////////////////////////

reWindowProperties::reWindowProperties(reWindowMain &windowMain) :
igdeTabBook(windowMain.GetEnvironment()),
pWindowMain(windowMain)
{
	SetWidgetGuiThemeName(igdeGuiThemeNames::properties);
	
	pPanelRig = reWPRig::Ref::New(*this);
	AddChild(pPanelRig, "Rig");
	
	pPanelBone = reWPBone::Ref::New(*this);
	AddChild(pPanelBone, "Bone");
	
	pPanelShape = reWPShape::Ref::New(*this);
	AddChild(pPanelShape, "Shape");
	
	pPanelConstraint = reWPConstraint::Ref::New(*this);
	AddChild(pPanelConstraint, "Constraint");
	
	pPanelPush = reWPPush::Ref::New(*this);
	AddChild(pPanelPush, "Push");
	
	pPanelView = reWPView::Ref::New(*this);
	AddChild(pPanelView, "View");
	
	pPanelUndoHistory = reWPUndoHistory::Ref::New(GetEnvironment());
	AddChild(pPanelUndoHistory, "Undo");
	
	SetActivePanel(5); // view
}

reWindowProperties::~reWindowProperties(){
}



// Management
///////////////

void reWindowProperties::SetRig(reRig *rig){
	pPanelRig->SetRig(rig);
	pPanelBone->SetRig(rig);
	pPanelShape->SetRig(rig);
	pPanelConstraint->SetRig(rig);
	pPanelPush->SetRig(rig);
	pPanelView->SetRig(rig);
	pPanelUndoHistory->SetRig(rig);
}
