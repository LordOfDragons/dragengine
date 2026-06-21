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

#include "igdeMetaPropertyBoolean.h"
#include "undo/igdeMetaPropertyBooleanUndo.h"
#include "widget/igdeMetaPropertyBooleanWidget.h"
#include "../igdeMetaContext.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyBoolean
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyBoolean::igdeMetaPropertyBoolean(
	const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description),
pDefaultValue(false){
}

igdeMetaPropertyBoolean::igdeMetaPropertyBoolean(const char *id, const char *translationTag) :
igdeMetaProperty(id, translationTag),
pDefaultValue(false){
}

igdeMetaPropertyBoolean::~igdeMetaPropertyBoolean() = default;


// Management
///////////////

void igdeMetaPropertyBoolean::SetDefaultValue(bool value){
	pDefaultValue = value;
}

void igdeMetaPropertyBoolean::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

igdeMetaPropertyBooleanUndo::Ref igdeMetaPropertyBoolean::ChangePropertyValue(
const igdeMetaContext::Ref &context, bool newValue, const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem()){
		const auto undo = igdeMetaPropertyBooleanUndo::Ref::New(
			*this, context, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyBoolean::CreateWidget(){
	return igdeMetaPropertyBooleanWidget::Ref::New(*this);
}


// igdeMetaPropertyBooleanStorage
///////////////////////////////////

igdeMetaPropertyBooleanStorage::igdeMetaPropertyBooleanStorage(
	const char *id, const char *name, const char *description) :
igdeMetaPropertyBoolean(id, name, description){
}

igdeMetaPropertyBooleanStorage::igdeMetaPropertyBooleanStorage(
	const char *id, const char *translationTag) :
igdeMetaPropertyBoolean(id, translationTag){
}

igdeMetaPropertyBooleanStorage::~igdeMetaPropertyBooleanStorage() = default;

bool igdeMetaPropertyBooleanStorage::GetPropertyValue(const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyBooleanStorage::SetPropertyValue(
const igdeMetaContext::Ref &context, bool value){
	GetStorage(context).SetValue(value);
}
