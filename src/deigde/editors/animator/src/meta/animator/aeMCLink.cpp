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

#include "aeMCLink.h"
#include "../../animator/link/aeLink.h"
#include "../../animator/aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/common/exceptions.h>

	
// Class aeMCLink
///////////////////

// Constructor, destructor
////////////////////////////

aeMCLink::aeMCLink(aeWindowMain &windowMain, aeLink *link) :
igdeMetaContext("link"),
pWindowMain(windowMain),
pLink(link)
{
	SetLabel("Link");
	SetDescription("Link properties");
	SetProperties(windowMain.GetMCAnimatorProperties().link.metaProperties);
}

aeMCLink::~aeMCLink() = default;


// Management
///////////////

aeLink &aeMCLink::GetLinkRef() const{
	DEASSERT_NOTNULL(pLink)
	return *pLink;
}

aeMCLink::Ref aeMCLink::Capture() const{
	auto context = Ref::New(pWindowMain, pLink);
	context->pGuardLink = pLink;
	return context;
}

igdeEnvironment &aeMCLink::GetEnvironment() const{
	return pWindowMain.GetEnvironment();
}

igdeUndoSystem *aeMCLink::GetUndoSystem() const{
	return pLink && pLink->GetAnimator() ? pLink->GetAnimator()->GetUndoSystem() : nullptr;
}

igdeClipboard *aeMCLink::GetClipboard() const{
	return &pWindowMain.GetClipboard();
}
