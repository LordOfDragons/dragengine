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

#include "igdeMetaPropertyUndoHistory.h"
#include "widget/igdeMetaPropertyUndoHistoryWidget.h"
#include "../igdeMetaContext.h"


// Class igdeMetaPropertyUndoHistory
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyUndoHistory::igdeMetaPropertyUndoHistory(const char *id, const char *translationTag) :
igdeMetaProperty(id, translationTag)
{
	SetCanUndo(false);
}

igdeMetaPropertyUndoHistory::igdeMetaPropertyUndoHistory(const char *id, const char *name,
	const char *description) :
igdeMetaProperty(id, name, description)
{
	SetCanUndo(false);
}

igdeMetaPropertyUndoHistory::~igdeMetaPropertyUndoHistory() = default;


// Management
///////////////

void igdeMetaPropertyUndoHistory::SetRows(int rows){
	pRows = decMath::max(rows, 1);
}

void igdeMetaPropertyUndoHistory::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

bool igdeMetaPropertyUndoHistory::IsValid(const ContextRef &context) const{
	return context && !context->IsDisposed();
}

igdeUndoSystem *igdeMetaPropertyUndoHistory::GetPropertyValue(const ContextRef &context) const{
	return context && !context->IsDisposed() ? context->GetUndoSystem() : nullptr;
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyUndoHistory::CreateWidget(){
	return igdeMetaPropertyUndoHistoryWidget::Ref::New(*this);
}
