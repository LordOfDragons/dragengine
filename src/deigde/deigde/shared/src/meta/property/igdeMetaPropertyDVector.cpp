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

#include "igdeMetaPropertyDVector.h"
#include "undo/igdeMetaPropertyDVectorUndo.h"
#include "widget/igdeMetaPropertyDVectorWidget.h"
#include "../igdeMetaContext.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyDVector
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyDVector::igdeMetaPropertyDVector(
	const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description),
pPrecision(3){
}

igdeMetaPropertyDVector::~igdeMetaPropertyDVector() = default;


// Management
///////////////

void igdeMetaPropertyDVector::SetPrecision(int precision){
	pPrecision = precision;
}

void igdeMetaPropertyDVector::SetDefaultValue(const decDVector &value){
	pDefaultValue = value;
}

void igdeMetaPropertyDVector::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

igdeMetaPropertyDVectorUndo::Ref igdeMetaPropertyDVector::ChangePropertyValue(
const igdeMetaContext::Ref &context, const decDVector &newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem()){
		const auto undo = igdeMetaPropertyDVectorUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyDVector::CreateWidget(
const igdeMetaContext::Ref &context){
	return igdeMetaPropertyDVectorWidget::Ref::New(*this, context);
}


// igdeMetaPropertyDVectorStorage
///////////////////////////////////

igdeMetaPropertyDVectorStorage::igdeMetaPropertyDVectorStorage(
	const char *id, const char *name, const char *description) :
igdeMetaPropertyDVector(id, name, description){
}

igdeMetaPropertyDVectorStorage::~igdeMetaPropertyDVectorStorage() = default;

const decDVector &igdeMetaPropertyDVectorStorage::GetPropertyValue(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyDVectorStorage::SetPropertyValue(
const igdeMetaContext::Ref &context, const decDVector &value){
	GetStorage(context).SetValue(value);
}
