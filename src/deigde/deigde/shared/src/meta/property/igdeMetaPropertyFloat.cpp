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

#include "igdeMetaPropertyFloat.h"
#include "widget/igdeMetaPropertyFloatWidget.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyFloat::Listener
//////////////////////////////////////////

void igdeMetaPropertyFloat::Listener::OnLimitsChanged(igdeMetaPropertyFloat*, const ContextRef&){
}


// Class igdeMetaPropertyFloat
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyFloat::igdeMetaPropertyFloat(const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description),
pPrecision(3),
pDefaultValue(0.0f),
pLowerLimit(0.0f),
pUpperLimit(1.0f),
pTickSpacing(0.1f),
pEnableLowerLimit(false),
pEnableUpperLimit(false)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyFloat::igdeMetaPropertyFloat(const char *id, const char *translationTag) :
igdeMetaProperty(id, translationTag),
pPrecision(3),
pDefaultValue(0.0f),
pLowerLimit(0.0f),
pUpperLimit(1.0f),
pTickSpacing(0.1f),
pEnableLowerLimit(false),
pEnableUpperLimit(false)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyFloat::~igdeMetaPropertyFloat() = default;


// Management
///////////////

void igdeMetaPropertyFloat::SetPrecision(int precision){
	pPrecision = decMath::max(precision, 0);
}

void igdeMetaPropertyFloat::SetDefaultValue(float value){
	pDefaultValue = value;
}


void igdeMetaPropertyFloat::SetLowerLimit(float value){
	pLowerLimit = value;
}

void igdeMetaPropertyFloat::SetUpperLimit(float value){
	pUpperLimit = value;
}

void igdeMetaPropertyFloat::SetTickSpacing(float value){
	pTickSpacing = decMath::max(value, 0.0f);
}

void igdeMetaPropertyFloat::SetEnableLowerLimit(bool enable){
	pEnableLowerLimit = enable;
}

void igdeMetaPropertyFloat::SetEnableUpperLimit(bool enable){
	pEnableUpperLimit = enable;
}

void igdeMetaPropertyFloat::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

void igdeMetaPropertyFloat::NotifyLimitsChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnLimitsChanged(this, context);
	});
}

igdeMetaPropertyFloatUndo::Ref igdeMetaPropertyFloat::ChangePropertyValue(
const igdeMetaContext::Ref &context, float newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem()){
		const auto undo = igdeMetaPropertyFloatUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

float igdeMetaPropertyFloat::GetPropertyLowerLimit(const ContextRef &context) const{
	return pLowerLimit;
}

float igdeMetaPropertyFloat::GetPropertyUpperLimit(const ContextRef &context) const{
	return pUpperLimit;
}

float igdeMetaPropertyFloat::GetPropertyTickSpacing(const ContextRef &context) const{
	return pTickSpacing;
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyFloat::CreateWidget(){
	return igdeMetaPropertyFloatWidget::Ref::New(*this);
}


// igdeMetaPropertyFloatStorage
///////////////////////////////////

igdeMetaPropertyFloatStorage::igdeMetaPropertyFloatStorage(
	const char *id, const char *name, const char *description) :
igdeMetaPropertyFloat(id, name, description){
}

igdeMetaPropertyFloatStorage::igdeMetaPropertyFloatStorage(
	const char *id, const char *translationTag) :
igdeMetaPropertyFloat(id, translationTag){
}

igdeMetaPropertyFloatStorage::~igdeMetaPropertyFloatStorage() = default;

float igdeMetaPropertyFloatStorage::GetPropertyValue(const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyFloatStorage::SetPropertyValue(const igdeMetaContext::Ref &context, float value){
	GetStorage(context).SetValue(value);
}

float igdeMetaPropertyFloatStorage::GetPropertyLowerLimit(const ContextRef &context) const{
	return GetStorage(context).GetLowerLimit();
}

float igdeMetaPropertyFloatStorage::GetPropertyUpperLimit(const ContextRef &context) const{
	return GetStorage(context).GetUpperLimit();
}

float igdeMetaPropertyFloatStorage::GetPropertyTickSpacing(const ContextRef &context) const{
	return GetStorage(context).GetTickSpacing();
}
