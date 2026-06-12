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

#include "igdeMetaPropertyVector3.h"
#include "undo/igdeMetaPropertyVector3Undo.h"
#include "widget/igdeMetaPropertyVector3Widget.h"
#include "../igdeMetaContext.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyVector3
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyVector3::igdeMetaPropertyVector3(
	const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description){
}

igdeMetaPropertyVector3::~igdeMetaPropertyVector3() = default;


// Management
///////////////

void igdeMetaPropertyVector3::SetDefaultValue(const decVector &value){
	pDefaultValue = value;
}

void igdeMetaPropertyVector3::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

igdeMetaPropertyVector3Undo::Ref igdeMetaPropertyVector3::ChangePropertyValue(
const igdeMetaContext::Ref &context, const decVector &newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context && context->GetUndoSystem()){
		auto undo = igdeMetaPropertyVector3Undo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		undo->Redo();
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyVector3::CreateWidget(const igdeMetaContext::Ref &context){
	return igdeMetaPropertyVector3Widget::Ref::New(*this, context);
}


// igdeMetaPropertyVector3Storage
///////////////////////////////////

igdeMetaPropertyVector3Storage::igdeMetaPropertyVector3Storage(
	const char *id, const char *name, const char *description) :
igdeMetaPropertyVector3(id, name, description){
}

igdeMetaPropertyVector3Storage::~igdeMetaPropertyVector3Storage() = default;

const decVector &igdeMetaPropertyVector3Storage::GetPropertyValue(const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyVector3Storage::SetPropertyValue(const igdeMetaContext::Ref &context, const decVector &value){
	GetStorage(context).SetValue(value);
}
