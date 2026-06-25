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

#include "aeMCAttachment.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/attachment/aeAttachment.h"
#include "../../gui/aeWindowMain.h"

#include <dragengine/common/exceptions.h>


// Class aeMCAttachment
/////////////////////////

// Constructor, destructor
////////////////////////////

aeMCAttachment::aeMCAttachment(aeWindowMain &windowMain, aeAttachment *attachment) :
igdeMetaContext("attachment"),
pWindowMain(windowMain),
pAttachment(attachment)
{
	SetLabel("Attachment");
	SetDescription("Attachment properties");
	SetProperties(windowMain.GetMCAnimatorProperties().attachment.metaProperties);
}

aeMCAttachment::~aeMCAttachment() = default;


// Management
///////////////

aeAttachment &aeMCAttachment::GetAttachmentRef() const{
	DEASSERT_NOTNULL(pAttachment)
	return *pAttachment;
}

aeMCAttachment::Ref aeMCAttachment::Capture() const{
	auto context = Ref::New(pWindowMain, pAttachment);
	context->pGuardAttachment = pAttachment;
	return context;
}

igdeEnvironment &aeMCAttachment::GetEnvironment() const{
	return pWindowMain.GetEnvironment();
}

igdeUndoSystem *aeMCAttachment::GetUndoSystem() const{
	return pAttachment && pAttachment->GetAnimator()
		? pAttachment->GetAnimator()->GetUndoSystem() : nullptr;
}

igdeClipboard *aeMCAttachment::GetClipboard() const{
	return &pWindowMain.GetClipboard();
}
