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

#include "igdeMetaPropertyPoint.h"
#include "undo/igdeMetaPropertyPointUndo.h"
#include "widget/igdeMetaPropertyPointWidget.h"
#include "../igdeMetaContext.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyPoint
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyPoint::igdeMetaPropertyPoint(
	const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description){
}

igdeMetaPropertyPoint::igdeMetaPropertyPoint(const char *id, const char *translationTag) :
igdeMetaProperty(id, translationTag){
}

igdeMetaPropertyPoint::~igdeMetaPropertyPoint() = default;


// Management
///////////////

void igdeMetaPropertyPoint::SetDefaultValue(const decPoint &value){
	pDefaultValue = value;
}

void igdeMetaPropertyPoint::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

igdeMetaPropertyPointUndo::Ref igdeMetaPropertyPoint::ChangePropertyValue(
const igdeMetaContext::Ref &context, const decPoint &newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem()){
		const auto undo = igdeMetaPropertyPointUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyPoint::CreateWidget(){
	return igdeMetaPropertyPointWidget::Ref::New(*this);
}


// igdeMetaPropertyPointStorage
///////////////////////////////////

igdeMetaPropertyPointStorage::igdeMetaPropertyPointStorage(
	const char *id, const char *name, const char *description) :
igdeMetaPropertyPoint(id, name, description){
}

igdeMetaPropertyPointStorage::igdeMetaPropertyPointStorage(const char *id, const char *translationTag) :
igdeMetaPropertyPoint(id, translationTag){
}

igdeMetaPropertyPointStorage::~igdeMetaPropertyPointStorage() = default;

const decPoint &igdeMetaPropertyPointStorage::GetPropertyValue(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyPointStorage::SetPropertyValue(
const igdeMetaContext::Ref &context, const decPoint &value){
	GetStorage(context).SetValue(value);
}
