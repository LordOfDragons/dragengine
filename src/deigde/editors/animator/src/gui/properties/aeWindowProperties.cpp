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

#include "aeWindowProperties.h"
#include "aeWPAnimator.h"
#include "aeWPController.h"
#include "aeWPLink.h"
#include "aeWPRule.h"
#include "aeWPView.h"
#include "aeWPPlayground.h"
#include "aeWPUndoHistory.h"
#include "../aeWindowMain.h"

#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>


// Class aeWindowProperties
/////////////////////////////

// Constructor, destructor
////////////////////////////

aeWindowProperties::aeWindowProperties(aeWindowMain &windowMain) :
igdeTabBook(windowMain.GetEnvironment()),
pWindowMain(windowMain)
{
	SetWidgetGuiThemeName(igdeGuiThemeNames::properties);
	
	pPropAnimator = aeWPAnimator::Ref::New(*this);
	AddChild(pPropAnimator, "Animator");
	
	pPropController = aeWPController::Ref::New(*this);
	AddChild(pPropController, "Controller");
	
	pPropLink = aeWPLink::Ref::New(*this);
	AddChild(pPropLink, "Link");
	
	pPropRule = aeWPRule::Ref::New(*this);
	AddChild(pPropRule, "Rule");
	
	pPropPlayground = aeWPPlayground::Ref::New(*this);
	AddChild(pPropPlayground, "Playground");
	
	pPropView = aeWPView::Ref::New(*this);
	AddChild(pPropView, "View");
	
	pPanelUndoHistory = aeWPUndoHistory::Ref::New(GetEnvironment());
	AddChild(pPanelUndoHistory, "History");
	
	SetActivePanel(1); // controller
}

aeWindowProperties::~aeWindowProperties(){
}



// Management
///////////////

void aeWindowProperties::SetAnimator(aeAnimator *animator){
	pPropAnimator->SetAnimator(animator);
	pPropController->SetAnimator(animator);
	pPropLink->SetAnimator(animator);
	pPropRule->SetAnimator(animator);
	pPropView->SetAnimator(animator);
	pPropPlayground->SetAnimator(animator);
	((aeWPUndoHistory&)(igdeWPUndoHistory&)pPanelUndoHistory).SetAnimator(animator);
}

void aeWindowProperties::OnAnimatorPathChanged(){
	pPropAnimator->OnAnimatorPathChanged();
	pPropRule->OnAnimatorPathChanged();
	pPropView->OnAnimatorPathChanged();
}
