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

#include "igdeMetaPropertyInteger.h"
#include "widget/igdeMetaPropertyIntegerWidget.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyInteger::Listener
////////////////////////////////////////////

void igdeMetaPropertyInteger::Listener::OnLimitsChanged(igdeMetaPropertyInteger*, const ContextRef&){
}


// Class igdeMetaPropertyInteger
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyInteger::igdeMetaPropertyInteger(const char *id, const char *name,
	const char *description) :
igdeMetaProperty(id, name, description),
pDefaultValue(0),
pLowerLimit(0),
pUpperLimit(100),
pTickSpacing(10),
pEnableLowerLimit(false),
pEnableUpperLimit(false),
pEnableSpin(false)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyInteger::igdeMetaPropertyInteger(const char *id, const char *translationTag) :
igdeMetaProperty(id, translationTag),
pDefaultValue(0),
pLowerLimit(0),
pUpperLimit(100),
pTickSpacing(10),
pEnableLowerLimit(false),
pEnableUpperLimit(false),
pEnableSpin(false)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyInteger::~igdeMetaPropertyInteger() = default;


// Management
///////////////

void igdeMetaPropertyInteger::SetDefaultValue(int value){
	pDefaultValue = value;
}


void igdeMetaPropertyInteger::SetLowerLimit(int value){
	pLowerLimit = value;
}

void igdeMetaPropertyInteger::SetUpperLimit(int value){
	pUpperLimit = value;
}

void igdeMetaPropertyInteger::SetTickSpacing(int value){
	pTickSpacing = decMath::max(value, 1);
}

void igdeMetaPropertyInteger::SetEnableLowerLimit(bool enable){
	pEnableLowerLimit = enable;
}

void igdeMetaPropertyInteger::SetEnableUpperLimit(bool enable){
	pEnableUpperLimit = enable;
}

void igdeMetaPropertyInteger::SetEnableSpin(bool enable){
	pEnableSpin = enable;
}

void igdeMetaPropertyInteger::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

void igdeMetaPropertyInteger::NotifyLimitsChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnLimitsChanged(this, context);
	});
}

igdeMetaPropertyIntegerUndo::Ref igdeMetaPropertyInteger::ChangePropertyValue(
const igdeMetaContext::Ref &context, int newValue, const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem() && GetCanUndo()){
		const auto undo = igdeMetaPropertyIntegerUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

int igdeMetaPropertyInteger::GetPropertyLowerLimit(const ContextRef&) const{
	return pLowerLimit;
}

int igdeMetaPropertyInteger::GetPropertyUpperLimit(const ContextRef&) const{
	return pUpperLimit;
}

int igdeMetaPropertyInteger::GetPropertyTickSpacing(const ContextRef&) const{
	return pTickSpacing;
}

igdeMetaPropertyInteger::PresetList igdeMetaPropertyInteger::GetPropertyPresets(
const ContextRef&) const{
	return {};
}


igdeMetaPropertyWidget::Ref igdeMetaPropertyInteger::CreateWidget(){
	return igdeMetaPropertyIntegerWidget::Ref::New(*this);
}


// igdeMetaPropertyIntegerStorage
///////////////////////////////////

igdeMetaPropertyIntegerStorage::igdeMetaPropertyIntegerStorage(
	const char *id, const char *name, const char *description) :
igdeMetaPropertyInteger(id, name, description){
}

igdeMetaPropertyIntegerStorage::igdeMetaPropertyIntegerStorage(
	const char *id, const char *translationTag) :
igdeMetaPropertyInteger(id, translationTag){
}

igdeMetaPropertyIntegerStorage::~igdeMetaPropertyIntegerStorage() = default;

int igdeMetaPropertyIntegerStorage::GetPropertyValue(const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyIntegerStorage::SetPropertyValue(const igdeMetaContext::Ref &context, int value){
	GetStorage(context).SetValue(value);
}

int igdeMetaPropertyIntegerStorage::GetPropertyLowerLimit(const ContextRef &context) const{
	return GetStorage(context).GetLowerLimit();
}

int igdeMetaPropertyIntegerStorage::GetPropertyUpperLimit(const ContextRef &context) const{
	return GetStorage(context).GetUpperLimit();
}

int igdeMetaPropertyIntegerStorage::GetPropertyTickSpacing(const ContextRef &context) const{
	return GetStorage(context).GetTickSpacing();
}


// Test
#if 0
class TestInteger{
public:
	igdeMetaPropertyIntegerStorage::Storage value;
	
	TestInteger(igdeMetaPropertyIntegerStorage &property, const igdeMetaContext::Ref &context) :
	value(property, context){
	}
	
	void Testing(){
		const int cv = value; // implicit conversion
		value = cv + 5; // implicit assignment. triggers notification
		const int cv2 = value.GetValue();
		value.SetValue(cv + 5); // same value so no notification triggered
		value = cv2; // same value so no notification triggered
	}
};
#endif
