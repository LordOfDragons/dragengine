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

#include "aeMCAnimator.h"
#include "../../animator/aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/common/exceptions.h>


// Class aeMCAnimator
///////////////////////

// Constructor, destructor
////////////////////////////

aeMCAnimator::aeMCAnimator(aeWindowMain &windowMain, aeAnimator *animator) :
igdeMetaContext("animator"),
pWindowMain(windowMain),
pAnimator(animator)
{
	SetLabel("Animator");
	SetDescription("Animator properties");
	SetProperties(windowMain.GetMCAnimatorProperties().metaProperties);
}
	
aeMCAnimator::~aeMCAnimator() = default;


// Management
///////////////

aeAnimator &aeMCAnimator::GetAnimatorRef() const{
	DEASSERT_NOTNULL(pAnimator)
	return *pAnimator;
}

aeMCAnimator::Ref aeMCAnimator::Capture() const{
	auto context = Ref::New(pWindowMain, pAnimator);
	context->pGuardAnimator = pAnimator;
	return context;
}

igdeEnvironment &aeMCAnimator::GetEnvironment() const{
	return pWindowMain.GetEnvironment();
}

igdeUndoSystem *aeMCAnimator::GetUndoSystem() const{
	return pAnimator ? pAnimator->GetUndoSystem() : nullptr;
}

igdeClipboard *aeMCAnimator::GetClipboard() const{
	return &pWindowMain.GetClipboard();
}


// Class aeMCAnimatorController
/////////////////////////////////

aeMCAnimatorController::aeMCAnimatorController(aeWindowMain &windowMain, aeAnimator *animator) :
aeMCAnimator(windowMain, animator){
	SetLabel("Controller");
	SetDescription("Controller properties");
	SetProperties(windowMain.GetMCAnimatorProperties().metaPropertiesController);
}

aeMCAnimatorController::~aeMCAnimatorController() = default;


// Class aeMCAnimatorLink
///////////////////////////

aeMCAnimatorLink::aeMCAnimatorLink(aeWindowMain &windowMain, aeAnimator *animator) :
aeMCAnimator(windowMain, animator){
	SetLabel("Link");
	SetDescription("Link properties");
	SetProperties(windowMain.GetMCAnimatorProperties().metaPropertiesLink);
}

aeMCAnimatorLink::~aeMCAnimatorLink() = default;


// Class aeMCAnimatorRule
///////////////////////////

aeMCAnimatorRule::aeMCAnimatorRule(aeWindowMain &windowMain, aeAnimator *animator) :
aeMCAnimator(windowMain, animator){
	SetLabel("Rule");
	SetDescription("Rule properties");
	SetProperties(windowMain.GetMCAnimatorProperties().metaPropertiesRule);
}

aeMCAnimatorRule::~aeMCAnimatorRule() = default;
