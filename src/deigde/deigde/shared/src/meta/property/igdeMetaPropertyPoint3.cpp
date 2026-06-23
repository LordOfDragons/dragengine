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

#include "igdeMetaPropertyPoint3.h"
#include "undo/igdeMetaPropertyPoint3Undo.h"
#include "widget/igdeMetaPropertyPoint3Widget.h"
#include "../igdeMetaContext.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyPoint3
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyPoint3::igdeMetaPropertyPoint3(const char *id, const char *name,
	const char *description) :
igdeMetaProperty(id, name, description)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyPoint3::igdeMetaPropertyPoint3(const char *id, const char *translationTag) :
igdeMetaProperty(id, translationTag)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyPoint3::~igdeMetaPropertyPoint3() = default;


// Management
///////////////

void igdeMetaPropertyPoint3::SetDefaultValue(const decPoint3 &value){
	pDefaultValue = value;
}

void igdeMetaPropertyPoint3::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

igdeMetaPropertyPoint3Undo::Ref igdeMetaPropertyPoint3::ChangePropertyValue(
const igdeMetaContext::Ref &context, const decPoint3 &newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem()){
		const auto undo = igdeMetaPropertyPoint3Undo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyPoint3::CreateWidget(){
	return igdeMetaPropertyPoint3Widget::Ref::New(*this);
}


// igdeMetaPropertyPoint3Storage
//////////////////////////////////

igdeMetaPropertyPoint3Storage::igdeMetaPropertyPoint3Storage(
	const char *id, const char *name, const char *description) :
igdeMetaPropertyPoint3(id, name, description){
}

igdeMetaPropertyPoint3Storage::igdeMetaPropertyPoint3Storage(const char *id, const char *translationTag) :
igdeMetaPropertyPoint3(id, translationTag){
}

igdeMetaPropertyPoint3Storage::~igdeMetaPropertyPoint3Storage() = default;

const decPoint3 &igdeMetaPropertyPoint3Storage::GetPropertyValue(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyPoint3Storage::SetPropertyValue(
const igdeMetaContext::Ref &context, const decPoint3 &value){
	GetStorage(context).SetValue(value);
}
