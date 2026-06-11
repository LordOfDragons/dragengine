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

aeMCAnimator::aeMCAnimator(aeWindowMain *window) :
igdeMetaContext("animator"),
pWindow(window)
{
	SetLabel("Animator");
	SetDescription("Animator properties");
	
	if(window){
		SetProperties(window->GetMCAnimatorProperties().metaProperties);
	}
}

aeMCAnimator::~aeMCAnimator() = default;


// Management
///////////////

aeAnimator::Ref aeMCAnimator::GetAnimator() const{
	return pWindow ? pWindow->GetAnimator() : aeAnimator::Ref(pAnimator);
}

const aeController::Ref &aeMCAnimator::GetActiveController() const{
	auto animator = GetAnimator();
	return animator ? animator->GetActiveController() : pController;
}

const aeLink::Ref &aeMCAnimator::GetActiveLink() const{
	auto animator = GetAnimator();
	return animator ? animator->GetActiveLink() : pLink;
}

const aeRule::Ref &aeMCAnimator::GetActiveRule() const{
	auto animator = GetAnimator();
	return animator ? animator->GetActiveRule() : pRule;
}

aeMCAnimator::Ref aeMCAnimator::CaptureAnimator() const{
	auto context = Ref::New(nullptr);
	context->pAnimator = GetAnimator();
	return context;
}

aeMCAnimator::Ref aeMCAnimator::CaptureController() const{
	auto context = CaptureAnimator();
	context->pController = GetActiveController();
	return context;
}

aeMCAnimator::Ref aeMCAnimator::CaptureLink() const{
	auto context = CaptureAnimator();
	context->pLink = GetActiveLink();
	return context;
}

aeMCAnimator::Ref aeMCAnimator::CaptureRule() const{
	auto context = CaptureAnimator();
	context->pRule = GetActiveRule();
	return context;
}

igdeUndoSystem *aeMCAnimator::GetUndoSystem() const{
	auto animator = GetAnimator();
	return animator ? animator->GetUndoSystem() : nullptr;
}
