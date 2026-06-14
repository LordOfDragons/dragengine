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

#include "igdeMetaPropertyString.h"
#include "undo/igdeMetaPropertyStringUndo.h"
#include "widget/igdeMetaPropertyStringWidget.h"
#include "../igdeMetaContext.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyString::Listener
///////////////////////////////////////////

void igdeMetaPropertyString::Listener::OnStringListChanged(
igdeMetaPropertyString*, const igdeMetaContext::Ref&){
}


// Class igdeMetaPropertyString
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyString::igdeMetaPropertyString(
	const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description),
pEnableStringList(false){
}

igdeMetaPropertyString::~igdeMetaPropertyString() = default;


// Management
///////////////

void igdeMetaPropertyString::SetDefaultValue(const decString &value){
	pDefaultValue = value;
}

void igdeMetaPropertyString::SetEnableStringList(bool enable){
	pEnableStringList = enable;
}

void igdeMetaPropertyString::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

void igdeMetaPropertyString::NotifyStringListChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnStringListChanged(this, context);
	});
}

igdeMetaPropertyStringUndo::Ref igdeMetaPropertyString::ChangePropertyValue(
const igdeMetaContext::Ref &context, const char *newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem()){
		const auto undo = igdeMetaPropertyStringUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyString::CreateWidget(
const igdeMetaContext::Ref &context){
	return igdeMetaPropertyStringWidget::Ref::New(*this, context);
}


// igdeMetaPropertyStringStorage
//////////////////////////////////

igdeMetaPropertyStringStorage::igdeMetaPropertyStringStorage(
	const char *id, const char *name, const char *description) :
igdeMetaPropertyString(id, name, description){
}

igdeMetaPropertyStringStorage::~igdeMetaPropertyStringStorage() = default;

const decString &igdeMetaPropertyStringStorage::GetPropertyValue(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyStringStorage::SetPropertyValue(
const igdeMetaContext::Ref &context, const decString &value){
	GetStorage(context).SetValue(value);
}
