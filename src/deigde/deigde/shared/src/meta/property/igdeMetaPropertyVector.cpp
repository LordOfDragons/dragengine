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

#include "igdeMetaPropertyVector.h"
#include "widget/igdeMetaPropertyVectorWidget.h"
#include "../igdeMetaContext.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyVector
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyVector::igdeMetaPropertyVector(
	const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description){
}

igdeMetaPropertyVector::igdeMetaPropertyVector(const char *id, const char *translationTag) :
igdeMetaProperty(id, translationTag){
}

igdeMetaPropertyVector::~igdeMetaPropertyVector() = default;


// Management
///////////////

void igdeMetaPropertyVector::SetDefaultValue(const decVector &value){
	pDefaultValue = value;
}

void igdeMetaPropertyVector::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

igdeMetaPropertyVectorUndo::Ref igdeMetaPropertyVector::ChangePropertyValue(
const igdeMetaContext::Ref &context, const decVector &newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem()){
		const auto undo = igdeMetaPropertyVectorUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyVector::CreateWidget(){
	return igdeMetaPropertyVectorWidget::Ref::New(*this);
}


// igdeMetaPropertyVectorStorage
///////////////////////////////////

igdeMetaPropertyVectorStorage::igdeMetaPropertyVectorStorage(
	const char *id, const char *name, const char *description) :
igdeMetaPropertyVector(id, name, description){
}

igdeMetaPropertyVectorStorage::igdeMetaPropertyVectorStorage(
	const char *id, const char *translationTag) :
igdeMetaPropertyVector(id, translationTag){
}

igdeMetaPropertyVectorStorage::~igdeMetaPropertyVectorStorage() = default;

const decVector &igdeMetaPropertyVectorStorage::GetPropertyValue(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyVectorStorage::SetPropertyValue(
const igdeMetaContext::Ref &context, const decVector &value){
	GetStorage(context).SetValue(value);
}


// igdeMetaPropertyVectorStorageQuaternion
////////////////////////////////////////////

igdeMetaPropertyVectorStorageQuaternion::igdeMetaPropertyVectorStorageQuaternion(
	const char *id, const char *name, const char *description) :
igdeMetaPropertyVector(id, name, description){
}

igdeMetaPropertyVectorStorageQuaternion::igdeMetaPropertyVectorStorageQuaternion(
	const char *id, const char *translationTag) :
igdeMetaPropertyVector(id, translationTag){
}

igdeMetaPropertyVectorStorageQuaternion::~igdeMetaPropertyVectorStorageQuaternion() = default;

const decVector &igdeMetaPropertyVectorStorageQuaternion::GetPropertyValue(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetEulerAngles();
}

void igdeMetaPropertyVectorStorageQuaternion::SetPropertyValue(
const igdeMetaContext::Ref &context, const decVector &value){
	GetStorage(context).SetEulerAngles(value);
}
