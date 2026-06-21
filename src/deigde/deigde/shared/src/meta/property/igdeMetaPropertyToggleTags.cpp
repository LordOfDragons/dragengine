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

#include "igdeMetaPropertyToggleTags.h"
#include "undo/igdeMetaPropertyToggleTagsUndo.h"
#include "widget/igdeMetaPropertyToggleTagsWidget.h"
#include "../igdeMetaContext.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyToggleTags
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyToggleTags::igdeMetaPropertyToggleTags(
	const char *id, const char *name, const char *description, int rows) :
igdeMetaProperty(id, name, description),
pRows(rows){
}

igdeMetaPropertyToggleTags::igdeMetaPropertyToggleTags(
	const char *id, const char *translationTag, int rows) :
igdeMetaProperty(id, translationTag),
pRows(rows){
}

igdeMetaPropertyToggleTags::~igdeMetaPropertyToggleTags() = default;


// Management
///////////////

void igdeMetaPropertyToggleTags::SetRows(int rows){
	pRows = decMath::max(rows, 1);
}

void igdeMetaPropertyToggleTags::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

igdeMetaPropertyToggleTagsUndo::Ref igdeMetaPropertyToggleTags::ChangePropertyValue(
const igdeMetaContext::Ref &context, const decStringSet &newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem()){
		const auto undo = igdeMetaPropertyToggleTagsUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyToggleTags::CreateWidget(){
	return igdeMetaPropertyToggleTagsWidget::Ref::New(*this);
}


// igdeMetaPropertyToggleTagsStorage
//////////////////////////////////////

igdeMetaPropertyToggleTagsStorage::igdeMetaPropertyToggleTagsStorage(
	const char *id, const char *name, const char *description, int rows) :
igdeMetaPropertyToggleTags(id, name, description, rows){
}

igdeMetaPropertyToggleTagsStorage::igdeMetaPropertyToggleTagsStorage(
	const char *id, const char *translationTag, int rows) :
igdeMetaPropertyToggleTags(id, translationTag, rows){
}

igdeMetaPropertyToggleTagsStorage::~igdeMetaPropertyToggleTagsStorage() = default;

const decStringSet &igdeMetaPropertyToggleTagsStorage::GetPropertyValue(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyToggleTagsStorage::SetPropertyValue(
const igdeMetaContext::Ref &context, const decStringSet &value){
	GetStorage(context).SetValue(value);
}
