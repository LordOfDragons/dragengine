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

#include "igdeMetaPropertyContext.h"
#include "widget/igdeMetaPropertyContextWidget.h"
#include "../igdeMetaContext.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyContext
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyContext::igdeMetaPropertyContext(const char *id,
	const char *label, const char *description) :
igdeMetaProperty(id, label, description){
	SetHideLabel(true);
}

igdeMetaPropertyContext::igdeMetaPropertyContext(const char *id, const char *translationTag) :
igdeMetaProperty(id, translationTag){
	SetHideLabel(true);
}

igdeMetaPropertyContext::~igdeMetaPropertyContext() = default;


// Management
///////////////

void igdeMetaPropertyContext::SetDefaultValue(const ContextRef &value){
	pDefaultValue = value;
}

void igdeMetaPropertyContext::SetProperties(const igdeMetaContext::PropertyList::Ref &properties){
	pProperties = properties;
}


void igdeMetaPropertyContext::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyContext::CreateWidget(){
	return igdeMetaPropertyContextWidget::Ref::New(*this);
}


// igdeMetaPropertyContextStorage
///////////////////////////////////

igdeMetaPropertyContextStorage::igdeMetaPropertyContextStorage(const char *id,
	const char *label, const char *description) :
igdeMetaPropertyContext(id, label, description){
}

igdeMetaPropertyContextStorage::~igdeMetaPropertyContextStorage() = default;

const igdeMetaContext::Ref &igdeMetaPropertyContextStorage::GetPropertyValue(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyContextStorage::SetPropertyValue(
const igdeMetaContext::Ref &context, const igdeMetaContext::Ref &value){
	GetStorage(context).SetValue(value);
}
