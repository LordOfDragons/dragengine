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

#include "igdeMetaPropertyColor.h"
#include "widget/igdeMetaPropertyColorWidget.h"
#include "../igdeMetaContext.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyColor
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyColor::igdeMetaPropertyColor(const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyColor::igdeMetaPropertyColor(const char *id, const char *translationTag) :
igdeMetaProperty(id, translationTag)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyColor::~igdeMetaPropertyColor() = default;


// Management
///////////////

void igdeMetaPropertyColor::SetDefaultValue(const decColor &value){
	pDefaultValue = value;
}

void igdeMetaPropertyColor::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

igdeMetaPropertyColorUndo::Ref igdeMetaPropertyColor::ChangePropertyValue(
const igdeMetaContext::Ref &context, const decColor &newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem() && GetCanUndo()){
		const auto undo = igdeMetaPropertyColorUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyColor::PresetList igdeMetaPropertyColor::GetPropertyPresets(const ContextRef&) const{
	return {};
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyColor::CreateWidget(){
	return igdeMetaPropertyColorWidget::Ref::New(*this);
}


// igdeMetaPropertyColorStorage
/////////////////////////////////

igdeMetaPropertyColorStorage::igdeMetaPropertyColorStorage(
	const char *id, const char *name, const char *description) :
igdeMetaPropertyColor(id, name, description){
}

igdeMetaPropertyColorStorage::igdeMetaPropertyColorStorage(
	const char *id, const char *translationTag) :
igdeMetaPropertyColor(id, translationTag){
}

igdeMetaPropertyColorStorage::~igdeMetaPropertyColorStorage() = default;

const decColor &igdeMetaPropertyColorStorage::GetPropertyValue(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyColorStorage::SetPropertyValue(
const igdeMetaContext::Ref &context, const decColor &value){
	GetStorage(context).SetValue(value);
}


// Test
#if 0
class TestColor{
public:
	igdeMetaPropertyColorStorage::Storage value;
	
	TestColor(igdeMetaPropertyColorStorage &property, const deTObjectReference<igdeMetaContext> &context) :
	value(property, context){
	}
	
	void Testing(){
		const decColor cv = value; // implicit conversion
		value = cv + decColor(0.1f, 0.1f, 0.1f); // implicit assignment. triggers notification
		const decColor cv2 = value.GetValue();
		value.SetValue(cv + decColor(0.1f, 0.1f, 0.1f)); // same value so no notification triggered
		value = cv2; // same value so no notification triggered
	}
};
#endif
