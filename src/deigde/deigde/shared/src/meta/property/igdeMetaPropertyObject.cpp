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

#include "igdeMetaPropertyObject.h"
#include "widget/igdeMetaPropertyObjectWidget.h"
#include "../igdeMetaContext.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyObject::Listener
///////////////////////////////////////////

void igdeMetaPropertyObject::Listener::OnAllowedObjectsChanged(
igdeMetaPropertyObject*, const igdeMetaContext::Ref&){
}

void igdeMetaPropertyObject::Listener::OnObjectItemInfoChanged(
igdeMetaPropertyObject*, const igdeMetaContext::Ref&){
}


// Class igdeMetaPropertyObject
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyObject::igdeMetaPropertyObject(const char *id, const char *name,
	const char *description) :
igdeMetaProperty(id, name, description)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyObject::igdeMetaPropertyObject(const char *id, const char *translationTag) :
igdeMetaProperty(id, translationTag)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyObject::~igdeMetaPropertyObject() = default;


// Management
///////////////

void igdeMetaPropertyObject::SetDefaultValue(const ObjectRef &value){
	pDefaultValue = value;
}

void igdeMetaPropertyObject::SetSorted(bool sorted){
	pSorted = sorted;
}

void igdeMetaPropertyObject::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

void igdeMetaPropertyObject::NotifyAllowedObjectsChanged(const ContextRef &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnAllowedObjectsChanged(this, context);
	});
}

void igdeMetaPropertyObject::NotifyObjectItemInfoChanged(const ContextRef &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnObjectItemInfoChanged(this, context);
	});
}

igdeMetaPropertyObjectUndo::Ref igdeMetaPropertyObject::ChangePropertyValue(
const igdeMetaContext::Ref &context, const deObject::Ref &newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem()){
		const auto undo = igdeMetaPropertyObjectUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyObject::CreateWidget(){
	return igdeMetaPropertyObjectWidget::Ref::New(*this);
}
