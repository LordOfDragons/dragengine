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

#include "igdeMetaPropertyTags.h"
#include "undo/igdeMetaPropertyTagsUndo.h"
#include "widget/igdeMetaPropertyTagsWidget.h"
#include "../igdeMetaContext.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyTags
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyTags::igdeMetaPropertyTags(
	const char *id, const char *name, const char *description, int rows) :
igdeMetaProperty(id, name, description),
pRows(rows){
}

igdeMetaPropertyTags::~igdeMetaPropertyTags() = default;


// Management
///////////////

void igdeMetaPropertyTags::SetRows(int rows){
	pRows = decMath::max(rows, 1);
}

void igdeMetaPropertyTags::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

igdeMetaPropertyTagsUndo::Ref igdeMetaPropertyTags::ChangePropertyValue(
const igdeMetaContext::Ref &context, const decStringSet &newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem()){
		const auto undo = igdeMetaPropertyTagsUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyTags::CreateWidget(const igdeMetaContext::Ref &context){
	return igdeMetaPropertyTagsWidget::Ref::New(*this, context);
}


// igdeMetaPropertyTagsStorage
////////////////////////////////

igdeMetaPropertyTagsStorage::igdeMetaPropertyTagsStorage(
	const char *id, const char *name, const char *description, int rows) :
igdeMetaPropertyTags(id, name, description, rows){
}

igdeMetaPropertyTagsStorage::~igdeMetaPropertyTagsStorage() = default;

const decStringSet &igdeMetaPropertyTagsStorage::GetPropertyValue(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyTagsStorage::SetPropertyValue(
const igdeMetaContext::Ref &context, const decStringSet &value){
	GetStorage(context).SetValue(value);
}
